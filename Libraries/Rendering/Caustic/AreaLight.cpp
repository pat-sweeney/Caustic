//**********************************************************************
// Copyright Patrick Sweeney 2015-2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.Caustic.IAreaLight;
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.AreaLight;
import Rendering.Caustic.ILight;
import Rendering.Caustic.IAreaLight;

//**********************************************************************
// File: AreaLight.cpp
// Contains implementation of <IAreaLight>
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Function: CreateAreaLight
    // Creates a rectangular area light
    //
    // Parameters:
    // pos - center position of the area light
    // dir - normal direction of the light surface
    // up - up vector for orienting the rectangle
    // clr - color of the light
    // intensity - light intensity
    // width - width of the rectangular emitter
    // height - height of the rectangular emitter
    // castsShadows - does this light participate in shadow mapping?
    // castsLight - does this light illuminate the scene?
    //
    // Returns:
    // Returns the created area light
    //**********************************************************************
    CRefObj<IAreaLight> CreateAreaLight(Vector3& pos, Vector3& dir, Vector3& up,
        FRGBColor& color, float intensity,
        float width, float height,
        bool castsShadows, bool castsLight)
    {
        return CRefObj<IAreaLight>(new CAreaLight(pos, dir, up, color, intensity, width, height, castsShadows, castsLight));
    }
};
