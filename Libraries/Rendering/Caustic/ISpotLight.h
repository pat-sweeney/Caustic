//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
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
// File: ISpotLight.h
// This file defines the published interface for spot lights
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Interface: ISpotLight
    // Defines a spot light
    //
    // Header:
    // [Link:Rendering/Caustic/ISpotLight.h]
    //**********************************************************************
    struct ISpotLight : public ILight
    {
        //**********************************************************************
        // Method: SetAngles
        // Sets the inner (angles.x) and outer (angles.y) angles for the spotlight.
        // The inner angle defines the area around the direction vector where
        // the light is full intensity. The light intensity then falls off from
        // the inner angle to the outer angle. Areas outside the outer angle
        // are not effected by the light.
        //
        // Parameters:
        // angles - inner and out angles
        //**********************************************************************
        virtual void SetAngles(float2 angles) = 0;

        //**********************************************************************
        // Method: GetAngles
        // Gets the inner (angles.x) and outer (angles.y) angles for the spotlight.
        // The inner angle defines the area around the direction vector where
        // the light is full intensity. The light intensity then falls off from
        // the inner angle to the outer angle. Areas outside the outer angle
        // are not effected by the light.
        //
        // Returns:
        // Returns the inner and out angles
        //**********************************************************************
        virtual float2 GetAngles() = 0;

        //**********************************************************************
        // Method: SetDirection
        // Sets the direction of the light
        //
        // Parameters:
        // dir - direction vector
        //**********************************************************************
        virtual void SetDirection(Vector3& dir) = 0;

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
    // Function: CreateSpotLight
    // Creates a spot light
    //
    // Parameters:
    // pos - position in world coordinates for the light
    // dir - direction vector
    // clr - color of light
    // intensity - intensity of light
    // innerAngle - inner angle defining where light is full intensity
    // outerAngle - outer angle. Light falls off from full to no intensity between inner and outer angle
    // casts - does this light participate in shadow mapping
    //
    // Header:
    // [Link:Rendering/Caustic/IPointLight.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ISpotLight> CreateSpotLight(Vector3& pos, Vector3 &dir, FRGBColor& color, float intensity = 1000.0f, float innerAngle = 30.0f, float outerAngle = 45.0f, bool casts = true);
}
