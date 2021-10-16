//**********************************************************************
// Copyright Patrick Sweeney 2021
// All Rights Reserved
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Math.Ray;
import Base.Math.MCSampler;

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
