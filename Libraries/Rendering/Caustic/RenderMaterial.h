//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"

namespace Caustic
{
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
        CRefObj<ITexture> m_spAmbientTexture;
        CRefObj<ISampler> m_spAmbientSampler;
        CRefObj<ITexture> m_spDiffuseTexture;
        CRefObj<ISampler> m_spDiffuseSampler;
        CRefObj<ITexture> m_spSpecularTexture;
        CRefObj<ISampler> m_spSpecularSampler;
    public:
        friend class CRenderer;

		CRenderMaterial(IMaterialAttrib *pMaterialAttrib, IShader *pShader)
		{
			m_spMaterial = pMaterialAttrib;
			m_spShader = pShader;
		}

		CRenderMaterial(IMaterialAttrib *pMaterialAttrib, IShader *pShader,
			ITexture *pAmbientTexture, ISampler *pAmbientSampler,
			ITexture *pDiffuseTexture, ISampler *pDiffuseSampler,
			ITexture *pSpecularTexture, ISampler *pSpecularSampler)
		{
			m_spMaterial = pMaterialAttrib;
			m_spShader = pShader;
		}

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
            *ppMaterial = m_spMaterial.p;
            if (m_spMaterial.p)
                (*ppMaterial)->AddRef();
        }
        virtual void SetDiffuseTexture(IRenderer *pRenderer, ITexture *pTexture) override;
		virtual CRefObj<ITexture> GetDiffuseTexture() override;
		virtual void SetSpecularTexture(IRenderer *pRenderer, ITexture *pTexture) override;
		virtual CRefObj<ITexture> GetSpecularTexture() override;
		virtual void SetAmbientTexture(IRenderer *pRenderer, ITexture *pTexture) override;
		virtual CRefObj<ITexture> GetAmbientTexture() override;
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) override;
    };
}
