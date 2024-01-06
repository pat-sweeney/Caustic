//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Cameras.CameraBase.ICamera;
import Base.Core.Core;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Base.Math.Matrix;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Interface: IAudioFrame
    // Defines the interface to an audio buffer
    //
    // Module:
    // {Link:import Cameras.CameraBase.ICamera;{Cameras/CameraBase/ICamera.ixx}}
    //**********************************************************************
    struct IAudioFrame : public IRefCount
    {
    };

    //**********************************************************************
    // Struct: CameraIntrinsics
    // Defines intrinsics for a camera
    //
    // Header:
    // {Link:import Cameras.CameraBase.ICamera;{Cameras/CameraBase/ICamera.ixx}}
    //**********************************************************************
    struct CameraIntrinsics
    {
        float cx;
        float cy;
        float fx;
        float fy;
        float k1;
        float k2;
        float k3;
        float k4;
        float k5;
        float k6;
        float p1;
        float p2;
        float codx;
        float cody;
        float metricRadius;
        int type;
    };

    //**********************************************************************
    // Interface: ICameraDevice
    // Base interface shared across all camera types
    //
    // Header:
    // {Link:import Cameras.CameraBase.ICamera;{Cameras/CameraBase/ICamera.ixx}}
    //**********************************************************************
    struct ICameraDevice : public IRefCount
    {
        //**********************************************************************
        // Method: NextVideoFrame
        // Retrieves the next color/depth/ir image from the camera
        // Parameters:
        // ppColorImage - returns the next color image retrieved from the camera. Maybe nullptr.
        //**********************************************************************
        virtual bool NextVideoFrame(IImage** ppColorImage) = 0;
        
        //**********************************************************************
        // Method: NextAudioFrame
        // Retrieves the next audio frame from the audio device
        // Parameters:
        // ppAudioFrame - returns the next audio frame retrieved from the audio device. Maybe nullptr.
        //**********************************************************************
        virtual bool NextAudioFrame(IAudioFrame** ppAudioFrame) = 0;

        //**********************************************************************
        // Method: GetColorWidth
        // Returns the width in pixels of the color image returned by the camera
        //
        // Returns:
        // Width of the color image in pixels
        //**********************************************************************
        virtual uint32_t GetColorWidth() = 0;

        //**********************************************************************
        // Method: GetColorHeight
        // Returns the height in pixels of the color image returned by the camera
        //
        // Returns:
        // Height of the color image in pixels
        //**********************************************************************
        virtual uint32_t GetColorHeight() = 0;
    };

    //**********************************************************************
    // Interface: IDepthCameraDevice
    // Interface for depth cameras
    //
    // Header:
    // {Link:import Cameras.CameraBase.ICamera;{Cameras/CameraBase/ICamera.ixx}}
    //**********************************************************************
    struct IDepthCameraDevice : public ICameraDevice
    {
        using ICameraDevice::NextVideoFrame;

        //**********************************************************************
        // Method: NextVideoFrame
        // Retrieves the next color/depth/ir image from the camera
        //
        // Parameters:
        // ppColorImage - returns the next color image retrieved from the camera. Maybe nullptr.
        // ppDepthImage - returns the next depth image retrieved from the camera. Maybe nullptr.
        // ppIRImage - returns the next infrared image retrieved from the camera. Maybe nullptr.
        //**********************************************************************
        virtual bool NextVideoFrame(IImage** ppColorImage, IImage** ppDepthImage, IImage** ppIRImage) = 0;

        //**********************************************************************
        // Method: NextAudioFrame
        // Retrieves the next audio frame from the audio device
        //
        // Parameters:
        // ppAudioFrame - returns the next audio frame retrieved from the audio device. Maybe nullptr.
        //**********************************************************************
        virtual bool NextAudioFrame(IAudioFrame** ppAudioFrame) = 0;

        //**********************************************************************
        // Method: GetDepthWidth
        // Returns the width in pixels of the depth image returned by the camera
        //
        // Returns:
        // Width of the depth image in pixels
        //**********************************************************************
        virtual uint32_t GetDepthWidth() = 0;

        //**********************************************************************
        // Method: GetDepthHeight
        // Returns the height in pixels of the depth image returned by the camera
        //
        // Returns:
        // Height of the depthimage in pixels
        //**********************************************************************
        virtual uint32_t GetDepthHeight() = 0;

        //**********************************************************************
        // Method: ColorExtrinsics
        // Returns the extrinsics matrix for the color camera.
        //
        // Returns:
        // Matrix4x4 for the extrinsics for the color camera
        //**********************************************************************
        virtual Matrix4x4 ColorExtrinsics() = 0;

        //**********************************************************************
        // Method: ColorIntrinsics
        // Returns the intrinsics matrix for the color camera.
        //
        // Returns:
        // Matrix4x4 for the intrinsics for the color camera
        //**********************************************************************
        virtual Matrix3x3 ColorIntrinsics() = 0;

        //**********************************************************************
        // Method: DepthExtrinsics
        // Returns the extrinsics matrix for the depth camera.
        //
        // Returns:
        // Matrix4x4 for the extrinsics for the depth camera
        //**********************************************************************
        virtual Matrix4x4 DepthExtrinsics() = 0;

        //**********************************************************************
        // Method: DepthIntrinsics
        // Returns the intrinsics matrix for the depth camera.
        //
        // Returns:
        // Matrix4x4 for the intrinsics for the depth camera
        //**********************************************************************
        virtual Matrix3x3 DepthIntrinsics() = 0;
    };
}
