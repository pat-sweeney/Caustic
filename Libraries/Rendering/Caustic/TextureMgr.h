//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\ITextureMgr.h"
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include <d3d12.h>

namespace Caustic
{
    // There will be a single CGPUTexture for each ITexture that has
    // been uploaded to the GPU.
    class CGPUTexture : public CRefCount
    {
        CComPtr<ID3D12Resource> m_spUploadTexture;
        CComPtr<ID3D12Resource> m_spTexture;

        friend class CTextureMgr;
    public:
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }
    };

    struct CUploadTexture
    {
        CComPtr<ID3D12Resource> m_spUploadTexture;
        bool m_inUse;

        CUploadTexture() {}

        CUploadTexture(ID3D12Resource *pTexture)
        {
            m_spUploadTexture = pTexture;
            m_inUse = false;
        }
    };

    class CDescriptors
    {
        CComPtr<ID3D12DescriptorHeap> m_spSRVDescriptorHeap;
        uint32 m_numTextures; // Number of textures under the under managers control
        uint32 m_maxTextures; // Number number of textures per descriptor heap
        
        friend class CTextureMgr;
    public:
    };

    class CTextureMgr :
        public ITextureMgr,
        public CRefCount
    {
    protected:
        std::map<ITexture*, CRefObj<CGPUTexture>> m_textures;
        CComPtr<ID3D12Heap> m_spUploadTextureHeap;
        CComPtr<ID3D12Heap> m_spTextureHeap;
        std::vector<CUploadTexture> m_uploadTextures;
        UINT64 m_heapOffset;
        uint32 m_currentUpload;
#if 0
        std::vector<CDescriptors> m_descriptors;
#endif
        
        UINT64 DetermineTextureSize(ID3D12Device *pDevice, int w, int h);
        void AllocateHeaps(IRenderer *pRenderer, uint32 numTexUploads, uint64 texMemSize);
        void AllocateTexture(IRenderer *pRenderer, ITexture *pTexture, bool isRenderTarget, ID3D12Resource **ppD3DTexture);
#if 0
        void SetupDescriptors(IRenderer *pRenderer);
        void AllocateShaderResourceView(IRenderer *pRenderer, ID3D12Resource *pTexture);
#endif
    public:
        CTextureMgr(IRenderer *pRenderer, uint32 numTexUploads, uint64 texMemSize);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ITextureMgr
        //**********************************************************************
        virtual void Activate(IRenderer *pRenderer, ITexture *pTexture, ID3D12Resource **ppD3DTexture) override;
        virtual void Deactivate(ITexture *pTexture) override;
        virtual void GetDescriptorHeap(ID3D12DescriptorHeap **ppDescriptorHeap) override;
    };
}