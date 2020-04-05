//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Vector.h"
#include "Imaging\Image\Image.h"
#include "Cameras\AzureKinect\IAzureKinect.h"

// Namespace: Caustic
namespace Caustic
{
    class AzureKinectDevice : public IAzureKinect, public CRefCount
    {
        int m_deviceIndex;
        k4a_device_t m_device;
        k4a_device_configuration_t m_config;
        k4a_calibration_t m_calibration;
        k4a_image_t m_colorImage;
        k4a_image_t m_depthImage;
        k4a_capture_t m_capture;
        bool m_cameraStarted;
    public:
        AzureKinectDevice();
        ~AzureKinectDevice();

        void Startup(int deviceId, AzureKinect::ColorMode colorMode, AzureKinect::DepthMode depthMode, AzureKinect::FPSMode fpsMode);

        //**********************************************************************
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ICamera
        //**********************************************************************
        void NextFrame(IImage** ppColorImage, IImage** ppDepthImage, IImage** ppIRImage);
    };
}
