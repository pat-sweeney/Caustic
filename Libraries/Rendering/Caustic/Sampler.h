//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\error.h"
#include <d3d11.h>

namespace Caustic
{
    //**********************************************************************
    // Struct: CSamplerRef
    // CSamplerRef only exists because I was having trouble
    // adding CRefObj<ISampler> to std::any(). This seemed like
    // the simpliest fix
    //
    // Header:
    // [Link:Rendering/Caustic/Sampler.h]
    //**********************************************************************
    struct CSamplerRef
    {
        CRefObj<ISampler> m_spSampler;

        CSamplerRef(ISampler *pSampler)
        {
            m_spSampler = pSampler;
        }
    };

    //**********************************************************************
    // Class: CSampler
    // Implementation of <ISampler>
    //
    // Header:
    // [Link:Rendering/Caustic/Sampler.h]
    //**********************************************************************
    class CSampler : public ISampler, public CRefCount
    {
        D3D11_FILTER m_Filter;
        D3D11_TEXTURE_ADDRESS_MODE m_AddressU;
        D3D11_TEXTURE_ADDRESS_MODE m_AddressV;
        CRefObj<ITexture> m_spTexture;
        CComPtr<ID3D11SamplerState> m_spSamplerState;
    public:
        CSampler(IRenderer *pRenderer, ITexture *pTexture);
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISampler
        //**********************************************************************
        virtual D3D11_FILTER GetFilter() override { return m_Filter; }
        virtual void SetFilter(D3D11_FILTER filter) override { m_Filter = filter; }
        virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressU() override { return m_AddressU; }
        virtual void SetAddressU(D3D11_TEXTURE_ADDRESS_MODE address) override { m_AddressU = address; }
        virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressV() override { return m_AddressV; }
        virtual void SetAddressV(D3D11_TEXTURE_ADDRESS_MODE address) override { m_AddressV = address; }
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) override;
        virtual CRefObj<ITexture> GetTexture() override;
    };
};
