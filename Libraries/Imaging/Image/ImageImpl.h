//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include <memory>
#include <Windows.h>
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Image.h"
#include "ImageIter.h"

namespace Caustic
{
    class CImage : public IImage, public CRefCount
    {
        CRefObj<CImage> m_spParent;     // parent image if image is a subimage
        uint32 m_subx;                  // X offset into parent image
        uint32 m_suby;                  // Y offset into parent image
        uint8 *m_pData;                // pointer to image data
        bool m_ownData;                 // Did this object allocate m_pData?
        uint32 m_width;                 // width of image in pixels
        uint32 m_height;                // height of image in pixels
        uint32 m_bytesPerPixel;         // width of each pixel in bytes

        friend class CImageIter;
        friend void LoadImage(const wchar_t *pFilename, IImage **ppImage);
        friend void StoreImage(const wchar_t *pFilename, IImage *pImage);
        friend void CreateImage(uint32 width, uint32 height, uint32 bpp, IImage **ppImage);
    public:
        CImage()
        {
        }
        
        CImage(int w, int h)
        {
            m_width = w;
            m_height = h;
            m_bytesPerPixel = 4;
            m_pData = new BYTE[h * GetStride()];
        }

        virtual ~CImage()
        {
            if (m_ownData)
                delete [] m_pData;
        }

        //**********************************************************************
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageBase
        //**********************************************************************
        virtual uint8 *GetData() override { return m_pData; }
        virtual uint32 GetWidth() override { return m_width; }
        virtual uint32 GetHeight() override { return m_height; }
        virtual uint32 GetSubX() override { return m_subx; }
        virtual uint32 GetSubY() override { return m_suby; }
        virtual uint32 GetBPP() override { return m_bytesPerPixel * 8; }
        virtual uint32 GetStride() override { return m_width * GetBytesPerPixel(); }
        virtual uint32 GetBytesPerPixel() override { return m_bytesPerPixel; }

        //**********************************************************************
        // IImage
        //**********************************************************************
        virtual void Clear() override;
        virtual void Clone(IImage **ppImage) override;
        virtual void TakeDataOwnership() override {}
        virtual void SetPixel(uint32 x, uint32 y, uint8 color[4]) override;
        virtual void SetPixel(uint32 x, uint32 y, uint8 gray) override;
        virtual void SetPixel(uint32 x, uint32 y, uint16 v) override;
        virtual void DrawCircle(Vector2 &center, uint32 radius, uint8 color[4]) override;
        virtual void DrawLine(Vector2 &v0, Vector2 &v1, uint8 color[4]) override;
    };

    class CIntegralImage : public IIntegralImage, public CRefCount
    {
        std::unique_ptr<uint32> m_spData;
        uint32 m_width;
        uint32 m_height;
        CRefObj<IImage> m_spImage;

        friend extern void CreateIntegralImage(IImage *pImage, IIntegralImage **ppImage);
    public:
        CIntegralImage()
        {
        }

        CIntegralImage(int w, int h)
        {
            m_width = w;
            m_height = h;
            m_spData.reset(new uint32[w * h * 3]);
        }

        //**********************************************************************
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageBase
        //**********************************************************************
        virtual BYTE *GetData() override { return (BYTE*)m_spData.get(); }
        virtual uint32 GetStride() override { return m_width * sizeof(uint32) * 3; }
        virtual uint32 GetBytesPerPixel() override { return sizeof(uint32) * 3; }
        virtual uint32 GetWidth() override { return m_width; }
        virtual uint32 GetHeight() override { return m_height; }
        virtual uint32 GetSubX() override { return 0; }
        virtual uint32 GetSubY() override { return 0; }
        virtual uint32 GetBPP() override { return 24; }

        //**********************************************************************
        // IIntegralImage
        //**********************************************************************
        virtual uint32 GetSum(int channel, int x1, int y1, int x2, int y2) override;
        virtual void BoxBlur(int width, int height, IImage **ppImage) override;
    };
}
