//**********************************************************************
// Copyright Patrick Sweeney 1995-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include <memory>
#include <stdarg.h>
#include <winerror.h>
import Base.Core.Core;
import Base.Core.Error;
#include "Path2.h"
#include "Base/Math/Helper.h"

namespace Caustic {
	//**********************************************************************
	// Function: CreatePath2
	// Creates a new path
	//
	// Returns:
	// Returns new path object
	//**********************************************************************
	CRefObj<IPath2> CreatePath2()
	{
		return CRefObj<IPath2>(new CPath2());
	}

	void CPath2::BezierFlatten(Vector2* p0, Vector2* p1, Vector2* p2, Vector2* p3, float tolerance)
	{
		//**********************************************************************
		// We are given the 4 control points for the current segment
		// We are considered to be "flat" (i.e. within tolerance) if the middle two
		// control points are within 'tolerance' units of the line created by p0=>p3.
		//**********************************************************************
		float dist1 = Caustic::DistancePointToLine(*p0, *p3, *p1);
		float dist2 = Caustic::DistancePointToLine(*p0, *p3, *p2);
		if (dist1 < tolerance && dist2 < tolerance)
		{
			// Can't flatten it anymore. Just add a line segment
			this->AddPathElem(EPathElem::LineTo, p3);
			return;
		}
	
		//**********************************************************************
		// The curve is not yet flat enough, so divide the curve in half.
		// To do this, we can divide the curve at t=0.5 resulting in two new
		// sets of control points, c0-c3 (covering the 0<=t<=0.5 half of the curve)
		// and d0-d3 (covering the 0.5<=t<=1.0 half of the curve)
		//
		// See "Mathematical Elements for Computer Graphics", Rogers, page 304
		// for details on how these weightings are derived.
		//
		// c0 = p0
		// c1 = (1/2) (p1 + p0)
		// c2 = (1/4) (p2 + 2*p1 + p0)
		// c3 = (1/8) (p3 + 3*p2 + 3*p1 + p0)
		// d0 = (1/8) (p3 + 3*p2 + 3*p1 + p0)
		// d1 = (1/4) (p3 + 2*p2 + p1)
		// d2 = (1/2) (p3 + p2)
		// d0 = p3
		//**********************************************************************
		Vector2 p1x2 = *p1 * 2.0f;
		Vector2 p1x3 = *p1 * 3.0f;
		Vector2 p2x2 = *p2 * 2.0f;
		Vector2 p2x3 = *p2 * 3.0f;
		Vector2 c0 = *p0;
		Vector2 c1 = (*p1 + *p0) * 0.5f;
		Vector2 c2 = (*p2 + p1x2 + *p0) * 0.25f;
		Vector2 c3 = (*p3 + p2x3 + p1x3 + *p0) * 0.125f;
		Vector2 d0 = (*p3 + p2x3 + p1x3 + *p0) * 0.125f;
		Vector2 d1 = (*p3 + p2x2 + *p1) * 0.25f;
		Vector2 d2 = (*p3 + *p2) * 0.5f;
		Vector2 d3 = *p3;
		BezierFlatten(&c0, &c1, &c2, &c3, tolerance);
		BezierFlatten(&d0, &d1, &d2, &d3, tolerance);
	}

	CRefObj<IPath2> CPath2::Flatten(float flatness, BBox2* bb)
	{
		std::unique_ptr<CPath2> pNewPath(new CPath2());
		for (size_t i = 0; i < m_elems.size(); i++)
		{
			EPathElem elemType = m_elems[i].ElemType();
			switch (elemType)
			{
			case EPathElem::MoveTo:
			case EPathElem::LineTo:
				if (bb)
					bb->AddPoint(m_elems[i].pt[0]);
				pNewPath->AddPathElem(elemType, &m_elems[i].pt[0]);
				break;
			case EPathElem::CurveTo:
				if (bb)
				{
					bb->AddPoint(m_elems[i].pt[0]);
					bb->AddPoint(m_elems[i].pt[1]);
					bb->AddPoint(m_elems[i].pt[2]);
				}
				pNewPath->BezierFlatten(&m_elems[i - 1].pt[0], &m_elems[i].pt[2], &m_elems[i].pt[1], &m_elems[i].pt[0], flatness);
				break;
			case EPathElem::SmoothClosePath:
			case EPathElem::ClosePath:
				pNewPath->AddPathElem(elemType);
				break;
			}
		}
		return CRefObj<IPath2>(pNewPath.release());
	}

	bool CPath2::PointOnPath(Vector2 &pt, Vector2* closestpt, float tolerance)
	{
		int firstPt = 0;
		BBox2 bbox;

		int numElems = (int)m_elems.size();
		if (numElems < 2)
			return false;
		CT((m_elems[0].elemtype != EPathElem::MoveTo) ? E_UNEXPECTED : S_OK);
		Vector2* p1 = &m_elems[0].pt[0];
		Vector2* p2 = nullptr;
		for (int i = 0; i < numElems; i++)
		{
			if (m_elems[i].elemtype == EPathElem::MoveTo)
			{
				p1 = &m_elems[i].pt[0];
				firstPt = i;
				continue;
			}
			if (m_elems[i].elemtype == EPathElem::ClosePath)
				p2 = &m_elems[firstPt].pt[0];
			else
				p2 = &m_elems[i].pt[0];
			//
			// First check if point is inside line's expanded bounding box
			//
			bbox.AddPoint(*p1);
			bbox.AddPoint(*p2);
			bbox.minPt.x -= tolerance;
			bbox.minPt.y -= tolerance;
			bbox.maxPt.x += tolerance;
			bbox.maxPt.y += tolerance;
			if (bbox.PointInside(pt))
			{
				//
				// Compute distance.
				// [This needs review (code is 25 years old and not sure what I was trying to do here)]
				//
				float dx = p2->x - p1->x;
				float dy = p2->y - p1->y;
				float len = (float)sqrt(dx * dx + dy * dy);
				float t1 = -tolerance / len;
				float t2 = 1.0F - t1;
				float r = (dx * (pt.x - p1->x) + dy * (pt.y - p1->y)) / (len * len);
				if (r > t1 && r < t2)
				{
					float s = ((p1->y - pt.y) * dx - (p1->x - pt.x) * dy) / (len * len);
					float dist = s * len;
					if (dist < 0)
						dist = -dist;
					//
					// NOTE! This is not quite correct. This will currently find the intersection
					// point with the first edge that is within tolerance units of the line.
					// It is possible that another edge in the path is actually closer than
					// the computed point.
					//
					if (dist < tolerance)
					{
						if (r < 0.0F)
						{
							closestpt->x = p1->x;
							closestpt->y = p1->y;
						}
						else if (r > 1.0F)
						{
							closestpt->x = p2->x;
							closestpt->y = p2->y;
						}
						else
						{
							closestpt->x = p1->x + r * dx;
							closestpt->y = p1->y + r * dy;
						}
						return true;
					}
				}
			}
			p1 = p2;
		}
		return false;
	}

	BBox2 CPath2::GetBBox()
	{
		BBox2 bb;
		for (int i = 0; i < (int)m_elems.size(); i++)
		{
			if (m_elems[i].elemtype == EPathElem::CurveTo)
			{
				bb.AddPoint(m_elems[i].pt[1]);
				bb.AddPoint(m_elems[i].pt[2]);
			}
			bb.AddPoint(m_elems[i].pt[0]);
		}
		return bb;
	}

	bool CPath2::PathInsideBbox(BBox2& bbox)
	{
		Vector2* cp = nullptr;
		Vector2* lp = nullptr;
		Vector2* firstp = nullptr;
		for (int i = 0; i < (int)m_elems.size(); i++)
		{
			switch (m_elems[i].elemtype)
			{
			case EPathElem::MoveTo:
				firstp = lp = &m_elems[i].pt[0];
				continue;
			case EPathElem::LineTo:
				cp = &m_elems[i].pt[0];
				break;
			case EPathElem::CurveTo:
				cp = &m_elems[i].pt[0];
				break;
			case EPathElem::ClosePath:
				cp = firstp;
				break;
			}
			if (bbox.LineInside(*lp, *cp))
				return true;
			lp = cp;
		}
		return false;
	}

	void CPath2::AppendPathElems(EPathElem elemType, ...)
	{
		va_list args;
		va_start(args, elemType);
		while (elemType != EPathElem::Null)
		{
			Vector2 p0;
			switch (elemType)
			{
			case EPathElem::MoveTo:
			case EPathElem::LineTo:
				p0.x = (float)va_arg(args, double);
				p0.y = (float)va_arg(args, double);
				AddPathElem(elemType, &p0);
				break;
			case EPathElem::CurveTo:
			{
				Vector2 p1, p2;
				p0.x = (float)va_arg(args, double);
				p0.y = (float)va_arg(args, double);
				p1.x = (float)va_arg(args, double);
				p1.y = (float)va_arg(args, double);
				p2.x = (float)va_arg(args, double);
				p2.y = (float)va_arg(args, double);
				AddPathElem(elemType, &p0, &p1, &p2);
			}
				break;
			case EPathElem::ClosePath:
			case EPathElem::SmoothClosePath:
				AddPathElem(elemType, &p0);
				break;
			}
			elemType = va_arg(args, EPathElem);
		}
		va_end(args);
	}

	void CPath2::AddPathElem(EPathElem elemType, Vector2* p0, Vector2* p1, Vector2* p2)
	{
		PathElem pe;
		pe.elemtype = elemType;
		switch (elemType)
		{
		case EPathElem::MoveTo:
		case EPathElem::LineTo:
			pe.pt[0] = *p0;
			break;
		case EPathElem::CurveTo:
			pe.pt[0] = *p2;
			pe.pt[1] = *p1;
			pe.pt[2] = *p0;
			break;
		case EPathElem::ClosePath:
			for (int i = (int)m_elems.size() - 1; i >= 0; i--)
			{
				if (m_elems[i].elemtype == EPathElem::MoveTo)
				{
					pe.pt[0] = m_elems[i].pt[0];
					break;
				}
			}
			break;
		case EPathElem::SmoothClosePath:
			break;
		}
		m_elems.push_back(pe);
	}
}