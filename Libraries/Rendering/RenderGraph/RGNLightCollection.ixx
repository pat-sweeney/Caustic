//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>

export module Rendering.RenderGraph.RGNLightCollection;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Math.Vector;
import Rendering.RenderGraph.IRGNCompute;
import Rendering.RenderGraph.RenderGraphNode;
import Rendering.RenderGraph.ILightCollection;
import Rendering.RenderGraph.LightCollection;
import Rendering.RenderGraph.IRGNLightCollection;

export namespace Caustic
{
    class CRenderGraphNode_LightCollection : public IRenderGraphNode_LightCollection, public CRenderGraphNode, public CRefCount
    {
    protected:
        CRefObj<IRenderGraphPin> m_spLightsPin;
        CRefObj<ILightCollection> m_spLights;
    public:
        CRenderGraphNode_LightCollection()
        {
            std::any empty;
            m_spLights = CRefObj(new CLightCollection());
            m_spLightsPin = CreatePin(this, false, "lights", ERenderGraphDataType::LightCollection, empty);
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
        virtual void SetParent(IRenderGraphNode* pParent) override { CRenderGraphNode::SetParent(pParent); }
        virtual std::any GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx) override
        {
            if (pPin == m_spLightsPin)
                return std::any(m_spLights);
            return std::any();
        }

        //**********************************************************************
        // IRenderGraphNode_LightCollection
        //**********************************************************************
        virtual void AddLight(IPointLight* pLight) override { m_spLights->AddLight(pLight); }
        virtual void Clear() override { m_spLights->Clear(); }
        virtual std::vector<CRefObj<ILight>> ToVector() override { return m_spLights->ToVector(); }
        virtual CRefObj<ILightCollection> GetLights() override { return m_spLights; }
    };
}
