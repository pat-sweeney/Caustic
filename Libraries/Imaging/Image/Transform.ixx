//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <map>
#include <string>
#include <any>

export module Imaging.Image.ImageFilter.Transform;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Color;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;
import Base.Math.Matrix.Matrix3x2;

export namespace Caustic
{
    //**********************************************************************
    // Class: CTransformFilter
    // Image filter for transforming an image.
    //
    // Image Filter Parameters:
    // "Matrix" : <Matrix3x2> - Matrix to transform the image by
    //**********************************************************************
    class CTransformFilter : public IImageFilter, public CRefCount
    {
        template<typename Iter>
        CRefObj<IImage> Transform(IImage* pImage, Matrix3x2 &tm);
    public:
        CTransformFilter()
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
    // Function: CreateTransformFilter
    // Creates a filter for applying an affine transform to an image.
    //
    // Returns:
    // Returns the filter for performing rotation.
    //**********************************************************************
    CRefObj<IImageFilter> CreateTransformFilter()
    {
        return CRefObj<IImageFilter>(new CTransformFilter());
    }
}
