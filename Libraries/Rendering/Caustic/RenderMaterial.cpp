//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Renderer.h"
#include "RenderMaterial.h"
#include "MaterialAttrib.h"
#include "CausticFactory.h"
#include "Sampler.h"
#include <string>

namespace Caustic
{
    CAUSTICAPI void CreateMaterialAttrib(IMaterialAttrib** ppMaterialAttrib)
    {
        std::unique_ptr<CMaterialAttrib> spMaterialAttrib(new CMaterialAttrib());
        *ppMaterialAttrib = spMaterialAttrib.release();
        (*ppMaterialAttrib)->AddRef();
    }

    CAUSTICAPI void CreateRenderMaterial(IGraphics* pGraphics, IMaterialAttrib* pMaterialAttrib, IShader* pShader, IRenderMaterial** ppRenderMaterial)
    {
        std::unique_ptr<CRenderMaterial> spRenderMaterial(new CRenderMaterial());
        CRenderMaterial* wpRenderMaterial = spRenderMaterial.get();
        spRenderMaterial->m_spMaterial = pMaterialAttrib;
        spRenderMaterial->m_spShader = pShader;
        if (pMaterialAttrib)
        {
            pMaterialAttrib->EnumerateTextures(
                [pGraphics, wpRenderMaterial](const wchar_t* pName, IImage* pImage) {
                    CRefObj<ITexture> spTexture;
                    Caustic::CCausticFactory::Instance()->CreateTexture(pGraphics, pImage, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_SHADER_RESOURCE, &spTexture);
                    wpRenderMaterial->SetTexture(pGraphics, pName, spTexture, EShaderAccess::PixelShader);
                });
        }

        *ppRenderMaterial = spRenderMaterial.release();
        (*ppRenderMaterial)->AddRef();

    }

    void CRenderMaterial::SetTexture(IGraphics* pGraphics, const wchar_t* pName, ITexture* pTexture, EShaderAccess access)
    {
        std::map<std::wstring, CRenderTexture>::iterator it = m_textures.find(pName);
        if (it != m_textures.end())
            m_textures.erase(it);
        CRenderTexture tex;
        tex.m_spTexture = pTexture;
        tex.m_access = access;
        CCausticFactory::Instance()->CreateSampler(pGraphics, tex.m_spTexture, &tex.m_spSampler);

        // pName is the name of our texture. We don't push textures to the shader, we push samplers. So rename it.
        std::wstring name(pName);
        size_t index = name.find(L"Texture");
        if (index != name.npos)
            name.replace(index, 7, L"Sampler");
        m_textures.insert(std::make_pair(name.c_str(), tex));
    }

    void CRenderMaterial::SetShader(IShader *pShader) 
    {
        m_spShader = pShader;
    }

    void CRenderMaterial::GetShader(IShader **ppShader)
    {
        if (m_spShader == nullptr)
        {
            CRefObj<IImage> spDiffuseTexture = m_spMaterial->GetTexture(L"diffuseTexture");
            if (spDiffuseTexture == nullptr)
                CShaderMgr::Instance()->FindShader(L"Default", &m_spShader);
            else
                CShaderMgr::Instance()->FindShader(L"Textured", &m_spShader);
        }
        *ppShader = m_spShader;
        if (m_spShader)
            (*ppShader)->AddRef();
    }

    void CRenderMaterial::Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx * /*pRenderCtx*/, IShader *spShader)
    {
        // First make sure defaults are set
        Float4 vAmbient(0.1f, 0.1f, 0.1f, 0.1f);
        Float4 vDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
        Float4 vSpecular(0.7f, 0.7f, 0.7f, 1.0f);
        Float4 vSpecularExp(60.0f, 60.0f, 60.0f, 60.0f);
        float transparency = 1.0f;
        spShader->SetPSParam(L"diffuseColor", std::any(vDiffuse));
        spShader->SetPSParam(L"specularColor", std::any(vSpecular));
        spShader->SetPSParam(L"specularExp", std::any(vSpecularExp));
        spShader->SetPSParam(L"transparency", std::any(Float4(transparency, transparency, transparency, transparency)));

        if (m_spMaterial)
        {
            m_spMaterial->EnumerateColors([spShader](std::wstring name, Vector3& v) {
                Float4 sv(v.x, v.y, v.z, 1.0);
                spShader->SetPSParam(name, std::any(sv));
                });
            m_spMaterial->EnumerateScalars([spShader](std::wstring name, float v) {
                Float4 sv(v, v, v, 1.0);
                spShader->SetPSParam(name, std::any(sv));
                });
            for (auto t : m_textures)
            {
                if (t.second.m_spTexture)
                    t.second.m_spTexture->Update(pGraphics);
                if (t.second.m_spSampler && (t.second.m_access == EShaderAccess::Both || t.second.m_access == EShaderAccess::PixelShader))
                    spShader->SetPSParam(t.first, std::any(CSamplerRef(t.second.m_spSampler)));
                if (t.second.m_spSampler && (t.second.m_access == EShaderAccess::Both || t.second.m_access == EShaderAccess::VertexShader))
                    spShader->SetVSParam(t.first, std::any(CSamplerRef(t.second.m_spSampler)));
            }
        }

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
    }
}
