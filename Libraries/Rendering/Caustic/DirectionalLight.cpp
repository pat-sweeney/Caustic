//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\DirectionalLight.h"
#include "Rendering\Caustic\IDirectionalLight.h"

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
    // dir - direction vector
    // clr - color of light
    // intensity - intensity of light
    //
    // Returns:
    // Returns the created directional light
    //**********************************************************************
    CAUSTICAPI CRefObj<IDirectionalLight> CreateDirectionalLight(Vector3& dir, FRGBColor& color, float intensity, bool casts)
    {
        return CRefObj<IDirectionalLight>(new CDirectionalLight(dir, color, intensity, casts));
    }
}
