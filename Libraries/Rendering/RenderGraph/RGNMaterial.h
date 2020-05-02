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
#include "IRGNMaterial.h"

namespace Caustic
{
    //**********************************************************************
    // Class: CRenderGraphNode_Material
    // Implements <IRenderGraphNode_Material>
    //**********************************************************************
    class CRenderGraphNode_Material : public IRenderGraphNode_Material, public CRenderGraphNode, public CRefCount
    {
    protected:
        CRefObj<IRenderMaterial> m_spRenderMaterial;
        CRefObj<IMaterialAttrib> m_spMaterial;
        CRefObj<IShader> m_spShader;
        CRefObj<IRenderGraphPin> m_spMaterialPin;
        CRefObj<IRenderGraphPin> m_spShaderPin;
        CRefObj<IRenderGraphPin> m_spRenderMaterialPin;
    public:
        CRenderGraphNode_Material(IShader* pShader) 
        { 
            m_spShader = pShader; 
            std::any empty;
            m_spMaterialPin = CreatePin(this, true, "materialAttrib", ERenderGraphDataType::Material, empty);
            m_spShaderPin = CreatePin(this, true, "shader", ERenderGraphDataType::Shader, empty);
            m_spRenderMaterialPin = CreatePin(this, false, "material", ERenderGraphDataType::RenderMaterial, std::any());
        }

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
        virtual void SetParent(IRenderGraphNode *pParent) override { CRenderGraphNode::SetParent(pParent); }
        virtual std::any GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx) override
        {
            if (pPin == m_spRenderMaterialPin)
            {
                std::any materialVal = m_spMaterialPin->GetValue(pRenderer, pRenderCtx);
                std::any shaderVal = m_spShaderPin->GetValue(pRenderer, pRenderCtx);
                if (materialVal.has_value() && shaderVal.has_value())
                {
                    CRefObj<IGraphics> spGraphics = pRenderer->GetGraphics();
                    if (pRenderCtx->GetMostRecentEpoch() > m_lastEpochModified || // Somebody preceeding us was modified
                        m_lastEpochModified == 0)
                    {
                        CRefObj<ICausticFactory> spCausticFactory;
                        Caustic::CreateCausticFactory(&spCausticFactory);
                        spCausticFactory->CreateRenderMaterial(pRenderer, m_spMaterial, m_spShader, &m_spRenderMaterial);
                        m_lastEpochModified = pRenderCtx->GetEpoch();
                    }
                    return std::any(m_spRenderMaterial);
                }
            }
            return std::any();
        }
        
        //**********************************************************************
        // IRenderGraphNode_Material
        //**********************************************************************
    };
}
