//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include <Windows.h>

//**********************************************************************
// File: Caustic.h
// This file defines the published interface for talking to the Caustic Renderer.
// This include is usually the top level include that clients will use.
//**********************************************************************

// Namespace: Caustic
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
        virtual void Initialize(HWND hwnd) = 0;
        virtual void Shutdown() = 0;
        virtual void SetMaxCmdLength() = 0;
        virtual void GetRenderer(IRenderer **ppRenderer) = 0;
        virtual void LoadTexture(const wchar_t *pPath, ITexture **ppTexture) = 0;
        virtual void LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture) = 0;
        virtual void SetSceneGraph(ISceneGraph *pSceneGraph) = 0;
        virtual void SaveScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) = 0;
        virtual void LoadScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) = 0;
    };
}