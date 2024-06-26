//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.Caustic.IDirectionalLight;
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.DirectionalLight;
import Rendering.Caustic.IDirectionalLight;

//**********************************************************************
// File: DirectionalLight.cpp
// Contains implementation of <IDirectionalLight>
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Function: CreateDirectionalLight
    // Creates a directional light
    //
    // Parameters:
    // pos - light's position in world coordinates
    // dir - direction vector
    // clr - color of light
    // intensity - intensity of light
    // castsShadows - does light cause shadows?
    // castsLight - does light participate in illuminating the scene?
    //
    // Returns:
    // Returns the created directional light
    //**********************************************************************
    CRefObj<IDirectionalLight> CreateDirectionalLight(Vector3 &pos, Vector3& dir, FRGBColor& color, float intensity, bool castsShadows, bool castsLight)
    {
        return CRefObj<IDirectionalLight>(new CDirectionalLight(pos, dir, color, intensity, castsShadows, castsLight));
    }
}
