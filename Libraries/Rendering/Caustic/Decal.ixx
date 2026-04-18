//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.Decal;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Base.Math.Matrix;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IDecal;

//**********************************************************************
// File: Decal.ixx
// Contains declaration for CDecal
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CDecal
    // Implementation of IDecal. A projected decal defined by an OBB
    // transform and an albedo texture.
    //
    // Module:
    // {Link:import Rendering.Caustic.Decal;{Rendering/Caustic/Decal.ixx}}
    //**********************************************************************
    class CDecal : public IDecal, public CRefCount
    {
        Matrix4x4 m_transform;
        CRefObj<ITexture> m_spAlbedoTexture;
        float m_opacity;
    public:
        CDecal() : m_opacity(1.0f) {}

        CDecal(Matrix4x4& transform, ITexture* pAlbedoTexture, float opacity)
            : m_transform(transform), m_opacity(opacity)
        {
            if (pAlbedoTexture)
                m_spAlbedoTexture = pAlbedoTexture;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IDecal
        //**********************************************************************
        virtual void SetTransform(Matrix4x4& mat) override { m_transform = mat; }
        virtual Matrix4x4 GetTransform() override { return m_transform; }
        virtual void SetAlbedoTexture(ITexture* pTexture) override { m_spAlbedoTexture = pTexture; }
        virtual CRefObj<ITexture> GetAlbedoTexture() override { return m_spAlbedoTexture; }
        virtual void SetOpacity(float opacity) override { m_opacity = opacity; }
        virtual float GetOpacity() override { return m_opacity; }
    };
}
