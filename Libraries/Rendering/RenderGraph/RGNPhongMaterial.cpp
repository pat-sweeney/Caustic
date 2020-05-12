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
#include "RGNPhongMaterial.h"

namespace Caustic
{
    CRenderGraphNode_PhongMaterial::CRenderGraphNode_PhongMaterial()
    {
        std::any empty;
        m_spDiffuseColorPin = CreatePin(this, true, "diffuseColor", ERenderGraphDataType::Float3, empty);
        m_spSpecularColorPin = CreatePin(this, true, "specularColor", ERenderGraphDataType::Float3, empty);
        m_spSpecularPowerPin = CreatePin(this, true, "specularPower", ERenderGraphDataType::Float, empty);
        m_spAmbientColorPin = CreatePin(this, true, "ambientColor", ERenderGraphDataType::Float3, empty);
        m_spDiffuseTexturePin = CreatePin(this, true, "diffuseTexture", ERenderGraphDataType::Image, empty);
        m_spSpecularTexturePin = CreatePin(this, true, "specularTexture", ERenderGraphDataType::Image, empty);
        m_spAmbientTexturePin = CreatePin(this, true, "ambientTexture", ERenderGraphDataType::Image, empty);
        m_spMatPin = CreatePin(this, false, "material", ERenderGraphDataType::RenderMaterial, std::any());
        m_spCausticFactory = Caustic::CreateCausticFactory();
    }
    
    std::any CRenderGraphNode_PhongMaterial::GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        if (m_spShader == nullptr)
        {
            pRenderer->GetShaderMgr()->FindShader(L"Textured", &m_spShader);
        }
        std::any diffuseTexVal = m_spDiffuseTexturePin->GetValue(pRenderer, pRenderCtx);
        if (pRenderCtx->GetMostRecentEpoch() > m_lastEpochModified || // Somebody preceeding us was modified
            m_lastEpochModified == 0)
        {
            if (diffuseTexVal.has_value())
            {
                CRefObj<IImage> spDiffuseImage = std::any_cast<CRefObj<IImage>>(diffuseTexVal);
                if (spDiffuseImage)
                {
                    CRefObj<IGraphics> spGraphics = pRenderer->GetGraphics();
                    CRefObj<IMaterialAttrib> spMaterial = m_spCausticFactory->CreateMaterialAttrib();
                    spMaterial->SetTexture(L"diffuseTexture", spDiffuseImage, EShaderAccess::PixelShader);
                    m_spRenderMaterial = m_spCausticFactory->CreateRenderMaterial(pRenderer, spMaterial, m_spShader);
                }
            }
            m_lastEpochModified = pRenderCtx->GetEpoch();
        }
        if (pPin == m_spMatPin)
            return std::any(m_spRenderMaterial);
        return std::any();
    }
}
