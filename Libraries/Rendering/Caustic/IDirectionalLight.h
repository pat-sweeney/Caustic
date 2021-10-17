//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Color;
#include "Rendering\Caustic\ILight.h"

//**********************************************************************
// File: IDirectionLight.h
// This file defines the published interface for directional lights
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Interface: IDirectionalLight
    // Defines a directional light (i.e. the Sun)
    //
    // Header:
    // {Link:#include "Rendering/Caustic/IDirectionalLight.h"{Rendering/Caustic/IDirectionalLight.h}}
    //**********************************************************************
    struct IDirectionalLight : public ILight
    {
        //**********************************************************************
        // Method: SetDirection
        // Sets the direction of the light
        //
        // Parameters:
        // dir - direction vector
        //**********************************************************************
        virtual void SetDirection(Vector3 &dir) = 0;

        //**********************************************************************
        // Method: GetDirection
        // Gets the direction of the light
        //
        // Returns:
        // Returns the direction of the light
        //**********************************************************************
        virtual Vector3 GetDirection() = 0;
    };

    //**********************************************************************
    // Function: CreateDirectionalLight
    // Creates a directional light
    //
    // Parameters:
    // pos - position of light in world coordinates
    // dir - direction light is pointing in world coordinates
    // color - color of light
    // intensity - intensity of light
    //
    // Header:
    // {Link:#include "Rendering/Caustic/IPointLight.h"{Rendering/Caustic/IPointLight.h}}
    //**********************************************************************
    CRefObj<IDirectionalLight> CreateDirectionalLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity = 1000.0f, bool casts = true);
}
