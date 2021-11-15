//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.ISpotLight;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Color;
import Rendering.Caustic.ILight;

//**********************************************************************
// File: ISpotLight.h
// This file defines the published interface for spot lights
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISpotLight
    // Defines a spot light
    //
    // Module:
    // {Link:import Rendering.Caustic.ISpotLight;{Rendering/Caustic/ISpotLight.ixx}}
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
    // Module:
    // {Link:import Rendering.Caustic.ISpotLight;{Rendering/Caustic/ISpotLight.ixx}}
    //**********************************************************************
    CRefObj<ISpotLight> CreateSpotLight(Vector3& pos, Vector3 &dir, FRGBColor& color, float intensity = 1000.0f, float innerAngle = 30.0f, float outerAngle = 45.0f, bool casts = true);
}
