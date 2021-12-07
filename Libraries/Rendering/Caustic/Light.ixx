//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.Light;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Color;

//**********************************************************************
// File: Light.ixx
// Contains declaration for CLightBase
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CLightBase
    // Contains the implementation of stuff common to all lights
    //
    // Module:
    // {Link:import Rendering.Caustic.Light;{Rendering/Caustic/Light.ixx}}
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
