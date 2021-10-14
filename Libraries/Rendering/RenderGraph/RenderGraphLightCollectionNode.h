//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.IRefCount;
#include "Base\Core\error.h"
#include "RenderGraphNode.h"
#include "ILightCollection.h"
#include <vector>

namespace Caustic
{
    class CRenderGraphLightCollectionNode : public CRenderGraphNode, public IRenderGraphNode, public CRefCount
    {
        CRefObj<ILightCollection> m_spLightCollection;
        CRefObj<IRenderGraphPin> m_spLightsPin;
    public:
        CRenderGraphLightCollectionNode()
        {
            m_spLightsPin = CreatePin(this, true, "lights");
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
        virtual bool HasOutput() override { return CRenderGraphNode::HasOutput(); }
        virtual uint32 NumberPins() override { return CRenderGraphNode::NumberPins(); }
        virtual CRefObj<IRenderGraphPin> GetPin(int index) override { return CRenderGraphNode::GetPin(index); }
        virtual CRefObj<IRenderGraphPin> FindPin(std::string name) override { return CRenderGraphNode::FindPin(name); }
        virtual bool Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx) override { }
        virtual void SetPreRenderCallback(std::function<bool(int pass)> prerenderCallback) override { CRenderGraphNode::SetPreRenderCallback(prerenderCallback); }
        virtual void SetPostRenderCallback(std::function<void(int pass)> postrenderCallback) override { CRenderGraphNode::SetPostRenderCallback(postrenderCallback); }
        virtual std::any GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx) override
        {
            if (m_spLightsPin != pPin)
                CT(E_UNEXPECTED);
            return std::any(m_spLightCollection);
        }
    };
}