//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include <memory>
#include <Windows.h>
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Image.h"

namespace Caustic
{
    class CImage : public IImage, public CRefCount
    {
        std::unique_ptr<BYTE> m_spData;
        uint32 m_width;
        uint32 m_height;

        friend void LoadImage(const wchar_t *pFilename, IImage **ppImage);
        friend void StoreImage(const wchar_t *pFilename, IImage *pImage);
        friend void CreateImage(uint32 width, uint32 height, IImage **ppImage);
    public:
        CImage()
        {
        }
        
        CImage(int w, int h)
        {
            m_width = w;
            m_height = h;
            m_spData.reset(new BYTE[h * GetStride()]);
        }

        //**********************************************************************
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::AddRef(); }

        //**********************************************************************
        // IImage
        //**********************************************************************
        virtual BYTE *GetData() override { return m_spData.get(); }
        virtual uint32 GetWidth() override { return m_width; }
        virtual uint32 GetHeight() override { return m_height; }
        virtual uint32 GetBytesPerPixel() override { return 4; }
        virtual uint32 GetStride() override { return m_width * GetBytesPerPixel(); }
        virtual void GaussianBlur(float sigma, IImage **ppImage) override;
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
        virtual uint32 Release() override { return CRefCount::AddRef(); }

        //**********************************************************************
        // IImage
        //**********************************************************************
        virtual BYTE *GetData() override { return (BYTE*)m_spData.get(); }
        virtual uint32 GetStride() override { return m_width * sizeof(uint32) * 3; }
        virtual uint32 GetBytesPerPixel() override { return sizeof(uint32) * 3; }
        virtual uint32 GetWidth() override { return m_width; }
        virtual uint32 GetHeight() override { return m_height; }

        //**********************************************************************
        // IIntegralImage
        //**********************************************************************
        virtual uint32 GetSum(int channel, int x1, int y1, int x2, int y2) override;
        virtual void BoxBlur(int width, int height, IImage **ppImage) override;
    };
}
