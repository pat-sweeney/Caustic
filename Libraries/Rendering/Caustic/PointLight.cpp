//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
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
    //
    // Returns:
    // Returns the created point light
    //**********************************************************************
    CAUSTICAPI CRefObj<IPointLight> CreatePointLight(Vector3 &pos, Vector3 &color)
    {
        std::unique_ptr<CPointLight> spPointLight(new CPointLight());
        spPointLight->SetPosition(pos);
        spPointLight->SetColor(color);
        return CRefObj<IPointLight>(spPointLight.release());
    }
}
