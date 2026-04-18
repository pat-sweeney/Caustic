//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
// File: ParticleSimulate.cs
//
// Compute shader for GPU particle simulation. Each thread handles one
// particle slot. Dead particles (age < 0) may be recycled for emission.
// Live particles are updated with gravity, wind, and aging.
//**********************************************************************

struct Particle
{
    float3 pos;
    float  age;         // < 0 means dead slot
    float3 vel;
    float  lifetime;
    float  size;
    float  pad;
    float4 color;
};

RWStructuredBuffer<Particle> particles : register(u0);
RWStructuredBuffer<uint>     emitCounter : register(u1);

cbuffer CS_CONSTANT_BUFFER
{
    float deltaTime;
    float gravityX;
    float gravityY;
    float gravityZ;
    float windX;
    float windY;
    float windZ;
    int   maxParticles;
    int   emitCount;
    float emitPosX;
    float emitPosY;
    float emitPosZ;
    float emitSpread;
    float emitSpeed;
    float emitLifetime;
    float emitSize;
    float emitColorR;
    float emitColorG;
    float emitColorB;
    float emitColorA;
    int   frameIndex;
    float pad1;
    float pad2;
    float pad3;
};

//**********************************************************************
// PCG hash for pseudo-random number generation in compute shaders
//**********************************************************************
uint pcg_hash(uint input)
{
    uint state = input * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float rand01(uint seed)
{
    return float(pcg_hash(seed)) / 4294967295.0;
}

float3 randomDirection(uint seed)
{
    float u = rand01(seed) * 2.0 - 1.0;
    float theta = rand01(seed + 1u) * 6.28318530718;
    float r = sqrt(1.0 - u * u);
    return float3(r * cos(theta), u, r * sin(theta));
}

[numthreads(256, 1, 1)]
void CS(uint3 dtid : SV_DispatchThreadID)
{
    uint idx = dtid.x;
    if (idx >= (uint)maxParticles)
        return;

    Particle p = particles[idx];

    if (p.age < 0.0)
    {
        // Dead particle — try to claim an emission slot
        uint remaining;
        InterlockedAdd(emitCounter[0], -1, remaining);
        if (remaining > 0 && remaining <= (uint)emitCount)
        {
            // Emit a new particle
            uint seed = idx * 17u + (uint)frameIndex * 7919u;
            float3 dir = randomDirection(seed);

            // Cone emission: blend between up direction and random direction
            float3 upDir = float3(0.0, 1.0, 0.0);
            dir = normalize(lerp(upDir, dir, emitSpread));

            p.pos = float3(emitPosX, emitPosY, emitPosZ);
            // Add slight random offset
            p.pos += randomDirection(seed + 3u) * 0.01;
            p.vel = dir * emitSpeed * (0.8 + 0.4 * rand01(seed + 5u));
            p.age = 0.0;
            p.lifetime = emitLifetime * (0.8 + 0.4 * rand01(seed + 7u));
            p.size = emitSize * (0.8 + 0.4 * rand01(seed + 9u));
            p.color = float4(emitColorR, emitColorG, emitColorB, emitColorA);
        }
        else
        {
            // No emission slot available — undo the decrement
            if (remaining == 0 || remaining > (uint)emitCount)
                InterlockedAdd(emitCounter[0], 1, remaining);
        }
        particles[idx] = p;
        return;
    }

    // Update live particle
    p.age += deltaTime;
    if (p.age >= p.lifetime)
    {
        p.age = -1.0;
        particles[idx] = p;
        return;
    }

    // Apply forces
    float3 gravity = float3(gravityX, gravityY, gravityZ);
    float3 wind = float3(windX, windY, windZ);
    p.vel += (gravity + wind) * deltaTime;

    // Integrate position
    p.pos += p.vel * deltaTime;

    // Fade alpha over lifetime
    float lifeFrac = p.age / p.lifetime;
    p.color.a = emitColorA * (1.0 - lifeFrac * lifeFrac);

    particles[idx] = p;
}
