//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Renderer.h"
#include "RenderMaterial.h"
#include "Sampler.h"

namespace Caustic
{
    CAUSTICAPI void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial)
    {
        std::unique_ptr<CRenderMaterial> spRenderMaterial(new CRenderMaterial());
        spRenderMaterial->m_spMaterial = pMaterialAttrib;
        spRenderMaterial->m_spShader = pShader;

        if (pMaterialAttrib)
        {
            std::string fnDiffuse = pMaterialAttrib->GetDiffuseTexture();
            if (!fnDiffuse.empty())
            {
                std::wstring wfn(fnDiffuse.begin(), fnDiffuse.end());
                Caustic::LoadTexture(wfn.c_str(), pGraphics, &spRenderMaterial->m_spDiffuseTexture);
                CreateSampler(pGraphics, spRenderMaterial->m_spDiffuseTexture.p, &spRenderMaterial->m_spDiffuseSampler);
            }

            std::string fnSpecular = pMaterialAttrib->GetSpecularTexture();
            if (!fnSpecular.empty())
            {
                std::wstring wfn(fnSpecular.begin(), fnSpecular.end());
                Caustic::LoadTexture(wfn.c_str(), pGraphics, &spRenderMaterial->m_spSpecularTexture);
                CreateSampler(pGraphics, spRenderMaterial->m_spSpecularTexture.p, &spRenderMaterial->m_spSpecularSampler);
            }

            std::string fnAmbient = pMaterialAttrib->GetAmbientTexture();
            if (!fnAmbient.empty())
            {
                std::wstring wfn(fnAmbient.begin(), fnAmbient.end());
                Caustic::LoadTexture(wfn.c_str(), pGraphics, &spRenderMaterial->m_spAmbientTexture);
                CreateSampler(pGraphics, spRenderMaterial->m_spAmbientTexture.p, &spRenderMaterial->m_spAmbientSampler);
            }
        }

        *ppRenderMaterial = spRenderMaterial.release();
        (*ppRenderMaterial)->AddRef();

    }

    void CRenderMaterial::SetDiffuseTexture(IGraphics *pGraphics, ITexture *pTexture)
    {
        m_spDiffuseTexture = pTexture;
        m_spDiffuseSampler = nullptr;
        CreateSampler(pGraphics, m_spDiffuseTexture.p, &m_spDiffuseSampler);
    }

    void CRenderMaterial::SetSpecularTexture(IGraphics *pGraphics, ITexture *pTexture)
    {
        m_spAmbientTexture = pTexture;
        m_spSpecularSampler = nullptr;
        CreateSampler(pGraphics, m_spAmbientTexture.p, &m_spSpecularSampler);
    }

    void CRenderMaterial::SetAmbientTexture(IGraphics *pGraphics, ITexture *pTexture)
    {
        m_spSpecularTexture = pTexture;
        m_spAmbientSampler = nullptr;
        CreateSampler(pGraphics, m_spSpecularTexture.p, &m_spAmbientSampler);
    }

    void CRenderMaterial::SetShader(IShader *pShader) 
    {
        m_spShader = pShader;
    }

    void CRenderMaterial::GetShader(IShader **ppShader)
    {
        if (m_spShader == nullptr)
        {
            if (m_spDiffuseTexture.p == nullptr)
                CShaderMgr::GetInstance()->FindShader(L"Default", &m_spShader);
            else
                CShaderMgr::GetInstance()->FindShader(L"Textured", &m_spShader);
        }
        *ppShader = m_spShader.p;
        if (m_spShader.p)
            (*ppShader)->AddRef();
    }

    void CRenderMaterial::Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx * /*pRenderCtx*/, IShader *spShader)
    {
        Float4 vAmbient(0.1f, 0.1f, 0.1f, 0.1f);
        Float4 vDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
        Float4 vSpecular(0.7f, 0.7f, 0.7f, 1.0f);
        Float4 vSpecularExp(60.0f, 60.0f, 60.0f, 60.0f);
        float transparency = 1.0f;
        if (m_spMaterial.p)
        {
            vAmbient.x = m_spMaterial->GetAmbientColor().x;
            vAmbient.y = m_spMaterial->GetAmbientColor().y;
            vAmbient.z = m_spMaterial->GetAmbientColor().z;
            vAmbient.w = 1.0f;
            vDiffuse.x = m_spMaterial->GetDiffuseColor().x;
            vDiffuse.y = m_spMaterial->GetDiffuseColor().y;
            vDiffuse.z = m_spMaterial->GetDiffuseColor().z;
            vDiffuse.w = 1.0f;
            vSpecular.x = m_spMaterial->GetSpecularColor().x;
            vSpecular.y = m_spMaterial->GetSpecularColor().y;
            vSpecular.z = m_spMaterial->GetSpecularColor().z;
            vSpecular.w = 1.0f;
            vSpecularExp.x = m_spMaterial->GetSpecularExp();
            vSpecularExp.y = m_spMaterial->GetSpecularExp();
            vSpecularExp.z = m_spMaterial->GetSpecularExp();
            vSpecularExp.w = 0.0f;
            transparency = m_spMaterial->GetAlpha();
        }
        spShader->SetPSParam(L"ambientColor", std::any(vAmbient));
        spShader->SetPSParam(L"diffuseColor", std::any(vDiffuse));
        spShader->SetPSParam(L"specularColor", std::any(vSpecular));
        spShader->SetPSParam(L"specularExp", std::any(vSpecularExp));
        spShader->SetPSParam(L"transparency", std::any(Float4(transparency, transparency, transparency, transparency)));
        spShader->SetPSParam(L"numLights", std::any());
        int numLights = (int)lights.size();
        if (numLights > 4)
            numLights = 4; // TODO: Replace with constant shared with shaders
        spShader->SetPSParam(L"numLights", std::any(Int(numLights)));
        for (int i = 0; i < numLights; i++)
        {
            Float4 v(0.0f, 0.0f, 0.0f, 0.0f);
            Vector3 pos = lights[i]->GetPosition();
            v.x = pos.x;
            v.y = pos.y;
            v.z = pos.z;
            v.w = 0.0f;
            spShader->SetPSParam(L"lightPosWS", i, std::any(v));
            Vector3 lightColor = lights[i]->GetColor();
            v.x = lightColor.x;
            v.y = lightColor.y;
            v.z = lightColor.z;
            v.w = 0.0f;
            spShader->SetPSParam(L"lightColor", i, std::any(v));
        }
        if (m_spDiffuseTexture.p)
            m_spDiffuseTexture->Update(pGraphics);
        if (m_spAmbientTexture.p)
            m_spAmbientTexture->Update(pGraphics);
        if (m_spSpecularTexture.p)
            m_spSpecularTexture->Update(pGraphics);
        if (m_spDiffuseSampler.p)
            spShader->SetPSParam(L"diffuseSampler", std::any(CSamplerRef(m_spDiffuseSampler.p)));
        if (m_spSpecularSampler.p)
            spShader->SetPSParam(L"specularSampler", std::any(CSamplerRef(m_spSpecularSampler.p)));
        if (m_spAmbientSampler.p)
            spShader->SetPSParam(L"ambientSampler", std::any(CSamplerRef(m_spAmbientSampler.p)));
    }
}
