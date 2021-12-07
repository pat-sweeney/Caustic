//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.IShaderMgr;
import Base.Core.Core;
import Base.Core.IRefCount;
import Rendering.Caustic.IShader;

//**********************************************************************
// File: IShaderMgr.ixx
// This file defines the published interface for shader manager.
// Each shader in use by the system is registered with this manager.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: IShaderMgr
    // Manages shaders within the renderer
    //
    // Module:
    // {Link:import Rendering.Caustic.IShaderMgr;{Rendering/Caustic/IShaderMgr.ixx}}
    //**********************************************************************
    struct IShaderMgr : public IRefCount
    {
        //**********************************************************************
        // Method: FindShader
        // Finds an existing shader by name
        //
        // Parameters:
        // pShaderName - Name of shader to find.
        //
        // Returns:
        // Returns the found shader. Nullptr if no matching shader is found.
        //**********************************************************************
        virtual CRefObj<IShader> FindShader(const wchar_t *pShaderName) = 0;

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
