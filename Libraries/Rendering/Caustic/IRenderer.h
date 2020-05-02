//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Rendering\Caustic\IGraphics.h"
#include "Geometry\Mesh\IMesh.h"
#include "Rendering\Caustic\ITexture.h"
#include "Rendering\Caustic\IRenderCtx.h"
#include "Rendering\Caustic\IPointLight.h"
#include "Rendering\Caustic\IShader.h"
#include <functional>

//**********************************************************************
// File: IRenderer.h
// This file defines the published interface for the renderer.
//**********************************************************************

namespace Caustic
{
    struct ISceneGraph;
    struct IRenderSubMesh;

    //**********************************************************************
    // Interface: IRenderer 
    // Defines our basic renderer. IRenderer handles all rendering commands.
    // It is generally expected that this object is running on its own thread.
    //**********************************************************************
    struct IRenderer : public IGraphics
    {
        //**********************************************************************
        // Method: Setup
        // Initializes the renderer. Applications need to call this method at startup.
        //
        // Parameters:
        // hwnd - window to bind renderer to
        // shaderFolder - path to shader folder
        // createDebugDevice - True if application wants the debug D3D device. False otherwise.
        //**********************************************************************
        virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) = 0;

        //**********************************************************************
        // Method: DrawMesh
        // Renders a mesh by adding it to the Renderable list that is drawn each time a frame is rendered
        //
        // TODO: This method needs to be refactored. There is no point in sending in both a pMaterial
        // and a pTexture (the material already should define the diffuse texture). Also, we should pass
        // in both a front and back face material
        //
        // Parameters:
        // pMesh - mesh to render
        // pMaterial - material for front faces
        // pTexture - texture for diffuse component
        // pShader - shader to use to draw mesh
        // mat - matrix to transform mesh by
        //**********************************************************************
        virtual void DrawMesh(IRenderSubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) = 0;

        //**********************************************************************
        // Method: RenderLoop
        // Defines the main entry point for our renderer
        //
        // Parameters:
        // renderCallback - render callback
        //**********************************************************************
        virtual void RenderLoop(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) = 0;

        //**********************************************************************
        // Method: RenderFrame
        // Renders the next frame
        //
        // Parameters:
        // renderCallback - callback for rendering custom objects
        //**********************************************************************
        virtual void RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) = 0;

        //**********************************************************************
        // Method: SetCamera
        // Assigns a camera to the renderer
        //
        // Parameters:
        // pCamera - camera to use when rendering
        //**********************************************************************
        virtual void SetCamera(ICamera *pCamera) = 0;

        //**********************************************************************
        // Method: AddPointLight
        // Adds a point light which the renderer uses
        //
        // Parameters:
        // pLight - light to render meshes with
        //**********************************************************************
        virtual void AddPointLight(IPointLight *pLight) = 0;

        //**********************************************************************
        // Method: GetRenderCtx
        // Returns the current render context
        //**********************************************************************
        virtual CRefObj<IRenderCtx> GetRenderCtx() = 0;

        //**********************************************************************
        // Method: DrawLine
        // Renders a line
        //
        // Parameters:
        // p1 - starting point of line
        // p2 - ending point of line
        // clr - color to use while rendering line
        //**********************************************************************
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) = 0;

        //**********************************************************************
        // Method: GetGraphics
        // Returns the current graphics device the renderer is using
        //**********************************************************************
        virtual CRefObj<IGraphics> GetGraphics() = 0;
    };
}
