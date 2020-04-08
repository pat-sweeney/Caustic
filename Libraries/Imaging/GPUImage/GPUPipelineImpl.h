//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "GPUPipeline.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"
#include <vector>
#include <atlbase.h>
#include <d3d11.h>

namespace Caustic
{
    //**********************************************************************
    class CGPUImageBase : public CRefCount
    {
    protected:
        uint32 m_numInputs; // Number of inputs expected by this node
        std::vector<IGPUImageNode*> m_sourceNodes; // Weak reference to IGPUImageNode
        uint32 m_width; // Width of output generated by this node
        uint32 m_height; // Height of output generated by this node
        CRefObj<ITexture> m_spOutputTexture;
        CRefObj<IShader> m_spShader;
        uint32 m_cpuFlags;
        uint32 m_bindFlags;
    public:
        CGPUImageBase(uint32 numInputs)
        {
            m_numInputs = numInputs;
            m_sourceNodes.resize(numInputs);
            m_cpuFlags = 0;
            m_bindFlags = 0;
        }
        
        void SetShader(IShader *pShader);
        void GetShader(IShader **ppShader);
        uint32 GetNumberInputs() { return m_numInputs; }
        void GetInput(uint32 index, IGPUImageNode **ppNode);
        void SetInput(uint32 index, IGPUImageNode *pNode);
        void SetOutputSize(uint32 width, uint32 height) { m_width = width; m_height = height; };
        uint32 GetOutputWidth() { return m_width; }
        uint32 GetOutputHeight() { return m_height; }
        void GetOutputTexture(IGPUPipeline *pPipeline, ITexture **ppTexture);
        virtual void Process(IGPUPipeline *pPipeline);
    };

    //**********************************************************************
    class CGPUImageNode : public CGPUImageBase, public IGPUImageNode
    {
    public:
        CGPUImageNode(uint32 numInputs) : CGPUImageBase(numInputs)
        {
            m_cpuFlags = 0;
            m_bindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        }
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGPUImageNode
        //**********************************************************************
        virtual uint32 GetNumberInputs() override { return CGPUImageBase::GetNumberInputs(); }
        virtual void SetShader(IShader *pShader) override { CGPUImageBase::SetShader(pShader); }
        virtual void GetShader(IShader **ppShader) override { CGPUImageBase::GetShader(ppShader); }
        virtual void GetInput(uint32 index, IGPUImageNode **ppNode) override { CGPUImageBase::GetInput(index, ppNode); }
        virtual void SetInput(uint32 index, IGPUImageNode *pNode) override { CGPUImageBase::SetInput(index, pNode); }
        virtual void SetOutputSize(uint32 width, uint32 height) override { CGPUImageBase::SetOutputSize(width, height); }
        virtual uint32 GetOutputWidth() override { return CGPUImageBase::GetOutputWidth(); }
        virtual uint32 GetOutputHeight() override { return CGPUImageBase::GetOutputHeight(); }
        virtual void GetOutputTexture(IGPUPipeline *pPipeline, ITexture **ppTexture) override { CGPUImageBase::GetOutputTexture(pPipeline, ppTexture); }
        virtual void Process(IGPUPipeline *pPipeline) override { CGPUImageBase::Process(pPipeline); }
    };

    //**********************************************************************
    class CGPUImageSourceNode : public CGPUImageBase, public IGPUImageSourceNode
    {
        CComPtr<ID3D11Texture2D> m_spSourceImage;
    public:
        CGPUImageSourceNode() : CGPUImageBase(1)
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
        // IGPUImageNode
        //**********************************************************************
        virtual uint32 GetNumberInputs() override { return CGPUImageBase::GetNumberInputs(); }
        virtual void SetShader(IShader *pShader) override { CGPUImageBase::SetShader(pShader); }
        virtual void GetShader(IShader **ppShader) override { CGPUImageBase::GetShader(ppShader); }
        virtual void GetInput(uint32 index, IGPUImageNode **ppNode) override { CGPUImageBase::GetInput(index, ppNode); }
        virtual void SetInput(uint32 index, IGPUImageNode *pNode) override { CGPUImageBase::SetInput(index, pNode); }
        virtual void SetOutputSize(uint32 width, uint32 height) override { CGPUImageBase::SetOutputSize(width, height); }
        virtual uint32 GetOutputWidth() override { return CGPUImageBase::GetOutputWidth(); }
        virtual uint32 GetOutputHeight() override { return CGPUImageBase::GetOutputHeight(); }
        virtual void GetOutputTexture(IGPUPipeline *pPipeline, ITexture **ppTexture) override;
        virtual void Process(IGPUPipeline *pPipeline) override { }

        //**********************************************************************
        // IGPUImageSourceNode
        //**********************************************************************
        virtual void SetSource(IGPUPipeline *pPipeline, IImage *pSource) override;
    };

    //**********************************************************************
    class CGPUImageSinkNode : public CGPUImageBase, public IGPUImageSinkNode
    {
    public:
        CGPUImageSinkNode(uint32 numInputs) : CGPUImageBase(numInputs)
        {
            m_cpuFlags = D3D11_CPU_ACCESS_READ;
            m_bindFlags = 0;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGPUImageNode
        //**********************************************************************
        virtual uint32 GetNumberInputs() override { return CGPUImageBase::GetNumberInputs(); }
        virtual void SetShader(IShader *pShader) override { CGPUImageBase::SetShader(pShader); }
        virtual void GetShader(IShader **ppShader) override { CGPUImageBase::GetShader(ppShader); }
        virtual void GetInput(uint32 index, IGPUImageNode **ppNode) override { CGPUImageBase::GetInput(index, ppNode); }
        virtual void SetInput(uint32 index, IGPUImageNode *pNode) override { CGPUImageBase::SetInput(index, pNode); }
        virtual void SetOutputSize(uint32 width, uint32 height) override { CGPUImageBase::SetOutputSize(width, height); }
        virtual uint32 GetOutputWidth() override { return CGPUImageBase::GetOutputWidth(); }
        virtual uint32 GetOutputHeight() override { return CGPUImageBase::GetOutputHeight(); }
        virtual void GetOutputTexture(IGPUPipeline *pPipeline, ITexture **ppTexture) override 
        { 
            CGPUImageBase::GetOutputTexture(pPipeline, ppTexture);
        }
        virtual void Process(IGPUPipeline *pPipeline) override;
        
        //**********************************************************************
        // CGPUImageSinkNode
        //**********************************************************************
        virtual void GetOutput(IGPUPipeline *pPipeline, IImage **ppSource) override;
    };

    //**********************************************************************
    class CGPUPipeline : public IGPUPipeline, public CRefCount
    {
        std::vector<CRefObj<IGPUImageNode>> m_nodes; // List of interior graph nodes
        std::vector<CRefObj<IGPUImageSinkNode>> m_sinkNodes; // List of sink nodes (end of pipeline)
        std::vector<CRefObj<IGPUImageSourceNode>> m_sourceNodes; // List of source nodes (beginning of pipeline)
        CRefObj<IGraphics> m_spGraphics;
        CComPtr<ID3D11Buffer> m_spFullQuadVB;
        CComPtr<ID3D11Buffer> m_spFullQuadIB;
        
        void Initialize(IGraphics *pGraphics);
    public:
        CGPUPipeline()
        {
        }
        friend void CreateGPUPipeline(IGraphics *pGraphics, IGPUPipeline **ppPipeline);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGPUPipeline
        //**********************************************************************
        virtual void GetGraphics(IGraphics **ppGraphics) override;
        virtual void RenderQuad(IShader *pShader) override;
        virtual void Process() override;
        virtual void CreateSourceNode(IImage *pImage, IGPUImageSourceNode **ppNewNode) override;
        virtual void CreateSinkNode(uint32 numInputs, IShader *pShader, IGPUImageSinkNode **ppNewNode) override;
        virtual void CreateNode(uint32 numInputs, IShader *pShader, IGPUImageNode **ppNewNode) override;
        virtual void CreatePredefinedNode(wchar_t *pShaderName, IGPUImageNode **ppNewNode) override;
        virtual void CreateShader(BYTE *pShaderCode, uint32 shaderCodeSize, ShaderDefs *pShaderParams, uint32 shaderParamSize, IShader **ppShader) override;
    };
}
