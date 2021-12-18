//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <k4a/k4a.h>
#include <k4abt.h>
#include <memory>

module Cameras.AzureKinect.IAzureKinect;
import Base.Core.Core;
import Base.Core.IRefCount;
import Cameras.AzureKinect.AzureKinect;

// Namespace: Caustic
namespace Caustic
{
    namespace AzureKinect
    {
        //**********************************************************************
        // Function: CreateAzureKinect
        // CreateAzureKinect instantiates an instance of an Azure Kinect camera
        //
        // Parameters:
        // deviceId - indicates which device to use
        // colorMode - output size of color images
        // depthMode - output size of depth images
        // fpsMode - requested frame rate
        // captureBodies - should skeleton tracking be used
        //
        // Returns:
        // Returns the created Azure Kinect device
        //**********************************************************************
        CRefObj<IAzureKinect> CreateAzureKinect(int deviceId, ColorMode colorMode, DepthMode depthMode, FPSMode fpsMode, bool captureBodies /* = false */)
        {
            std::unique_ptr<CAzureKinectDevice> pCamera(new CAzureKinectDevice());
            pCamera->Startup(deviceId, colorMode, depthMode, fpsMode, captureBodies);
            return CRefObj<IAzureKinect>(pCamera.release());
        }
    }
}