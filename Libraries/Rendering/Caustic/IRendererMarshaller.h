//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include <Windows.h>
#include <functional>

//**********************************************************************
// File: IRendererMarshaller.h
// This file defines the published interface for talking to marshalled version
// of the renderer. The marshalled version allows for IRenderer methods to be called
// from threads other than the render thread.
//**********************************************************************

namespace Caustic
{
    struct IRenderer;
    struct ITexture;
    struct ISceneGraph;
    
    //**********************************************************************
    // Interface: IRendererMarshaller
    // Creates a renderer that runs on its own thread and method calls
    // are marshalled from the client to render thread.
    // renderer is on a separate thread where the client is calling from)
    //**********************************************************************
    struct IRendererMarshaller : public IRefCount
    {
        //**********************************************************************
        // Method: Initialize
        // Initializes the renderer
        //
        // Parameters:
        // hwnd - Window associated with the renderer
        // shaderFolder - path to directory containing shaders
        // renderCallback - callback function that is called each time a new frame is rendered
        //**********************************************************************
        virtual void Initialize(HWND hwnd, std::wstring &shaderFolder, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) = 0;

        //**********************************************************************
        // Method: Shutdown
        // Shuts down the renderer
        //**********************************************************************
        virtual void Shutdown() = 0;

        //**********************************************************************
        // Method: GetRenderer
        // Returns the underlying renderer
        //
        // Parameters:
        // ppRenderer - returns the renderer
        //**********************************************************************
        virtual void GetRenderer(IRenderer **ppRenderer) = 0;

        //**********************************************************************
        // Method: LoadTexture
        // Loads a texture from the specified path
        //
        // Parameters:
        // pPath - path to texture file
        // pPTexture - Returns the created texture
        //**********************************************************************
        virtual void LoadTexture(const wchar_t *pPath, ITexture **ppTexture) = 0;

        //**********************************************************************
        // Method: LoadVideoTexture
        // Loads a video texture from the specified path
        //
        // Parameters:
        // pPath - path to video texture file
        // pPTexture - Returns the created texture
        //**********************************************************************
        virtual void LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture) = 0;

        //**********************************************************************
        // Method: SaveScene
        // Saves the scene graph to the specified file
        //
        // TODO: This method is obsolete and will be removed (it doesn't belong here)
        //
        // Parameters:
        // pFilename - file to save scene graph to
        // pSceneGraph - scene graph to save
        //**********************************************************************
        virtual void SaveScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) = 0;

        //**********************************************************************
        // Method: SetSceneGraph
        // Loads the scene graph from the specified file
        //
        // TODO: This method is obsolete and will be removed (it doesn't belong here)
        //
        // Parameters:
        // pFilename - file to load scene graph from
        // pSceneGraph - scene graph to load into
        //**********************************************************************
        virtual void LoadScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) = 0;
    };
}