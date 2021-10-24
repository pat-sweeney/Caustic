//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <winerror.h>
#include <any>
#include <string>

export module Imaging.Image.ImageFilter.MedianFilter;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Color;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;
import Geometry.Rast.Bresenham;

export namespace Caustic
{
	//**********************************************************************
	// Class: CMedianFilter
	// Defines an image filter for performing median filtering.
	//
	// Image Filter Parameters:
	// None
	//**********************************************************************
	class CMedianFilter : public IImageFilter, public CRefCount
	{
	public:
		CMedianFilter()
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
		virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams *pParams) override { return false; }
	};
	
	//**********************************************************************
	// Method: Apply
	// Defines an image filter for performing median filtering.
	//
	// Parameters:
	// pImage - image to perform filtering on. Must be 32bpp image.
	// pParams - filter parameters.
	//
	// Returns:
	// Returns the filtered image.
	//**********************************************************************
	CRefObj<IImage> CMedianFilter::Apply(IImage* pImage, ImageFilterParams *pParams)
	{
		CRefObj<IImage> spResult = CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP());
		if (pImage->GetBPP() != 32)
			CT(E_UNEXPECTED); // Only 32bbp images supported
		RGBColor clr[3][3];
		CImageIter32 riter[3];
		CImageIter32 citer[3];
		CImageIter1 citer1;
		CImageIter1 riter1;

		bool hasMask = pParams && pParams->spMask;
		uint32 w = pImage->GetWidth();
		uint32 h = pImage->GetHeight();
		uint32 w1 = w - 1;
		uint32 h1 = h - 1;
		riter[0] = CImageIter32(pImage, 0, 0);
		riter[1] = CImageIter32(pImage, 0, 0);
		riter[2] = CImageIter32(pImage, 0, (h > 1) ? 1 : 0);
		CImageIter32 dstriter = CImageIter32(spResult, 0, 0);
		if (hasMask)
			riter1 = CImageIter1(pParams->spMask, 0, 0);
		for (int y = 0; y < (int)h; y++)
		{
			CImageIter32 dstciter = dstriter;
			citer[0] = riter[0];
			citer[1] = riter[1];
			citer[2] = riter[2];
			citer1 = riter1;
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
			int x = (int)w;
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
				clr[2][2].g = citer[2].GetGreen();
				clr[2][2].r = citer[2].GetRed();
				clr[2][2].b = citer[2].GetBlue();
				if (!hasMask || (hasMask && citer1.GetBit()))
				{
					//
					// Find median value
					//
					RGBColor* valr[9];
					RGBColor* valg[9];
					RGBColor* valb[9];
					valr[0] = valg[0] = valb[0] = &clr[0][0];
					valr[1] = valg[1] = valb[1] = &clr[0][1];
					valr[2] = valg[2] = valb[2] = &clr[0][2];
					valr[3] = valg[3] = valb[3] = &clr[1][0];
					valr[4] = valg[4] = valb[4] = &clr[1][1];
					valr[5] = valg[5] = valb[5] = &clr[1][2];
					valr[6] = valg[6] = valb[6] = &clr[2][0];
					valr[7] = valg[7] = valb[7] = &clr[2][1];
					valr[8] = valg[8] = valb[8] = &clr[2][1];
					//
					// Sort list
					//
					for (int i = 0; i < 9; i++)
					{
						for (int j = i + 1; j < 9; j++)
						{
							if (valr[i]->r > valr[j]->r)
							{
								RGBColor* tmp = valr[i];
								valr[i] = valr[j];
								valr[j] = tmp;
							}
							if (valg[i]->g > valg[j]->g)
							{
								RGBColor* tmp = valg[i];
								valg[i] = valg[j];
								valg[j] = tmp;
							}
							if (valb[i]->b > valb[j]->b)
							{
								RGBColor* tmp = valb[i];
								valb[i] = valb[j];
								valb[j] = tmp;
							}
						}
					}
					dstciter.SetRed(valr[5]->r);
					dstciter.SetGreen(valg[5]->g);
					dstciter.SetBlue(valb[5]->b);
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
				if (hasMask)
					citer1.Step(CImageIter::Right);
			}
			if (y > 0)
				riter[0].Step(CImageIter::Down);
			riter[1].Step(CImageIter::Down);
			if (y < (int)h1 - 1)
				riter[2].Step(CImageIter::Down);
			dstriter.Step(CImageIter::Down);
			if (hasMask)
				riter1.Step(CImageIter::Down);
		}
		return spResult;
	}
	
	//**********************************************************************
	// Function: CreateMedian
	// Creates a filter that performs median filtering on an image.
	//
	// Returns:
	// Returns the newly created filter.
	//**********************************************************************
	CRefObj<IImageFilter> CreateMedian()
	{
		return CRefObj<IImageFilter>(new CMedianFilter());
	}
}