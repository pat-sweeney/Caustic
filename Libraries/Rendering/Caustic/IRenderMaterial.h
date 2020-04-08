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
        // Parameters:
        // ppShader - Returns the shader
        //**********************************************************************
        virtual void GetShader(IShader **ppShader) = 0;

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
        // Parameters:
        // ppMaterial - returns material definition
        //**********************************************************************
        virtual void GetMaterial(IMaterialAttrib **ppMaterial) = 0;

        //**********************************************************************
        // Method: SetDiffuseTexture
        // Sets the diffuse texture
        //
        // Parameters:
        // pGraphics - graphics device
        // pTexture - texture
        //**********************************************************************
        virtual void SetDiffuseTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;

        //**********************************************************************
        // Method: SetSpecularTexture
        // Sets the specular texture
        //
        // Parameters:
        // pGraphics - graphics device
        // pTexture - texture
        //**********************************************************************
        virtual void SetSpecularTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;

        //**********************************************************************
        // Method: SetAmbientTexture
        // Sets the ambient texture
        //
        // Parameters:
        // pGraphics - graphics device
        // pTexture - texture
        //**********************************************************************
        virtual void SetAmbientTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;

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