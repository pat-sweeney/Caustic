//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
import Base.Core.Error;
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\SpotLight.h"
#include "Rendering\Caustic\ISpotLight.h"

//**********************************************************************
// File: SpotLight.cpp
// Contains implementation of <ISpotLight>
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Function: CreateSpotLight
    // Creates a spot light
    //
    // Parameters:
    // pos - position in world coordinates for the light
    // clr - color of light
    // intensity - intensity of light
    // innerAngle - inner angle defining where light is full intensity
    // outerAngle - outer angle. Light falls off from full to no intensity between inner and outer angle
    // dir - direction vector
    // casts - does this light participate in shadow mapping
    //
    // Returns:
    // Returns the created point light
    //**********************************************************************
    CRefObj<ISpotLight> CreateSpotLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity, float innerAngle, float outerAngle, bool casts)
    {
        return CRefObj<ISpotLight>(new CSpotLight(pos, dir, color, intensity, innerAngle, outerAngle, casts));
    }
}
