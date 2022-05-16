//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>

export module Rendering.RenderGraph.RGNCompute;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Rendering.RenderGraph.IRGNCompute;
import Rendering.RenderGraph.RenderGraphNode;

export namespace Caustic
{
    class CRenderGraphNode_Compute : public IRenderGraphNode_Compute, public CRenderGraphNode, public CRefCount
    {
    protected:
        CRefObj<IShader> m_spComputeShader;
        CRefObj<IRenderGraphPin> m_spComputeBufferPin;
        uint32 m_xThreads, m_yThreads, m_zThreads;
    public:
        CRenderGraphNode_Compute(IShader *pShader);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraphNode
        //**********************************************************************
        virtual ERenderGraphNodeType GetType() override { return ERenderGraphNodeType::MeshNode; }
        virtual std::string GetName() { return m_name; }
        virtual void SetName(std::string name) { CRenderGraphNode::SetName(name); }
        virtual void SetProperty(std::string name, std::any value) { CRenderGraphNode::SetProperty(name, value); }
        virtual uint32 NumberInputPins() { return CRenderGraphNode::NumberInputPins(); }
        virtual uint32 NumberOutputPins() { return CRenderGraphNode::NumberOutputPins(); }
        virtual CRefObj<IRenderGraphPin> GetInputPin(int index) { return CRenderGraphNode::GetInputPin(index); }
        virtual CRefObj<IRenderGraphPin> GetOutputPin(int index) { return CRenderGraphNode::GetOutputPin(index); }
        virtual CRefObj<IRenderGraphPin> FindInputPin(std::string name) { return CRenderGraphNode::FindInputPin(name); }
        virtual CRefObj<IRenderGraphPin> FindOutputPin(std::string name) { return CRenderGraphNode::FindOutputPin(name); }
        virtual CRefObj<IRenderGraphNode> GetParent() { return CRenderGraphNode::GetParent(); }
        virtual void SetParent(IRenderGraphNode* pParent) override { CRenderGraphNode::SetParent(pParent); }
        virtual std::any GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx) override;

        //**********************************************************************
        // IRenderGraphNode_Compute
        //**********************************************************************
        virtual CRefObj<IShader> GetShader() override { return m_spComputeShader; }
        virtual void SetNumberThreads(int xThreads, int yThreads, int zThreads) override
        {
            m_xThreads = xThreads;
            m_yThreads = yThreads;
            m_zThreads = zThreads;
        }
        virtual void SetInputThreads(uint32 width, uint32 height, uint32 depth /* = 1 */) override;
        virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) override;
        virtual void SetShaderParam(const wchar_t* pParamName, float value) override;
        virtual void SetBuffer(IRenderer* pRenderer, const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride);
    };
}
