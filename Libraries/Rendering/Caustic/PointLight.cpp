//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\PointLight.h"

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
    CAUSTICAPI CRefObj<IPointLight> CreatePointLight(Vector3 &pos, FRGBColor &color, float intensity, bool casts)
    {
        return CRefObj<IPointLight>(new CPointLight(pos, color, intensity, casts));
    }
}
