//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"
#include <map>

namespace Caustic
{
    //**********************************************************************
    // Class: CRenderTexture
    // Internal class used to store texture and sampler information.
    //
    // Header:
    // [Link:Rendering/Caustic/RenderMaterial.h]
    //**********************************************************************
    struct CRenderTexture
    {
        EShaderAccess m_access;
        CRefObj<ITexture> m_spTexture;
        std::wstring m_samplerName;
        CRefObj<ISampler> m_spSampler;
    };

    //**********************************************************************
    // Class: CRenderMaterial
	// Defines a render material (material attributes, shaders, and textures).
	// This can be applied to a renderable object.
    //
    // Header:
    // [Link:Rendering/Caustic/RenderMaterial.h]
    //**********************************************************************
    class CRenderMaterial :
        public IRenderMaterial,
        public CRefCount
    {
    protected:
        CRefObj<IMaterialAttrib> m_spMaterial;
        CRefObj<IShader> m_spShader;
        std::map<std::wstring, CRenderTexture> m_textures;
    public:
        friend class CRenderer;
        friend CAUSTICAPI CRefObj<IRenderMaterial> CreateRenderMaterial(IRenderer* pRenderer, IMaterialAttrib *pMaterialAttrib, IShader *pShader);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderMaterial
        //**********************************************************************
        virtual void SetShader(IShader *pShader) override;
        virtual CRefObj<IShader> GetShader() override;
        virtual void SetMaterial(IMaterialAttrib *pMaterial) override { m_spMaterial = pMaterial; }
        virtual CRefObj<IMaterialAttrib> GetMaterial() override { return m_spMaterial; }
        virtual void SetTexture(IRenderer* pRenderer, const wchar_t *pName, ITexture* pTexture, EShaderAccess access) override;
        virtual CRefObj<ITexture> GetTexture(const wchar_t* pName) override {
            std::map<std::wstring, CRenderTexture>::iterator it = m_textures.find(pName);
            return CRefObj<ITexture>(it->second.m_spTexture);
        }
        virtual void Render(IRenderer* pRenderer, std::vector<CRefObj<ILight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) override;
    };
}
