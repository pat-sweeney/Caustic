//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <Windows.h>
#include <vector>
#include <cinttypes>

export module Imaging.Image.ImagePool;
import Base.Core.Core;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.IImagePool;

export namespace Caustic
{
    class CImagePool;
    
    //**********************************************************************
    // Class: CImagePoolImg
    // Implements an image in our image pool. This is just a wrapper around
    // the default image so we can catch it before its freed and place it back
    // in the pool.
    //**********************************************************************
    class CImagePoolImg : public IImage, public CRefCount
    {
        CRefObj<IImage> m_spImage;
        CImagePool *m_wpPool;
    public:
        CImagePoolImg() : m_wpPool(nullptr)
        {
        }

        CImagePoolImg(IImage *pImg, CImagePool *pPool)
        {
            m_spImage = pImg;
            m_wpPool = pPool;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override {
            return CRefCount::AddRef();
        }
        virtual uint32_t Release() override;

        //**********************************************************************
        // IImageBase
        //**********************************************************************
        virtual EImageType GetImageType() override { return m_spImage->GetImageType(); }
        virtual uint8_t* GetData() override { return m_spImage->GetData(); }
        virtual uint32_t GetWidth() override { return m_spImage->GetWidth(); }
        virtual uint32_t GetHeight() override { return m_spImage->GetHeight(); }
        virtual int32_t GetSubX() override { return m_spImage->GetSubX(); }
        virtual int32_t GetSubY() override { return m_spImage->GetSubY(); }
        virtual CRefObj<IImageBase> GetParent() override { return m_spImage->GetParent(); }
        virtual uint32_t GetBPP() override { return m_spImage->GetBPP(); }
        virtual uint32_t GetStride() override { return m_spImage->GetStride(); }

        //**********************************************************************
        // IImage
        //**********************************************************************
        virtual void Clear() override { m_spImage->Clear(); }
        virtual CRefObj<IImage> Clone() override { return m_spImage->Clone(); }
        virtual void TakeDataOwnership() override { m_spImage->TakeDataOwnership(); }
        virtual void SetPixel(int32_t x, int32_t y, uint8_t color[4]) override { m_spImage->SetPixel(x, y, color); }
        virtual void SetPixel(int32_t x, int32_t y, uint8_t gray) override { m_spImage->SetPixel(x, y, gray); }
        virtual void SetPixel(int32_t x, int32_t y, uint16_t v) override { m_spImage->SetPixel(x, y, v); }
        virtual void DrawCircle(Vector2& center, uint32_t radius, uint8_t color[4]) override { return m_spImage->DrawCircle(center, radius, color); }
        virtual void DrawLine(const Vector2& v0, const Vector2& v1, uint8_t color[4]) override { return m_spImage->DrawLine(v0, v1, color); }
        virtual void DrawPath(IPath2* pPath) override { return m_spImage->DrawPath(pPath); }
    };
    
    //**********************************************************************
    // Class: CImagePool
    // Members:
    // std::vector< <CRefObj> < <CImagePoolImg> > > m_available - list of free images in the pool
    // std::vector< <CRefObj> < <CImagePoolImg> > > m_inUse - list of inUse images in the pool
    // <uint32_t at Caustic::uint32_t> m_maxImages - maximum number of images to allocate in the pool. If 0 then there is no max.
    // <uint32_t at Caustic::uint32_t> m_width - width in pixels of each image in the pool
    // <uint32_t at Caustic::uint32_t> m_height - height in pixels of each image in the pool
    // <uint32_t at Caustic::uint32_t> m_bpp - width of each pixel in bits
    //
    // Header:
    // {Link:#include "Imaging/Image/ImagePool.h"{Imaging/Image/ImagePool.h}}
    //**********************************************************************
    class CImagePool : public IImagePool, public CRefCount
    {
    protected:
        std::vector<CImagePoolImg*> m_available;
        std::vector<CImagePoolImg*> m_inUse;
        uint32_t m_maxImages; // Maximum number of images to allocate in the pool. If 0 then there is no max
        uint32_t m_width;  // Width in pixels for each image in the pool
        uint32_t m_height; // Height in pixels for each image in the pool
        uint32_t m_bpp;    // Bits per pixel for each image in the pool

        friend class CImagePoolImg;
    public:
        CImagePool() :
            m_width(0),
            m_height(0),
            m_bpp(0)
        {
        }

        CImagePool(uint32_t maxImages, uint32_t w, uint32_t h, EImageType imageType);

        virtual ~CImagePool()
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImagePool
        //**********************************************************************
        virtual CRefObj<IImage> Acquire() override;
    };
    
    CRefObj<IImagePool> CreateImagePool(uint32_t maxImages, uint32_t width, uint32_t height, EImageType imageType);
}
