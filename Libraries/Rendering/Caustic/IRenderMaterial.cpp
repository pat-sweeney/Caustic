//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <memory>

module Rendering.Caustic.IRenderMaterial;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.MaterialAttrib;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.Caustic.Renderer;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.RenderMaterial;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.ITexture;
//import Geometry.Mesh.IMaterialAttrib;

namespace Caustic
{
    struct IShader;

    CRefObj<IRenderMaterial> CreateRenderMaterial(IRenderer* pRenderer, IMaterialAttrib* pMaterialAttrib, IShader* pShader)
    {
        std::unique_ptr<CRenderMaterial> spRenderMaterial(new CRenderMaterial());
        CRenderMaterial* wpRenderMaterial = spRenderMaterial.get();
        spRenderMaterial->SetMaterial(pMaterialAttrib);
        spRenderMaterial->SetShader(pShader);
        if (pMaterialAttrib)
        {
            pMaterialAttrib->EnumerateTextures(
                [pRenderer, wpRenderMaterial](const wchar_t* pName, IImage* pImage, EShaderAccess access) {
                    if (pImage != nullptr)
                    {
                        CRefObj<ITexture> spTexture = Caustic::CCausticFactory::Instance()->CreateTexture(pRenderer, pImage, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_SHADER_RESOURCE);
                        wpRenderMaterial->SetTexture(pRenderer, pName, spTexture, access);
                    }
                });
        }

        return CRefObj<IRenderMaterial>(spRenderMaterial.release());
    }
}
