//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <Windows.h>
#include <cinttypes>

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
    // <uint32_t at Caustic::uint32_t> m_subx - X offset into parent image
    // <uint32_t at Caustic::uint32_t> m_suby - Y offset into parent image
    // <uint8 at Caustic::uint8>* m_pData - pointer to image data
    // bool m_ownData - Did this object allocate m_pData?
    // <uint32_t at Caustic::uint32_t> m_width - width of image in pixels
    // <uint32_t at Caustic::uint32_t> m_height - height of image in pixels
    // <uint32_t at Caustic::uint32_t> m_bytesPerPixel - width of each pixel in bytes
    // bool m_isRGB - Order pixel data is layed out in memory
    //
    // Header:
    // {Link:#include "Imaging/Image/ImageImpl.h"{Imaging/Image/ImageImpl.h}}
    //**********************************************************************
    class CImage : public IImage, public CRefCount
    {
        EImageType m_imageType;         // Type of data stored in the image
        CRefObj<CImage> m_spParent;     // parent image if image is a subimage
        int32_t m_subx;                  // X offset into parent image
        int32_t m_suby;                  // Y offset into parent image
        uint8_t *m_pData;                 // pointer to image data
        bool m_ownData;                 // Did this object allocate m_pData?
        uint32_t m_width;                 // width of image in pixels
        uint32_t m_height;                // height of image in pixels
        uint32_t m_bitsPerPixel;          // width of each pixel in bits
        uint32_t m_stride;                // Number of bytes in a row of pixels

        friend class CImageIter;
        friend CRefObj<IImage> LoadImageFileImpl(const wchar_t *pFilename);
        friend void StoreImageImpl(const wchar_t *pFilename, IImage *pImage);
    public:
        CImage() :
            m_imageType(EImageType::Unknown),
            m_width(0),
            m_height(0),
            m_bitsPerPixel(0),
            m_stride(0),
            m_spParent(nullptr),
            m_subx(0),
            m_suby(0),
            m_pData(nullptr),
            m_ownData(true)
        {
        }
        
        CImage(int w, int h, EImageType imageType)
        {
            m_width = w;
            m_height = h;
            m_spParent = nullptr;
            m_subx = 0;
            m_suby = 0;

            int numBytes = 0;
            if (imageType == EImageType::NV12_420)
            {
                m_bitsPerPixel = 0; // Mostly meaningless for YUV (although technically it's 12b)
                m_stride = m_width;
                numBytes = m_width * m_height + m_width * m_height / 2;
            }
            else
            {
                switch (imageType)
                {
                case EImageType::BW_1bpp: m_bitsPerPixel = 1; break;
                case EImageType::Gray_8bpp: m_bitsPerPixel = 8; break;
                case EImageType::Gray_16bpp: m_bitsPerPixel = 16; break;
                case EImageType::RGB_24bpp: m_bitsPerPixel = 24; break;
                case EImageType::RGBA_32bpp: m_bitsPerPixel = 32; break;
                case EImageType::RGBX_32bpp: m_bitsPerPixel = 32; break;
                case EImageType::BGR_24bpp: m_bitsPerPixel = 24; break;
                case EImageType::BGRA_32bpp: m_bitsPerPixel = 32; break;
                case EImageType::BGRX_32bpp: m_bitsPerPixel = 32; break;
                }
                m_stride = (m_width * m_bitsPerPixel + 7) / 8;
                numBytes = m_stride * m_height;
            }
            m_pData = new BYTE[numBytes];
            m_ownData = true;
            m_imageType = imageType;
        }

        virtual ~CImage()
        {
            if (m_ownData)
                delete [] m_pData;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageBase
        //**********************************************************************
        virtual EImageType GetImageType() override { return m_imageType; }
        virtual uint8_t*GetData() override { return m_pData; }
        virtual uint32_t GetWidth() override { return m_width; }
        virtual uint32_t GetHeight() override { return m_height; }
        virtual int32_t GetSubX() override { return m_subx; }
        virtual int32_t GetSubY() override { return m_suby; }
        virtual CRefObj<IImageBase> GetParent() { return CRefObj((IImageBase*)m_spParent); }
        virtual uint32_t GetBPP() override { return m_bitsPerPixel; }
        virtual uint32_t GetStride() override { return m_stride; }

        //**********************************************************************
        // IImage
        //**********************************************************************
        virtual void Clear() override;
        virtual CRefObj<IImage> Clone() override;
        virtual void TakeDataOwnership() override {}
        virtual void SetPixel(int32_t x, int32_t y, uint8_t color[4]) override;
        virtual void SetPixel(int32_t x, int32_t y, uint8_t gray) override;
        virtual void SetPixel(int32_t x, int32_t y, uint16_t v) override;
        virtual void DrawCircle(Vector2 &center, uint32_t radius, uint8_t color[4]) override;
        virtual void DrawLine(const Vector2 &v0, const Vector2 &v1, uint8_t color[4]) override;
        virtual void DrawPath(IPath2* pPath) override;
    };

    class CIntegralImage : public IIntegralImage, public CRefCount
    {
        std::unique_ptr<uint32_t> m_spData;
        uint32_t m_width;
        uint32_t m_height;
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
            m_spData.reset(new uint32_t[w * h * 3]);
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageBase
        //**********************************************************************
        virtual EImageType GetImageType() override { return EImageType::Float4_128bpp; }
        virtual BYTE *GetData() override { return (BYTE*)m_spData.get(); }
        virtual uint32_t GetStride() override { return m_width * sizeof(uint32_t) * 3; }
        virtual uint32_t GetWidth() override { return m_width; }
        virtual uint32_t GetHeight() override { return m_height; }
        virtual int32_t GetSubX() override { return 0; }
        virtual int32_t GetSubY() override { return 0; }
        virtual CRefObj<IImageBase> GetParent() override { return nullptr; }
        virtual uint32_t GetBPP() override { return sizeof(uint32_t) * 3 * 8; }

        //**********************************************************************
        // IIntegralImage
        //**********************************************************************
        virtual uint32_t GetSum(int channel, int x1, int y1, int x2, int y2) override;
        virtual CRefObj<IImage> BoxBlur(int width, int height) override;
    };

    CRefObj<IImage> CreateImageImpl(uint32_t width, uint32_t height, EImageType imageType);
    CRefObj<IIntegralImage> CreateIntegralImageImpl(IImage* pImage);
    CRefObj<IImage> LoadImageFileImpl(const wchar_t* pFilename);
    void StoreImageImpl(const wchar_t* pFilename, IImage* pImage);
}
