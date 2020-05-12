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
	//**********************************************************************
	// Interface: IImageFilter
	// Interface for objects that apply some sort of filter on a <IImage>
	//**********************************************************************
	struct IImageFilter : public IRefCount
	{
		//**********************************************************************
		// Method: Apply
		// Applies the filter to the specified image. The specified mask defines
		// which pixels are effected by the filter (values > 0 in the mask will
		// have the filter applied).
		//
		// Parameters:
		// pImage - image to apply filter to
		// pMask - per pixel mask indicating which pixels have the filter applied.
		// Values > 0 have the filter applied.
		//
		// Returns:
		// Returns a new image with the filter applied
		//**********************************************************************
		virtual CRefObj<IImage> Apply(IImage* pImage, IImage *pMask) = 0;

		//**********************************************************************
		// Method: ApplyInPlace
		// Similar to the <Apply> method except the filter is applied in place
		// on the original image.
		//
		// Parameters:
		// pImage - image to apply filter to
		// pMask - per pixel mask indicating which pixels have the filter applied.
		// Values > 0 have the filter applied.
		//
		// Returns:
		// True if filter supports in place application. False otherwise.
		//**********************************************************************
		virtual bool ApplyInPlace(IImage* pImage, IImage* pMask) = 0;
	};

	CAUSTICAPI CRefObj<IImageFilter> CreateGaussianBlur(float sigma);
	CAUSTICAPI CRefObj<IImageFilter> CreateColorize();
	CAUSTICAPI CRefObj<IImageFilter> CreateColorize(int maxDepth);
	CAUSTICAPI CRefObj<IImageFilter> CreateMedian();
	CAUSTICAPI CRefObj<IImageFilter> CreateCustomFilter(int kernelWidth, int kernelHeight, float* kernelWeights);
}
