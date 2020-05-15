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
#include <map>
#include <string>
#include <any>

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Interface: ImageFilterParams
	// Parameters used by image filters when Apply() is called
	//
	// Members:
	// <CRefObj < IImage > > spMask - per pixel mask indicating which pixels have the filter applied. Values > 0 have the filter applied.
	// <std::map< std::string, std::any> > params - parameters specific to a filter type
	//**********************************************************************
	struct ImageFilterParams
	{
		CRefObj<IImage> spMask;
		std::map<std::string, std::any> params; // Filter specific properties
	};

	//**********************************************************************
    // Interface: IImageFilter
    // Interface for objects that apply some sort of filter on a <IImage>
    //**********************************************************************
    struct IImageFilter : public IRefCount
	{
		//**********************************************************************
		// Method: Apply
		// Applies the filter to the specified image.
		//
		// Parameters:
		// pImage - image to apply filter to
		// pParams - list of parameters used by the specific filter
		//
		// Returns:
		// Returns a new image with the filter applied
		//**********************************************************************
		virtual CRefObj<IImage> Apply(IImage* pImage, ImageFilterParams *pParams) = 0;

		//**********************************************************************
		// Method: ApplyInPlace
		// Similar to the <Apply> method except the filter is applied in place
		// on the original image.
		//
		// Parameters:
		// pImage - image to apply filter to
		// pParams - list of parameters used by the specific filter
		//
		// Returns:
		// True if filter supports in place application. False otherwise.
		//**********************************************************************
		virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams* pParams) = 0;
	};
    
	CAUSTICAPI CRefObj<IImageFilter> CreateGaussianBlur(float sigma);
	CAUSTICAPI CRefObj<IImageFilter> CreateGaussianBlur();
	CAUSTICAPI CRefObj<IImageFilter> CreateColorize();
    CAUSTICAPI CRefObj<IImageFilter> CreateColorize(int maxDepth);
    CAUSTICAPI CRefObj<IImageFilter> CreateMedian();
    CAUSTICAPI CRefObj<IImageFilter> CreateCustomFilter(int kernelWidth, int kernelHeight, float* kernelWeights);
    CAUSTICAPI CRefObj<IImageFilter> CreateHarrisCornerFilter();
    CAUSTICAPI CRefObj<IImageFilter> CreateSobelFilter(bool horizontal, int kernelSize = 3);
	CAUSTICAPI CRefObj<IImageFilter> CreateSharpenFilter();
	CAUSTICAPI CRefObj<IImageFilter> CreateUnsharpMaskFilter();
}
