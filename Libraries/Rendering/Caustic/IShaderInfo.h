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
        virtual D3D11_PRIMITIVE_TOPOLOGY GetTopologyType() = 0;
		virtual std::vector<ShaderParamDef> &PixelShaderParameterDefs() = 0;
		virtual std::vector<ShaderParamDef> &VertexShaderParameterDefs() = 0;
		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> &VertexLayout() = 0;
		virtual uint32 GetVertexSize() = 0;
	};
}
