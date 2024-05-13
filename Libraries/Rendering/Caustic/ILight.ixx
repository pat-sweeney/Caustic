//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.ILight;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Color;

//**********************************************************************
// File: ILight.h
// This file defines the published base interface for dealing with various types of lights
//**********************************************************************

export namespace Caustic
{
    enum class ELightType
    {
        PointLight,
        DirectionalLight,
        SpotLight,
        AreaLight
    };

    //**********************************************************************
    // Interface: ILight
    // Defines a properties common to all lights
    //
    // Module:
    // {Link:import Rendering.Caustic.ILight;{Rendering/Caustic/ILight.ixx}}
    //**********************************************************************
    struct ILight : public IRefCount
    {
        //**********************************************************************
        // Method: GetType
        // Returns the type of light
        //**********************************************************************
        virtual ELightType GetType() = 0;

        //**********************************************************************
        // Method: GetOnOff
        // Returns whether the light is currently on
        //**********************************************************************
        virtual bool GetOnOff() = 0;

        //**********************************************************************
        // Method: SetOnOff
        // Returns a light on or off
        // 
        // Parameters:
        // f - if true, turn light on. Otherwise turn it off
        //**********************************************************************
        virtual void SetOnOff(bool f) = 0;

        //**********************************************************************
        // Method: SetIntensity
        // Sets the lights intensity (some rough notion of radiant intensity)
        // This value falls off as intensity/D^2 (where D is distance in meters from object to light)
        //
        // Parameters:
        // intensity - intensity of light
        //**********************************************************************
        virtual void SetIntensity(float intensity) = 0;

        //**********************************************************************
        // Method: GetIntensity
        // Returns the lights intensity (some rough notion of radiant intensity)
        // This value falls off as intensity/D^2 (where D is distance in meters from object to light)
        //
        // Return:
        // Lights intensity
        //**********************************************************************
        virtual float GetIntensity() = 0;

        //**********************************************************************
        // Method: SetCastsLight
        // Indicates whether this light illuminates the scene. By default all lights
        // do, however it is possible to create a light that only is used for shadow
        // calculations by turning this flag to false and CastsShadows to true.
        //
        // Parameters:
        // casts - True if light should participates in illuminating the scene. False otherwise.
        //**********************************************************************
        virtual void SetCastsLight(bool casts) = 0;

        //**********************************************************************
        // Method: GetCastsLight
        // Returns whether this light participates in illuminating the scene
        //
        // Return:
        // True if light does participate in illuminating the scene. False otherwise.
        //**********************************************************************
        virtual bool GetCastsLight() = 0;

        //**********************************************************************
        // Method: SetCastsShadows
        // Indicates whether this light participates in shadow mapping
        //
        // Parameters:
        // casts - True if light should participate in shadow mapping. False otherwise.
        //**********************************************************************
        virtual void SetCastsShadows(bool casts) = 0;

        //**********************************************************************
        // Method: GetCastsShadows
        // Returns whether this light participates in shadow mapping
        //
        // Return:
        // True if light does participate in shadow mapping. False otherwise.
        //**********************************************************************
        virtual bool GetCastsShadows() = 0;

        //**********************************************************************
        // Method: SetPosition
        // Sets the position of the light
        //
        // Parameters:
        // pos - Position of the light in world coordinates
        //**********************************************************************
        virtual void SetPosition(Vector3& pos) = 0;

        //**********************************************************************
        // Method: GetPosition
        // Gets the position of the light
        //
        // Returns:
        // Position of the light in world coordinates
        //**********************************************************************
        virtual Vector3 GetPosition() = 0;

        //**********************************************************************
        // Method: GetDirection
        // Gets the direction the light is pointing. For nondirectional lights
        // this will return a zero vector
        //
        // Returns:
        // Direction of the light in world coordinates
        //**********************************************************************
        virtual Vector3 GetDirection() = 0;

        //**********************************************************************
        // Method: SetDirection
        // Sets the direction the light is pointing. For nondirectional lights
        // this will be a zero vector
        //
        // Parameters:
        // dir - Direction of the light in world coordinates
        //**********************************************************************
        virtual void SetDirection(Vector3& dir) = 0;

        //**********************************************************************
        // Method: SetColor
        // Sets the color of the light
        //
        // Parameters:
        // color - Color of the light
        //**********************************************************************
        virtual void SetColor(FRGBColor& color) = 0;

        //**********************************************************************
        // Method: GetColor
        // Gets the color of the light
        //
        // Returns:
        // Color of the light
        //**********************************************************************
        virtual FRGBColor GetColor() = 0;
    };
}
