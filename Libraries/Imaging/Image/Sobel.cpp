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
#include "ImageImpl.h"
#include "ImageFilter.h"
#include "ImageIter.h"
#include "Geometry\Rast\Bresenham.h"
#include "Imaging\Color\Color.h"

namespace Caustic
{
	//**********************************************************************
	// Class: CSobelFilter
	// Filter for performing edge detection using a Sobel filter.
	//
	// Image Filter Parameters:
	// None
	//**********************************************************************
	class CSobelFilter : public IImageFilter, public CRefCount
	{
		int m_kernelSize;
		bool m_horizontal;
		CRefObj<IImageFilter> m_spArbitrarySobel;
	public:
		CSobelFilter(bool horizontal)
		{
			m_horizontal = horizontal;
			m_kernelSize = 3;
		}

		CSobelFilter(int kernelSize, bool horizontal)
		{
			if (kernelSize == 3)
			{
				m_horizontal = horizontal;
				m_kernelSize = 3;
				return;
			}
			m_kernelSize = kernelSize;
			m_horizontal = horizontal;
			float* weights = nullptr;
			float weights5x5[] = {
				-5,-4,0,4,5,
				-8,-10,0,10,8,
				-10,-20,0,20,10,
				-8,-10,0,10,8,
				-5,-4,0,4,5
			};
			float weights7x7[] = {
				-130,-120,-78,0,78,120,130,
				-180,-195,-156,0,156,195,180,
				-234,-312,-390,0,390,312,234,
				-260,-390,-780,0,780,390,260,
				-234,-312,-390,0,390,312,234,
				-180,-195,-156,0,156,195,180,
				-130,-120,-78,0,78,120,130
			};
			if (kernelSize == 5)
				weights = weights5x5;
			else if (kernelSize == 7)
				weights = weights7x7;
			else
				CT(E_INVALIDARG);
			m_spArbitrarySobel = CreateCustomFilter(kernelSize, kernelSize, weights);
		}

		~CSobelFilter()
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
		virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams* pParams) override { return false; }
	};

	//**********************************************************************
	// Method: Apply
	// Defines an image filter for edge detectiong via a 3x3 Sobel filter.
	//
	// Parameters:
	// pImage - image to perform filtering on. Must be 32bpp image.
	// pParams - filter parameters.
	//
	// Returns:
	// Returns the filtered image.
	//**********************************************************************
	CRefObj<IImage> CSobelFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
	{
		if (m_kernelSize != 3)
			return m_spArbitrarySobel->Apply(pImage, pParams);
		CRefObj<IImage> spResult = CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP());
		if (pImage->GetBPP() != 32)
			CT(E_UNEXPECTED); // Only 32bbp images supported

		RGBColor clr[3][3];
		CImageIter32 riter[3];
		CImageIter32 citer[3];
		bool hasMask = pParams && pParams->spMask;

		uint32 w1 = pImage->GetWidth() - 1;
		uint32 h1 = pImage->GetHeight() - 1;
		riter[0] = CImageIter32(pImage, 0, 0);
		riter[1] = CImageIter32(pImage, 0, 0);
		riter[2] = CImageIter32(pImage, 0, (pImage->GetHeight() > 1) ? 1 : 0);
		CImageIter32 dstriter(spResult, 0, 0);
		CImageIter1 riter1;
		if (hasMask)
			riter1 = CImageIter1(pParams->spMask, 0, 0);
		uint32 w = pImage->GetWidth();
		uint32 h = pImage->GetHeight();
		for (uint32 y = 0; y < h; y++)
		{
			CImageIter32 dstciter = dstriter;
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
				clr[2][2].g = citer[2].GetGreen();
				clr[2][2].r = citer[2].GetRed();
				clr[2][2].b = citer[2].GetBlue();
				if (!hasMask || (hasMask && citer1.GetBit()))
				{
					int sumr, sumg, sumb;
					if (m_horizontal)
					{
						sumr = ( clr[0][0].r +  2 * clr[0][1].r +  clr[0][2].r +
						        -clr[2][0].r + -2 * clr[2][1].r + -clr[2][2].r);
						sumg = ( clr[0][0].r +  2 * clr[0][1].r +  clr[0][2].r +
						        -clr[2][0].r + -2 * clr[2][1].r + -clr[2][2].r);
						sumb = ( clr[0][0].r +  2 * clr[0][1].r +  clr[0][2].r +
						        -clr[2][0].r + -2 * clr[2][1].r + -clr[2][2].r);
					}
					else
					{
						sumr = ( clr[0][0].r +  2 * clr[1][0].r +  clr[2][0].r +
						        -clr[0][2].r + -2 * clr[1][2].r + -clr[2][2].r);
						sumg = ( clr[0][0].r +  2 * clr[1][0].r +  clr[2][0].r +
						        -clr[0][2].r + -2 * clr[1][2].r + -clr[2][2].r);
						sumb = ( clr[0][0].r +  2 * clr[1][0].r +  clr[2][0].r +
						        -clr[0][2].r + -2 * clr[1][2].r + -clr[2][2].r);
					}
					sumr = 255 * (sumr + 1024) / 2048;
					sumg = 255 * (sumg + 1024) / 2048;
					sumb = 255 * (sumb + 1024) / 2048;
					dstciter.SetRed(Clamp<uint8>((uint8)sumr, 0, 255));
					dstciter.SetGreen(Clamp<uint8>((uint8)sumg, 0, 255));
					dstciter.SetBlue(Clamp<uint8>((uint8)sumb, 0, 255));
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
			if (y < h1 - 1)
				riter[2].Step(CImageIter::Down);
			dstriter.Step(CImageIter::Down);
			if (hasMask)
				riter1.Step(CImageIter::Down);
		}
		return spResult;
	}

	//**********************************************************************
	// Function: CreateSobelFilter
	// Creates a filter that performs Sobel edge detection
	//
	// Paramters:
	// horizontal - are we computing the X gradient (true) or Y gradient (false)
	// kernelSize - size of kernel. Default is 3 for 3x3.
	//
	// Returns:
	// Returns the newly created filter.
	//**********************************************************************
	CRefObj<IImageFilter> CreateSobelFilter(bool horizontal, int kernelSize /*= 3*/)
	{
		return CRefObj<IImageFilter>(new CSobelFilter(kernelSize, horizontal));
	}
}
