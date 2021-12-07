//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <map>

module Rendering.Caustic.ShaderMgr;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IShader;

namespace Caustic
{
    //**********************************************************************
    // Method: FindShader
    // Returns the requested shader
    //
    // Parameters:
    // pShaderName - Name of shader to locate
    //
    // Returns:
    // Returns the requested shader
    //
    // Module:
    // {Link:import Rendering.Caustic.ShaderMgr;{Rendering/Caustic/ShaderMgr.cpp}}
    //**********************************************************************
    CRefObj<IShader> CShaderMgr::FindShader(const wchar_t* pShaderName)
    {
        std::map<std::wstring, CRefObj<IShader>>::iterator it = m_shaders.find(pShaderName);
        if (it == m_shaders.end())
            return CRefObj<IShader>(nullptr);
        return it->second;
    }

    //**********************************************************************
    // Method: RegisterShader
    // Registers the specified shader with the shader manager
    //
    // Parameters:
    // pShaderName - Name of shader
    // pShader - Shader to register
    // 
    // Module:
    // {Link:import Rendering.Caustic.ShaderMgr;{Rendering/Caustic/ShaderMgr.cpp}}
    //**********************************************************************
    void CShaderMgr::RegisterShader(const wchar_t* pShaderName, IShader* pShader)
    {
        std::map<std::wstring, CRefObj<IShader>>::iterator it = m_shaders.find(pShaderName);
        if (it == m_shaders.end())
        {
            m_shaders[std::wstring(pShaderName)] = CRefObj<IShader>(pShader);
        }
    }
};
