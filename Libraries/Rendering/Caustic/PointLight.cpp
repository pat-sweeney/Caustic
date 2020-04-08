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
    // ppLight - Returns the created point light
    //**********************************************************************
    CAUSTICAPI void CreatePointLight(Vector3 &pos, IPointLight **ppLight)
    {
        std::unique_ptr<CPointLight> spPointLight(new CPointLight());
        spPointLight->SetPosition(pos);
        *ppLight = spPointLight.release();
        (*ppLight)->AddRef();
    }
}
