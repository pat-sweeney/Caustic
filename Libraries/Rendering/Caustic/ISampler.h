//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base/Core/IRefCount.h"
#include <d3d12.h>

namespace Caustic
{
	//**********************************************************************
	// Interface: ISampler
	// Defines a sampler. Samplers define how the underlying texture is accessed by shaders.
	//**********************************************************************
	struct ISampler : public IRefCount
	{
		//**********************************************************************
		// Method: GetFilter
		// Returns the current filter state
		//**********************************************************************
		virtual D3D12_FILTER GetFilter() = 0;

		//**********************************************************************
		// Method: SetFilter
		// Set the current filter state
		//
		// Parameters:
		// filter - filter state to set
		//**********************************************************************
		virtual void SetFilter(D3D12_FILTER filter) = 0;

		//**********************************************************************
		// Method: GetAddressU
		// Returns the current addressing mode in the U direction
		//**********************************************************************
		virtual D3D12_TEXTURE_ADDRESS_MODE GetAddressU() = 0;

		//**********************************************************************
		// Method: SetAddressU
		// Set the current addressing mode in the U direction
		//
		// Parameters:
		// mode - addressing mode to set
		//**********************************************************************
		virtual void SetAddressU(D3D12_TEXTURE_ADDRESS_MODE mode) = 0;

		//**********************************************************************
		// Method: GetAddressV
		// Returns the current addressing mode in the V direction
		//**********************************************************************
		virtual D3D12_TEXTURE_ADDRESS_MODE GetAddressV() = 0;

		//**********************************************************************
		// Method: SetAddressU
		// Set the current addressing mode in the V direction
		//
		// Parameters:
		// mode - addressing mode to set
		//**********************************************************************
		virtual void SetAddressV(D3D12_TEXTURE_ADDRESS_MODE mode) = 0;

		//**********************************************************************
		// Method: Render
		// Sets up the sampler for the current rendering
		//
		// Parameters:
		// pRenderer - Renderer
		// slot - texture slot to which this sampler should be assigned
		//**********************************************************************
		virtual void Render(IRenderer *pRenderer, int slot) = 0;

		//**********************************************************************
		// Method: GetTexture
		// Returns the texture associated with this sampler
		//**********************************************************************
		virtual void GetTexture(ITexture **ppTexture) = 0;
	};
}
