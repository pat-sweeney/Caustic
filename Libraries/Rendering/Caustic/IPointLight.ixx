//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.IPointLight;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Color;
import Rendering.Caustic.ILight;

//**********************************************************************
// File: IPointLight.h
// This file defines the published interface for dealing with point lights
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: IPointLight
    // Defines a point light
    //
    // Module:
    // {Link:import Rendering.Caustic.IPointLight;{Rendering/Caustic/IPointLight.ixx}}
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
    // castsShadows - indicates whether this light participates in shadow mapping
    // castsLight - Does this light illuminate the scene?
    //
    // Module:
    // {Link:import Rendering.Caustic.IPointLight;{Rendering/Caustic/IPointLight.ixx}}
    //**********************************************************************
    CRefObj<IPointLight> CreatePointLight(Vector3& pos, FRGBColor& color, float intensity, bool castsShadows = true, bool castsLight = true);
}
