//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
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
	// Interface: IShaderInfo
	// Defines primary interface used for getting information about pre-compiled
	// shaders. This isn't the shader itself. It is simply the data that we
	// load from disk to create a shader (i.e. data from .shi file)
	//**********************************************************************
	struct IShaderInfo : public IRefCount
	{
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
        virtual std::vector<ShaderParamDef> &VertexShaderParameterDefs() = 0;

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
	};
}
