//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Sampler.h"
#include "CausticFactory.h"
#include <d3dx12.h>

namespace Caustic
{
	CSampler::CSampler(IRenderer *pRenderer, ITexture *pTexture) :
        m_Filter(D3D12_FILTER::D3D12_FILTER_ANISOTROPIC),
        m_AddressU(D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP),
        m_AddressV(D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP)
    {
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		CT(pRenderer->GetDevice()->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_spDescriptorHeap));
		m_hSampler = m_spDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		
		D3D12_SAMPLER_DESC desc = {};
		desc.AddressU = m_AddressU;
		desc.AddressV = m_AddressV;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.Filter = m_Filter;
		pRenderer->GetDevice()->CreateSampler(&desc, m_hSampler);
        m_spTexture = pTexture;
    }

    
    void CSampler::Render(IRenderer *pRenderer, int slot)
    {
#if 0
		pRenderer->GetContext()->PSSetSamplers(slot, 1, &m_spSamplerState.p);
        if (m_spTexture != nullptr)
        {
            CComPtr<ID3D11ShaderResourceView> spResource = m_spTexture->GetD3DTextureRV();
			pRenderer->GetContext()->PSSetShaderResources(slot, 1, &spResource.p);
        }
        else
        {
            // No valid texture?! Checkerboard it
            CRefObj<ITexture> spTexture = CCausticFactory::Instance()->CheckerboardTexture(pGraphics);
            CComPtr<ID3D11ShaderResourceView> spResource = spTexture->GetD3DTextureRV();
			pRenderer->GetContext()->PSSetShaderResources(slot, 1, &spResource.p);
        }
#endif
    }

    void CSampler::GetTexture(ITexture **ppTexture)
    {
        *ppTexture = m_spTexture.p;
        if (m_spTexture.p)
            (*ppTexture)->AddRef();
    }
};
