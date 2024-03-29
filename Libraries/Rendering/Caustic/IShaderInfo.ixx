//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <d3d11.h>

export module Rendering.Caustic.IShaderInfo;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;

//**********************************************************************
// File: IShaderInfo.ixx
// This file defines the published interface for IShaderInfo.
// Each shader has an associated IShaderInfo that defines which parameters
// and vertex formats the shader uses.
//**********************************************************************

export namespace Caustic
{
    struct IShader;
    struct ShaderParamDef;

    //**********************************************************************
    // Enum: EShaderType
    // Defines the various types of shaders
    //
    // Module:
    // {Link:import Rendering.Caustic.IShaderInfo;{Rendering/Caustic/IShaderInfo.ixx}}
    //**********************************************************************
    enum EShaderType
    {
        TypePixelShader = 0x1,
        TypeVertexShader = 0x2,
        TypeGeometryShader = 0x4,
        TypeHullShade = 0x8,
        TypeDomainShader = 0x10,
        TypeComputeShader = 0x20
    };

    //**********************************************************************
    // Interface: IShaderInfo
    // Defines primary interface used for getting information about pre-compiled
    // shaders. This isn't the shader itself. It is simply the data that we
    // load from disk to create a shader (i.e. data from .shi file)
    //
    // Module:
    // {Link:import Rendering.Caustic.IShaderInfo;{Rendering/Caustic/IShaderInfo.ixx}}
    //**********************************************************************
    struct IShaderInfo : public IRefCount
    {
        //**********************************************************************
        // Method: PSUsesVariable
        // Returns whether the Pixel shader uses the specified variable
        //
        // Parameters:
        // pName - name of variable
        //**********************************************************************
        virtual bool PSUsesVariable(const wchar_t *pName) = 0;

        //**********************************************************************
        // Method: VSUsesVariable
        // Returns whether the Vertex shader uses the specified variable
        //
        // Parameters:
        // pName - name of variable
        //**********************************************************************
        virtual bool VSUsesVariable(const wchar_t *pName) = 0;

        //**********************************************************************
        // Method: HasShader
        // Returns:
        // Returns whether the Caustic shader contains the specified D3D shader type
        //**********************************************************************
        virtual bool HasShader(EShaderType type) = 0;

        //**********************************************************************
        // Method: GetTopologyType
        // Returns:
        // Returns the type of mesh this shader renders (triangles/points/lines/etc)
        //**********************************************************************
        virtual D3D11_PRIMITIVE_TOPOLOGY GetTopologyType() = 0;

        //**********************************************************************
        // Method: PixelShaderParameterDefs
        // Returns:
        // Returns list of pixel parameter definitions that the shader uses
        //**********************************************************************
        virtual std::vector<ShaderParamDef>& PixelShaderParameterDefs() = 0;

        //**********************************************************************
        // Method: VertexShaderParameterDefs
        // Returns:
        // Returns list of vertex parameter definitions that the shader uses
        //**********************************************************************
        virtual std::vector<ShaderParamDef>& VertexShaderParameterDefs() = 0;

        //**********************************************************************
        // Method: ComputeShaderParameterDefs
        // Returns:
        // Returns list of compute parameter definitions that the shader uses
        //**********************************************************************
        virtual std::vector<ShaderParamDef>& ComputeShaderParameterDefs() = 0;

        //**********************************************************************
        // Method: GetPixelShaderParameterDef
        // Returns information about a specific variable found in the shader
        // 
        // Parameters:
        // paramName - name of parameter
        // pDef - parameter definition info
        // 
        // Returns:
        // True if parameter is found. Otherwise false.
        //**********************************************************************
        virtual bool GetPixelShaderParameterDef(const wchar_t* pParamName, ShaderParamDef* pDef) = 0;
        
        //**********************************************************************
        // Method: GetVertexShaderParameterDef
        // Returns information about a specific variable found in the shader
        // 
        // Parameters:
        // paramName - name of parameter
        // pDef - parameter definition info
        // 
        // Returns:
        // True if parameter is found. Otherwise false.
        //**********************************************************************
        virtual bool GetVertexShaderParameterDef(const wchar_t* pParamName, ShaderParamDef* pDef) = 0;
        
        //**********************************************************************
        // Method: GetComputeShaderParameterDef
        // Returns information about a specific variable found in the shader
        // 
        // Parameters:
        // paramName - name of parameter
        // pDef - parameter definition info
        // 
        // Returns:
        // True if parameter is found. Otherwise false.
        //**********************************************************************
        virtual bool GetComputeShaderParameterDef(const wchar_t* pParamName, ShaderParamDef* pDef) = 0;

        //**********************************************************************
        // Method: VertexLayout
        // Returns:
        // Returns the vertex format for the shader
        //**********************************************************************
        virtual std::vector<D3D11_INPUT_ELEMENT_DESC> &VertexLayout() = 0;

        //**********************************************************************
        // Method: GetVertexSize
        // Returns:
        // Returns the size of each vertex in bytes
        //**********************************************************************
        virtual uint32_t GetVertexSize() = 0;

        //**********************************************************************
        // Method: GetThreadGroupSize
        // Returns:
        // pXThreads - Number of threads in X (as defined by numthreads() in compute shader)
        // pYThreads - Number of threads in Y (as defined by numthreads() in compute shader)
        // pZThreads - Number of threads in Z (as defined by numthreads() in compute shader)
        //**********************************************************************
        virtual void GetThreadGroupSize(uint32_t* pXThreads, uint32_t* pYThreads, uint32_t* pZThreads) = 0;
    };
}
