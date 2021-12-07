//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.RendererFlags;
import Base.Core.Core;

//**********************************************************************
// File: RendererFlags.ixx
// This file defines the published interface for the renderer.
//**********************************************************************

export namespace Caustic
{

	//**********************************************************************
	// Group: Pass Flags
	// c_PassFirst - first pass
	// c_PassObjID - renders pass writing object IDs to output texture
	// c_PassShadow - pass that renders shadow maps
	// c_PassOpaque - pass rendering the opaque objects
	// c_PassTransparent - pass rendering transparent objects
	// c_PassEnvironment - pass rendering environment maps
	// c_PassLast - last pass
	// c_PassAllPasses - combination of all the pass flags
	//
	// Group: Shadow map selection
	// c_HiResShadowMap - selects the hi-res shadow map
	// c_MidResShadowMap - selects the mid-res shadow map
	// c_LowResShadowMap - selects the low-res shadow map
	//
	// Modules:
	// {Link:import Rendering.Caustic.RendererFlags;{Rendering/Caustic/RendererFlags.ixx}}
	//**********************************************************************
	const int c_PassFirst = 0;
	const int c_PassObjID = 0;
	const int c_PassShadow = 1;
	const int c_PassOpaque = 2;
	const int c_PassTransparent = 3;
	const int c_PassEnvironment = 4;
	const int c_PassLast = c_PassEnvironment;
	const int c_PassAllPasses = (1 << c_PassLast) - 1;

	// Each shadow map will support up to 16 lights in a single map.
	// For instance, the hires map is 8096x8096. We divide this map
	// into 16 2048x2048 shadow maps.
	const int c_MaxLights = 4;
	const int c_HiResShadowMap = 0;
	const int c_MidResShadowMap = 1;
	const int c_LowResShadowMap = 2;
	const int c_MaxShadowMaps = 3; // Hi-res, Mid-res, Low-res
}