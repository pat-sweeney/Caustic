//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <DirectXMath.h>

export module Rendering.Caustic.ICamera;
import Base.Core.Core;
import Base.Core.ISerialize;
import Base.Math.Vector;

//**********************************************************************
// File: ICamera.h
// This file defines the published interface camera objects
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: ICamera
    // The CCamera object defines our camera. We pass this to our IRenderer
    // to determine where to render from.
    //
    // Header:
    // {Link:import Rendering.Caustic.ICamera{Rendering/Caustic/ICamera.ixx}}
    //**********************************************************************
    struct ICamera : public ISerialize
    {
        //**********************************************************************
        // Method: BuildMatrices
        // Reconstructs the view and projection matrices for a camera
        //**********************************************************************
        virtual void BuildMatrices() = 0;

        //**********************************************************************
        // Method: SetParams
        // Sets the camera parameters
        //
        // Parameters:
        // fov - field of view in radians
        // aspectRatio - Camera's aspect ratio
        // nearZ - distance to near plane
        // farZ - distance to far plane
        //**********************************************************************
        virtual void SetParams(float fov, float aspectRatio, float nearZ, float farZ) = 0;

        //**********************************************************************
        // Method: GetParams
        // Gets the camera parameters
        //
        // Parameters:
        // fov - field of view in radians
        // aspectRatio - Camera's aspect ratio
        // nearZ - distance to near plane
        // farZ - distance to far plane
        //**********************************************************************
        virtual void GetParams(float *fov, float *aspectRatio, float *nearZ, float *farZ) = 0;

        //**********************************************************************
        // Method: SetPosition
        // Sets the camera's position.
        //
        // Parameters:
        // eye - camera's world position
        // look - camera's look point. Used to define the camera's view direction
        // up - camera's up point. The eye point and up point define the up vector.
        //**********************************************************************
        virtual void SetPosition(const Vector3 &eye, const Vector3 &look, const Vector3 &up) = 0;

        //**********************************************************************
        // Method: GetPosition
        // Gets the camera's position.
        //
        // Parameters:
        // eye - returns the camera's world position. Maybe nullptr.
        // look - returns the camera's look point. Maybe nullptr.
        // up - returns the camera's up point. Maybe nullptr.
        // pU - Returns the camera's X axis. Maybe nullptr.
        // pV - Returns the camera's Y axis. Maybe nullptr.
        // pN - Returns the camera's Z axis. Maybe nullptr.
        //**********************************************************************
        virtual void GetPosition(Vector3 *pEye, Vector3 *pLook, Vector3 *pUp, Vector3 *pU, Vector3 *pV, Vector3 *pN) = 0;

        //**********************************************************************
        // Method: SetOffset
        // Sets an offset that is applied to the camera's eye and look point.
        //
        // Parameters:
        // offset - offset to apply
        //**********************************************************************
        virtual void SetOffset(Vector3 &offset) = 0;

        //**********************************************************************
        // Method: GetOffset
        // Returns the current offset
        //
        // Parameters:
        // offset - Returns the offset that is being applied to the camera's eye and look point
        //**********************************************************************
        virtual void GetOffset(Vector3 &offset) = 0;

        //**********************************************************************
        // Method: GetProjection
        // Returns the camera's projection matrix
        //**********************************************************************
        virtual DirectX::XMMATRIX GetProjection() = 0;

        //**********************************************************************
        // Method: GetView
        // Returns the camera's view matrix
        //**********************************************************************
        virtual DirectX::XMMATRIX GetView() = 0;
        virtual void SetView(DirectX::XMMATRIX& mat) = 0;
        virtual void SetProjection(DirectX::XMMATRIX& mat) = 0;

        //**********************************************************************
        // Method: GetNear
        // Returns the camera's near plane distance
        //**********************************************************************
        virtual float GetNear() = 0;

        //**********************************************************************
        // Method: GetFar
        // Returns the camera's far plane distance
        //**********************************************************************
        virtual float GetFar() = 0;

        //**********************************************************************
        // Method: GetUVN
        // Returns the camera axis in world coordinates
        //
        // Parameters:
        // u - X axis
        // v - Y axis
        // n - Z axis
        //**********************************************************************
        virtual void GetUVN(Vector3 *u, Vector3 *v, Vector3 *n) = 0;
    };

    //**********************************************************************
    // Function: CreateCamera
    // Global function for creating a camera. This method should generally
    // not be called. Use the ICausticFactory to create new Caustic objects.
    //
    // Parameters:
    // leftHanded - indicates whether we are creating a left handed coordinate system
    //    or a right handed coordinates system.
    //
    // Returns:
    // Returns the newly created camera.
    //
    // Header:
    // {Link:import Rendering.Caustic.ICamera{Rendering/Caustic/ICamera.ixx}}
    //**********************************************************************
    CRefObj<ICamera> CreateCamera(bool leftHanded)
    {
        extern CRefObj<ICamera> CreateCameraInternal(bool leftHanded);
        return CreateCameraInternal(leftHanded);
    }
}
