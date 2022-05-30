//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <math.h>
#include <any>
#include <string>

export module Imaging.Image.ImageFilter.Scale;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;

export namespace Caustic
{
    enum EScaleUpMode
    {
        PixelReplicate,
        Bilinear
    };

    enum EScaleDownMode
    {
        NearestNeighbor,
        Jitter,
        Average
    };

    //**********************************************************************
    // Class: CScaleFilter
    // Defines an image filter that resizes an image
    //
    // Image Filter Parameters:
    // "ScaleX" : <float> - Scaling factor in X direction
    // "ScaleY" : <float> - Scaling factor in Y direction
    // "ScaleUpMode" : <EScaleUpMode> - Sampling mode to use when scaling up
    // "ScaleDownMode" : <EScaleDownMode> - Sampling mode to use when scaling down
    //**********************************************************************
    class CScaleFilter : public IImageFilter, public CRefCount
    {
        float m_scaleX;
        float m_scaleY;
        EScaleUpMode m_scaleUpMode;
        EScaleDownMode m_scaleDownMode;
    public:
        CScaleFilter()
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

        template <typename Iter>
        void Scale(IImage* pSrcImage, IImage* pDstImage, EScaleUpMode scaleUpMode, EScaleDownMode scaleDownMode, float sx, float sy);
    };

    //**********************************************************************
    // Function: CreateScale
    // Creates an image scale filter.
    //
    // Returns:
    // Returns the newly created filter.
    //**********************************************************************
    CRefObj<IImageFilter> CreateScale()
    {
        return CRefObj<IImageFilter>(new CScaleFilter());
    }
}
