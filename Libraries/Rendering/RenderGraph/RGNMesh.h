//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.RefCount;
#include "Base\Math\Vector.h"
#include "IRGNMesh.h"
#include "RenderGraphNode.h"
#include "ILightCollection.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    class CRenderGraphNode_Mesh : public IRenderGraphNode_Mesh, public CRenderGraphNode, public CRefCount
    {
    protected:
        CRefObj<ICausticFactory> m_spCausticFactory;
        CRefObj<IShader> m_spDefaultShader;
        CRefObj<IRenderMaterial> m_spDefaultMaterial;
        CRefObj<IMesh> m_spMesh;
        CRefObj<IRenderMesh> m_spRenderMesh;
        CRefObj<ILightCollection> m_spLights;
        std::vector<CRefObj<ILight>> m_lights;
        CRefObj<IRenderGraphPin> m_spRenderPin;
        CRefObj<IRenderGraphPin> m_spMeshPin;
        CRefObj<IRenderGraphPin> m_spFrontMatPin;
        CRefObj<IRenderGraphPin> m_spBackMatPin;
        CRefObj<IRenderGraphPin> m_spShaderPin;
        CRefObj<IRenderGraphPin> m_spLightsPin;
    public:
        CRenderGraphNode_Mesh();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraphNode
        //**********************************************************************
        virtual ERenderGraphNodeType GetType() override { return ERenderGraphNodeType::MeshNode; }
        virtual std::string GetName() override { return m_name; }
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
        virtual std::any GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx) override;
    };
}
