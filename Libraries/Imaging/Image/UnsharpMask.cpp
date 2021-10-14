//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
#include "Image.h"
#include "ImageFilter.h"
#include "ImageIter.h"
#include <memory>
#include <winerror.h>

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Class: CUnsharpMaskFilter
	// Defines an image filter for performing unsharp masking
	//
	// Image Filter Parameters:
	// "Strength"   : <int> - Strength of sharpening
	//**********************************************************************
	class CUnsharpMaskFilter : public IImageFilter, public CRefCount
	{
		template <typename T>
		CRefObj<IImage> UnsharpMask(IImage* pImage, IImage* pMask, int strength);
	public:
		CUnsharpMaskFilter()
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
	// Function: CreateUnsharpMaskFilter
	// Creates a filter that performs unsharp masking
	//
	// Returns:
	// Returns filter that performs unsharp masking.
	//**********************************************************************
	CRefObj<IImageFilter> CreateUnsharpMaskFilter()
	{
		return CRefObj<IImageFilter>(new CUnsharpMaskFilter());
	}

	template <typename T>
	CRefObj<IImage> CUnsharpMaskFilter::UnsharpMask(IImage *pImage, IImage *pMask, int strength)
	{
		CRefObj<IImage> spImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP());
		RGBColor clr[3][3];
		T riter[3];
		T citer[3];
		int32 sumr, sumg, sumb;
		int32 avgr, avgg, avgb;

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
				if (pMask == nullptr || citer1.GetBit())
				{
					avgr = (clr[0][0].r + clr[0][1].r + clr[0][2].r +
						clr[1][0].r + clr[1][1].r + clr[1][2].r +
						clr[2][0].r + clr[2][1].r + clr[2][2].r) / 9;
					avgg = (clr[0][0].g + clr[0][1].g + clr[0][2].g +
						clr[1][0].g + clr[1][1].g + clr[1][2].g +
						clr[2][0].g + clr[2][1].g + clr[2][2].g) / 9;
					avgb = (clr[0][0].b + clr[0][1].b + clr[0][2].b +
						clr[1][0].b + clr[1][1].b + clr[1][2].b +
						clr[2][0].b + clr[2][1].b + clr[2][2].b) / 9;
					sumr = (clr[1][1].r + (strength * (clr[1][1].r - avgr)) / 255);
					sumg = (clr[1][1].g + (strength * (clr[1][1].g - avgg)) / 255);
					sumb = (clr[1][1].b + (strength * (clr[1][1].b - avgb)) / 255);
					if (sumr < 0) sumr = 0;
					if (sumr > 255) sumr = 255;
					if (sumg < 0) sumg = 0;
					if (sumg > 255) sumg = 255;
					if (sumb < 0) sumb = 0;
					if (sumb > 255) sumb = 255;
					//
					// Scale value into range
					//
					dstciter.SetRed((uint8)sumr);
					dstciter.SetGreen((uint8)sumg);
					dstciter.SetBlue((uint8)sumb);
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
	// See <IImageFilter::Apply>
	//**********************************************************************
	CRefObj<IImage> CUnsharpMaskFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
	{
		int strength = (pParams == nullptr) ? 30 : std::any_cast<int>(pParams->params.find("Strength"));
		if (pImage->GetBPP() == 24)
			return UnsharpMask<CImageIter24>(pImage, (pParams == nullptr) ? nullptr : pParams->spMask, strength);
		else if (pImage->GetBPP() == 32)
			return UnsharpMask<CImageIter32>(pImage, (pParams == nullptr) ? nullptr : pParams->spMask, strength);
		CT(E_NOTIMPL);
		return CRefObj<IImage>(nullptr);
	}

	bool CUnsharpMaskFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
	{
		return false; // Not supported
	}
}
