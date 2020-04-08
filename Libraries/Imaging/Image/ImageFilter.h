//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"
#include "Imaging\Image\Image.h"

// Namespace: Caustic
namespace Caustic
{
	struct IImageFilter : public IRefCount
	{
		virtual void Apply(IImage* pImage, IImage *pMask, IImage** ppResult) = 0;
		virtual bool ApplyInPlace(IImage* pImage, IImage* pMask) = 0;
	};

	extern void CreateGaussianBlur(float sigma, IImageFilter** ppFilter);
	extern void CreateColorize(IImageFilter** ppFilter);
	extern void CreateColorize(int maxDepth, IImageFilter** ppFilter);
	extern void CreateMedian(IImageFilter** ppFilter);
}
