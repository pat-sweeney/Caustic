//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Rendering.Caustic.IRenderMaterial;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.Caustic.ITexture;

//**********************************************************************
// File: IRenderMaterial.ixx
// This file defines the published interface for render materials.
//**********************************************************************

export namespace Caustic
{
    struct IShader;
    //struct IMaterialAttrib;
    struct ILight;
    struct IRenderCtx;
    struct IRenderer;

    //**********************************************************************
    // Interface: IRenderMaterial
    // Defines a material that has its associated
    // render element (as opposed to an IMaterialAttrib which only defines
    // the attributes of the material).
    //
    // Module:
    // {Link:import Rendering.Caustic.IRenderMaterial;{Rendering/Caustic/IRenderMaterial.ixx}}
    //**********************************************************************
    struct IRenderMaterial : public IRefCount
    {
        //**********************************************************************
        // Method: SetShader
        // Sets shader to use for this material
        //
        // Parameters:
        // pShader - shader
        //**********************************************************************
        virtual void SetShader(IShader *pShader) = 0;
        
        //**********************************************************************
        // Method: GetShader
        // Gets shader used by this material
        //
        // Returns:
        // Returns the shader
        //**********************************************************************
        virtual CRefObj<IShader> GetShader() = 0;

        //**********************************************************************
        // Method: SetMaterial
        // Sets the material definition
        //
        // Parameters:
        // pMaterial - material definition
        //**********************************************************************
        virtual void SetMaterial(IMaterialAttrib *pMaterial) = 0;

        //**********************************************************************
        // Method: GetMaterial
        // Returns the material definition used by this material
        //
        // Returns:
        // Returns material definition
        //**********************************************************************
        virtual CRefObj<IMaterialAttrib> GetMaterial() = 0;

        //**********************************************************************
        // Method: SetTexture
        // Sets a texture
        //
        // Parameters:
        // pRenderer - graphics device
        // name - name of texture (as referenced by the shader)
        // pTexture - texture
        //**********************************************************************
        virtual void SetTexture(IRenderer *pRenderer, const wchar_t *pName, ITexture* pTexture, EShaderAccess access) = 0;

        //**********************************************************************
        // Method: GetTexture
        // Returns the texture with the specified name
        //
        // Parameters:
        // name - name of texture (as referenced by the shader)
        //
        // Returns:
        // Returns the associated texture
        //**********************************************************************
        virtual CRefObj<ITexture> GetTexture(const wchar_t* pName) = 0;
        
        //**********************************************************************
        // Method: Render
        // Renders the material (pushes it to the shader)
        //
        // Parameters:
        // pRenderer - graphics device
        // lights - list of lights to use
        // pRenderCtx - render context to use
        // pOverrideShader - shader override (shader to use instead of material's shader)
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, std::vector<CRefObj<ILight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) = 0;
    };

    CRefObj<IRenderMaterial> CreateRenderMaterial(IRenderer* pRenderer, IMaterialAttrib* pMaterialAttrib, IShader* pShader);
}