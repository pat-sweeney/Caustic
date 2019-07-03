//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Imaging/Image/Image.h"
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include <d3d12.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>

namespace Caustic
{
    struct CVideoFormat
    {
        uint32 m_width;
        uint32 m_height;
        bool m_topDown;
        RECT m_rect;    // Corrected for pixel aspect ratio

        CVideoFormat() :
            m_width(0),
            m_height(0),
            m_topDown(false)
        {
            SetRectEmpty(&m_rect);
        }
    };

    class CVideoTexture :
        public ITexture,
        public CRefCount
    {
    protected:
        CComPtr<IMFSourceReader> m_spSourceReader;
        uint32 m_Width;
        uint32 m_Height;
        CVideoFormat m_format;
        CRefObj<ITexture> m_spTexture;

        void GetVideoFormat(CVideoFormat *pFormat);
    public:
        CVideoTexture(IRenderer *pRenderer);
        ~CVideoTexture();

        void LoadFromFile(const wchar_t *pFilename, IRenderer *pRenderer);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ITexture
        //**********************************************************************
        virtual CRefObj<IImage> GetImageData() override { return CRefObj((IImage*)nullptr); }
        virtual uint32 GetWidth() override;
        virtual uint32 GetHeight() override;
        virtual DXGI_FORMAT GetFormat() { return DXGI_FORMAT_B8G8R8A8_UNORM; }
        virtual void Update(IRenderer *pRenderer) override;
        virtual CComPtr<ID3D12Resource> GetD3DTexture() override { return m_spTexture->GetD3DTexture(); }
        virtual void GenerateMips(IRenderer * /*pRenderer*/) { _ASSERT(FALSE); return; }
    };
};
