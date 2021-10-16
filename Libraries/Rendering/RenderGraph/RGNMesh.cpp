//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
#include "IRGNMesh.h"
#include "RGNMesh.h"
#include "ILightCollection.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    CRenderGraphNode_Mesh::CRenderGraphNode_Mesh()
    {
        std::any empty;
        m_spCausticFactory = Caustic::CreateCausticFactory();
        m_spRenderPin = CreatePin(this, false, "render", ERenderGraphDataType::Void, empty);
        m_spMeshPin = CreatePin(this, true, "mesh", ERenderGraphDataType::Mesh, empty);
        m_spFrontMatPin = CreatePin(this, true, "frontMaterial", ERenderGraphDataType::RenderMaterial, empty);
        m_spBackMatPin = CreatePin(this, true, "backMaterial", ERenderGraphDataType::RenderMaterial, empty);
        m_spShaderPin = CreatePin(this, true, "shader", ERenderGraphDataType::Shader, empty);
        m_spLightsPin = CreatePin(this, true, "lights", ERenderGraphDataType::LightCollection, empty);

    }

    std::any CRenderGraphNode_Mesh::GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        if (m_spDefaultShader == nullptr)
        {
            CRefObj<IShaderMgr> spShaderMgr = pRenderer->GetShaderMgr();
            m_spDefaultShader = spShaderMgr->FindShader(L"Textured");
            CRefObj<IMaterialAttrib> spMaterialAttrib = m_spCausticFactory->CreateMaterialAttrib();
            FRGBColor clr(1.0f, 1.0f, 1.0f);
            spMaterialAttrib->SetColor(L"diffuse", clr);
            m_spDefaultMaterial = m_spCausticFactory->CreateRenderMaterial(pRenderer, spMaterialAttrib, m_spDefaultShader);
        }
        std::any meshVal = m_spMeshPin->GetValue(pRenderer, pRenderCtx);
        std::any frontMatVal = m_spFrontMatPin->GetValue(pRenderer, pRenderCtx);
        std::any backMatVal = m_spBackMatPin->GetValue(pRenderer, pRenderCtx);
        std::any shaderVal = m_spShaderPin->GetValue(pRenderer, pRenderCtx);
        if (!meshVal.has_value())
            CT(E_UNEXPECTED);

        CRefObj<IRenderMaterial> spFrontMaterial;
        CRefObj<IRenderMaterial> spBackMaterial;
        CRefObj<IShader> spShader;
        if (!frontMatVal.has_value())
            spFrontMaterial = m_spDefaultMaterial;
        else
            spFrontMaterial = std::any_cast<CRefObj<IRenderMaterial>>(frontMatVal);
        if (!backMatVal.has_value())
            spBackMaterial = m_spDefaultMaterial;
        else
            spBackMaterial = std::any_cast<CRefObj<IRenderMaterial>>(backMatVal);
        if (!shaderVal.has_value())
            spShader = m_spDefaultShader;
        else
            spShader = std::any_cast<CRefObj<IShader>>(shaderVal);
        m_spMesh = std::any_cast<CRefObj<IMesh>>(meshVal);
        if (pRenderCtx->GetMostRecentEpoch() > m_lastEpochModified || // Somebody preceeding us was modified
            m_lastEpochModified == 0)
        {
            // We need to update. Otherwise, nothing has changed
            m_spRenderMesh = m_spMesh->ToRenderMesh(pRenderer, spShader);
            m_spMesh->ToRenderMaterials(pRenderer, spShader, m_spRenderMesh, nullptr);
            std::any lightsVal = m_spLightsPin->GetValue(pRenderer, pRenderCtx);
            if (lightsVal.has_value())
            {
                m_spLights = std::any_cast<CRefObj<ILightCollection>>(lightsVal);
                m_lights = m_spLights->ToVector();
            }
            m_lastEpochModified = pRenderCtx->GetEpoch();
        }
        m_spRenderMesh->Render(pRenderer, pRenderCtx, spFrontMaterial, spBackMaterial, m_lights, nullptr);
        if (spBackMaterial)
            m_spRenderMesh->Render(pRenderer, pRenderCtx, spFrontMaterial, spBackMaterial, m_lights, nullptr);
        return std::any();
    }
}
