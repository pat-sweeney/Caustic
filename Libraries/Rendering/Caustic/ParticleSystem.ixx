//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <DirectXMath.h>
#include <vector>
#include <cinttypes>

export module Rendering.Caustic.ParticleSystem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.IParticleSystem;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IShader;
import Rendering.Caustic.ITexture;

export namespace Caustic
{
    //**********************************************************************
    // Struct: GPUParticle
    // Must match the HLSL Particle struct exactly (56 bytes).
    //**********************************************************************
    struct GPUParticle
    {
        float pos[3];       // 12
        float age;           //  4  (< 0 = dead)
        float vel[3];       // 12
        float lifetime;      //  4
        float size;          //  4
        float pad;           //  4
        float color[4];     // 16
    };                       // Total: 56 bytes
    static_assert(sizeof(GPUParticle) == 56, "GPUParticle must be 56 bytes to match HLSL");

    //**********************************************************************
    // Class: CParticleSystem
    // GPU-driven particle system with compute simulation and billboard rendering.
    //**********************************************************************
    class CParticleSystem : public IParticleSystem, public CRefCount
    {
        uint32_t m_maxParticles;
        uint32_t m_frameCounter;

        // Emitter parameters
        Vector3 m_emitPos;
        float m_emissionRate;
        float m_emitAccumulator;
        float m_particleLifetime;
        float m_initialSpeed;
        float m_emitSpread;
        float m_particleSize;
        Vector4 m_particleColor;

        // Physics
        Vector3 m_gravity;
        Vector3 m_wind;

        // GPU resources
        CComPtr<ID3D11Buffer> m_spParticleBuffer;
        CComPtr<ID3D11UnorderedAccessView> m_spParticleUAV;
        CComPtr<ID3D11ShaderResourceView> m_spParticleSRV;
        CComPtr<ID3D11Buffer> m_spEmitCounterBuffer;
        CComPtr<ID3D11UnorderedAccessView> m_spEmitCounterUAV;
        CComPtr<ID3D11Buffer> m_spQuadVB;
        CComPtr<ID3D11Buffer> m_spQuadIB;
        CComPtr<ID3D11BlendState> m_spAdditiveBlend;
        CComPtr<ID3D11DepthStencilState> m_spDepthNoWrite;
        CComPtr<ID3D11RasterizerState> m_spNoCullRS;

        // Shaders
        CRefObj<IShader> m_spSimulateShader;
        CRefObj<IShader> m_spBillboardShader;

        bool m_initialized;

        void InitGPUResources(IRenderer* pRenderer);
    public:
        CParticleSystem(uint32_t maxParticles);

        // IRefCount
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        // IParticleSystem
        virtual void SetEmitterPosition(Vector3& pos) override { m_emitPos = pos; }
        virtual void SetEmissionRate(float pps) override { m_emissionRate = pps; }
        virtual void SetParticleLifetime(float s) override { m_particleLifetime = s; }
        virtual void SetInitialSpeed(float speed) override { m_initialSpeed = speed; }
        virtual void SetEmitSpread(float rad) override { m_emitSpread = rad; }
        virtual void SetParticleSize(float size) override { m_particleSize = size; }
        virtual void SetParticleColor(Vector4& c) override { m_particleColor = c; }
        virtual void SetGravity(Vector3& g) override { m_gravity = g; }
        virtual void SetWind(Vector3& w) override { m_wind = w; }
        virtual uint32_t GetMaxParticles() override { return m_maxParticles; }
        virtual void Update(IRenderer* pRenderer, float deltaTime) override;
        virtual void Render(IRenderer* pRenderer) override;
    };
}
