//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"
#include "Imaging\Color\Color.h"
#include "Rendering\Caustic\ILight.h"

//**********************************************************************
// File: IPointLight.h
// This file defines the published interface for dealing with point lights
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Interface: IPointLight
    // Defines a point light
    //
    // Header:
    // [Link:Rendering/Caustic/IPointLight.h]
    //**********************************************************************
    struct IPointLight : public ILight
    {
    };

    //**********************************************************************
    // Function: CreatePointLight
    // Creates a point light
    //
    // Parameters:
    // pos - position of light in world coordinates
    // color - color of light
    // intensity - intensity of light
    // casts - indicates whether this light participates in shadow mapping
    //
    // Header:
    // [Link:Rendering/Caustic/IPointLight.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<IPointLight> CreatePointLight(Vector3& pos, FRGBColor& color, float intensity, bool casts = true);
}
