//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Renderer.h"
#include "RenderMaterial.h"
#include "CausticFactory.h"
#include "Sampler.h"

namespace Caustic
{
    void CRenderMaterial::SetDiffuseTexture(IRenderer *pRenderer, ITexture *pTexture)
    {
        m_spDiffuseTexture = pTexture;
        m_spDiffuseSampler = nullptr;
		CCausticFactory::Instance()->CreateSampler(pRenderer, m_spDiffuseTexture, &m_spDiffuseSampler);
    }

	CRefObj<ITexture> CRenderMaterial::GetDiffuseTexture()
	{
		return m_spDiffuseTexture;
	}

    void CRenderMaterial::SetSpecularTexture(IRenderer *pRenderer, ITexture *pTexture)
    {
		m_spSpecularTexture = pTexture;
        m_spSpecularSampler = nullptr;
		CCausticFactory::Instance()->CreateSampler(pRenderer, m_spSpecularTexture, &m_spSpecularSampler);
    }

	CRefObj<ITexture> CRenderMaterial::GetSpecularTexture()
	{
		return m_spSpecularTexture;
	}

    void CRenderMaterial::SetAmbientTexture(IRenderer *pRenderer, ITexture *pTexture)
    {
        m_spAmbientTexture = pTexture;
        m_spAmbientSampler = nullptr;
		CCausticFactory::Instance()->CreateSampler(pRenderer, m_spAmbientTexture, &m_spAmbientSampler);
    }

	CRefObj<ITexture> CRenderMaterial::GetAmbientTexture()
	{
		return m_spAmbientTexture;
	}

	void CRenderMaterial::SetShader(IShader *pShader)
    {
        m_spShader = pShader;
    }

    void CRenderMaterial::GetShader(IShader **ppShader)
    {
        if (m_spShader == nullptr)
        {
            if (m_spDiffuseTexture == nullptr)
                CShaderMgr::Instance()->FindShader(L"Default", &m_spShader);
            else
                CShaderMgr::Instance()->FindShader(L"Textured", &m_spShader);
        }
        *ppShader = m_spShader;
        if (m_spShader)
            (*ppShader)->AddRef();
    }

    void CRenderMaterial::Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx * /*pRenderCtx*/, IShader *spShader)
    {
        Float4 vAmbient(0.1f, 0.1f, 0.1f, 0.1f);
        Float4 vDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
        Float4 vSpecular(0.7f, 0.7f, 0.7f, 1.0f);
        Float4 vSpecularExp(60.0f, 60.0f, 60.0f, 60.0f);
        float transparency = 1.0f;
        if (m_spMaterial)
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
        if (m_spDiffuseTexture)
            m_spDiffuseTexture->Update(pRenderer);
        if (m_spAmbientTexture)
            m_spAmbientTexture->Update(pRenderer);
        if (m_spSpecularTexture)
            m_spSpecularTexture->Update(pRenderer);
        if (m_spDiffuseSampler)
            spShader->SetPSParam(L"diffuseSampler", std::any(CSamplerRef(m_spDiffuseSampler)));
        if (m_spSpecularSampler)
            spShader->SetPSParam(L"specularSampler", std::any(CSamplerRef(m_spSpecularSampler)));
        if (m_spAmbientSampler)
            spShader->SetPSParam(L"ambientSampler", std::any(CSamplerRef(m_spAmbientSampler)));
    }
}
