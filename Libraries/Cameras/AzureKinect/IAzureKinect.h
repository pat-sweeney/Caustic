//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"
#include "Base\Math\BBox.h"
#include "Cameras\CameraBase\ICamera.h"
#include <k4a/k4a.h>
#include <vector>

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Interface: IAzureKinect
    // Base interface shared across all image types
    //**********************************************************************
    struct IAzureKinect : public Caustic::IDepthCameraDevice
    {
        using ICameraDevice::NextFrame;
        using IDepthCameraDevice::NextFrame;

        virtual bool NextFrame(IImage** ppColorImage, std::vector<Vector3>& pts, std::vector<Vector3>& normals, BBox3 &bbox) = 0;
        virtual void BuildRayMap(uint32 w, uint32 h, IImage** ppImage) = 0;
    };

    namespace AzureKinect
    {
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

        //**********************************************************************
        // Function: CreateAzureKinect
        // Creates a new instance of the Azure Kinect.
        //
        // Parameters:
        // deviceId - index of device to use
        // colorMode - color mode for the device
        // depthMode - depth mode for the device
        // fpsMode - frames per second mode
        //
        // Returns:
        // Returns the newly created camera
        //**********************************************************************
        CAUSTICAPI CRefObj<IAzureKinect> CreateAzureKinect(int deviceId, ColorMode colorMode, DepthMode depthMode, FPSMode fpsMode);
    }
}
