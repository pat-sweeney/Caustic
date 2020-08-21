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
    CAUSTICAPI CRefObj<IRenderMaterial> CreateRenderMaterial(IGraphics* pGraphics, IMaterialAttrib* pMaterialAttrib, IShader* pShader)
    {
        std::unique_ptr<CRenderMaterial> spRenderMaterial(new CRenderMaterial());
        CRenderMaterial* wpRenderMaterial = spRenderMaterial.get();
        spRenderMaterial->SetMaterial(pMaterialAttrib);
        spRenderMaterial->SetShader(pShader);
        if (pMaterialAttrib)
        {
            pMaterialAttrib->EnumerateTextures(
                [pGraphics, wpRenderMaterial](const wchar_t* pName, IImage* pImage, EShaderAccess access) {
                    if (pImage != nullptr)
                    {
                        CRefObj<ITexture> spTexture = Caustic::CCausticFactory::Instance()->CreateTexture(pGraphics, pImage, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_SHADER_RESOURCE);
                        wpRenderMaterial->SetTexture(pGraphics, pName, spTexture, access);
                    }
                });
        }
        return CRefObj<IRenderMaterial>(spRenderMaterial.release());
    }

    void CRenderMaterial::SetTexture(IGraphics* pGraphics, const wchar_t* pName, ITexture* pTexture, EShaderAccess access)
    {
        std::map<std::wstring, CRenderTexture>::iterator it = m_textures.find(pName);
        if (it != m_textures.end())
            m_textures.erase(it);
        CRenderTexture tex;
        tex.m_spTexture = pTexture;
        tex.m_access = access;
        tex.m_spSampler = CCausticFactory::Instance()->CreateSampler(pGraphics, tex.m_spTexture);

        // pName is the name of our texture. We don't push textures to the shader, we push samplers. So rename it.
        tex.m_samplerName = std::wstring(pName);
        size_t index = tex.m_samplerName.find(L"Texture");
        if (index != tex.m_samplerName.npos)
            tex.m_samplerName.replace(index, 7, L"Sampler");
        m_textures.insert(std::make_pair(pName, tex));
    }

    void CRenderMaterial::SetShader(IShader *pShader) 
    {
        m_spShader = pShader;
    }

    CRefObj<IShader> CRenderMaterial::GetShader()
    {
        if (m_spShader == nullptr)
        {
            CT(E_NOTIMPL);
//            CRefObj<IImage> spDiffuseTexture = m_spMaterial->GetTexture(L"diffuseTexture");
//            if (spDiffuseTexture == nullptr)
//                CShaderMgr::Instance()->FindShader(L"Default", &m_spShader);
//            else
//                CShaderMgr::Instance()->FindShader(L"Textured", &m_spShader);
        }
        return m_spShader;
    }

    void CRenderMaterial::Render(IGraphics *pGraphics, std::vector<CRefObj<ILight>> &lights, IRenderCtx * /*pRenderCtx*/, IShader *spShader)
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
            m_spMaterial->EnumerateColors([spShader](std::wstring name, FRGBColor& v) {
                Float4 sv(v.r, v.g, v.b, 1.0);
                spShader->SetPSParam(name, std::any(sv));
                });
            m_spMaterial->EnumerateScalars([spShader](std::wstring name, float v) {
                Float4 sv(v, v, v, 1.0);
                spShader->SetPSParam(name, std::any(sv));
                });
            for (auto t : m_textures)
            {
                if (t.second.m_spTexture)
                {
                    t.second.m_spTexture->Update(pGraphics);
                    if (t.second.m_access == EShaderAccess::Both || t.second.m_access == EShaderAccess::PixelShader)
                        spShader->SetPSParam(t.first, std::any(t.second.m_spTexture));
                    if (t.second.m_access == EShaderAccess::Both || t.second.m_access == EShaderAccess::VertexShader)
                        spShader->SetVSParam(t.first, std::any(t.second.m_spTexture));
                }
                if (t.second.m_spSampler && (t.second.m_access == EShaderAccess::Both || t.second.m_access == EShaderAccess::PixelShader))
                    spShader->SetPSParam(t.second.m_samplerName, std::any(CSamplerRef(t.second.m_spSampler)));
                if (t.second.m_spSampler && (t.second.m_access == EShaderAccess::Both || t.second.m_access == EShaderAccess::VertexShader))
                    spShader->SetVSParam(t.second.m_samplerName, std::any(CSamplerRef(t.second.m_spSampler)));
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
            FRGBColor lightColor = lights[i]->GetColor();
            v.x = lightColor.r;
            v.y = lightColor.g;
            v.z = lightColor.b;
            v.w = 0.0f;
            spShader->SetPSParam(L"lightColor", i, std::any(v));
        }
    }
}
