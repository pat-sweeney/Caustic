//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.PointLight;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Rendering.Caustic.Light;
import Rendering.Caustic.IPointLight;

//**********************************************************************
// File: PointLight.ixx
// Contains declaration for CPointLight
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CPointLight
    // Implementation of IPointLight
    //
    // Module:
    // {Link:import Rendering.Caustic.PointLight;{Rendering/Caustic/PointLight.ixx}}
    //**********************************************************************
    class CPointLight : public CLightBase, public IPointLight, public CRefCount
    {
    public:
        CPointLight()
        {
        }

        CPointLight(Vector3& pos, FRGBColor& color, float intensity, bool castsShadows, bool castsLight)
        {
            m_pos = pos;
            m_color = color;
            m_intensity = intensity;
            m_castsShadows = castsShadows;
            m_castsLight = castsLight;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ILight
        //**********************************************************************
        virtual ELightType GetType() override { return ELightType::PointLight; }
        virtual bool GetOnOff() override { return m_enabled; }
        virtual void SetOnOff(bool f) override { m_enabled = f; }
        virtual void SetIntensity(float intensity) override { m_intensity = intensity; }
        virtual float GetIntensity() override { return m_intensity; }
        virtual void SetCastsShadows(bool casts) override { m_castsShadows = casts; }
        virtual bool GetCastsShadows() override { return m_castsShadows; }
        virtual void SetCastsLight(bool casts) override { m_castsLight = casts; }
        virtual bool GetCastsLight() override { return m_castsLight; }
        virtual void SetPosition(Vector3& pos) override { m_pos = pos; }
        virtual Vector3 GetPosition() override { return m_pos; }
        virtual Vector3 GetDirection() override { return Vector3(0.0f, 0.0f, 0.0f); }
        virtual void SetDirection(Vector3& dir) override { }
        virtual void SetColor(FRGBColor& color) override { m_color = color; }
        virtual FRGBColor GetColor() override { return m_color; }

        //**********************************************************************
        // IPointLight
        //**********************************************************************
    };
}
