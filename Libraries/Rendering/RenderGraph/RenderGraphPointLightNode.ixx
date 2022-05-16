//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>

export module Rendering.RenderGraph.RenderGraphPointLightNode;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Rendering.Caustic.CausticFactory;
import Rendering.RenderGraph.RenderGraphNode;
import Rendering.RenderGraph.IRenderGraphPointLightNode;

export namespace Caustic
{
    class CRenderGraphPointLightNode : public CRenderGraphNode, public IRenderGraphPointLightNode, public CRefCount
    {
        CRefObj<IPointLight> m_spPointLight;
    public:
        CRenderGraphPointLightNode()
        {
            Vector3 lightPos(0.0f, 0.0f, 0.0f);
            FRGBColor lightColor(1.0f, 0.0f, 0.0f);
            m_spPointLight = Caustic::CCausticFactory::Instance()->CreatePointLight(lightPos, lightColor, 1.0f);
            CreatePin(this, true, "position", ERenderGraphDataType::Float3, std::any(nullptr));
            CreatePin(this, true, "color", ERenderGraphDataType::Float3, std::any(nullptr));
            CreatePin(this, false, "out", ERenderGraphDataType::Float3, std::any(nullptr));
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraphNode
        //**********************************************************************
        virtual ERenderGraphNodeType GetType() override { return ERenderGraphNodeType::PointLightNode; }
        virtual std::string GetName() override { return m_name; }
        virtual void SetName(std::string name) override { m_name = name; }
        virtual void SetProperty(std::string name, std::any value) override { CRenderGraphNode::SetProperty(name, value); }
//        virtual uint32 NumberPins() override { return CRenderGraphNode::NumberPins(); }
//        virtual CRefObj<IRenderGraphPin> GetPin(int index) override { return CRenderGraphNode::GetPin(index); }
//        virtual CRefObj<IRenderGraphPin> FindPin(std::string name) override { return CRenderGraphNode::FindPin(name); }
//        virtual bool Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx) override
//        {
//            if (m_prerenderCallback)
//                if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
//                    return;
//            pSceneCtx->m_lights.push_back(m_spPointLight);
//            if (m_postrenderCallback)
//                m_postrenderCallback(pRenderCtx->GetCurrentPass());
//        }

        //**********************************************************************
        // IRenderGraphPointNode
        //**********************************************************************
        virtual void SetPosition(Caustic::Vector3& pos) override
        {
            m_spPointLight->SetPosition(pos);
        }

        virtual void SetColor(Caustic::Vector3& clr) override
        {
            FRGBColor color(clr.x, clr.y, clr.z);
            m_spPointLight->SetColor(color);
        }
    };
}
