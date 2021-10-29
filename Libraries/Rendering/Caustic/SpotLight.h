#pragma once
//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Light.h"

//**********************************************************************
// File: SpotLight.h
// Contains declaration for CSpotLight
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Class: CSpotLight
    // Implementation of ISpotLight
    //
    // Header:
    // {Link:#include "Rendering/Caustic/SpotLight.h"{Rendering/Caustic/SpotLight.h}}
    //**********************************************************************
    class CSpotLight : public CLightBase, public ISpotLight, public CRefCount
    {
        float m_innerAngle;
        float m_outerAngle;
        Vector3 m_dir;
    public:
        CSpotLight()
        {
        }

        CSpotLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity, float innerAngle, float outerAngle, bool casts)
        {
            m_pos = pos;
            m_dir = dir;
            m_color = color;
            m_intensity = intensity;
            m_innerAngle = innerAngle;
            m_outerAngle = outerAngle;
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
        virtual ELightType GetType() override { return ELightType::SpotLight; }
        virtual void TurnOn() override { m_enabled = true; }
        virtual void TurnOff() override { m_enabled = false; }
        virtual void SetIntensity(float intensity) override { m_intensity = intensity; }
        virtual float GetIntensity() override { return m_intensity; }
        virtual void SetCastsShadows(bool casts) override { m_casts = casts; }
        virtual bool GetCastsShadows() override { return m_casts; }
        virtual void SetPosition(Vector3& pos) override { m_pos = pos; }
        virtual Vector3 GetPosition() override { return m_pos; }
        virtual void SetDirection(Vector3& dir) override { m_dir = dir; }
        virtual Vector3 GetDirection() override { return m_dir; }
        virtual void SetColor(FRGBColor& color) override { m_color = color; }
        virtual FRGBColor GetColor() override { return m_color; }

        //**********************************************************************
        // ISpotLight
        //**********************************************************************
        virtual void SetAngles(float2 angles) { m_innerAngle = angles.x; m_outerAngle = angles.y; }
        virtual float2 GetAngles() { return float2(m_innerAngle, m_outerAngle); }
    };
}
