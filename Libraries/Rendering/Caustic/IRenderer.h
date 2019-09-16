//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Rendering\Caustic\IGraphics.h"
#include "Geometry\Mesh\IMesh.h"
#include "Rendering\Caustic\ITexture.h"
#include "Rendering\Caustic\IRenderCtx.h"
#include "Rendering\Caustic\IPointLight.h"

//**********************************************************************
// File: Caustic.h
// This file defines the published interface for talking to the Caustic Renderer.
// This include is usually the top level include that clients will use.
//**********************************************************************

// Namespace: Caustic
namespace Caustic
{
    struct ISceneGraph;

    //**********************************************************************
    // Interface: IRenderer 
    // Defines the interface clients use to talk to the renderer
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
        virtual void DrawMesh(ISubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) = 0; // Draws a mesh
        virtual void RenderLoop() = 0; // Renderer entry point
        virtual void RenderFrame() = 0; // Have renderer draw and present next frame
        virtual void SetCamera(ICamera *pCamera) = 0; // Sets camera
        virtual void AddPointLight(IPointLight *pLight) = 0;
        virtual void GetRenderCtx(IRenderCtx **ppCtx) = 0;
        virtual void SetSceneGraph(ISceneGraph *pSceneGraph) = 0;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) = 0;
        virtual void GetGraphics(IGraphics **ppGraphics) = 0;
    };
}
