//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"
#include "Cameras\CameraBase\ICamera.h"
#include <k4a/k4a.h>

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Interface: IAzureKinect
    // Base interface shared across all image types
    //**********************************************************************
    struct IAzureKinect : public ICamera
    {
        //**********************************************************************
        // Method: NextFrame
        // Retrieves the next color/depth/ir image from the camera
        // Parameters:
        // ppColorImage - returns the next color image retrieved from the camera. Maybe nullptr.
        // ppDepthImage - returns the next depth image retrieved from the camera. Maybe nullptr.
        // ppIRImage - returns the next infrared image retrieved from the camera. Maybe nullptr.
        //**********************************************************************
        virtual void NextFrame(IImage** ppColorImage, IImage** ppDepthImage, IImage** ppIRImage) = 0;
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
        extern void CreateAzureKinect(int deviceId, ColorMode colorMode, DepthMode depthMode, FPSMode fpsMode, IAzureKinect** ppCamera);
    }
}
