//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include <vector>

//**********************************************************************
// File: Caustic.h
// This file defines the published interface for talking to the Caustic Renderer.
// This include is usually the top level include that clients will use.
//**********************************************************************

// Namespace: Caustic
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
        virtual void SetShader(IShader *pShader) = 0;
        virtual void GetShader(IShader **ppShader) = 0;
        virtual void SetMaterial(IMaterialAttrib *pMaterial) = 0;
        virtual void GetMaterial(IMaterialAttrib **ppMaterial) = 0;
        virtual void SetDiffuseTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;
        virtual void SetSpecularTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;
        virtual void SetAmbientTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;
        virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) = 0;
    };
}