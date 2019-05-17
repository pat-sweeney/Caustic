//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base/Core/IRefCount.h"
#include <DirectXMath.h>
#include <string>
#include <any>

namespace Caustic
{
	struct IRenderer;

	//**********************************************************************
	// Interface: IShader
	// Defines the public interface for using CShader
	//**********************************************************************
	struct IShader : public IRefCount
	{
		virtual std::wstring &Name() = 0; // Returns the name of this shader
		virtual void BeginRender(IRenderer *pRenderer, DirectX::XMMATRIX *pWorld = nullptr) = 0; // Called before rendering. This method sets the device with the shader
		virtual void SetPSParam(std::wstring paramName, std::any &value) = 0; // Sets a pixel shader parameter
		virtual void SetVSParam(std::wstring paramName, std::any &value) = 0; // Sets a vertex shader parameter
		virtual void SetPSParam(std::wstring paramName, int index, std::any &value) = 0; // Sets a pixel shader array member parameter
		virtual void SetVSParam(std::wstring paramName, int index, std::any &value) = 0; // Sets a vertex shader array member parameter
		virtual void EndRender(IRenderer *pRenderer) = 0; // Called after the rendering using the shader has completed
		virtual CRefObj<IShaderInfo> GetShaderInfo() = 0;
	};
}
