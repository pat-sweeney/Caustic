//**********************************************************************
// Copyright Patrick Sweeney 1995-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Math\vector.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\core.h"
#include "Base\Math\BBox.h"
#include <vector>
#include "IPath2.h"

namespace Caustic
{
	class CPath2 : public IPath2, public CRefCount
	{
		std::vector<PathElem> m_elems;

		void BezierFlatten(Vector2* p0, Vector2* p1, Vector2* p2, Vector2* p3, float tolerance);
	public:
		CPath2()
		{
		}

		CPath2(uint32 numElems)
		{
			m_elems.resize(numElems);
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::AddRef(); }

		//**********************************************************************
		// IPath2
		//**********************************************************************
		virtual uint32 GetNumberElems() override { return (uint32)m_elems.size(); }
		virtual void AddPathElem(EPathElem elemType, Vector2* p0 = nullptr, Vector2* p1 = nullptr, Vector2* p2 = nullptr) override;
		virtual void AppendPathElems(EPathElem elemType, ...) override;
		virtual const PathElem* GetElement(int indx) override { return &m_elems[indx]; }
		virtual CRefObj<IPath2> Flatten(float flatness, BBox2* bb = nullptr) override;
		virtual bool PointOnPath(Vector2& pt, Vector2* closestpt, float tolerance) override;
		virtual BBox2 GetBBox() override;
		virtual bool PathInsideBbox(BBox2& bbox) override;
	};
}
