//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <atlbase.h>
#include <d3d11.h>
#include <vector>
#include <any>
#include <map>

export module Rendering.Caustic.ShaderMgr;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderInfo;
import Rendering.Caustic.IShaderMgr;

// A shader is a object that manages the vertex and pixel shader
//
export namespace Caustic
{

    //**********************************************************************
    // Class: CShaderMgr
    // Defines our shader manager. Each shader is registered with this manager.
    //
    // Module:
    // {Link:import Rendering.Caustic.Shader;{Rendering/Caustic/Shader.ixx}}
    //**********************************************************************
    class CShaderMgr : public IShaderMgr, public CRefCount
    {
        std::map<std::wstring, CRefObj<IShader>> m_shaders; // List of registered shaders
    public:
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IShaderMgr
        //**********************************************************************
        virtual CRefObj<IShader> FindShader(const wchar_t* pShaderName) override;
        virtual void RegisterShader(const wchar_t* pShaderName, IShader* pShader) override;
    };
}