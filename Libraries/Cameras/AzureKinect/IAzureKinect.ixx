//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Cameras.AzureKinect.IAzureKinect;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Base.Math.BBox;
import Cameras.CameraBase.ICamera;

// Namespace: Caustic
export namespace Caustic
{
    namespace AzureKinect
    {
        enum Joint
        {
            Pelvis = 0,
            SpineNavel,
            SpineChest,
            Neck,
            ClavicleLeft,
            ShoulderLeft,
            ElbowLeft,
            WristLeft,
            HandLeft,
            HandTipLeft,
            ThumbLeft,
            ClavicleRight,
            ShoulderRight,
            ElbowRight,
            WristRight,
            HandRight,
            HandTipRight,
            ThumbRight,
            HipLeft,
            KneeLeft,
            AnkleLeft,
            FootLeft,
            HipRight,
            KneeRight,
            AnkleRight,
            FootRight,
            Head,
            Nose,
            EyeLeft,
            EarLeft,
            EyeRight,
            EarRight,
            Count
        };

        extern const wchar_t* JointNames[];
        
        enum DepthMode
        {
            DepthOff,
            Depth320x288,
            Depth640x576,
            Depth512x512,
            Depth1024x1024,
            DepthIROnly
        };

        enum ColorMode
        {
            ColorOff,
            Color720p,
            Color1080p,
            Color1440p,
            Color1536p,
            Color2160p,
            Color3072p
        };

        enum FPSMode
        {
            FPS5,
            FPS15,
            FPS30
        };
    }

    //**********************************************************************
    // Interface: IAzureKinect
    // Base interface shared across all image types
    //
    // Header:
    // {Link:import Cameras.AzureKinect.IAzureKinect{Cameras/AzureKinect/IAzureKinect.ixx}}
    //**********************************************************************
    struct IAzureKinect : public Caustic::IDepthCameraDevice
    {
        using ICameraDevice::NextVideoFrame;
        using IDepthCameraDevice::NextVideoFrame;

        virtual bool NextVideoFrame(IImage** ppColorImage, std::vector<Vector3>& pts, std::vector<Vector3>& normals, BBox3 &bbox) = 0;
        virtual CRefObj<IImage> BuildRayMap(uint32_t w, uint32_t h, bool forDepth = true) = 0;
        virtual CameraIntrinsics GetAzureColorIntrinsics() = 0;
        virtual CameraIntrinsics GetAzureDepthIntrinsics() = 0;

        virtual bool BodyTrackingOn() = 0;
        virtual int NumberBodiesDetected() = 0;
        virtual AzureKinect::Joint GetParentJoint(AzureKinect::Joint joint) = 0;
        virtual Matrix4x4 GetJointMatrix(int bodyIndex, AzureKinect::Joint joint) = 0;
    };

    namespace AzureKinect
    {
        //**********************************************************************
        // Function: CreateAzureKinect
        // Creates a new instance of the Azure Kinect.
        //
        // Parameters:
        // deviceId - index of device to use
        // colorMode - color mode for the device
        // depthMode - depth mode for the device
        // fpsMode - frames per second mode
        // captureBodies - should skeleton tracking be used
        //
        // Returns:
        // Returns the newly created camera
        //
        // Header:
    // {Link:import Cameras.AzureKinect.IAzureKinect{Cameras/AzureKinect/IAzureKinect.ixx}}
        //**********************************************************************
        CRefObj<IAzureKinect> CreateAzureKinect(int deviceId, ColorMode colorMode, DepthMode depthMode, FPSMode fpsMode, bool captureBodies = false);
    }
}
