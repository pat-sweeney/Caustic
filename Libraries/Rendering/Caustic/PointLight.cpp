//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.Caustic.IPointLight;
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
    // castsShadows - Does this light cast a shadow?
    // castsLight - Does this light illuminate the scene?
    //
    // Returns:
    // Returns the created point light
    //**********************************************************************
    CRefObj<IPointLight> CreatePointLight(Vector3 &pos, FRGBColor &color, float intensity, bool castsShadows, bool castsLight)
    {
        return CRefObj<IPointLight>(new CPointLight(pos, color, intensity, castsShadows, castsLight));
    }
}
