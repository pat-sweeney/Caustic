//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Rendering\Caustic\IShader.h"

//**********************************************************************
// File: Caustic.h
// This file defines the published interface for talking to the Caustic Renderer.
// This include is usually the top level include that clients will use.
//**********************************************************************

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Interface: IShaderMgr
    // Manages shaders within the renderer
    //**********************************************************************
    struct IShaderMgr : public IRefCount
    {
        //**********************************************************************
        // Method: FindShader
        // Finds an existing shader by name
        //
        // Parameters:
        // pShaderName - Name of shader to find.
        // ppShader - Returns the found shader. Nullptr if no matching shader is found.
        //**********************************************************************
        virtual void FindShader(const wchar_t *pShaderName, IShader **ppShader) = 0;

        //**********************************************************************
        // Method: RegisterShader
        // Registers a new shader
        //
        // Parameters:
        // pShaderName - Name of shader
        // pShader - Shader to register
        //**********************************************************************
        virtual void RegisterShader(const wchar_t *pShaderName, IShader *pShader) = 0;
    };
}
