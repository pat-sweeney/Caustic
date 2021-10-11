//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Vector.h"
#include "IRGNCompute.h"
#include "RenderGraphNode.h"
#include "ILightCollection.h"
#include "LightCollection.h"
#include "IRGNLightCollection.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
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
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraphNode
        //**********************************************************************
        virtual ERenderGraphNodeType GetType() override { return ERenderGraphNodeType::LightCollectionNode; }
        virtual std::string GetName() override { return CRenderGraphNode::GetName(); }
        virtual void SetName(std::string name) override { CRenderGraphNode::SetName(name); }
        virtual void SetProperty(std::string name, std::any value) override { CRenderGraphNode::SetProperty(name, value); }
        virtual uint32 NumberInputPins() override { return CRenderGraphNode::NumberInputPins(); }
        virtual uint32 NumberOutputPins() override { return CRenderGraphNode::NumberOutputPins(); }
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
