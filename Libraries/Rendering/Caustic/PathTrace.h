//**********************************************************************
// Copyright Patrick Sweeney 2021
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Math\Ray.h"
#include "Base\Math\MCSampler.h"

namespace Caustic
{
	struct PathTraceCtx
	{
		CUniformHemisphere3D sampler;
		int spp; // samples per pixel
		int maxDepth; // rays per path
		bool russianRoulette; // Use russian roulette for path termination
	};
}
