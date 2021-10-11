//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Image.h"
#include "ImageImpl.h"
#include "ImageFilter.h"
#include "ImageIter.h"

namespace Caustic
{
	//**********************************************************************
	// Class: CSharpenFilter
	// Defines an image filter for performing sharpening.
	//
	// Image Filter Parameters:
	// "Threshold" : <int> - Threshold for determining whether a pixel is an edge
	// "Boost"     : <int> - Amount to boost signal at edge pixels
	//**********************************************************************
	class CSharpenFilter : public IImageFilter, public CRefCount
	{
		template<typename T>
		CRefObj<IImage> Sharpen(int threshold, int boost, IImage* pImage, IImage* pMask);
	public:
		CSharpenFilter()
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
	// Function: CreateSharpenFilter
	// Creates a sharpen filter.
	//
	// Returns:
	// Returns the filter for performing image sharpening.
	//**********************************************************************
	CRefObj<IImageFilter> CreateSharpenFilter()
	{
		return CRefObj<IImageFilter>(new CSharpenFilter());
	}

	template<typename T>
	CRefObj<IImage> CSharpenFilter::Sharpen(int threshold, int boost, IImage* pImage, IImage* pMask)
	{
		RGBColor clr[3][3];
		T riter[3];
		T citer[3];

		CRefObj<IImage> spImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP());
		uint32 w1 = pImage->GetWidth() - 1;
		uint32 h1 = pImage->GetHeight() - 1;
		riter[0] = T(pImage, 0, 0);
		riter[1] = T(pImage, 0, 0);
		riter[2] = T(pImage, 0, (pImage->GetHeight() > 1) ? 1 : 0);
		T dstriter(spImage, 0, 0);
		CImageIter1 riter1;
		if (pMask)
			riter1 = CImageIter1(pMask, 0, 0);
		uint32 w = pImage->GetWidth();
		uint32 h = pImage->GetHeight();
		for (uint32 y = 0; y < h; y++)
		{
			T dstciter = dstriter;
			citer[0] = riter[0];
			citer[1] = riter[1];
			citer[2] = riter[2];
			CImageIter1 citer1 = riter1;
			clr[0][0].r = clr[0][1].r = citer[0].GetRed();
			clr[1][0].r = clr[1][1].r = citer[1].GetRed();
			clr[2][0].r = clr[2][1].r = citer[2].GetRed();
			clr[0][0].g = clr[0][1].g = citer[0].GetGreen();
			clr[1][0].g = clr[1][1].g = citer[1].GetGreen();
			clr[2][0].g = clr[2][1].g = citer[2].GetGreen();
			clr[0][0].b = clr[0][1].b = citer[0].GetBlue();
			clr[1][0].b = clr[1][1].b = citer[1].GetBlue();
			clr[2][0].b = clr[2][1].b = citer[2].GetBlue();
			if (w > 1)
			{
				citer[0].Step(CImageIter::Right);
				citer[1].Step(CImageIter::Right);
				citer[2].Step(CImageIter::Right);
			}
			uint32 x = pImage->GetWidth();
			while (x--)
			{
				//
				// Apply mask
				//
				clr[0][2].r = citer[0].GetRed();
				clr[0][2].g = citer[0].GetGreen();
				clr[0][2].b = citer[0].GetBlue();
				clr[1][2].r = citer[1].GetRed();
				clr[1][2].g = citer[1].GetGreen();
				clr[1][2].b = citer[1].GetBlue();
				if (y < h1)
				{
					clr[2][2].r = citer[2].GetRed();
					clr[2][2].g = citer[2].GetGreen();
					clr[2][2].b = citer[2].GetBlue();
				}
				if (pMask == nullptr || (pMask != nullptr && citer1.GetBit()))
				{
					int xgradient = (-1 * clr[0][0].r + -2 * clr[0][1].r + -1 * clr[0][2].r +
						              1 * clr[2][0].r +  2 * clr[2][1].r +  1 * clr[2][2].r);
					int ygradient = (-1 * clr[0][0].r + -2 * clr[1][0].r + -1 * clr[2][0].r +
						              1 * clr[0][2].r +  2 * clr[1][2].r +  1 * clr[2][2].r);
					if (xgradient < 0)
						xgradient = -xgradient;
					if (ygradient < 0)
						ygradient = -ygradient;
					dstciter.SetRed((uint8)clr[1][1].r + ((xgradient + ygradient > threshold) ? boost : 0));
					xgradient = (-1 * clr[0][0].g + -2 * clr[0][1].g + -1 * clr[0][2].g +
						          1 * clr[2][0].g +  2 * clr[2][1].g +  1 * clr[2][2].g);
					ygradient = (-1 * clr[0][0].g + -2 * clr[1][0].g + -1 * clr[2][0].g +
						          1 * clr[0][2].g +  2 * clr[1][2].g +  1 * clr[2][2].g);
					if (xgradient < 0)
						xgradient = -xgradient;
					if (ygradient < 0)
						ygradient = -ygradient;
					dstciter.SetGreen((uint8)clr[1][1].g + ((xgradient + ygradient > threshold) ? boost : 0));
					xgradient = (-1 * clr[0][0].b + -2 * clr[0][1].b + -1 * clr[0][2].b +
						          1 * clr[2][0].b +  2 * clr[2][1].b +  1 * clr[2][2].b);
					ygradient = (-1 * clr[0][0].b + -2 * clr[1][0].b + -1 * clr[2][0].b +
						          1 * clr[0][2].b +  2 * clr[1][2].b +  1 * clr[2][2].b);
					if (xgradient < 0)
						xgradient = -xgradient;
					if (ygradient < 0)
						ygradient = -ygradient;
					dstciter.SetBlue((uint8)clr[1][1].b + ((xgradient + ygradient > threshold) ? boost : 0));
					dstciter.SetAlpha(255);
				}
				clr[0][0] = clr[0][1];
				clr[1][0] = clr[1][1];
				clr[2][0] = clr[2][1];
				clr[0][1] = clr[0][2];
				clr[1][1] = clr[1][2];
				clr[2][1] = clr[2][2];
				if (x > 1)
				{
					citer[0].Step(CImageIter::Right);
					citer[1].Step(CImageIter::Right);
					citer[2].Step(CImageIter::Right);
				}
				dstciter.Step(CImageIter::Right);
				if (pMask)
					citer1.Step(CImageIter::Right);
			}
			if (y > 0)
				riter[0].Step(CImageIter::Down);
			riter[1].Step(CImageIter::Down);
			if (y < h1)
				riter[2].Step(CImageIter::Down);
			dstriter.Step(CImageIter::Down);
			if (pMask)
				riter1.Step(CImageIter::Down);
		}
		return spImage;
	}

	//**********************************************************************
	// Method: Apply
	// Defines an image filter for performing gaussian blurring.
	//
	// Parameters:
	// pImage - image to perform filtering on.
	// pParams - filter parameters.
	//
	// Returns:
	// Returns the filtered image.
	//**********************************************************************
	CRefObj<IImage> CSharpenFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
	{
		int threshold = 80;
		int boost = 20;
		if (pParams != nullptr)
		{
			std::map<std::string, std::any>::iterator it;
			it = pParams->params.find("Threshold");
			if (it != pParams->params.end())
				threshold = std::any_cast<int>(it->second);
			it = pParams->params.find("Boost");
			if (it != pParams->params.end())
				boost = std::any_cast<int>(it->second);
		}

		if (pImage->GetBPP() == 24)
		{
			return Sharpen<CImageIter24>(threshold, boost, pImage, (pParams && pParams->spMask) ? pParams->spMask : nullptr);
		}
		else if (pImage->GetBPP() == 32)
		{
			return Sharpen<CImageIter32>(threshold, boost, pImage, (pParams && pParams->spMask) ? pParams->spMask : nullptr);
		}
		return nullptr;
	}

	bool CSharpenFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
	{
		return false; // Inplace transformation not supported
	}
}
