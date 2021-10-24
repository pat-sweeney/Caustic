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
            return 0; // Lied and say our ref count went to 0
        }
        return refcnt;
    }

    CImagePool::CImagePool(uint32 maxImages, uint32 w, uint32 h, uint32 bpp)
    {
        m_maxImages = maxImages;
        m_width = w;
        m_height = h;
        m_bpp = bpp;
        for (uint32 i = 0; i < maxImages; i++)
        {
            auto img = Caustic::CreateImage(w, h, bpp);
            auto poolimg = new CImagePoolImg(img, this);
            poolimg->AddRef();
            m_available.push_back(poolimg);
        }
    }
    
    // For now we ignore w/h/bpp since we assume all images in the pool
    // are the same size. This method has this signature to support mixed
    // pools in the future.
    CRefObj<IImage> CImagePool::Acquire(uint32 /*w*/, uint32 /*h*/ , uint32 /*bpp*/)
    {
        if (m_available.size() == 0)
            CT(E_NOTIMPL);
        CImagePoolImg* wpImg = m_available[0];
        m_available.erase(m_available.begin());
        m_inUse.push_back(wpImg);
        return CRefObj<IImage>(wpImg);
    }

    CRefObj<IImagePool> CreateImagePool(uint32 maxImages, uint32 width, uint32 height, uint32 bpp)
    {
        return CRefObj<IImagePool>(new CImagePool(maxImages, width, height, bpp));
    }
}
