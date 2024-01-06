//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>

export module Rendering.Caustic.Sampler;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.ISampler;

export namespace Caustic
{
    //**********************************************************************
    // Struct: CSamplerRef
    // CSamplerRef only exists because I was having trouble
    // adding CRefObj<ISampler> to std::any(). This seemed like
    // the simpliest fix
    //
    // Module:
    // {Link:import Rendering.Caustic.Sampler;{Rendering/Caustic/Sampler.ixx}}
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
    // Module:
    // {Link:import Rendering.Caustic.Sampler;{Rendering/Caustic/Sampler.ixx}}
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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISampler
        //**********************************************************************
        virtual D3D11_FILTER GetFilter() override { return m_Filter; }
        virtual void SetFilter(IRenderer* pRenderer, D3D11_FILTER filter) override;
        virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressU() override { return m_AddressU; }
        virtual void SetAddressU(IRenderer* pRenderer, D3D11_TEXTURE_ADDRESS_MODE address) override;
        virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressV() override { return m_AddressV; }
        virtual void SetAddressV(IRenderer* pRenderer, D3D11_TEXTURE_ADDRESS_MODE address) override;
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) override;
        virtual CRefObj<ITexture> GetTexture() override;
    };
};
