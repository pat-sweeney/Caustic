//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Geometry\Mesh\Mesh.h"
#include <Windows.h>
#include <atlbase.h>
#include <d3d12.h>

//**********************************************************************
// This file defines the published interface for talking to the Caustic Renderer
//**********************************************************************

// Namespace: Caustic
namespace Caustic
{
	struct IRenderer;

	//**********************************************************************
	// Interface: ITexture
	// Defines how clients interact with textures
	//**********************************************************************
	struct ITexture : public IRefCount
	{
		//**********************************************************************
		// Method: GetWidth
		// Returns the width of the texture in pixels
		//**********************************************************************
		virtual uint32 GetWidth() = 0;

		//**********************************************************************
		// Method: GetHeight
		// Returns the height of the texture in pixels
		//**********************************************************************
		virtual uint32 GetHeight() = 0;

		//**********************************************************************
		// Method: GetFormat
		// Returns the pixel format of the texture
		//**********************************************************************
		virtual DXGI_FORMAT GetFormat() = 0; // Returns the DirectX format of the texture

		//**********************************************************************
		// Method: Update
		// Updates the underlying graphics card texture. Primarily used for
		// video textures.
		//
		// Parameters:
		// pRenderer - rendering device
		//**********************************************************************
		virtual void Update(IRenderer *pRenderer) = 0; // Called to update texture

		//**********************************************************************
		// Method: GetD3DTexture
		// Returns the underlying D3D texture
		//**********************************************************************
		virtual CComPtr<ID3D12Resource> GetD3DTexture() = 0;

		//**********************************************************************
		// Method: GenerateMips
		// Generates a set of mipmaps for the texture
		//
		// Parameters:
		// pGraphics - graphics device
		//**********************************************************************
		virtual void GenerateMips(IRenderer *pRenderer) = 0;
	};
}
