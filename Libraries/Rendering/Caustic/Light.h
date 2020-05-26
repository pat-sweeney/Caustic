#pragma once
//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\RefCount.h"

//**********************************************************************
// File: Light.h
// Contains declaration for CLightBase
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Class: CLightBase
    // Contains the implementation of stuff common to all lights
    //
    // Header:
    // [Link:Rendering/Caustic/Light.h]
    //**********************************************************************
    class CLightBase
    {
    protected:
        bool m_enabled;
        bool m_casts;
        float m_intensity;
        Vector3 m_pos;
        FRGBColor m_color;
    public:
        CLightBase() :
            m_enabled(true),
            m_casts(true),
            m_intensity(1000.0f),
            m_pos(0.0f, 0.0f, 0.0f),
            m_color(1.0f, 1.0f, 1.0f)
        {
        }
    };
}
