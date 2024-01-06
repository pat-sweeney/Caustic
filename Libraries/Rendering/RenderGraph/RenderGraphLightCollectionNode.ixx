//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <windows.h>

export module Rendering.RenderGraph.RenderGraphLightCollectionNode;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Error;
import Rendering.RenderGraph.RenderGraphNode;
import Rendering.RenderGraph.IRenderGraphPin;
import Rendering.RenderGraph.ILightCollection;

export namespace Caustic
{
    class CRenderGraphLightCollectionNode : public CRenderGraphNode, public IRenderGraphNode, public CRefCount
    {
        CRefObj<ILightCollection> m_spLightCollection;
        CRefObj<IRenderGraphPin> m_spLightsPin;
    public:
        CRenderGraphLightCollectionNode()
        {
            m_spLightsPin = CreatePin(this, true, "lights", ERenderGraphDataType::LightCollection, std::any(nullptr));
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraphNode
        //**********************************************************************
        virtual ERenderGraphNodeType GetType() override { return ERenderGraphNodeType::LightCollectionNode; }
        virtual std::string GetName() override { return CRenderGraphNode::GetName(); }
        virtual void SetName(std::string name) override { CRenderGraphNode::SetName(name); }
        virtual void SetProperty(std::string name, std::any value) override { CRenderGraphNode::SetProperty(name, value); }
        virtual uint32_t NumberInputPins() override { return CRenderGraphNode::NumberInputPins(); }
        virtual uint32_t NumberOutputPins() override { return CRenderGraphNode::NumberOutputPins(); }
        virtual CRefObj<IRenderGraphPin> GetInputPin(int index) override { return CRenderGraphNode::GetInputPin(index); }
        virtual CRefObj<IRenderGraphPin> GetOutputPin(int index) override { return CRenderGraphNode::GetOutputPin(index); }
        virtual CRefObj<IRenderGraphPin> FindInputPin(std::string name) override { return CRenderGraphNode::FindInputPin(name); }
        virtual CRefObj<IRenderGraphPin> FindOutputPin(std::string name) override { return CRenderGraphNode::FindOutputPin(name); }
        virtual CRefObj<IRenderGraphNode> GetParent() override { return CRenderGraphNode::GetParent(); }
        virtual void SetParent(IRenderGraphNode* pParent) override { return CRenderGraphNode::SetParent(pParent); }
        virtual std::any GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx) override
        {
            if (m_spLightsPin != pPin)
                CT(E_UNEXPECTED);
            return std::any(m_spLightCollection.p);
        }
    };
}