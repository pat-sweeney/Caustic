//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <any>
#include <string>
#include <memory>
#include <map>

export module Imaging.Image.ImageFilter.Rotate;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Color;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;

export namespace Caustic
{
    //**********************************************************************
    // Class: CRotateFilter
    // Image filter for rotating an image.
    //
    // Image Filter Parameters:
    // "Threshold" : <int> - Threshold for determining whether a pixel is an edge
    // "Boost"     : <int> - Amount to boost signal at edge pixels
    //**********************************************************************
    class CRotateFilter : public IImageFilter, public CRefCount
    {
        template<typename Iter>
        CRefObj<IImage> Rotate(IImage* pImage, float degrees);

    public:
        CRotateFilter()
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
    // Function: CreateRotateFilter
    // Creates a filter for rotating an image.
    //
    // Returns:
    // Returns the filter for performing rotation.
    //**********************************************************************
    CRefObj<IImageFilter> CreateRotateFilter()
    {
        return CRefObj<IImageFilter>(new CRotateFilter());
    }
}
