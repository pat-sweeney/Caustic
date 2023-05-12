//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
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
#include <memory>

export module Rendering.Caustic.VideoTexture;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IVideoTexture;

export namespace Caustic
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
    // Module:
    // {Link:import Rendering.Caustic.VideoTexture;{Rendering/Caustic/VideoTexture.ixx}}
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
    // Module:
    // {Link:import Rendering.Caustic.VideoTexture;{Rendering/Caustic/VideoTexture.ixx}}
    //**********************************************************************
    class CVideoTexture :
        public IVideoTexture,
        public CRefCount
    {
    protected:
        CComPtr<IMFSourceReader> m_spSourceReader;
        uint32 m_Width;
        uint32 m_Height;
        CVideoFormat m_format;
        CRefObj<ITexture> m_spTexture;
        bool m_eos;

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
        virtual void Update(IRenderer* pRenderer) override;
        virtual CComPtr<ID3D11Texture2D> GetD3DTexture() override { return m_spTexture->GetD3DTexture(); }
        virtual CComPtr<ID3D11ShaderResourceView> GetD3DTextureRV() { return m_spTexture->GetD3DTextureRV(); }
        virtual void GenerateMips(IRenderer* /*pRenderer*/) { _ASSERT(FALSE); return; }
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) override;
        virtual void CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap = false) override;
        virtual CRefObj<IImage> CopyToImage(IRenderer* pRenderer) override;
        virtual void CopyToImage(IRenderer* pRenderer, IImage* pImage) override;
        virtual void Copy(IRenderer* pRenderer, ITexture* pDst) override;

        //**********************************************************************
        // IVideoTexture
        //**********************************************************************
        virtual bool EndOfStream() override;
        virtual void Restart() override;
    };

    //**********************************************************************
    // Function: LoadVideoTexture
    // LoadVideoTexture loads a video and uses it as the texture source
    //
    // Parameters:
    // pFilename - Name of file to load
    // pRenderer - Renderer
    //
    // Returns:
    // Returns the new texture
    //**********************************************************************
    CRefObj<IVideoTexture> LoadVideoTexture(const wchar_t* pFilename, IRenderer* pRenderer)
    {
        std::unique_ptr<CVideoTexture> spTexture(new CVideoTexture(pRenderer));
        spTexture->LoadFromFile(pFilename, pRenderer);
        return CRefObj<IVideoTexture>(spTexture.release());
    }

    //**********************************************************************
    // Function: VideoTextureFromWebcam
    // VideoTextureFromWebcam uses a webcam to receive a video texture
    //
    // Parameters:
    // pRenderer - Renderer
    //
    // Returns:
    // Returns the new texture
    //**********************************************************************
    CRefObj<ITexture> VideoTextureFromWebcam(IRenderer* pRenderer)
    {
        std::unique_ptr<CVideoTexture> spTexture(new CVideoTexture(pRenderer));
        spTexture->CreateFromWebcam(pRenderer);
        return CRefObj<ITexture>(spTexture.release());
    }
};
