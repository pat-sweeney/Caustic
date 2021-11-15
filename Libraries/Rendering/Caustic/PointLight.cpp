//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Rendering\Caustic\Caustic.h"
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.PointLight;

//**********************************************************************
// File: PointLight.cpp
// Contains implementation of <IPointLight>
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Function: CreatePointLight
    // Creates a point light
    //
    // Parameters:
    // pos - position in world coordinates for the light
    // color - color of the light
    // intensity - intensity of the light
    //
    // Returns:
    // Returns the created point light
    //**********************************************************************
    CRefObj<IPointLight> CreatePointLight(Vector3 &pos, FRGBColor &color, float intensity, bool casts)
    {
        return CRefObj<IPointLight>(new CPointLight(pos, color, intensity, casts));
    }
}
