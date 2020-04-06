//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
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
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageFilter
        //**********************************************************************
        virtual void Apply(IImage* pImage, IImage** ppResult) override;
        virtual bool ApplyInPlace(IImage* pImage) override;
    };

    void CreateColorize(IImageFilter** ppFilter)
    {
        CreateColorize(8000, ppFilter);
    }
    
    void CreateColorize(int maxDepth, IImageFilter** ppFilter)
    {
        std::unique_ptr<CColorize> spFilter(new CColorize(maxDepth));
        *ppFilter = spFilter.release();
        (*ppFilter)->AddRef();
    }

    void CColorize::Apply(IImage* pImage, IImage** ppResult)
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

        CRefObj<IImage> spImage;
        Caustic::CreateImage(pImage->GetWidth(), pImage->GetHeight(), 32, &spImage);

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
        *ppResult = spImage.Detach();
    }
    
    bool CColorize::ApplyInPlace(IImage* pImage)
    {
        return false; // Not supported
    }
}