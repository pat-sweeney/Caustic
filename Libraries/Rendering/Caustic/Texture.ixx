//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>

export module Rendering.Caustic.Texture;
import Base.Core.Core;
import Base.Core.RefCount;
import Rendering.Caustic.ITexture;

export namespace Caustic
{
    struct IRenderer;

    //**********************************************************************
    // Class: CTexture
    // Implementation of ITexture
    //
    // Module:
    // {Link:import Rendering.Caustic.Texture;{Rendering/Caustic/Texture.ixx}}
    //**********************************************************************
    class CTexture :
        public ITexture,
        public CRefCount
    {
    protected:
        uint32_t m_Width;
        uint32_t m_Height;
        DXGI_FORMAT m_Format;
        CComPtr<ID3D11Texture2D> m_spTexture;
        CComPtr<ID3D11ShaderResourceView> m_spTextureRV;
    public:
        CTexture(IRenderer* pRenderer, uint32_t width, uint32_t height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags);
        CTexture(ID3D11Texture2D *pD3DTexture, ID3D11ShaderResourceView *pD3DRV);
        ~CTexture();

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
        virtual DXGI_FORMAT GetFormat() override;
        virtual void Update(IRenderer * /*pRenderer*/) override {}
        virtual CComPtr<ID3D11Texture2D> GetD3DTexture() override { return m_spTexture; }
        virtual CComPtr<ID3D11ShaderResourceView> GetD3DTextureRV() { return m_spTextureRV; }
        virtual void GenerateMips(IRenderer* pRenderer) override;
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) override;
        virtual void CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap = false) override;
        virtual CRefObj<IImage> CopyToImage(IRenderer* pRenderer) override;
        virtual void CopyToImage(IRenderer* pRenderer, IImage *pImage) override;
        virtual void Copy(IRenderer* pRenderer, ITexture* pDst) override;
    };
};
