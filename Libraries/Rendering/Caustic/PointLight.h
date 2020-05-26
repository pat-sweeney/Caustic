#pragma once
//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Light.h"
#include "Base\Core\RefCount.h"

//**********************************************************************
// File: PointLight.h
// Contains declaration for CPointLight
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Class: CPointLight
    // Implementation of IPointLight
    //
    // Header:
    // [Link:Rendering/Caustic/PointLight.h]
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
        virtual void TurnOn() override { m_enabled = true; }
        virtual void TurnOff() override { m_enabled = false; }
        virtual void SetIntensity(float intensity) override { m_intensity = intensity; }
        virtual float GetIntensity() override { return m_intensity; }
        virtual void SetCastsShadows(bool casts) override { m_casts = casts; }
        virtual bool GetCastsShadows() override { return m_casts; }
        virtual void SetPosition(Vector3& pos) override { m_pos = pos; }
        virtual Vector3 GetPosition() override { return m_pos; }
        virtual void SetColor(FRGBColor& color) override { m_color = color; }
        virtual FRGBColor GetColor() override { return m_color; }

        //**********************************************************************
        // IPointLight
        //**********************************************************************
    };
}
