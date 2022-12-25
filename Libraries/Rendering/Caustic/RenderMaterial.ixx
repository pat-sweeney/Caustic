//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <map>
#include <string>

export module Rendering.Caustic.RenderMaterial;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.IShader;
import Geometry.Mesh.IMaterialAttrib;

export namespace Caustic
{
    //**********************************************************************
    // Class: CRenderTexture
    // Internal class used to store texture and sampler information.
    //
    // Module:
    // {Link:import Rendering.Caustic.RenderMaterial;{Rendering/Caustic/RenderMaterial.ixx}}
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
    // Module:
    // {Link:import Rendering.Caustic.RenderMaterial;{Rendering/Caustic/RenderMaterial.ixx}}
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
        friend CRefObj<IRenderMaterial> CreateRenderMaterial(IRenderer* pRenderer, IMaterialAttrib *pMaterialAttrib, IShader *pShader);

        CRenderMaterial()
        {
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
