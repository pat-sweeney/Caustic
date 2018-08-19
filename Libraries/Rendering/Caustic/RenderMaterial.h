//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"

namespace Caustic
{
    //**********************************************************************
    //! \brief CRenderable defines a self contained renderable object
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
        friend void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial);

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
        virtual void SetDiffuseTexture(IGraphics *pGraphics, ITexture *pTexture) override;
        virtual void SetSpecularTexture(IGraphics *pGraphics, ITexture *pTexture) override;
        virtual void SetAmbientTexture(IGraphics *pGraphics, ITexture *pTexture) override;
        virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) override;
    };
}
