//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include <vector>

//**********************************************************************
// File: IRenderMaterial.h
// This file defines the published interface for render materials.
//**********************************************************************

namespace Caustic
{
    struct IShader;
    struct IMaterialAttrib;
    struct IGraphics;
    struct ITexture;
    struct IPointLight;
    struct IRenderCtx;

    //**********************************************************************
    // Interface: IRenderMaterial
    // Defines a material that has its associated
    // render element (as opposed to an IMaterialAttrib which only defines
    // the attributes of the material).
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
        // pGraphics - graphics device
        // name - name of texture (as referenced by the shader)
        // pTexture - texture
        //**********************************************************************
        virtual void SetTexture(IGraphics* pGraphics, const wchar_t *pName, ITexture* pTexture, EShaderAccess access) = 0;

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
        // pGraphics - graphics device
        // lights - list of lights to use
        // pRenderCtx - render context to use
        // pOverrideShader - shader override (shader to use instead of material's shader)
        //**********************************************************************
        virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) = 0;
    };
}