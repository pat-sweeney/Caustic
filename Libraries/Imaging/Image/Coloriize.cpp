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
#include "ImageFilter.h"
#include "ImageIter.h"
#include <memory>

// Namespace: Caustic
namespace Caustic
{
    class CColorize : public IImageFilter, public CRefCount
    {
        int m_maxDepth;
    public:
        CColorize(int maxDepth) :
            m_maxDepth(maxDepth)
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
        virtual CRefObj<IImage> Apply(IImage* pImage, IImage* pMask) override;
        virtual bool ApplyInPlace(IImage* pImage, IImage* pMask) override;
    };

    //**********************************************************************
    // Function: CreateColorize
    // Creates a filter that converts depth values to a false color image.
    //
    // Returns:
    // Returns the newly created filter.
    //**********************************************************************
    CRefObj<IImageFilter> CreateColorize()
    {
        return CreateColorize(8000);
    }
    
    //**********************************************************************
    // Function: CreateColorize
    // Creates a filter that converts depth values to a false color image.
    //
    // Parameters:
    // maxDepth - max depth in millimeters (used for scaling)
    //
    // Returns:
    // Returns the newly created filter.
    //**********************************************************************
    CRefObj<IImageFilter> CreateColorize(int maxDepth)
    {
        return CRefObj<IImageFilter>(new CColorize(maxDepth));
    }

    //**********************************************************************
    // Method: Apply
    // See <IImageFilter::Apply>
    //**********************************************************************
    CRefObj<IImage> CColorize::Apply(IImage* pImage, IImage* pMask)
    {
        if (pImage->GetBPP() != 16)
            CT(E_UNEXPECTED);
        
        auto Saturate = [](int value) -> int
        {
            if (value < 0)
                return 0;
            if (value > 255)
                return 255;
            return value;
        };
        auto Abs = [](int v)->int
        {
            if (v < 0)
                return -v;
            return v;
        };

        CRefObj<IImage> spImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 32);

        unsigned short maxDepthVal = 0xFFFF;
        unsigned short minDepthVal = 0;
        CImageIter16 srcRow(pImage, 0, 0);
        CImageIter32 dstRow(spImage, 0, 0);
        for (int iy = 0; iy < (int)pImage->GetHeight(); iy++)
        {
            CImageIter16 srcCol = srcRow;
            CImageIter32 dstCol = dstRow;
            for (int ix = 0; ix < (int)pImage->GetWidth(); ix++)
            {
                uint16 depth = srcCol.GetGray();
                int normalizedDepth = (depth >= minDepthVal && depth <= maxDepthVal) ? (depth * 1024 / m_maxDepth) : 0;
                dstCol.SetRed((uint8)Saturate(384 - (int)Abs(normalizedDepth - 256)));
                dstCol.SetGreen((uint8)Saturate(384 - (int)Abs(normalizedDepth - 512)));
                dstCol.SetBlue((uint8)Saturate(384 - (int)Abs(normalizedDepth - 768)));
                dstCol.SetAlpha(255);
                srcCol.Step(CImageIter::Right);
                dstCol.Step(CImageIter::Right);
            }
            srcRow.Step(CImageIter::Down);
            dstRow.Step(CImageIter::Down);
        }
        return spImage;
    }
    
    bool CColorize::ApplyInPlace(IImage* pImage, IImage* pMask)
    {
        return false; // Not supported
    }
}
