//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
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
	//
	// Header:
	// {Link:#include "Imaging/Image/ImageFilter.h"{Imaging/Image/ImageFilter.h}}
	//**********************************************************************
	struct ImageFilterParams
	{
		CRefObj<IImage> spMask;
		std::map<std::string, std::any> params; // Filter specific properties
	};

	//**********************************************************************
    // Interface: IImageFilter
    // Interface for objects that apply some sort of filter on a <IImage>
	//
	// Header:
	// {Link:#include "Imaging/Image/ImageFilter.h"{Imaging/Image/ImageFilter.h}}
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
    
	CRefObj<IImageFilter> CreateGaussianBlur(float sigma);
	CRefObj<IImageFilter> CreateGaussianBlur();
	CRefObj<IImageFilter> CreateColorize();
    CRefObj<IImageFilter> CreateColorize(int maxDepth);
    CRefObj<IImageFilter> CreateMedian();
    CRefObj<IImageFilter> CreateCustomFilter(int kernelWidth, int kernelHeight, float* kernelWeights);
    CRefObj<IImageFilter> CreateHarrisCornerFilter();
    CRefObj<IImageFilter> CreateSobelFilter(bool horizontal, int kernelSize = 3);
	CRefObj<IImageFilter> CreateSharpenFilter();
	CRefObj<IImageFilter> CreateUnsharpMaskFilter();
	CRefObj<IImageFilter> CreateEqualizeFilter();
	CRefObj<IImageFilter> CreateMosaicFilter();
	CRefObj<IImageFilter> CreateMotionBlurFilter();
	CRefObj<IImageFilter> CreateBoxFilter(int kernelSize);
	CRefObj<IImageFilter> CreateQuantizeFilter();
}
