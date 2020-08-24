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

namespace Caustic
{
    //**********************************************************************
    // Class: CTexture
    // Implementation of ITexture
    //
    // Header:
    // [Link:Rendering/Caustic/Texture.h]
    //**********************************************************************
    class CTexture :
        public ITexture,
        public CRefCount
    {
    protected:
        uint32 m_Width;
        uint32 m_Height;
        DXGI_FORMAT m_Format;
        CComPtr<ID3D11Texture2D> m_spTexture;
        CComPtr<ID3D11ShaderResourceView> m_spTextureRV;
    public:
        CTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags);
        CTexture(ID3D11Texture2D *pD3DTexture, ID3D11ShaderResourceView *pD3DRV);
        ~CTexture();

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
        virtual DXGI_FORMAT GetFormat() override;
        virtual void Update(IGraphics * /*pGraphics*/) override {}
        virtual CComPtr<ID3D11Texture2D> GetD3DTexture() override { return m_spTexture; }
        virtual CComPtr<ID3D11ShaderResourceView> GetD3DTextureRV() { return m_spTextureRV; }
        virtual void GenerateMips(IGraphics *pGraphics) override;
        virtual void Render(IGraphics* pGraphics, int slot, bool isPixelShader) override;
    };
};
