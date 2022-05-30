//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <Windows.h>

export module Imaging.Image.Image;
import Base.Core.Core;
import Base.Core.RefCount;
import Geometry.GeomDS.IPath2;
import Imaging.Image.ImageIter;
import Imaging.Image.IImage;

export namespace Caustic
{
    //**********************************************************************
    // Class: CImage
    // Members:
    // <EImageType at Caustic::EImageType> m_imageType - Type of data stored in the image
    // <CRefObj> < <CImage> > m_spParent - parent image if image is a subimage
    // <uint32 at Caustic::uint32> m_subx - X offset into parent image
    // <uint32 at Caustic::uint32> m_suby - Y offset into parent image
    // <uint8 at Caustic::uint8>* m_pData - pointer to image data
    // bool m_ownData - Did this object allocate m_pData?
    // <uint32 at Caustic::uint32> m_width - width of image in pixels
    // <uint32 at Caustic::uint32> m_height - height of image in pixels
    // <uint32 at Caustic::uint32> m_bytesPerPixel - width of each pixel in bytes
    // bool m_isRGB - Order pixel data is layed out in memory
    //
    // Header:
    // {Link:#include "Imaging/Image/ImageImpl.h"{Imaging/Image/ImageImpl.h}}
    //**********************************************************************
    class CImage : public IImage, public CRefCount
    {
        EImageType m_imageType;         // Type of data stored in the image
        CRefObj<CImage> m_spParent;     // parent image if image is a subimage
        uint32 m_subx;                  // X offset into parent image
        uint32 m_suby;                  // Y offset into parent image
        uint8 *m_pData;                 // pointer to image data
        bool m_ownData;                 // Did this object allocate m_pData?
        uint32 m_width;                 // width of image in pixels
        uint32 m_height;                // height of image in pixels
        uint32 m_bytesPerPixel;         // width of each pixel in bytes
        bool m_isRGB;                   // Is pixel memory layed out in RGB or BGR order?

        friend class CImageIter;
        friend CRefObj<IImage> LoadImageFileImpl(const wchar_t *pFilename);
        friend void StoreImageImpl(const wchar_t *pFilename, IImage *pImage);
    public:
        CImage() :
            m_imageType(EImageType::Unknown),
            m_width(0),
            m_height(0),
            m_bytesPerPixel(0),
            m_spParent(nullptr),
            m_subx(0),
            m_suby(0),
            m_pData(nullptr),
            m_ownData(true)
        {
        }
        
        CImage(int w, int h, int bpp)
        {
            m_width = w;
            m_height = h;
            m_bytesPerPixel = bpp / 8;
            m_spParent = nullptr;
            m_subx = 0;
            m_suby = 0;
            uint32 stride = m_width * m_bytesPerPixel;
            uint32 numbytes = stride * m_height;
            m_pData = new BYTE[numbytes];
            m_ownData = true;
            switch (bpp)
            {
            case 1: m_imageType = EImageType::BW_1bpp; break;
            case 8: m_imageType = EImageType::Gray_8bpp; break;
            case 16: m_imageType = EImageType::Gray_16bpp; break;
            case 24: m_imageType = EImageType::RGB_24bpp; break;
            case 32: m_imageType = EImageType::RGBA_32bpp; break;
            }
        }

        virtual ~CImage()
        {
            if (m_ownData)
                delete [] m_pData;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageBase
        //**********************************************************************
        virtual EImageType GetImageType() override { return m_imageType; }
        virtual uint8 *GetData() override { return m_pData; }
        virtual uint32 GetWidth() override { return m_width; }
        virtual uint32 GetHeight() override { return m_height; }
        virtual uint32 GetSubX() override { return m_subx; }
        virtual uint32 GetSubY() override { return m_suby; }
        virtual CRefObj<IImageBase> GetParent() { return CRefObj((IImageBase*)m_spParent); }
        virtual uint32 GetBPP() override { return m_bytesPerPixel * 8; }
        virtual uint32 GetStride() override { return m_width * GetBytesPerPixel(); }
        virtual uint32 GetBytesPerPixel() override { return m_bytesPerPixel; }
        virtual bool GetRGBOrder() override { return m_isRGB; }
        virtual void SetRGBOrder(bool isRGB) override { m_isRGB = isRGB; }

        //**********************************************************************
        // IImage
        //**********************************************************************
        virtual void Clear() override;
        virtual CRefObj<IImage> Clone() override;
        virtual void TakeDataOwnership() override {}
        virtual void SetPixel(uint32 x, uint32 y, uint8 color[4]) override;
        virtual void SetPixel(uint32 x, uint32 y, uint8 gray) override;
        virtual void SetPixel(uint32 x, uint32 y, uint16 v) override;
        virtual void DrawCircle(Vector2 &center, uint32 radius, uint8 color[4]) override;
        virtual void DrawLine(const Vector2 &v0, const Vector2 &v1, uint8 color[4]) override;
        virtual void DrawPath(IPath2* pPath) override;
    };

    class CIntegralImage : public IIntegralImage, public CRefCount
    {
        std::unique_ptr<uint32> m_spData;
        uint32 m_width;
        uint32 m_height;
        CRefObj<IImage> m_spImage;

        friend extern CRefObj<IIntegralImage> CreateIntegralImageImpl(IImage *pImage);
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
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageBase
        //**********************************************************************
        virtual EImageType GetImageType() override { return EImageType::Float4_128bpp; }
        virtual BYTE *GetData() override { return (BYTE*)m_spData.get(); }
        virtual uint32 GetStride() override { return m_width * sizeof(uint32) * 3; }
        virtual uint32 GetBytesPerPixel() override { return sizeof(uint32) * 3; }
        virtual uint32 GetWidth() override { return m_width; }
        virtual uint32 GetHeight() override { return m_height; }
        virtual uint32 GetSubX() override { return 0; }
        virtual uint32 GetSubY() override { return 0; }
        virtual CRefObj<IImageBase> GetParent() override { return nullptr; }
        virtual uint32 GetBPP() override { return 24; }
        virtual bool GetRGBOrder() override { return m_spImage->GetRGBOrder(); }
        virtual void SetRGBOrder(bool isRGB) override { /* do nothing */ }

        //**********************************************************************
        // IIntegralImage
        //**********************************************************************
        virtual uint32 GetSum(int channel, int x1, int y1, int x2, int y2) override;
        virtual CRefObj<IImage> BoxBlur(int width, int height) override;
    };

    CRefObj<IImage> CreateImageImpl(uint32 width, uint32 height, uint32 bpp);
    CRefObj<IIntegralImage> CreateIntegralImageImpl(IImage* pImage);
    CRefObj<IImage> LoadImageFileImpl(const wchar_t* pFilename);
    void StoreImageImpl(const wchar_t* pFilename, IImage* pImage);
}
