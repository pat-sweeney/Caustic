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
        friend CAUSTICAPI void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderMaterial
        //**********************************************************************
        virtual void SetShader(IShader *pShader) override;
        virtual void GetShader(IShader **ppShader) override;
        virtual void SetMaterial(IMaterialAttrib *pMaterial) override { m_spMaterial = pMaterial; }
        virtual void GetMaterial(IMaterialAttrib **ppMaterial) override
        {
            *ppMaterial = m_spMaterial;
            if (m_spMaterial)
                (*ppMaterial)->AddRef();
        }
        virtual void SetTexture(IGraphics* pGraphics, const wchar_t *pName, ITexture* pTexture, EShaderAccess access) override;
        virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) override;
    };
}
