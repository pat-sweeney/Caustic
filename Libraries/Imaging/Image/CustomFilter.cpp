//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
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
#include "Geometry\Rast\Bresenham.h"
#include "Imaging\Color\Color.h"

namespace Caustic
{
	//**********************************************************************
	// Class: CCustomFilter
	// Defines an general image filter using a weighted kernel.
	//**********************************************************************
	class CCustomFilter : public IImageFilter, public CRefCount
	{
		int m_kernelWidth;
		int m_kernelHeight;
		float m_weightScale;
		float m_weightBias;
		float* m_kernelWeights;

		void FastApply(IImage* pImage, IImage* pMask, IImage* pResult);
		void SlowApply(IImage* pImage, IImage* pMask, IImage* pResult);
	public:
		CCustomFilter(int kernelWidth, int kernelHeight, float *kernelWeights)
		{
			m_kernelWidth = kernelWidth;
			m_kernelHeight = kernelHeight;
			m_kernelWeights = new float[kernelWidth * kernelHeight];
			float maxPos = 0.0f;
			float maxNeg = 0.0f;
			for (int i = 0; i < kernelWidth * kernelHeight; i++)
				if (kernelWeights[i] > 0.0f)
					maxPos += kernelWeights[i];
				else if (kernelWeights[i] < 0.0f)
					maxNeg += kernelWeights[i];
			m_weightScale = (1.0f / (maxPos - maxNeg));
			m_weightBias = 255.0f * (-maxNeg / (maxPos - maxNeg));
			memcpy(&m_kernelWeights[0], &kernelWeights[0], sizeof(float) * kernelWidth * kernelHeight);
		}

		~CCustomFilter()
		{
			delete[] m_kernelWeights;
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
	// Defines an image filter for performing median filtering.
	//
	// Parameters:
	// pImage - image to perform filtering on. Must be 32bpp image.
	// pParams - filter parameters
	//
	// Returns:
	// Returns the filtered image.
	//**********************************************************************
	CRefObj<IImage> CCustomFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
	{
		CRefObj<IImage> spResult = CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP());
		if (pImage->GetBPP() != 32)
			CT(E_UNEXPECTED); // Only 32bbp images supported
		if (m_kernelWidth == 3 && m_kernelHeight == 3)
			FastApply(pImage, pParams->spMask, spResult);
		else
			SlowApply(pImage, pParams->spMask, spResult);
		return spResult;
	}
	
	void CCustomFilter::SlowApply(IImage* pImage, IImage* pMask, IImage* pResult)
	{
		CT(E_UNEXPECTED);
	}

	void CCustomFilter::FastApply(IImage *pImage, IImage *pMask, IImage *pResult)
	{
		RGBColor clr[3][3];
		CImageIter32 riter[3];
		CImageIter32 citer[3];

		uint32 w1 = pImage->GetWidth() - 1;
		uint32 h1 = pImage->GetHeight() - 1;
		riter[0] = CImageIter32(pImage, 0, 0);
		riter[1] = CImageIter32(pImage, 0, 0);
		riter[2] = CImageIter32(pImage, 0, (pImage->GetHeight() > 1) ? 1 : 0);
		CImageIter32 dstriter(pResult, 0, 0);
		CImageIter1 riter1;
		if (pMask != nullptr)
			riter1 = CImageIter1(pMask, 0, 0);
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
				if (pMask == nullptr || citer1.GetBit())
				{
					int sumr, sumg, sumb;
					float fsumr = (m_kernelWeights[0] * clr[0][0].r +
						m_kernelWeights[1] * clr[0][1].r +
						m_kernelWeights[2] * clr[0][2].r +
						m_kernelWeights[3] * clr[1][0].r +
						m_kernelWeights[4] * clr[1][1].r +
						m_kernelWeights[5] * clr[1][2].r +
						m_kernelWeights[6] * clr[2][0].r +
						m_kernelWeights[7] * clr[2][1].r +
						m_kernelWeights[8] * clr[2][2].r);
					sumr = int(fsumr * m_weightScale + m_weightBias);
					sumg = int((m_kernelWeights[0] * clr[0][0].g +
						m_kernelWeights[1] * clr[0][1].g +
						m_kernelWeights[2] * clr[0][2].g +
						m_kernelWeights[3] * clr[1][0].g +
						m_kernelWeights[4] * clr[1][1].g +
						m_kernelWeights[5] * clr[1][2].g +
						m_kernelWeights[6] * clr[2][0].g +
						m_kernelWeights[7] * clr[2][1].g +
						m_kernelWeights[8] * clr[2][2].g) * m_weightScale + m_weightBias);
					sumb = int((m_kernelWeights[0] * clr[0][0].b +
						m_kernelWeights[1] * clr[0][1].b +
						m_kernelWeights[2] * clr[0][2].b +
						m_kernelWeights[3] * clr[1][0].b +
						m_kernelWeights[4] * clr[1][1].b +
						m_kernelWeights[5] * clr[1][2].b +
						m_kernelWeights[6] * clr[2][0].b +
						m_kernelWeights[7] * clr[2][1].b +
						m_kernelWeights[8] * clr[2][2].b) * m_weightScale + m_weightBias);
					//
					// Scale value into range
					//
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
				if (pMask != nullptr)
					citer1.Step(CImageIter::Right);
			}
			if (y > 0)
				riter[0].Step(CImageIter::Down);
			riter[1].Step(CImageIter::Down);
			if (y < h1 - 1)
				riter[2].Step(CImageIter::Down);
			dstriter.Step(CImageIter::Down);
			if (pMask != nullptr)
				riter1.Step(CImageIter::Down);
		}
	}

	//**********************************************************************
	// Function: CreateCustomFilter
	// Creates a filter that performs a kernal based filtering on an image.
	//
	// Returns:
	// Returns the newly created filter.
	//**********************************************************************
	CRefObj<IImageFilter> CreateCustomFilter(int kernelWidth, int kernelHeight, float *kernelWeights)
	{
		return CRefObj<IImageFilter>(new CCustomFilter(kernelWidth, kernelHeight, kernelWeights));
	}
}
