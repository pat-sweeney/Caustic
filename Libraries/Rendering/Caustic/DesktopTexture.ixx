//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>
#include <atlbase.h>

export module Rendering.Caustic.DesktopTexture;
import Base.Core.Core;
import Base.Core.RefCount;
import Rendering.Caustic.ITexture;

export namespace Caustic
{
    //**********************************************************************
    // Class: CVideoFormat
    // Defines the format for a video
    //
    // Members:
    // <uint32_t at Caustic::uint32_t> m_width - width of video in pixels
    // <uint32_t at Caustic::uint32_t> m_height - heightof video in pixels
    // bool m_topDown - is video oriented from top to bottom (pixel 0,0 in top left corner)
    // RECT m_rect - Video rect corrected for pixel aspect ratio
    //
    // Module:
    // {Link:import Rendering.Caustic.DesktopTexture;{Rendering/Caustic/DesktopTexture.ixx}}
    //**********************************************************************
    struct CVideoFormat
    {
        uint32_t m_width;
        uint32_t m_height;
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
    // Module:
    // {Link:import Rendering.Caustic.DesktopTexture;{Rendering/Caustic/DesktopTexture.ixx}}
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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ITexture
        //**********************************************************************
        virtual uint32_t GetWidth() override;
        virtual uint32_t GetHeight() override;
        virtual DXGI_FORMAT GetFormat() { return DXGI_FORMAT_B8G8R8A8_UNORM; }
        virtual void Update(IRenderer* pRenderer) override;
        virtual CComPtr<ID3D11Texture2D> GetD3DTexture() override { return m_spDesktopCopy->GetD3DTexture(); }
        virtual CComPtr<ID3D11ShaderResourceView> GetD3DTextureRV() { return m_spDesktopCopy->GetD3DTextureRV(); }
        virtual void GenerateMips(IRenderer* /*pRenderer*/) { _ASSERT(FALSE); return; }
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) override;
        virtual void CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap = false) override;
        virtual CRefObj<IImage> CopyToImage(IRenderer* pRenderer) override;
        virtual void CopyToImage(IRenderer* pRenderer, IImage* pImage) override;
        virtual void Copy(IRenderer* pRenderer, ITexture* pTex) override;
    };

    //**********************************************************************
    // Function: CreateDesktopTexture
    // CreateDesktopTexture creates a texture containing a copy of the windows desktop
    //
    // Parameters:
    // pRenderer - Renderer
    //
    // Returns:
    // Returns the new texture
    //
    // Module:
    // {Link:import Rendering.Caustic.ITexture;{Rendering/Caustic/ITexture.ixx}}
    //**********************************************************************
    CRefObj<ITexture> CreateDesktopTexture(IRenderer* pRenderer);
};
