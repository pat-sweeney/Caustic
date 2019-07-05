//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Error.h"
#include "Imaging/Image/Image.h"
#include "Texture.h"
#include "TextureMgr.h"
#include "CausticFactory.h"
#include <memory>
#include <wincodec.h>
#include <objbase.h>
#include <map>
#include <d3dx12.h>
#include <tuple>

namespace Caustic
{
    CTextureMgr::CTextureMgr(IRenderer *pRenderer, uint32 numTexUploads, uint64 texMemSize)
    {
        AllocateHeaps(pRenderer, numTexUploads, texMemSize);
        //SetupDescriptors(pRenderer);
    }

    void CTextureMgr::AllocateHeaps(IRenderer *pRenderer, uint32 numTexUploads, uint64 texMemSize)
    {
        CComPtr<ID3D12Device5> spDevice = pRenderer->GetDevice();

        // Determine the footprint for our upload textures
        D3D12_RESOURCE_DESC texDesc;
        texDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        texDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        texDesc.Width = c_MaxTextureSize;
        texDesc.Height = c_MaxTextureSize;
        texDesc.DepthOrArraySize = 1;
        texDesc.MipLevels = 1;
        texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        texDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

        UINT64 textureUploadBufferSize;
        spDevice->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);

        // Create our texture upload heap
        D3D12_HEAP_DESC heapDesc = {};
        heapDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        heapDesc.Flags = D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
        heapDesc.Properties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
        heapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapDesc.Properties.CreationNodeMask = 1;
        heapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
        heapDesc.Properties.VisibleNodeMask = 1;
        heapDesc.SizeInBytes = textureUploadBufferSize * numTexUploads;
        CT(spDevice->CreateHeap(&heapDesc, __uuidof(ID3D12Heap), (void**)&m_spUploadTextureHeap));
        m_spUploadTextureHeap->SetName(L"m_spUploadTextureHeap");

        // Now create our upload textures
        for (uint32 i = 0; i < numTexUploads; i++)
        {
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
            desc.Alignment = 0;
            desc.Width = c_MaxTextureSize * c_MaxTextureSize; // We assume that the largest texture we will upload is 2048x2048
            desc.Height = 1;
            desc.DepthOrArraySize = 1;
            desc.MipLevels = 1;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
            CComPtr<ID3D12Resource> spUploadTex;
            UINT64 heapOffset = i * textureUploadBufferSize;
            CT(spDevice->CreatePlacedResource(m_spUploadTextureHeap, heapOffset, &desc,
                D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr, __uuidof(ID3D12Resource), (void**)&spUploadTex));
            m_uploadTextures.push_back(CUploadTexture(spUploadTex));
        }

        // Create our texture heap (used to hold textures used by shaders)
        heapDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        heapDesc.Flags = D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES;
        heapDesc.Properties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
        heapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapDesc.Properties.CreationNodeMask = 1;
        heapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
        heapDesc.Properties.VisibleNodeMask = 1;
        heapDesc.SizeInBytes = texMemSize;
        CT(spDevice->CreateHeap(&heapDesc, __uuidof(ID3D12Heap), (void**)&m_spTextureHeap));
        m_spTextureHeap->SetName(L"m_spTextureHeap");
    }
    
    UINT64 CTextureMgr::DetermineTextureSize(ID3D12Device *pDevice, int w, int h)
    {
        // Determine the footprint for our upload textures
        D3D12_RESOURCE_DESC texDesc;
        texDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        texDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        texDesc.Width = w;
        texDesc.Height = h;
        texDesc.DepthOrArraySize = 1;
        texDesc.MipLevels = 1;
        texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        texDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

        UINT64 textureUploadBufferSize;
        pDevice->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);
        return textureUploadBufferSize;
    }

#if 0
    void CTextureMgr::SetupDescriptors(IRenderer *pRenderer)
    {
        CComPtr<ID3D12Device5> spDevice = pRenderer->GetDevice();
        
        CDescriptors descriptor;
        descriptor.m_maxTextures = 20;
        descriptor.m_numTextures = 0;

        D3D12_DESCRIPTOR_HEAP_DESC srvDescriptorHeap = {};
        srvDescriptorHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        srvDescriptorHeap.NodeMask = 0;
        srvDescriptorHeap.NumDescriptors = descriptor.m_maxTextures;
        srvDescriptorHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        CT(spDevice->CreateDescriptorHeap(&srvDescriptorHeap, __uuidof(ID3D12DescriptorHeap), (void**)&descriptor.m_spSRVDescriptorHeap));
        descriptor.m_spSRVDescriptorHeap->SetName(L"m_spSRVDescriptorHeap");
        m_descriptors.push_back(descriptor);
    }

    void CTextureMgr::AllocateShaderResourceView(IRenderer *pRenderer, ID3D12Resource *pTexture)
    {
        CComPtr<ID3D12Device5> spDevice = pRenderer->GetDevice();

        CDescriptors &descriptor = m_descriptors[m_descriptors.size() - 1];
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        D3D12_CPU_DESCRIPTOR_HANDLE h = descriptor.m_spSRVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        h.ptr += descriptor.m_numTextures * spDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        spDevice->CreateShaderResourceView(pTexture, &srvDesc, h);
        descriptor.m_numTextures++;
    }
#endif

    void CTextureMgr::AllocateTexture(IRenderer *pRenderer, ITexture *pTexture, bool isRenderTarget, ID3D12Resource **ppD3DTexture)
    {
        CComPtr<ID3D12Device5> spDevice = pRenderer->GetDevice();
        UINT64 textureSize = DetermineTextureSize(spDevice, pTexture->GetWidth(), pTexture->GetHeight());
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        desc.Alignment = 0;
        desc.Width = pTexture->GetWidth();
        desc.Height = pTexture->GetHeight();
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
        desc.Flags = (isRenderTarget) ? D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
        CComPtr<ID3D12Resource> spTexResource;
        CT(spDevice->CreatePlacedResource(m_spTextureHeap, m_heapOffset, &desc,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr, __uuidof(ID3D12Resource), (void**)&spTexResource));

        // Check if upload texture is available. If not wait.

        // Add texture to our map
        CRefObj<CGPUTexture> spGPUTexture(new CGPUTexture());
        spGPUTexture->m_spTexture = spTexResource;
        spGPUTexture->m_spUploadTexture = m_uploadTextures[m_currentUpload].m_spUploadTexture;
        m_uploadTextures[m_currentUpload].m_inUse = true;
        m_currentUpload++;
        if (m_currentUpload == m_uploadTextures.size())
            m_currentUpload = 0;
        m_textures.insert(std::make_pair(pTexture, spGPUTexture));

        // Start upload
        D3D12_SUBRESOURCE_DATA textureData = {};
        CRefObj<IImage> spImage = pTexture->GetImageData();
        textureData.pData = spImage->GetData();
        textureData.RowPitch = spImage->GetStride();
        textureData.SlicePitch = textureData.RowPitch * spImage->GetHeight();
        CComPtr<ID3D12GraphicsCommandList4> spCmdList = pRenderer->GetCommandList();
        UpdateSubresources(spCmdList, spGPUTexture->m_spTexture, spGPUTexture->m_spUploadTexture, 0, 0, 1, &textureData);

        spCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(spGPUTexture->m_spTexture, D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

//        AllocateShaderResourceView(pRenderer, spGPUTexture->m_spTexture);

        *ppD3DTexture = spGPUTexture->m_spTexture;
        (*ppD3DTexture)->AddRef();
    }

    void CTextureMgr::Activate(IRenderer *pRenderer, ITexture *pTexture, ID3D12Resource **ppD3DTexture)
    {
        // Check if texture is already uploaded to the GPU
        auto it = m_textures.find(pTexture);
        if (it == m_textures.end())
            AllocateTexture(pRenderer, pTexture, false, ppD3DTexture);
        else
        {
            *ppD3DTexture = it->second->m_spTexture;
            (*ppD3DTexture)->AddRef();
        }
    }

    void CTextureMgr::Deactivate(ITexture *pTexture)
    {
    }

    void CTextureMgr::GetDescriptorHeap(ID3D12DescriptorHeap **ppDescriptorHeap)
    {
//        *ppDescriptorHeap = m_descriptors[m_descriptors.size() - 1].m_spSRVDescriptorHeap;
//        if (*ppDescriptorHeap)
//            (*ppDescriptorHeap)->AddRef();
    }
}
