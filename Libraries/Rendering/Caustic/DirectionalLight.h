#pragma once
//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Light.h"
#include "Rendering\Caustic\IDirectionalLight.h"

//**********************************************************************
// File: SpotLight.h
// Contains declaration for CDirectionalLight
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Class: CDirectionalLight
    // Implementation of IDirectionalLight
    //
    // Header:
    // [Link:Rendering/Caustic/DirectionalLight.h]
    //**********************************************************************
    class CDirectionalLight : public CLightBase, public IDirectionalLight, public CRefCount
    {
        Vector3 m_dir;
    public:
        CDirectionalLight()
        {
        }

        CDirectionalLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity, bool casts)
        {
            m_pos = pos;
            m_dir = dir;
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
        virtual ELightType GetType() override { return ELightType::DirectionalLight; }
        virtual void TurnOn() override { m_enabled = true; }
        virtual void TurnOff() override { m_enabled = false; }
        virtual void SetIntensity(float intensity) override { m_intensity = intensity; }
        virtual float GetIntensity() override { return m_intensity; }
        virtual void SetCastsShadows(bool casts) override { m_casts = casts; }
        virtual bool GetCastsShadows() override { return m_casts; }
        virtual void SetPosition(Vector3& pos) override { m_pos = pos; }
        virtual Vector3 GetPosition() override { return Vector3(100.0f, 100.0f, 100.0f); }
        virtual void SetDirection(Vector3& dir) override { m_dir = dir; }
        virtual Vector3 GetDirection() override { return m_dir; }
        virtual void SetColor(FRGBColor& color) override { m_color = color; }
        virtual FRGBColor GetColor() override { return m_color; }

        //**********************************************************************
        // IDirectionalLight
        //**********************************************************************
    };
}
