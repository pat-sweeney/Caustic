//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.DirectionalLight;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Rendering.Caustic.Light;
import Rendering.Caustic.IDirectionalLight;

//**********************************************************************
// File: DirectionalLight.ixx
// Contains declaration for CDirectionalLight
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CDirectionalLight
    // Implementation of IDirectionalLight
    //
    // Module:
    // {Link:import Rendering.Caustic.DirectionalLight;{Rendering/Caustic/DirectionalLight.ixx}}
    //**********************************************************************
    class CDirectionalLight : public CLightBase, public IDirectionalLight, public CRefCount
    {
        Vector3 m_dir;
    public:
        CDirectionalLight()
        {
        }

        CDirectionalLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity, bool castsShadows, bool castsLight)
        {
            m_pos = pos;
            m_dir = dir;
            m_color = color;
            m_intensity = intensity;
            m_castsShadows = castsShadows;
            m_castsLight = castsLight;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ILight
        //**********************************************************************
        virtual ELightType GetType() override { return ELightType::DirectionalLight; }
        virtual bool GetOnOff() override { return m_enabled; }
        virtual void SetOnOff(bool f) override { m_enabled = f; }
        virtual void SetIntensity(float intensity) override { m_intensity = intensity; }
        virtual float GetIntensity() override { return m_intensity; }
        virtual void SetCastsLight(bool casts) override { m_castsLight = casts; }
        virtual bool GetCastsLight() override { return m_castsLight; }
        virtual void SetCastsShadows(bool casts) override { m_castsShadows = casts; }
        virtual bool GetCastsShadows() override { return m_castsShadows; }
        virtual void SetPosition(Vector3& pos) override { m_pos = pos; }
        virtual Vector3 GetPosition() override { return m_pos; }
        virtual void SetDirection(Vector3& dir) override { m_dir = dir; }
        virtual Vector3 GetDirection() override { return m_dir; }
        virtual void SetColor(FRGBColor& color) override { m_color = color; }
        virtual FRGBColor GetColor() override { return m_color; }

        //**********************************************************************
        // IDirectionalLight
        //**********************************************************************
    };
}
