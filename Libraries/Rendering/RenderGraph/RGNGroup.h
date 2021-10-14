//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.IRefCount;
#include "IRenderGraphNode.h"
#include "RenderGraphNode.h"
#include "IRGNGroup.h"

namespace Caustic
{
    class CRenderGraphNode_Group : public IRenderGraphNode_Group, public CRenderGraphNode, public CRefCount
    {
    protected:
        std::vector<CRefObj<IRenderGraphNode>> m_children;
        CRefObj<IRenderGraphPin> m_spRenderPin;
    public:
        CRenderGraphNode_Group();

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
        // IRenderGraphNode_Group
        //**********************************************************************
        virtual void AddChild(IRenderGraphNode* pChild) override;
        virtual CRefObj<IRenderGraphPin> AddInputPin(std::string pinName, ERenderGraphDataType dataType) override;
    };
}
