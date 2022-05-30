//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <Windows.h>
#include <vector>

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
        virtual uint32 AddRef() override {
            return CRefCount::AddRef();
        }
        virtual uint32 Release() override;

        //**********************************************************************
        // IImageBase
        //**********************************************************************
        virtual EImageType GetImageType() override { return m_spImage->GetImageType(); }
        virtual uint8* GetData() override { return m_spImage->GetData(); }
        virtual uint32 GetWidth() override { return m_spImage->GetWidth(); }
        virtual uint32 GetHeight() override { return m_spImage->GetHeight(); }
        virtual uint32 GetSubX() override { return m_spImage->GetSubX(); }
        virtual uint32 GetSubY() override { return m_spImage->GetSubY(); }
        virtual CRefObj<IImageBase> GetParent() override { return m_spImage->GetParent(); }
        virtual uint32 GetBPP() override { return m_spImage->GetBPP(); }
        virtual uint32 GetStride() override { return m_spImage->GetStride(); }
        virtual uint32 GetBytesPerPixel() override { return m_spImage->GetBytesPerPixel(); }
        virtual bool GetRGBOrder() override { return m_spImage->GetRGBOrder(); }
        virtual void SetRGBOrder(bool isRGB) override { m_spImage->SetRGBOrder(isRGB); }

        //**********************************************************************
        // IImage
        //**********************************************************************
        virtual void Clear() override { m_spImage->Clear(); }
        virtual CRefObj<IImage> Clone() override { return m_spImage->Clone(); }
        virtual void TakeDataOwnership() override { m_spImage->TakeDataOwnership(); }
        virtual void SetPixel(uint32 x, uint32 y, uint8 color[4]) override { m_spImage->SetPixel(x, y, color); }
        virtual void SetPixel(uint32 x, uint32 y, uint8 gray) override { m_spImage->SetPixel(x, y, gray); }
        virtual void SetPixel(uint32 x, uint32 y, uint16 v) override { m_spImage->SetPixel(x, y, v); }
        virtual void DrawCircle(Vector2& center, uint32 radius, uint8 color[4]) override { return m_spImage->DrawCircle(center, radius, color); }
        virtual void DrawLine(const Vector2& v0, const Vector2& v1, uint8 color[4]) override { return m_spImage->DrawLine(v0, v1, color); }
        virtual void DrawPath(IPath2* pPath) override { return m_spImage->DrawPath(pPath); }
    };
    
    //**********************************************************************
    // Class: CImagePool
    // Members:
    // std::vector< <CRefObj> < <CImagePoolImg> > > m_available - list of free images in the pool
    // std::vector< <CRefObj> < <CImagePoolImg> > > m_inUse - list of inUse images in the pool
    // <uint32 at Caustic::uint32> m_maxImages - maximum number of images to allocate in the pool. If 0 then there is no max.
    // <uint32 at Caustic::uint32> m_width - width in pixels of each image in the pool
    // <uint32 at Caustic::uint32> m_height - height in pixels of each image in the pool
    // <uint32 at Caustic::uint32> m_bpp - width of each pixel in bits
    //
    // Header:
    // {Link:#include "Imaging/Image/ImagePool.h"{Imaging/Image/ImagePool.h}}
    //**********************************************************************
    class CImagePool : public IImagePool, public CRefCount
    {
    protected:
        std::vector<CImagePoolImg*> m_available;
        std::vector<CImagePoolImg*> m_inUse;
        uint32 m_maxImages; // Maximum number of images to allocate in the pool. If 0 then there is no max
        uint32 m_width;  // Width in pixels for each image in the pool
        uint32 m_height; // Height in pixels for each image in the pool
        uint32 m_bpp;    // Bits per pixel for each image in the pool

        friend class CImagePoolImg;
    public:
        CImagePool() :
            m_width(0),
            m_height(0),
            m_bpp(0)
        {
        }

        CImagePool(uint32 maxImages, uint32 w, uint32 h, uint32 bpp);

        virtual ~CImagePool()
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImagePool
        //**********************************************************************
        virtual CRefObj<IImage> Acquire(uint32 w, uint32 h, uint32 bpp) override;
    };
    
    CRefObj<IImagePool> CreateImagePool(uint32 maxImages, uint32 width, uint32 height, uint32 bpp);
}
