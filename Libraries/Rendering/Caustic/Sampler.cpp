//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include "Rendering\Caustic\ISampler.h"

module Rendering.Caustic.Sampler;
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CSampler::CSampler(IRenderer *pRenderer, ITexture *pTexture) :
        m_Filter(D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR),
        m_AddressU(D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP),
        m_AddressV(D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP)
    {
        CD3D11_SAMPLER_DESC desc(D3D11_DEFAULT);
        desc.AddressU = m_AddressU;
        desc.AddressV = m_AddressV;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.Filter = m_Filter;
        desc.BorderColor[0] = 0.0f;
        desc.BorderColor[1] = 0.0f;
        desc.BorderColor[2] = 0.0f;
        desc.BorderColor[3] = 0.0f;
        CT(pRenderer->GetDevice()->CreateSamplerState(&desc, &m_spSamplerState));
        m_spTexture = pTexture;
    }

    void CSampler::SetFilter(IRenderer* pRenderer, D3D11_FILTER filter)
    {
        m_Filter = filter;
        D3D11_SAMPLER_DESC desc;
        m_spSamplerState->GetDesc(&desc);
        desc.Filter = filter;
        m_spSamplerState = nullptr;
        CT(pRenderer->GetDevice()->CreateSamplerState(&desc, &m_spSamplerState));
    }

    void CSampler::SetAddressU(IRenderer* pRenderer, D3D11_TEXTURE_ADDRESS_MODE address)
    {
        m_AddressU = address;
        D3D11_SAMPLER_DESC desc;
        m_spSamplerState->GetDesc(&desc);
        desc.AddressU = address;
        m_spSamplerState = nullptr;
        CT(pRenderer->GetDevice()->CreateSamplerState(&desc, &m_spSamplerState));
    }

    void CSampler::SetAddressV(IRenderer* pRenderer, D3D11_TEXTURE_ADDRESS_MODE address)
    {
        m_AddressV = address;
        D3D11_SAMPLER_DESC desc;
        m_spSamplerState->GetDesc(&desc);
        desc.AddressV = address;
        m_spSamplerState = nullptr;
        CT(pRenderer->GetDevice()->CreateSamplerState(&desc, &m_spSamplerState));
    }

    CRefObj<ISampler> CreateSampler(IRenderer *pRenderer, ITexture *pTexture)
    {
        return CRefObj<ISampler>(new CSampler(pRenderer, pTexture));
    }

    void CSampler::Render(IRenderer *pRenderer, int slot, bool isPixelShader)
    {
        auto ctx = pRenderer->GetContext();
        if (isPixelShader)
            ctx->PSSetSamplers(slot, 1, &m_spSamplerState.p);
        else
            ctx->VSSetSamplers(slot, 1, &m_spSamplerState.p);
    }

    CRefObj<ITexture> CSampler::GetTexture()
    {
        return m_spTexture;
    }
};
