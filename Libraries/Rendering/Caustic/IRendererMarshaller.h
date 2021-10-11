//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
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
    // The renderer is on a separate thread where the client is calling from.
    //
    // Header:
    // [Link:Rendering/Caustic/IRendererMarshaller.h]
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
        // prePresentCallback - callback function that is called right before Present()
        // startFrozen - Should renderer be started in a frozen state?
        // desktopIndex - index indicating which desktop to use with duplication service
        //**********************************************************************
        virtual void Initialize(HWND hwnd, std::wstring &shaderFolder, 
            std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback,
            bool startFrozen = false, int desktopIndex = 0) = 0;

        //**********************************************************************
        // Method: Shutdown
        // Shuts down the renderer
        //**********************************************************************
        virtual void Shutdown() = 0;

        //**********************************************************************
        // Method: RunOnRenderer
        // Runs the specified callback function on the renderer's thread
        //
        // Parameters:
        // callback - function to run
        //**********************************************************************
        virtual void RunOnRenderer(std::function<void(IRenderer*)> callback, bool wait = false) = 0;

        //**********************************************************************
        // Method: LoadTexture
        // Loads a texture from the specified path
        //
        // Parameters:
        // pPath - path to texture file
        //
        // Returns:
        // Returns the created texture
        //**********************************************************************
        virtual CRefObj<ITexture> LoadTexture(const wchar_t *pPath) = 0;

        //**********************************************************************
        // Method: LoadVideoTexture
        // Loads a video texture from the specified path
        //
        // Parameters:
        // pPath - path to video texture file
        //
        // Returns:
        // Returns the created texture
        //**********************************************************************
        virtual CRefObj<ITexture> LoadVideoTexture(const wchar_t *pPath) = 0;

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

        //**********************************************************************
        // Method: GetRenderer
        // Returns the renderer this object is marshalling calls to
        //
        // Returns:
        // Renderer this object is marshalling calls to
        //**********************************************************************
        virtual CRefObj<IRenderer> GetRenderer() = 0;
    };

    //**********************************************************************
    // Function: CreateRendererMarshaller
    // Global function for creating the RendererMarshaller. This method should generally
    // not be called. Use the ICausticFactory to create new Caustic objects.
    //
    // Returns:
    // Returns the newly created RendererMarshaller.
    //
    // Header:
    // [Link:Rendering/Caustic/IRendererMarshaller.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<IRendererMarshaller> CreateRendererMarshaller();
}
