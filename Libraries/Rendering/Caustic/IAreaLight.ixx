//**********************************************************************
// Copyright Patrick Sweeney 2015-2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.IAreaLight;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Color;
import Rendering.Caustic.ILight;

//**********************************************************************
// File: IAreaLight.ixx
// This file defines the published interface for area lights.
// Area lights are rectangular emitters defined by a position, two
// edge vectors (width/height), and a normal direction.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: IAreaLight
    // Defines a rectangular area light.
    //
    // Module:
    // {Link:import Rendering.Caustic.IAreaLight;{Rendering/Caustic/IAreaLight.ixx}}
    //**********************************************************************
    struct IAreaLight : public ILight
    {
        //**********************************************************************
        // Method: SetDimensions
        // Sets the width and height of the area light.
        //
        // Parameters:
        // width - width of the rectangular emitter
        // height - height of the rectangular emitter
        //**********************************************************************
        virtual void SetDimensions(float width, float height) = 0;

        //**********************************************************************
        // Method: GetWidth
        // Returns the width of the area light.
        //**********************************************************************
        virtual float GetWidth() = 0;

        //**********************************************************************
        // Method: GetHeight
        // Returns the height of the area light.
        //**********************************************************************
        virtual float GetHeight() = 0;

        //**********************************************************************
        // Method: SetUpVector
        // Sets the up vector used to orient the area light rectangle.
        //
        // Parameters:
        // up - up vector
        //**********************************************************************
        virtual void SetUpVector(Vector3& up) = 0;

        //**********************************************************************
        // Method: GetUpVector
        // Returns the up vector of the area light.
        //**********************************************************************
        virtual Vector3 GetUpVector() = 0;

        //**********************************************************************
        // Method: GetCorners
        // Returns the four world-space corner positions of the area light.
        //
        // Parameters:
        // corners - array of 4 Vector3 to receive corner positions
        //**********************************************************************
        virtual void GetCorners(Vector3 corners[4]) = 0;
    };

    //**********************************************************************
    // Function: CreateAreaLight
    // Creates a rectangular area light.
    //
    // Parameters:
    // pos - center position of the area light
    // dir - normal direction of the light surface
    // up - up vector for orienting the rectangle
    // clr - color of the light
    // intensity - light intensity
    // width - width of the rectangular emitter
    // height - height of the rectangular emitter
    // castsShadows - does this light participate in shadow mapping?
    // castsLight - does this light illuminate the scene?
    //
    // Module:
    // {Link:import Rendering.Caustic.IAreaLight;{Rendering/Caustic/IAreaLight.ixx}}
    //**********************************************************************
    CRefObj<IAreaLight> CreateAreaLight(Vector3& pos, Vector3& dir, Vector3& up,
        FRGBColor& color, float intensity = 1000.0f,
        float width = 1.0f, float height = 1.0f,
        bool castsShadows = false, bool castsLight = true);
}
