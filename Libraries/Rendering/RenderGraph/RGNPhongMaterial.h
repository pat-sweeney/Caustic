//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Rendering\Caustic\MaterialAttrib.h"
#include "Rendering\Caustic\IRenderMaterial.h"
#include "IRenderGraphNode.h"
#include "RenderGraphNode.h"
#include "IRGNPhongMaterial.h"

namespace Caustic
{
    //**********************************************************************
    // Class: CRenderGraphNode_PhongMaterial
    // Implements <IRenderGraphNode_PhongMaterial>
    //**********************************************************************
    class CRenderGraphNode_PhongMaterial : public IRenderGraphNode_PhongMaterial, public CRenderGraphNode, public CRefCount
    {
    protected:
        CRefObj<ICausticFactory> m_spCausticFactory;
        CRefObj<IRenderMaterial> m_spRenderMaterial;
        CRefObj<IShader> m_spShader;
        CRefObj<IRenderGraphPin> m_spDiffuseColorPin;
        CRefObj<IRenderGraphPin> m_spSpecularColorPin;
        CRefObj<IRenderGraphPin> m_spSpecularPowerPin;
        CRefObj<IRenderGraphPin> m_spAmbientColorPin;
        CRefObj<IRenderGraphPin> m_spDiffuseTexturePin;
        CRefObj<IRenderGraphPin> m_spSpecularTexturePin;
        CRefObj<IRenderGraphPin> m_spAmbientTexturePin;
        CRefObj<IRenderGraphPin> m_spMatPin;
    public:
        CRenderGraphNode_PhongMaterial();

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

        //**********************************************************************
        // IRenderGraphNode_Material
        //**********************************************************************
    };
}
