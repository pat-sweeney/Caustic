//**********************************************************************
// Copyright Patrick Sweeney 1996-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Geometry/Rast/Bresenham.h"
#include "Image.h"
#include "ImageFilter.h"
#include "ImageIter.h"
#include <memory>
#include <any>

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Class: CMotionBlurFilter
	// Defines an image filter for performing motion blur
	//
	// Image Filter Parameters:
	// "DeltaX" : <int> - X direction for motion
	// "DeltaY" : <int> - Y direction for motion
	//**********************************************************************
	class CMotionBlurFilter : public IImageFilter, public CRefCount
	{
		template <typename T>
		CRefObj<IImage> MotionBlur(IImage* pImage, ImageFilterParams* pParams);
	public:
		CMotionBlurFilter()
		{
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IImageFilter
		//**********************************************************************
		virtual CRefObj<IImage> Apply(IImage* pImage, ImageFilterParams* pParams) override;
		virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams* pParams) override;
	};

	//**********************************************************************
	// Function: CreateMotionBlurFilter
	// Creates a filter that applies motion blur to an image
	//
	// Returns:
	// Returns filter that applies motion blur to an image
	//**********************************************************************
	CRefObj<IImageFilter> CreateMotionBlurFilter()
	{
		return CRefObj<IImageFilter>(new CMotionBlurFilter());
	}

	//**********************************************************************
	template <typename T>
	CRefObj<IImage> CMotionBlurFilter::MotionBlur(IImage* pImage, ImageFilterParams* pParams)
	{
		CRefObj<IImage> spDest = CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP());
		bool hasMask = (pParams && pParams->spMask != nullptr) ? true : false;

		int deltax = 10;
		int deltay = 10;
		if (pParams != nullptr)
		{
			std::map<std::string, std::any>::iterator it = pParams->params.find("DeltaX");
			if (it != pParams->params.end())
				deltax = std::any_cast<int>(it->second);
			it = pParams->params.find("DeltaY");
			if (it != pParams->params.end())
				deltay = std::any_cast<int>(it->second);
		}

		if (deltax == 0 && deltay == 0)
			return pImage->Clone();

		CImageIter::EStepDirection rdir;
		CImageIter::EStepDirection cdir;
		int sx, sy;
		int ex, ey;
		int dx, dy;

		int w = (int)pImage->GetWidth();
		int h = (int)pImage->GetHeight();
		if (deltax > 0)
		{
			cdir = CImageIter::Left;
			sx = w - 1;
			ex = 0;
			dx = -1;
		}
		else
		{
			cdir = CImageIter::Right;
			sx = 0;
			ex = w - 1;
			dx = 1;
		}
		if (deltay > 0)
		{
			rdir = CImageIter::Up;
			sy = h - 1;
			ey = 0;
			dy = -1;
		}
		else
		{
			rdir = CImageIter::Down;
			sy = 0;
			ey = h - 1;
			dy = 1;
		}
		T srcriter(pImage, sx, sy);
		T dstriter(spDest, sx, sy);
		CImageIter1 riter1;
		if (hasMask)
			riter1 = CImageIter1(pParams->spMask, sx, sy);
		for (int ly = 0, y = sy; ly < h; ly++, y += dy)
		{
			T srcciter = srcriter;
			T dstciter = dstriter;
			CImageIter1 citer1 = riter1;
			for (int lx = 0, x = sx; lx < w; lx++, x += dx)
			{
				if (!hasMask || (hasMask && citer1.GetBit()))
				{
					Bresenham bham(x, y, x - deltax, y - deltay);
					int r = 0, g = 0, b = 0, n = 0;
					//
					// Walk line along blur direction
					//
					T iter = srcciter;
					while (!bham.eol())
					{
						int cx = bham.get_x();
						int cy = bham.get_y();
						if (cx >= 0 && cx < w && cy >= 0 && cy < h)
						{
							r += (int)iter.GetRed();
							g += (int)iter.GetGreen();
							b += (int)iter.GetBlue();
							n++;
						}
						else
							break;
						bham.step();
						if (cx < bham.get_x())
							iter.Step(CImageIter::Right);
						else if (cx > bham.get_x())
							iter.Step(CImageIter::Left);
						if (cy < bham.get_y())
							iter.Step(CImageIter::Down);
						else if (cy > bham.get_y())
							iter.Step(CImageIter::Up);
					}
					dstciter.SetRed(r / n);
					dstciter.SetGreen(g / n);
					dstciter.SetBlue(b / n);
				}
				srcciter.Step(cdir);
				dstciter.Step(cdir);
				citer1.Step(cdir);
			}
			srcriter.Step(rdir);
			dstriter.Step(rdir);
			riter1.Step(rdir);
		}
		return spDest;
	}

	//**********************************************************************
	// Method: Apply
	// See <IImageFilter::Apply>
	//**********************************************************************
	CRefObj<IImage> CMotionBlurFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
	{
		if (pImage->GetBPP() == 24)
			return MotionBlur<CImageIter24>(pImage, pParams);
		else if (pImage->GetBPP() == 32)
			return MotionBlur<CImageIter32>(pImage, pParams);
		else
			CT(E_NOTIMPL);
		return CRefObj<IImage>(nullptr);
	}

	//**********************************************************************
	// Method: ApplyInPlace
	// See <IImageFilter::ApplyInPlace>
	//**********************************************************************
	bool CMotionBlurFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
	{
		return false;
	}
}
