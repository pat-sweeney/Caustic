//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Base/Core/IRefCount.h"
#include <vector>
#include <d3d11.h>

//**********************************************************************
// File: IShaderInfo.h
// This file defines the published interface for IShaderInfo.
// Each shader has an associated IShaderInfo that defines which parameters
// and vertex formats the shader uses.
//**********************************************************************

namespace Caustic
{
	struct IShader;
	struct ShaderParamDef;

    //**********************************************************************
    // Enum: EShaderType
    // Defines the various types of shaders
    //
    // Header:
    // [Link:Rendering/Caustic/IShaderInfo.h]
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
    // Header:
    // [Link:Rendering/Caustic/IShaderInfo.h]
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
        virtual std::vector<ShaderParamDef> &PixelShaderParameterDefs() = 0;

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
        virtual uint32 GetVertexSize() = 0;

        //**********************************************************************
        // Method: GetThreadGroupSize
        // Returns:
        // pXThreads - Number of threads in X (as defined by numthreads() in compute shader)
        // pYThreads - Number of threads in Y (as defined by numthreads() in compute shader)
        // pZThreads - Number of threads in Z (as defined by numthreads() in compute shader)
        //**********************************************************************
        virtual void GetThreadGroupSize(uint32* pXThreads, uint32* pYThreads, uint32* pZThreads) = 0;
    };
}
