//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "AzureKinect.h"
#include "Base\Core\error.h"
#include <k4a/k4a.h>
#include <memory>

namespace Caustic
{
    namespace AzureKinect
    {
        //**********************************************************************
        // CreateAzureKinect instantiates an instance of an Azure Kinect camera
        // Parameters:
        // deviceId - indicates which device to use
        // colorMode - output size of color images
        // depthMode - output size of depth images
        // fpsMode - requested frame rate
        // Returns:
        // ppCamera - returns the created Azure Kinect device
        //**********************************************************************
        void CreateAzureKinect(int deviceId, ColorMode colorMode, DepthMode depthMode, FPSMode fpsMode, IAzureKinect** ppCamera)
        {
            std::unique_ptr<AzureKinectDevice> pCamera(new AzureKinectDevice());
            pCamera->Startup(deviceId, colorMode, depthMode, fpsMode);
            *ppCamera = pCamera.release();
            (*ppCamera)->AddRef();
        }
    }

    AzureKinectDevice::AzureKinectDevice() :
        m_deviceIndex(0),
        m_device(nullptr),
        m_colorImage(nullptr),
        m_depthImage(nullptr),
        m_capture(nullptr),
        m_cameraStarted(false)
    {
        m_config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
        m_config.depth_mode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
        m_config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
        m_config.camera_fps = K4A_FRAMES_PER_SECOND_30;
    }

    AzureKinectDevice::~AzureKinectDevice()
    {
        if (m_cameraStarted)
            k4a_device_stop_cameras(m_device);
        if (m_device != nullptr)
            k4a_device_close(m_device);
    }

    void AzureKinectDevice::Startup(int deviceId, AzureKinect::ColorMode colorMode, AzureKinect::DepthMode depthMode, AzureKinect::FPSMode fpsMode)
    {
        if (deviceId >= (int)k4a_device_get_installed_count())
            CT(E_FAIL);
        if (k4a_device_open(deviceId, &m_device) != K4A_RESULT_SUCCEEDED)
            CT(E_FAIL);

        k4a_fps_t ak_fps = K4A_FRAMES_PER_SECOND_30;
        switch (fpsMode)
        {
        case AzureKinect::FPSMode::FPS5:
            ak_fps = K4A_FRAMES_PER_SECOND_5;
            break;
        case AzureKinect::FPSMode::FPS15:
            ak_fps = K4A_FRAMES_PER_SECOND_15;
            break;
        case AzureKinect::FPSMode::FPS30:
            ak_fps = K4A_FRAMES_PER_SECOND_30;
            break;
        }

        k4a_color_resolution_t ak_colorMode = K4A_COLOR_RESOLUTION_OFF;
        switch (colorMode)
        {
        case AzureKinect::ColorMode::ColorOff:
            ak_colorMode = K4A_COLOR_RESOLUTION_OFF;
            break;
        case AzureKinect::ColorMode::Color720p:
            ak_colorMode = K4A_COLOR_RESOLUTION_720P;
            break;
        case AzureKinect::ColorMode::Color1080p:
            ak_colorMode = K4A_COLOR_RESOLUTION_1080P;
            break;
        case AzureKinect::ColorMode::Color1440p:
            ak_colorMode = K4A_COLOR_RESOLUTION_1440P;
            break;
        case AzureKinect::ColorMode::Color1536p:
            ak_colorMode = K4A_COLOR_RESOLUTION_1536P;
            break;
        case AzureKinect::ColorMode::Color2160p:
            ak_colorMode = K4A_COLOR_RESOLUTION_2160P;
            break;
        case AzureKinect::ColorMode::Color3072p:
            if (ak_fps == K4A_FRAMES_PER_SECOND_30)
                ak_fps = K4A_FRAMES_PER_SECOND_15;
            ak_colorMode = K4A_COLOR_RESOLUTION_3072P;
            break;
        }

        k4a_depth_mode_t ak_depthMode = K4A_DEPTH_MODE_OFF;
        switch (depthMode)
        {
        case AzureKinect::DepthMode::DepthOff:
            ak_depthMode = K4A_DEPTH_MODE_OFF;
            break;
        case AzureKinect::DepthMode::Depth320x288:
            ak_depthMode = K4A_DEPTH_MODE_NFOV_2X2BINNED;
            break;
        case AzureKinect::DepthMode::Depth640x576:
            ak_depthMode = K4A_DEPTH_MODE_NFOV_UNBINNED;
            break;
        case AzureKinect::DepthMode::Depth512x512:
            ak_depthMode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
            break;
        case AzureKinect::DepthMode::Depth1024x1024:
            if (ak_fps == K4A_FRAMES_PER_SECOND_30)
                ak_fps = K4A_FRAMES_PER_SECOND_15;
            ak_depthMode = K4A_DEPTH_MODE_WFOV_UNBINNED;
            break;
        case AzureKinect::DepthMode::DepthIROnly:
            ak_depthMode = K4A_DEPTH_MODE_PASSIVE_IR;
            break;
        }

        m_config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
        m_config.color_resolution = ak_colorMode;
        m_config.depth_mode = ak_depthMode;
        m_config.camera_fps = ak_fps;
        m_config.synchronized_images_only = false;
        m_config.depth_delay_off_color_usec = 0;
        m_config.wired_sync_mode = K4A_WIRED_SYNC_MODE_STANDALONE;
        m_config.subordinate_delay_off_master_usec = 0;
        m_config.disable_streaming_indicator = false;

        if (k4a_device_get_calibration(m_device, m_config.depth_mode, m_config.color_resolution, &m_calibration) != K4A_RESULT_SUCCEEDED)
            CT(E_FAIL);
        if (K4A_RESULT_SUCCEEDED != k4a_device_start_cameras(m_device, &m_config))
            CT(E_FAIL);
        m_cameraStarted = true;
    }
    
    void AzureKinectDevice::NextFrame(IImage** ppColorImage, IImage** ppDepthImage, IImage** ppIRImage)
    {
        if (k4a_device_get_capture(m_device, &m_capture, 3000) == K4A_RESULT_SUCCEEDED)
        {
            if (ppColorImage != nullptr)
            {
                auto colorimage = k4a_capture_get_color_image(m_capture);
                if (colorimage != nullptr)
                {
                    uint8_t* buffer = k4a_image_get_buffer(colorimage);
                    if (buffer != nullptr)
                    {
                        int w = k4a_image_get_width_pixels(colorimage);
                        int h = k4a_image_get_height_pixels(colorimage);
                        size_t s = k4a_image_get_size(colorimage);
                        CRefObj<IImage> spImage;
                        CreateImage(w, h, 32, &spImage);
                        memcpy(spImage->GetData(), buffer, spImage->GetHeight() * spImage->GetStride());
                        *ppColorImage = spImage.Detach();
                    }
                    k4a_image_release(colorimage);
                }
            }
            if (ppDepthImage != nullptr)
            {
                auto depthimage = k4a_capture_get_depth_image(m_capture);
                if (depthimage != nullptr)
                {
                    uint8_t* buffer = k4a_image_get_buffer(depthimage);
                    if (buffer != nullptr)
                    {
                        int w = k4a_image_get_width_pixels(depthimage);
                        int h = k4a_image_get_height_pixels(depthimage);
                        size_t s = k4a_image_get_size(depthimage);
                        CRefObj<IImage> spImage;
                        CreateImage(w, h, 16, &spImage);
                        memcpy(spImage->GetData(), buffer, spImage->GetHeight() * spImage->GetStride());
                        *ppDepthImage = spImage.Detach();
                    }
                    k4a_image_release(depthimage);
                }
            }
            k4a_capture_release(m_capture);
            m_capture = nullptr;
        }
    }
}
