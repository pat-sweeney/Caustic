//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.IParticleSystem;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;

//**********************************************************************
// File: IParticleSystem.ixx
// Defines the published interface for a GPU-driven particle system.
// Particles are simulated via compute shader and rendered as
// camera-facing billboards with additive blending.
//**********************************************************************

export namespace Caustic
{
    struct IRenderer;

    //**********************************************************************
    // Interface: IParticleSystem
    // Represents a GPU particle system with a single emitter.
    //
    // Module:
    // {Link:import Rendering.Caustic.IParticleSystem;{Rendering/Caustic/IParticleSystem.ixx}}
    //**********************************************************************
    struct IParticleSystem : public IRefCount
    {
        //**********************************************************************
        // Method: SetEmitterPosition
        // Sets the world-space position of the emitter.
        //**********************************************************************
        virtual void SetEmitterPosition(Vector3& pos) = 0;

        //**********************************************************************
        // Method: SetEmissionRate
        // Sets how many particles to emit per second.
        //**********************************************************************
        virtual void SetEmissionRate(float particlesPerSecond) = 0;

        //**********************************************************************
        // Method: SetParticleLifetime
        // Sets the lifetime of newly emitted particles in seconds.
        //**********************************************************************
        virtual void SetParticleLifetime(float seconds) = 0;

        //**********************************************************************
        // Method: SetInitialSpeed
        // Sets the initial speed of emitted particles.
        //**********************************************************************
        virtual void SetInitialSpeed(float speed) = 0;

        //**********************************************************************
        // Method: SetEmitSpread
        // Sets the emission cone half-angle in radians (0 = straight up).
        //**********************************************************************
        virtual void SetEmitSpread(float radians) = 0;

        //**********************************************************************
        // Method: SetParticleSize
        // Sets the billboard size of particles in world units.
        //**********************************************************************
        virtual void SetParticleSize(float size) = 0;

        //**********************************************************************
        // Method: SetParticleColor
        // Sets the RGBA color of emitted particles.
        //**********************************************************************
        virtual void SetParticleColor(Vector4& color) = 0;

        //**********************************************************************
        // Method: SetGravity
        // Sets gravity acceleration vector (e.g. {0, -9.8, 0}).
        //**********************************************************************
        virtual void SetGravity(Vector3& gravity) = 0;

        //**********************************************************************
        // Method: SetWind
        // Sets wind force vector applied to all particles.
        //**********************************************************************
        virtual void SetWind(Vector3& wind) = 0;

        //**********************************************************************
        // Method: GetMaxParticles
        // Returns the maximum particle count for this system.
        //**********************************************************************
        virtual uint32_t GetMaxParticles() = 0;

        //**********************************************************************
        // Method: Update
        // Dispatches the simulation compute shader. Call once per frame.
        //
        // Parameters:
        // pRenderer - renderer to use for dispatch
        // deltaTime - time since last frame in seconds
        //**********************************************************************
        virtual void Update(IRenderer* pRenderer, float deltaTime) = 0;

        //**********************************************************************
        // Method: Render
        // Renders the particles as camera-facing billboards with additive blend.
        //
        // Parameters:
        // pRenderer - renderer to use for drawing
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer) = 0;
    };

    //**********************************************************************
    // Function: CreateParticleSystem
    // Creates a new GPU particle system.
    //
    // Parameters:
    // pRenderer - renderer (used to create GPU buffers)
    // maxParticles - maximum number of particles (default 65536)
    //
    // Module:
    // {Link:import Rendering.Caustic.IParticleSystem;{Rendering/Caustic/IParticleSystem.ixx}}
    //**********************************************************************
    CRefObj<IParticleSystem> CreateParticleSystem(IRenderer* pRenderer, uint32_t maxParticles = 65536);
}
