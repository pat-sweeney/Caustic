//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Vector.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "RenderGraphNode.h"
#include "IRenderGraphPointLightNode.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    class CRenderGraphPointLightNode : public CRenderGraphNode, public IRenderGraphPointLightNode, public CRefCount
    {
        CRefObj<IPointLight> m_spPointLight;
    public:
        CRenderGraphPointLightNode()
        {
            Caustic::CCausticFactory::Instance()->CreatePointLight(Vector3(0.0f, 0.0f, 0.0f), &m_spPointLight);
            CreatePin(true, "position");
            CreatePin(true, "color");
            CreatePin(false, "out");
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
        virtual uint32 NumberPins() override { return CRenderGraphNode::NumberPins(); }
        virtual CRefObj<IRenderGraphPin> GetPin(int index) override { return CRenderGraphNode::GetPin(index); }
        virtual CRefObj<IRenderGraphPin> FindPin(std::string name) override { return CRenderGraphNode::FindPin(name); }
        virtual bool Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx) override
        {
            if (m_prerenderCallback)
                if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                    return;
            pSceneCtx->m_lights.push_back(m_spPointLight);
            if (m_postrenderCallback)
                m_postrenderCallback(pRenderCtx->GetCurrentPass());
        }

        //**********************************************************************
        // IRenderGraphPointNode
        //**********************************************************************
        virtual void SetPosition(Caustic::Vector3& pos) override
        {
            m_spPointLight->SetPosition(pos);
        }

        virtual void SetColor(Caustic::Vector3& clr) override
        {
            m_spPointLight->SetColor(clr);
        }
    };
}
