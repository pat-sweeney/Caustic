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
#include <vector>

namespace Caustic
{
    //**********************************************************************
    // Class: CBoxFilter
    // Defines an image filter for performing blurring via box filter.
    //**********************************************************************
    class CBoxFilter : public IImageFilter, public CRefCount
    {
        int m_kernelSize;
    public:
        CBoxFilter(int kernelSize)
        {
            m_kernelSize = kernelSize;
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
    // Function: CreateBoxFilter
    // Creates a box filter used for blurring.
    //
    // Parameters:
    // kernelSize - size of box filter
    //
    // Returns:
    // Returns box filter used for blurring.
    //**********************************************************************
    CRefObj<IImageFilter> CreateBoxFilter(int kernelSize)
    {
        return CRefObj<IImageFilter>(new CBoxFilter(kernelSize));
    }

    //**********************************************************************
    // Method: Apply
    // Image filter for blurring via box filtering.
    //
    // Parameters:
    // pImage - image to perform filtering on.
    // pParams - filter parameters.
    //
    // Returns:
    // Returns the filtered image.
    //**********************************************************************
    CRefObj<IImage> CBoxFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        CRefObj<IIntegralImage> spIntegralImage = CreateIntegralImage(pImage);
        return spIntegralImage->BoxBlur(40, 40);// m_kernelSize, m_kernelSize);
    }

    bool CBoxFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        return false; // Inplace transformation not supported
    }
}
