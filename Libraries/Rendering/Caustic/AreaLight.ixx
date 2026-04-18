//**********************************************************************
// Copyright Patrick Sweeney 2015-2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.AreaLight;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Math.Vector;
import Imaging.Color;
import Rendering.Caustic.Light;
import Rendering.Caustic.IAreaLight;

//**********************************************************************
// File: AreaLight.ixx
// Contains declaration for CAreaLight
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CAreaLight
    // Implementation of IAreaLight. Represents a rectangular area emitter.
    //
    // Module:
    // {Link:import Rendering.Caustic.AreaLight;{Rendering/Caustic/AreaLight.ixx}}
    //**********************************************************************
    class CAreaLight : public CLightBase, public IAreaLight, public CRefCount
    {
        float m_width;
        float m_height;
        Vector3 m_dir;
        Vector3 m_up;
    public:
        CAreaLight() : m_width(1.0f), m_height(1.0f),
            m_dir(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f)
        {
        }

        CAreaLight(Vector3& pos, Vector3& dir, Vector3& up, FRGBColor& color,
            float intensity, float width, float height,
            bool castsShadows, bool castsLight)
            : m_width(width), m_height(height), m_dir(dir), m_up(up)
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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ILight
        //**********************************************************************
        virtual ELightType GetType() override { return ELightType::AreaLight; }
        virtual bool GetOnOff() override { return m_enabled; }
        virtual void SetOnOff(bool f) override { m_enabled = f; }
        virtual void SetIntensity(float intensity) override { m_intensity = intensity; }
        virtual float GetIntensity() override { return m_intensity; }
        virtual void SetCastsLight(bool casts) override { m_castsLight = casts; }
        virtual bool GetCastsLight() override { return m_castsLight; }
        virtual void SetRange(float range) override { m_range = range; }
        virtual float GetRange() override { return m_range; }
        virtual void SetCastsShadows(bool casts) override { m_castsShadows = casts; }
        virtual bool GetCastsShadows() override { return m_castsShadows; }
        virtual void SetPosition(Vector3& pos) override { m_pos = pos; }
        virtual Vector3 GetPosition() override { return m_pos; }
        virtual void SetDirection(Vector3& dir) override { m_dir = dir; }
        virtual Vector3 GetDirection() override { return m_dir; }
        virtual void SetColor(FRGBColor& color) override { m_color = color; }
        virtual FRGBColor GetColor() override { return m_color; }

        //**********************************************************************
        // IAreaLight
        //**********************************************************************
        virtual void SetDimensions(float width, float height) override { m_width = width; m_height = height; }
        virtual float GetWidth() override { return m_width; }
        virtual float GetHeight() override { return m_height; }
        virtual void SetUpVector(Vector3& up) override { m_up = up; }
        virtual Vector3 GetUpVector() override { return m_up; }
        virtual void GetCorners(Vector3 corners[4]) override
        {
            // Compute right vector from direction and up
            Vector3 right = m_dir.Cross(m_up);
            right.Normalize();
            Vector3 up = right.Cross(m_dir);
            up.Normalize();
            float hw = m_width * 0.5f;
            float hh = m_height * 0.5f;
            corners[0] = m_pos - right * hw - up * hh; // bottom-left
            corners[1] = m_pos + right * hw - up * hh; // bottom-right
            corners[2] = m_pos + right * hw + up * hh; // top-right
            corners[3] = m_pos - right * hw + up * hh; // top-left
        }
    };
}
