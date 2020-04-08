//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

//**********************************************************************
// File: IRenderable.h
// This file defines the published interface for renderables.
// A Renderable is an mesh, front and back materials, and a shader. The renderer
// keeps a simple list of these objects which are rendered at each call
// to <IRenderer::RenderFrame>. The purpose of this object is to allow
// the renderer to render a set of objects without forcing the client
// to maintain an entire scene graph.
//**********************************************************************

namespace Caustic
{
    struct IGraphics;
    struct IPointLight;
    struct IRenderCtx;

    //**********************************************************************
    // Interface: IRenderable
    // A Renderable is an mesh, front and back materials, and a shader. The renderer
    // keeps a simple list of these objects which are rendered at each call
    // to <IRenderer::RenderFrame>. The purpose of this object is to allow
    // the renderer to render a set of objects without forcing the client
    // to maintain an entire scene graph.
    //**********************************************************************
    struct IRenderable : public IRefCount
    {
        //**********************************************************************
        // Method: GetPos
        // Returns the world position of the object
        //**********************************************************************
        virtual Vector3 GetPos() = 0;

        //**********************************************************************
        // Method: Render
        // Called by the renderer to render this object
        //
        // Parameters:
        // pGraphics - Graphics device
        // lights - List of current lights effecting this object
        // pRenderCtx - Current render context
        //**********************************************************************
        virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx) = 0;

        //**********************************************************************
        // Method: SetTransform
        // Sets the transform on the object
        //
        // Parameters:
        // mat - transform to set
        //**********************************************************************
        virtual void SetTransform(DirectX::XMMATRIX& mat) = 0;

        //**********************************************************************
        // Method: GetTransform
        // Returns the transforms associated with the object
        //**********************************************************************
        virtual DirectX::XMMATRIX& GetTransform() = 0;

        //**********************************************************************
        // Method: SetSubMesh
        // Sets the submesh
        //
        // Parameters:
        // pSubMesh - new mesh
        //**********************************************************************
        virtual void SetSubMesh(IRenderSubMesh* pSubMesh) = 0;

        //**********************************************************************
        // Method: GetSubMesh
        // Returns the submesh associated with this oject
        //**********************************************************************
        virtual CRefObj<IRenderSubMesh> GetSubMesh() = 0;

        //**********************************************************************
        // Method: SetFrontMaterial
        // Sets the front face material
        //
        // Parameters:
        // pMaterial - material
        //**********************************************************************
        virtual void SetFrontMaterial(IRenderMaterial* pMaterial) = 0;

        //**********************************************************************
        // Method: GetFrontMaterial
        // Returns the front face material associated with this oject
        //**********************************************************************
        virtual CRefObj<IRenderMaterial> GetFrontMaterial() = 0;

        //**********************************************************************
        // Method: SetBackMaterial
        // Sets the back face material
        //
        // Parameters:
        // pMaterial - material
        //**********************************************************************
        virtual void SetBackMaterial(IRenderMaterial* pMaterial) = 0;

        //**********************************************************************
        // Method: GetBackMaterial
        // Returns the back face material associated with this oject
        //**********************************************************************
        virtual CRefObj<IRenderMaterial> GetBackMaterial() = 0;

        //**********************************************************************
        // Method: InPass
        // Returns whether the object is part of the specified pass
        //
        // Parameters:
        // pass - render pass
        //
        // Returns:
        // Returns True if object is part of the specified pass. False otherwise.
        //**********************************************************************
        virtual bool InPass(int pass) = 0;
    };
}
