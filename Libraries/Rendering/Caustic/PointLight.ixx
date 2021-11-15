//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include "Rendering\Caustic\Caustic.h"

export module Rendering.Caustic.PointLight;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Rendering.Caustic.Light;

//**********************************************************************
// File: PointLight.h
// Contains declaration for CPointLight
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CPointLight
    // Implementation of IPointLight
    //
    // Header:
    // {Link:#include "Rendering/Caustic/PointLight.h"{Rendering/Caustic/PointLight.h}}
    //**********************************************************************
    class CPointLight : public CLightBase, public IPointLight, public CRefCount
    {
    public:
        CPointLight()
        {
        }

        CPointLight(Vector3& pos, FRGBColor& color, float intensity, bool casts)
        {
            m_pos = pos;
            m_color = color;
            m_intensity = intensity;
            m_casts = casts;
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
        virtual void TurnOn() override { m_enabled = true; }
        virtual void TurnOff() override { m_enabled = false; }
        virtual void SetIntensity(float intensity) override { m_intensity = intensity; }
        virtual float GetIntensity() override { return m_intensity; }
        virtual void SetCastsShadows(bool casts) override { m_casts = casts; }
        virtual bool GetCastsShadows() override { return m_casts; }
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
