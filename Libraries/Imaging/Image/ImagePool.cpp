//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <winerror.h>
#include <vector>

module Imaging.Image.ImagePool;
import Base.Core.Core;
import Base.Core.Error;
import Imaging.Image.IImagePool;
import Imaging.Image.ImagePool;

namespace Caustic
{
    //**********************************************************************
    uint32 CImagePoolImg::Release()
    {
        uint32 refcnt = CRefCount::Release();
        if (refcnt == 1)
        {
            // Last external reference is about to go away. Push image back into our pool.
            for (int i = 0; i < (int)m_wpPool->m_inUse.size(); i++)
            {
                if (m_wpPool->m_inUse[i] == this)
                {
                    m_wpPool->m_inUse.erase(m_wpPool->m_inUse.begin() + i);
                    m_wpPool->m_available.push_back(this);
                }
            }
            return 0; // Lie and say our ref count went to 0
        }
        return refcnt;
    }

    CImagePool::CImagePool(uint32 maxImages, uint32 w, uint32 h, EImageType imageType)
    {
        m_maxImages = maxImages;
        m_width = w;
        m_height = h;
        switch (imageType)
        {
        case EImageType::BW_1bpp: m_bpp = 1; break;
        case EImageType::Gray_8bpp: m_bpp = 8; break;
        case EImageType::Gray_16bpp: m_bpp = 16; break;
        case EImageType::RGB_24bpp: m_bpp = 24; break;
        case EImageType::RGBA_32bpp: m_bpp = 32; break;
        case EImageType::RGBX_32bpp: m_bpp = 32; break;
        case EImageType::BGR_24bpp: m_bpp = 24; break;
        case EImageType::BGRA_32bpp: m_bpp = 32; break;
        case EImageType::BGRX_32bpp: m_bpp = 32; break;
        case EImageType::Float1_32bpp: m_bpp = 32; break;
        case EImageType::Float2_64bpp: m_bpp = 64; break;
        case EImageType::Float3_96bpp: m_bpp = 96; break;
        case EImageType::Float4_128bpp: m_bpp = 128; break;
        default:
            CT(E_INVALIDARG);
        }
        for (uint32 i = 0; i < maxImages; i++)
        {
            auto img = Caustic::CreateImage(w, h, imageType);
            auto poolimg = new CImagePoolImg(img, this);
            poolimg->AddRef();
            m_available.push_back(poolimg);
        }
    }
    
    CRefObj<IImage> CImagePool::Acquire()
    {
        if (m_available.size() == 0)
            CT(E_NOTIMPL);
        CImagePoolImg* wpImg = m_available[0];
        m_available.erase(m_available.begin());
        m_inUse.push_back(wpImg);
        return CRefObj<IImage>(wpImg);
    }

    CRefObj<IImagePool> CreateImagePool(uint32 maxImages, uint32 width, uint32 height, EImageType imageType)
    {
        return CRefObj<IImagePool>(new CImagePool(maxImages, width, height, imageType));
    }
}
