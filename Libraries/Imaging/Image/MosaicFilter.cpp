//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
#include "ImageFilter.h"
#include <memory>
#include <any>
#include <winerror.h>

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Class: CMosaicFilter
	// Defines an image filter for performing mosaic filtering
	//
	// Image Filter Parameters:
	// "BlockWidth" : <int> - Width in pixels of a block
	// "BlockHeight" : <int> - Height in pixels of a block
	//**********************************************************************
	class CMosaicFilter : public IImageFilter, public CRefCount
	{
		template <typename T>
		void Mosaic(IImage* pImage, ImageFilterParams* pParams);
	public:
		CMosaicFilter()
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
	// Function: CreateMosaicFilter
	// Creates a filter that turns an image into a mosaic
	//
	// Returns:
	// Returns filter that turns an image into a mosaic
	//**********************************************************************
	CRefObj<IImageFilter> CreateMosaicFilter()
	{
		return CRefObj<IImageFilter>(new CMosaicFilter());
	}

	//**********************************************************************
	template <typename T>
	void CMosaicFilter::Mosaic(IImage* pImage, ImageFilterParams* pParams)
	{
		bool hasMask = (pParams && pParams->spMask != nullptr) ? true : false;

		int blockw = 10;
		int blockh = 10;
		if (pParams != nullptr)
		{
			std::map<std::string, std::any>::iterator it;
			it = pParams->params.find("BlockWidth");
			if (it != pParams->params.end())
				blockw = std::any_cast<int>(it->second);
			it = pParams->params.find("BlockHeight");
			if (it != pParams->params.end())
				blockh = std::any_cast<int>(it->second);
		}

		T srcriter(pImage, 0, 0);
		T riter(pImage, 0, 0);
		CImageIter1 riter1;
		if (hasMask)
			riter1 = CImageIter1(pParams->spMask, 0, 0);
		int ystep = blockh;
		int w = pImage->GetWidth();
		int h = pImage->GetHeight();
		int y = h;
		while (y--)
		{
			T srcciter = srcriter;
			T citer = riter;
			CImageIter1 citer1 = riter1;
			int xstep = blockw;
			int x = w;
			while (x--)
			{
				if (xstep == 0)
				{
					xstep = blockw;
					while (xstep--)
						srcciter.Step(CImageIter::Right);
					xstep = blockw;
				}
				if (!hasMask || (hasMask && citer1.GetBit()))
				{
					citer.SetRed(srcciter.GetRed());
					citer.SetGreen(srcciter.GetGreen());
					citer.SetBlue(srcciter.GetBlue());
				}
				xstep--;
				citer.Step(CImageIter::Right);
				citer1.Step(CImageIter::Right);
			}
			if (--ystep == 0)
			{
				ystep = blockh;
				while (ystep--)
					srcriter.Step(CImageIter::Down);
				ystep = blockh;
			}
			riter.Step(CImageIter::Down);
			riter1.Step(CImageIter::Down);
		}
		return;
	}

	//**********************************************************************
	// Method: Apply
	// See <IImageFilter::Apply>
	//**********************************************************************
	CRefObj<IImage> CMosaicFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
	{
		CRefObj<IImage> spDest = pImage->Clone();
		ApplyInPlace(spDest, pParams);
		return spDest;
	}

	//**********************************************************************
	// Method: ApplyInPlace
	// See <IImageFilter::ApplyInPlace>
	//**********************************************************************
	bool CMosaicFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
	{
		if (pImage->GetBPP() == 24)
			Mosaic<CImageIter24>(pImage, pParams);
		else if (pImage->GetBPP() == 32)
			Mosaic<CImageIter32>(pImage, pParams);
		else
			CT(E_NOTIMPL);
		return true;
	}
}
