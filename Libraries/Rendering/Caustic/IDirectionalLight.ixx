//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.IDirectionalLight;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Color;
import Rendering.Caustic.ILight;

//**********************************************************************
// File: IDirectionLight.h
// This file defines the published interface for directional lights
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: IDirectionalLight
    // Defines a directional light (i.e. the Sun)
    //
    // Module:
    // {Link:import Rendering.Caustic.IDirectionalLight;{Rendering/Caustic/IDirectionalLight.ixx}}
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
    // Module:
    // {Link:import Rendering.Caustic.IDirectionalLight;{Rendering/Caustic/IDirectionalLight.ixx}}
    //**********************************************************************
    CRefObj<IDirectionalLight> CreateDirectionalLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity = 1000.0f, bool casts = true);
}
