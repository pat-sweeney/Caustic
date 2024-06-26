//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <Windows.h>
#include <functional>
#include <string>
#include <cinttypes>

export module Rendering.RendererMarshaller.IRendererMarshaller;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IVideoTexture;
import Rendering.Caustic.ITexture;

//**********************************************************************
// File: IRendererMarshaller.ixx
// This file defines the published interface for talking to marshalled version
// of the renderer. The marshalled version allows for IRenderer methods to be called
// from threads other than the render thread.
//**********************************************************************

export namespace Caustic
{
    struct ISceneGraph;

    //**********************************************************************
    // Interface: IRendererMarshaller
    // Creates a renderer that runs on its own thread and method calls
    // are marshalled from the client to render thread.
    // The renderer is on a separate thread where the client is calling from.
    //
    // Module:
    // {Link:import Rendering.Caustic.IRendererMarshaller;{Rendering/Caustic/IRendererMarshaller.ixx}}
    //**********************************************************************
    struct IRendererMarshaller : public IRefCount
    {
        //**********************************************************************
        // Method: Initialize
        // Initializes the renderer
        //
        // Parameters:
        // hwnd - Window associated with the renderer
        // viewport - render viewport for final render
        // shaderFolder - path to directory containing shaders
        // renderCallback - callback function that is called each time a new frame is rendered
        // prePresentCallback - callback function that is called right before Present()
        // startFrozen - Should renderer be started in a frozen state?
        // desktopIndex - index indicating which desktop to use with duplication service
        //**********************************************************************
        virtual void Initialize(HWND hwnd, BBox2 &viewport, std::wstring& shaderFolder,
            std::function<void(IRenderer* pRenderer)> postCreate,
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
        virtual CRefObj<ITexture> LoadTexture(const wchar_t* pPath) = 0;

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
        virtual CRefObj<IVideoTexture> LoadVideoTexture(const wchar_t* pPath) = 0;

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
        virtual void SaveScene(const wchar_t* pFilename, ISceneGraph* pSceneGraph) = 0;

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
        virtual void LoadScene(const wchar_t* pFilename, ISceneGraph* pSceneGraph) = 0;

        //**********************************************************************
        // Method: GetRenderer
        // Returns the renderer this object is marshalling calls to
        //
        // Returns:
        // Renderer this object is marshalling calls to
        //**********************************************************************
        virtual CRefObj<IRenderer> GetRenderer() = 0;
    };

    CRefObj<IRendererMarshaller> CreateRendererMarshaller();
}
