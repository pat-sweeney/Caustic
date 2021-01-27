//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "GPUPipeline.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"
#include "Cameras\CameraBase\ICamera.h"
#include <vector>
#include <atlbase.h>
#include <d3d11.h>

namespace Caustic
{
    //**********************************************************************
    // Class: CGPUPipelineNodeBase
    // Define the base class for nodes in our pipeline
    //**********************************************************************
    class CGPUPipelineNodeBase : public CRefCount, public IGPUPipelineNode
    {
    protected:
#ifdef DEBUG
        std::wstring m_name;
#endif
        bool m_enabled;
        std::map<std::wstring, std::pair<IGPUPipelineNode*, std::wstring>> m_sourceNodes; // Weak reference to IGPUPipelineNode
        uint32 m_width; // Width of output generated by this node
        uint32 m_height; // Height of output generated by this node
        CRefObj<ITexture> m_spOutputTexture;
        CRefObj<IShader> m_spShader;
        D3D11_CPU_ACCESS_FLAG m_cpuFlags;
        D3D11_BIND_FLAG m_bindFlags;
        DXGI_FORMAT m_outputFormat;
    public:
        CGPUPipelineNodeBase(uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT outputFormat = DXGI_FORMAT_R8G8B8A8_UNORM) : m_enabled(true)
        {
            // Create our output texture and assign it as our render target
            m_width = outputWidth;
            m_height = outputHeight;
            m_cpuFlags = (D3D11_CPU_ACCESS_FLAG)0;
            m_bindFlags = (D3D11_BIND_FLAG)0;
            m_outputFormat = outputFormat;
        }
        
        uint32 GetNumberInputs();

        //**********************************************************************
        // IGPUPipelineNode
        //**********************************************************************
        virtual void SetName(const wchar_t* pName) override
        {
#ifdef DEBUG
            m_name = pName;
#endif
        }
        virtual bool IsEnabled() override { return m_enabled; }
        virtual void Enable() override { m_enabled = true; }
        virtual void Disable() override { m_enabled = false; }
        virtual void SetShader(IShader* pShader) override;
        virtual CRefObj<IShader> GetShader() override;
        virtual CRefObj<IGPUPipelineNode> GetInput(const wchar_t *pName) override;
        virtual void SetInput(const wchar_t* pName, const wchar_t* pSamplerName, IGPUPipelineNode*pNode) override;
        virtual void SetOutputSize(uint32 width, uint32 height) override { m_width = width; m_height = height; }
        virtual uint32 GetOutputWidth() override { return m_width; }
        virtual uint32 GetOutputHeight() override { return m_height; }
        virtual CRefObj<ITexture> GetOutputTexture(IGPUPipeline* pPipeline) override;
        virtual void Process(IGPUPipeline* pPipeline) override;
    };

    //**********************************************************************
    // Class: CGPUPipelineNode
    // Define class that implement IGPUPipelineNode
    //**********************************************************************
    class CGPUPipelineNode : public CGPUPipelineNodeBase
    {
    public:
        CGPUPipelineNode(uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) : CGPUPipelineNodeBase(outputWidth, outputHeight, format)
        {
            m_cpuFlags = (D3D11_CPU_ACCESS_FLAG)0;
            m_bindFlags = (D3D11_BIND_FLAG)(D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        }
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGPUPipelineNode
        //**********************************************************************
        virtual void SetName(const wchar_t* pName) override { CGPUPipelineNodeBase::SetName(pName); }
        virtual bool IsEnabled() override { return CGPUPipelineNodeBase::IsEnabled(); }
        virtual void Enable() override { CGPUPipelineNodeBase::Enable(); }
        virtual void Disable() override { CGPUPipelineNodeBase::Disable(); }
        virtual void SetShader(IShader *pShader) override { CGPUPipelineNodeBase::SetShader(pShader); }
        virtual CRefObj<IShader> GetShader() override { return CGPUPipelineNodeBase::GetShader(); }
        virtual CRefObj<IGPUPipelineNode> GetInput(const wchar_t *pName) override { return CGPUPipelineNodeBase::GetInput(pName); }
        virtual void SetInput(const wchar_t *pName, const wchar_t *pSamplerName, IGPUPipelineNode *pNode) override { CGPUPipelineNodeBase::SetInput(pName, pSamplerName, pNode); }
        virtual void SetOutputSize(uint32 width, uint32 height) override { CGPUPipelineNodeBase::SetOutputSize(width, height); }
        virtual uint32 GetOutputWidth() override { return CGPUPipelineNodeBase::GetOutputWidth(); }
        virtual uint32 GetOutputHeight() override { return CGPUPipelineNodeBase::GetOutputHeight(); }
        virtual CRefObj<ITexture> GetOutputTexture(IGPUPipeline *pPipeline) override { return CGPUPipelineNodeBase::GetOutputTexture(pPipeline); }
        virtual void Process(IGPUPipeline *pPipeline) override { CGPUPipelineNodeBase::Process(pPipeline); }
    };

    //**********************************************************************
    // Class: CGPUPipelineDepthMeshNode
    // Creates a mesh used for rendering a depth map as a mesh
    //**********************************************************************
    class CGPUPipelineDepthMeshNode : public CGPUPipelineNodeBase
    {
        CRefObj<IRenderSubMesh> m_spMesh;
    public:
        CGPUPipelineDepthMeshNode(IRenderer* pRenderer, IShader *pShader, uint32 depthInputWidth, uint32 depthInputHeight,
            uint32 outputColorWidth, uint32 outputColorHeight, CRefObj<ITexture> spRayTex, Matrix4x4& extrinsics, CameraIntrinsics& intrinsics,
            float minDepth, float maxDepth, DXGI_FORMAT format = DXGI_FORMAT_R16_UINT);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGPUPipelineNode
        //**********************************************************************
        virtual void SetName(const wchar_t* pName) override { CGPUPipelineNodeBase::SetName(pName); }
        virtual bool IsEnabled() override { return CGPUPipelineNodeBase::IsEnabled(); }
        virtual void Enable() override { CGPUPipelineNodeBase::Enable(); }
        virtual void Disable() override { CGPUPipelineNodeBase::Disable(); }
        virtual void SetShader(IShader* pShader) override { CGPUPipelineNodeBase::SetShader(pShader); }
        virtual CRefObj<IShader> GetShader() override { return CGPUPipelineNodeBase::GetShader(); }
        virtual CRefObj<IGPUPipelineNode> GetInput(const wchar_t* pName) override { return CGPUPipelineNodeBase::GetInput(pName); }
        virtual void SetInput(const wchar_t* pName, const wchar_t* pSamplerName, IGPUPipelineNode* pNode) override { CGPUPipelineNodeBase::SetInput(pName, pSamplerName, pNode); }
        virtual void SetOutputSize(uint32 width, uint32 height) override { CGPUPipelineNodeBase::SetOutputSize(width, height); }
        virtual uint32 GetOutputWidth() override { return CGPUPipelineNodeBase::GetOutputWidth(); }
        virtual uint32 GetOutputHeight() override { return CGPUPipelineNodeBase::GetOutputHeight(); }
        virtual CRefObj<ITexture> GetOutputTexture(IGPUPipeline* pPipeline) override { return CGPUPipelineNodeBase::GetOutputTexture(pPipeline); }
        virtual void Process(IGPUPipeline* pPipeline) override;
    };

    //**********************************************************************
    class CGPUPipelineSourceNode : public CGPUPipelineNodeBase, public IGPUPipelineSourceNode
    {
        CComPtr<ID3D11Texture2D> m_spSourceImage;
    public:
        CGPUPipelineSourceNode(uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) : CGPUPipelineNodeBase(outputWidth, outputHeight, format)
        {
            m_cpuFlags = D3D11_CPU_ACCESS_WRITE;
            m_bindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGPUPipelineNode
        //**********************************************************************
        virtual void SetName(const wchar_t* pName) override { CGPUPipelineNodeBase::SetName(pName); }
        virtual bool IsEnabled() override { return CGPUPipelineNodeBase::IsEnabled(); }
        virtual void Enable() override { CGPUPipelineNodeBase::Enable(); }
        virtual void Disable() override { CGPUPipelineNodeBase::Disable(); }
        virtual void SetShader(IShader *pShader) override { CGPUPipelineNodeBase::SetShader(pShader); }
        virtual CRefObj<IShader> GetShader() override { return CGPUPipelineNodeBase::GetShader(); }
        virtual CRefObj<IGPUPipelineNode> GetInput(const wchar_t *pName) override { return CGPUPipelineNodeBase::GetInput(pName); }
        virtual void SetInput(const wchar_t *pName, const wchar_t* pSamplerName, IGPUPipelineNode *pNode) override { CGPUPipelineNodeBase::SetInput(pName, pSamplerName, pNode); }
        virtual void SetOutputSize(uint32 width, uint32 height) override { CGPUPipelineNodeBase::SetOutputSize(width, height); }
        virtual uint32 GetOutputWidth() override { return CGPUPipelineNodeBase::GetOutputWidth(); }
        virtual uint32 GetOutputHeight() override { return CGPUPipelineNodeBase::GetOutputHeight(); }
        virtual CRefObj<ITexture> GetOutputTexture(IGPUPipeline *pPipeline) override;
        virtual void Process(IGPUPipeline *pPipeline) override { }

        //**********************************************************************
        // IGPUPipelineSourceNode
        //**********************************************************************
        virtual void SetSource(IGPUPipeline *pPipeline, IImage *pSource) override;
    };

    //**********************************************************************
    class CGPUPipelineSinkNode : public CGPUPipelineNodeBase, public IGPUPipelineSinkNode
    {
        CRefObj<ITexture> m_spOutputStagedTexture;
    public:
        CGPUPipelineSinkNode(uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) : CGPUPipelineNodeBase(outputWidth, outputHeight, format)
        {
            m_cpuFlags = (D3D11_CPU_ACCESS_FLAG)0;
            m_bindFlags = (D3D11_BIND_FLAG)(D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGPUPipelineNode
        //**********************************************************************
        virtual void SetName(const wchar_t* pName) override { CGPUPipelineNodeBase::SetName(pName); }
        virtual bool IsEnabled() override { return CGPUPipelineNodeBase::IsEnabled(); }
        virtual void Enable() override { CGPUPipelineNodeBase::Enable(); }
        virtual void Disable() override { CGPUPipelineNodeBase::Disable(); }
        virtual void SetShader(IShader *pShader) override { CGPUPipelineNodeBase::SetShader(pShader); }
        virtual CRefObj<IShader> GetShader() override { return CGPUPipelineNodeBase::GetShader(); }
        virtual CRefObj<IGPUPipelineNode> GetInput(const wchar_t *pName) override { return CGPUPipelineNodeBase::GetInput(pName); }
        virtual void SetInput(const wchar_t *pName, const wchar_t* pSamplerName, IGPUPipelineNode *pNode) override { CGPUPipelineNodeBase::SetInput(pName, pSamplerName, pNode); }
        virtual void SetOutputSize(uint32 width, uint32 height) override { CGPUPipelineNodeBase::SetOutputSize(width, height); }
        virtual uint32 GetOutputWidth() override { return CGPUPipelineNodeBase::GetOutputWidth(); }
        virtual uint32 GetOutputHeight() override { return CGPUPipelineNodeBase::GetOutputHeight(); }
        virtual CRefObj<ITexture> GetOutputTexture(IGPUPipeline *pPipeline) override 
        { 
            return m_spOutputStagedTexture;
        }
        virtual void Process(IGPUPipeline *pPipeline) override;
        
        //**********************************************************************
        // IGPUPipelineSinkNode
        //**********************************************************************
        virtual CRefObj<IImage> GetOutput(IGPUPipeline *pPipeline) override;
    };

    //**********************************************************************
    class CGPUPipeline : public IGPUPipeline, public CRefCount
    {
        std::vector<CRefObj<IGPUPipelineNode>> m_nodes; // List of interior graph nodes
        std::vector<CRefObj<IGPUPipelineSinkNode>> m_sinkNodes; // List of sink nodes (end of pipeline)
        std::vector<CRefObj<IGPUPipelineSourceNode>> m_sourceNodes; // List of source nodes (beginning of pipeline)
        CRefObj<IRenderer> m_spRenderer;
        CComPtr<ID3D11Buffer> m_spFullQuadVB;
        CComPtr<ID3D11Buffer> m_spFullQuadIB;
        
        void Initialize(IRenderer *pRenderer);
    public:
        CGPUPipeline()
        {
        }
        friend CRefObj<IGPUPipeline> CreateGPUPipeline(IRenderer* pRenderer);
        friend class CGPUPipelineNodeBase;

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGPUPipeline
        //**********************************************************************
        virtual CRefObj<IRenderer> GetRenderer() override;
        virtual void Process() override;
        virtual void RenderQuad(IShader* pShader) override;
        virtual CRefObj<IGPUPipelineSourceNode> CreateSourceNode(const wchar_t* pName, IImage *pImage, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) override;
        virtual CRefObj<IGPUPipelineSinkNode> CreateSinkNode(const wchar_t* pName, IShader *pShader, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) override;
        virtual CRefObj<IGPUPipelineNode> CreateNode(const wchar_t* pName, IShader *pShader, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM) override;
        virtual CRefObj<IGPUPipelineNode> CreatePredefinedNode(const wchar_t* pName, ...) override;
    };
}
