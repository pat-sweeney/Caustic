//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
// File: morph.h
//
// Shared morph target (blend shape) functionality for vertex shaders.
// Applies weighted morph target deltas to base position and normal.
// Morph data is stored in a StructuredBuffer with interleaved layout:
//   [target0_posDelta_v0, target0_normDelta_v0,
//    target0_posDelta_v1, target0_normDelta_v1, ...]
//**********************************************************************
#define MAX_MORPH_TARGETS 8

struct MorphDelta
{
    float3 posDelta;
    float3 normDelta;
};

StructuredBuffer<MorphDelta> morphDeltas : register(t0);

// Apply morph target deltas to base position and normal
void ApplyMorphTargets(uint vertexID, int morphTargetCount, int morphNumVertices,
    float4 morphWeights0, float4 morphWeights1,
    inout float3 pos, inout float3 norm)
{
    if (morphTargetCount <= 0)
        return;

    float weights[MAX_MORPH_TARGETS] = {
        morphWeights0.x, morphWeights0.y, morphWeights0.z, morphWeights0.w,
        morphWeights1.x, morphWeights1.y, morphWeights1.z, morphWeights1.w
    };

    for (int t = 0; t < morphTargetCount && t < MAX_MORPH_TARGETS; t++)
    {
        if (abs(weights[t]) > 0.0001)
        {
            uint idx = t * (uint)morphNumVertices + vertexID;
            MorphDelta d = morphDeltas[idx];
            pos += weights[t] * d.posDelta;
            norm += weights[t] * d.normDelta;
        }
    }
    norm = normalize(norm);
}
