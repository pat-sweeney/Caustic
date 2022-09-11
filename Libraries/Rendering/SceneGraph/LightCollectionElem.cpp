//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <functional>
#include <d3d11_4.h>
#include <atlbase.h>
#include <cstdlib>

module Rendering.SceneGraph.SceneLightCollectionElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Base.Math.BBox;
import Geometry.MeshImport;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ILight;
import Rendering.SceneGraph.SceneFactory;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.SceneGraph.ISceneLightCollectionElem;
import Rendering.Caustic.IShaderMgr;
import Rendering.SceneGraph.SceneCubeElem;

namespace Caustic
{
    CRefObj<IJSonObj> CSceneLightCollectionElem::AsJson(const char* pPropertyName, IJSonParser* pParser)
    {
        auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Lights", nullptr);
        auto spBase = CSceneGroupElem::AsJson(pPropertyName, pParser);
        spObj->AddElement(spBase);
        for (int index = 0; index < (int)m_lights.size(); index++)
        {
            auto spLight = m_lights[index];
            auto spJsonLight = pParser->CreateJSonMap(("Light" + std::to_string(index)).c_str(), nullptr);
            switch (spLight->GetType())
            {
            case ELightType::AreaLight: spJsonLight->AddElement(pParser->CreateJSon("Type", "Area")); break;
            case ELightType::DirectionalLight: spJsonLight->AddElement(pParser->CreateJSon("Type", "Directional")); break;
            case ELightType::PointLight: spJsonLight->AddElement(pParser->CreateJSon("Type", "Point")); break;
            case ELightType::SpotLight: spJsonLight->AddElement(pParser->CreateJSon("Type", "Spot")); break;
            }
            spJsonLight->AddElement(pParser->CreateJSon("Intensity", spLight->GetIntensity()));
            spJsonLight->AddElement(pParser->CreateJSon("CastsShadows", spLight->GetCastsShadows()));
            auto pos = spLight->GetPosition();
            spJsonLight->AddElement(
                pParser->CreateJSonMap("Position",
                    pParser->CreateJSon("x", pos.x),
                    pParser->CreateJSon("y", pos.y),
                    pParser->CreateJSon("z", pos.z),
                    nullptr));
            auto dir = spLight->GetDirection();
            spJsonLight->AddElement(
                pParser->CreateJSonMap("Direction",
                    pParser->CreateJSon("x", dir.x),
                    pParser->CreateJSon("y", dir.y),
                    pParser->CreateJSon("z", dir.z),
                    nullptr));
            auto clr = spLight->GetColor();
            spJsonLight->AddElement(
                pParser->CreateJSonMap("Color",
                    pParser->CreateJSon("x", clr.r),
                    pParser->CreateJSon("y", clr.g),
                    pParser->CreateJSon("z", clr.b),
                    nullptr));
            spObj->AddElement(spJsonLight);
        }
        return spObj;
    }

    uint32 CSceneLightCollectionElem::NumberLights()
    {
        return (uint32)m_lights.size();
    }

    CRefObj<ILight> CSceneLightCollectionElem::GetLight(int index)
    {
        return m_lights[index];
    }

    void CSceneLightCollectionElem::AddLight(ILight* pLight)
    {
        m_lights.push_back(pLight);
    }

    void CSceneLightCollectionElem::RemoveLight(ILight* pLight)
    {
        for (int i = 0; i < (int)m_lights.size(); i++)
            if (m_lights[i] == pLight)
            {
                m_lights.erase(m_lights.begin() + i);
            }
    }

    bool CSceneLightCollectionElem::sm_proxiesCreated = false;
    CRefObj<ISceneMaterialElem> CSceneLightCollectionElem::sm_spLightProxyMaterialElem;
    CRefObj<ISceneMaterialElem> CSceneLightCollectionElem::LoadLightProxies(IRenderer* pRenderer)
    {
        if (!sm_proxiesCreated)
        {
#pragma warning(push)
#pragma warning(disable: 4996)
            const char* pModelDir = std::getenv("AppModelDir");
#pragma warning(pop)
            if (pModelDir == nullptr)
                pModelDir = "D:\\Models";
            std::string fn = std::string(pModelDir) + "\\LightBulb.obj";
            std::wstring wfn = Caustic::str2wstr(fn);
            CRefObj<ISceneFactory> spSceneFactory = CSceneFactory::Instance();
            CRefObj<ISceneMeshElem> spMeshElem = spSceneFactory->CreateMeshElem();
            const wchar_t* pShaderName = L"ObjShader";
            CRefObj<IMesh> spMesh = Caustic::MeshImport::LoadObj(wfn.c_str(), nullptr);
            spMeshElem->SetMesh(spMesh);
            CRefObj<IShader> spShader = pRenderer->GetShaderMgr()->FindShader(pShaderName);
            sm_spLightProxyMaterialElem = spSceneFactory->CreateMaterialElem();
            auto spCausticFactory = Caustic::CreateCausticFactory();
            CRefObj<IMaterialAttrib> spMaterial = spCausticFactory->CreateMaterialAttrib();
            FRGBColor ambient(0.2f, 0.2f, 0.2f);
            FRGBColor diffuse(0.4f, 0.4f, 0.4f);
            spMaterial->SetColor(L"ambientColor", ambient);
            spMaterial->SetColor(L"diffuseColor", diffuse);
            sm_spLightProxyMaterialElem->SetMaterial(spMaterial);
            sm_spLightProxyMaterialElem->SetShader(spShader);
            sm_spLightProxyMaterialElem->AddChild(spMeshElem);
            sm_proxiesCreated = true;
        }
        return sm_spLightProxyMaterialElem;
    }

    void CSceneLightCollectionElem::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)
    {
        if (!(m_passes & pRenderCtx->GetCurrentPass()))
            return;
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(pRenderer->GetContext()->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
        spCtx2->BeginEventInt(L"LightCollection", 0);
#endif
        std::vector<CRefObj<ILight>> lights = pSceneCtx->m_lights;
        if (pSceneCtx->m_ShowProxyObjects)
        {
            CRefObj<IShader> spOldShader = pSceneCtx->m_spCurrentShader;
            pSceneCtx->m_spCurrentShader = pRenderer->GetShaderMgr()->FindShader(L"default");
            for (int i = 0; i < (int)m_lights.size(); i++)
            {
                auto lightPos = m_lights[i]->GetPosition();
                auto spLightProxyMaterialElem = LoadLightProxies(pRenderer);
                auto mat = Matrix4x4::TranslationMatrix(lightPos.x, lightPos.y, lightPos.z);
                spLightProxyMaterialElem->SetTransform(mat);
                spLightProxyMaterialElem->Render(pRenderer, pRenderCtx, pSceneCtx);
            }
            pSceneCtx->m_spCurrentShader = spOldShader;
        }
        if (pRenderCtx->GetCurrentPass() == Caustic::c_PassShadow)
        {
            for (int i = 0; i < (int)m_lights.size(); i++)
                if (m_lights[i]->GetOnOff())
                    pSceneCtx->m_lights.push_back(m_lights[i]);
            int totalLights = 0;
            for (int i = 0; i < (int)m_lights.size(); i++)
            {
                if (totalLights >= c_MaxLights)
                    break;
                if (m_lights[i]->GetCastsShadows() && m_lights[i]->GetType() == ELightType::DirectionalLight)
                {
                    pRenderer->PushShadowmapRT(c_HiResShadowMap, totalLights++, m_lights[i]->GetPosition(), m_lights[i]->GetDirection());
                    bool inShadowLightGroup = pSceneCtx->m_inShadowLightGroup;
                    pSceneCtx->m_inShadowLightGroup = true;
                    CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
                    pSceneCtx->m_inShadowLightGroup = inShadowLightGroup;
                    pRenderer->PopShadowmapRT();
                }
            }
        }
        else
        {
            for (int i = 0; i < (int)m_lights.size(); i++)
                if (m_lights[i]->GetOnOff() && m_lights[i]->GetCastsLight())
                    pSceneCtx->m_lights.push_back(m_lights[i]);
            CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
        }
        pSceneCtx->m_lights = lights;
#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }

    void CSceneLightCollectionElem::GetBBox(BBox3* pBBox)
    {
        CSceneGroupElem::GetBBox(pBBox);
    }
}
