//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include <d3d11.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>

namespace Caustic
{
    //**********************************************************************
    // Class: CVideoFormat
    // Defines the format for a video
    //
    // Members:
    // <uint32 at Caustic::uint32> m_width - width of video in pixels
    // <uint32 at Caustic::uint32> m_height - heightof video in pixels
    // bool m_topDown - is video oriented from top to bottom (pixel 0,0 in top left corner)
    // RECT m_rect - Video rect corrected for pixel aspect ratio
    //
    // Header:
    // [Link:Rendering/Caustic/VideoTexture.h]
    //**********************************************************************
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

    //**********************************************************************
    // Class: CVideoTexture
    // Defines class for a video as a texture
    //
    // Members:
    // CComPtr<IMFSourceReader> m_spSourceReader - Media Foundation's source reader
    // <uint32 at Caustic::uint32> m_Width - width of video in pixels
    // <uint32 at Caustic::uint32> m_Height - height of video in pixels
    // <CVideoFormat> m_format - format of the video
    // <CRefObj> < <ITexture> > m_spTexture - texture for decoded video frame
    //
    // Header:
    // [Link:Rendering/Caustic/VideoTexture.h]
    //**********************************************************************
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
        CVideoTexture(IRenderer* pRenderer);
        ~CVideoTexture();

        void CreateFromWebcam(IRenderer* pRenderer);
        void FromMediaSource(IMFSourceReader* pSourceReader, IRenderer* pRenderer);
        void LoadFromFile(const wchar_t *pFilename, IRenderer* pRenderer);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ITexture
        //**********************************************************************
        virtual uint32 GetWidth() override;
        virtual uint32 GetHeight() override;
        virtual DXGI_FORMAT GetFormat() { return DXGI_FORMAT_B8G8R8A8_UNORM; }
        virtual void Update(IRenderer *pRenderer) override;
        virtual CComPtr<ID3D11Texture2D> GetD3DTexture() override { return m_spTexture->GetD3DTexture(); }
        virtual CComPtr<ID3D11ShaderResourceView> GetD3DTextureRV() { return m_spTexture->GetD3DTextureRV(); }
        virtual void GenerateMips(IRenderer* /*pRenderer*/) { _ASSERT(FALSE); return; }
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) override;
        virtual void CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap = false) override;
    };
};
