//**********************************************************************
// Copyright Patrick Sweeney 2021
// All Rights Reserved
//**********************************************************************
export module Rendering.Caustic.PathTrace;
import Base.Core.Core;
import Base.Math.Ray;
import Base.Math.MCSampler;

export namespace Caustic
{
	struct PathTraceCtx
	{
		CUniformHemisphere3D sampler;
		int spp; // samples per pixel
		int maxDepth; // rays per path
		bool russianRoulette; // Use russian roulette for path termination
	};
}
