//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Sampler.h"
#include "CausticFactory.h"

namespace Caustic
{
    CSampler::CSampler(IGraphics *pGraphics, ITexture *pTexture) :
        m_Filter(D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR),
        m_AddressU(D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP),
        m_AddressV(D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP)
    {
        CD3D11_SAMPLER_DESC desc(D3D11_DEFAULT);
        desc.AddressU = m_AddressU;
        desc.AddressV = m_AddressV;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.Filter = m_Filter;
        CT(pGraphics->GetDevice()->CreateSamplerState(&desc, &m_spSamplerState));
        m_spTexture = pTexture;
    }

    
    CAUSTICAPI void CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler)
    {
        std::unique_ptr<CSampler> spSampler(new CSampler(pGraphics, pTexture));
        *ppSampler = spSampler.release();
        (*ppSampler)->AddRef();
    }

    void CSampler::Render(IGraphics *pGraphics, int slot, bool isPixelShader)
    {
        auto ctx = pGraphics->GetContext();
        if (isPixelShader)
            ctx->PSSetSamplers(slot, 1, &m_spSamplerState.p);
        else
            ctx->VSSetSamplers(slot, 1, &m_spSamplerState.p);
    }

    void CSampler::GetTexture(ITexture **ppTexture)
    {
        *ppTexture = m_spTexture;
        if (m_spTexture)
            (*ppTexture)->AddRef();
    }
};
