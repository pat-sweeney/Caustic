//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <map>
#include <string>
#include <any>

export module Imaging.Image.ImageFilter;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Image.IImage;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Interface: ImageFilterParams
    // Parameters used by image filters when Apply() is called
    //
    // Members:
    // <CRefObj < IImage > > spMask - per pixel mask indicating which pixels have the filter applied. Values > 0 have the filter applied.
    // <std::map< std::string, std::any> > params - parameters specific to a filter type
    //
    // Module:
    // {Link:import Imaging.Image.ImageFilter{Imaging/Image/ImageFilter.ixx}}
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
    // Module:
    // {Link:import Imaging.Image.ImageFilter{Imaging/Image/ImageFilter.ixx}}
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
        virtual CRefObj<IImage> Apply(IImage* pImage, ImageFilterParams* pParams) = 0;

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
}
