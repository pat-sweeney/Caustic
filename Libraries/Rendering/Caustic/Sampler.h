//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\error.h"
#include <d3d12.h>

namespace Caustic
{
    // CSamplerRef only exists because I was having trouble
    // adding CRefObj<ISampler> to std::any(). This seemed like
    // the simpliest fix
    struct CSamplerRef
    {
        CRefObj<ISampler> m_spSampler;

        CSamplerRef(ISampler *pSampler)
        {
            m_spSampler = pSampler;
        }
    };

    //**********************************************************************
    class CSampler : public ISampler, public CRefCount
    {
        D3D12_FILTER m_Filter;
        D3D12_TEXTURE_ADDRESS_MODE m_AddressU;
        D3D12_TEXTURE_ADDRESS_MODE m_AddressV;
        CRefObj<ITexture> m_spTexture;
		CComPtr<ID3D12PipelineState> m_spPipelineState;
		D3D12_CPU_DESCRIPTOR_HANDLE m_hSampler;
		CComPtr<ID3D12DescriptorHeap> m_spDescriptorHeap;
		//CComPtr<ID3D11SamplerState> m_spSamplerState;
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
        virtual D3D12_FILTER GetFilter() override { return m_Filter; }
        virtual void SetFilter(D3D12_FILTER filter) override { m_Filter = filter; }
        virtual D3D12_TEXTURE_ADDRESS_MODE GetAddressU() override { return m_AddressU; }
        virtual void SetAddressU(D3D12_TEXTURE_ADDRESS_MODE address) override { m_AddressU = address; }
        virtual D3D12_TEXTURE_ADDRESS_MODE GetAddressV() override { return m_AddressV; }
        virtual void SetAddressV(D3D12_TEXTURE_ADDRESS_MODE address) override { m_AddressV = address; }
        virtual void Render(IRenderer *pRenderer, int slot) override;
        virtual void GetTexture(ITexture **ppTexture) override;
    };
};
