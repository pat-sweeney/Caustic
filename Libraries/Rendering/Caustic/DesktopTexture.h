//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
import Base.Core.Core;
import Base.Core.RefCount;
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
    // Class: CDesktopTexture
    // Defines class for a texture that contains a copy of the windows desktop
    //
    // Members:
    // <CRefObj> < <ITexture> > m_spTexture - texture for decoded video frame
    //
    // Header:
    // [Link:Rendering/Caustic/VideoTexture.h]
    //**********************************************************************
    class CDesktopTexture :
        public ITexture,
        public CRefCount
    {
    protected:
        CComPtr<ID3D11ShaderResourceView> m_spTextureRV;
        D3D11_TEXTURE2D_DESC m_desc;
        CRefObj<ITexture> m_spDesktopCopy;

    public:
        CDesktopTexture(IRenderer* pRenderer);
        ~CDesktopTexture();

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
        virtual void Update(IRenderer* pRenderer) override;
        virtual CComPtr<ID3D11Texture2D> GetD3DTexture() override { return m_spDesktopCopy->GetD3DTexture(); }
        virtual CComPtr<ID3D11ShaderResourceView> GetD3DTextureRV() { return m_spDesktopCopy->GetD3DTextureRV(); }
        virtual void GenerateMips(IRenderer* /*pRenderer*/) { _ASSERT(FALSE); return; }
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) override;
        virtual void CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap = false) override;
        virtual CRefObj<IImage> CopyToImage(IRenderer* pRenderer) override;
        virtual void CopyToImage(IRenderer* pRenderer, IImage* pImage) override;
    };
};
