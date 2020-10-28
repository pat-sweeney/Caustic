//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "AzureKinect.h"
#include "Base\Core\error.h"
#include "Base\Math\BBox.h"
#include "Imaging\Image\ImageIter.h"
#include <k4a/k4a.h>
#include <memory>

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
        //
        // Returns:
        // Returns the created Azure Kinect device
        //**********************************************************************
        CRefObj<IAzureKinect> CreateAzureKinect(int deviceId, ColorMode colorMode, DepthMode depthMode, FPSMode fpsMode)
        {
            std::unique_ptr<CAzureKinectDevice> pCamera(new CAzureKinectDevice());
            pCamera->Startup(deviceId, colorMode, depthMode, fpsMode);
            return CRefObj<IAzureKinect>(pCamera.release());
        }
    }

    CAzureKinectDevice::CAzureKinectDevice() :
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

    CAzureKinectDevice::~CAzureKinectDevice()
    {
        if (m_cameraStarted)
            k4a_device_stop_cameras(m_device);
        if (m_device != nullptr)
            k4a_device_close(m_device);
    }

    void CAzureKinectDevice::Startup(int deviceId, AzureKinect::ColorMode colorMode, AzureKinect::DepthMode depthMode, AzureKinect::FPSMode fpsMode)
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
        uint32 colorW = 0, colorH = 0;
        switch (colorMode)
        {
        case AzureKinect::ColorMode::ColorOff:
            ak_colorMode = K4A_COLOR_RESOLUTION_OFF;
            break;
        case AzureKinect::ColorMode::Color720p:
            ak_colorMode = K4A_COLOR_RESOLUTION_720P;
            colorW = 1280;
            colorH = 720;
            break;
        case AzureKinect::ColorMode::Color1080p:
            ak_colorMode = K4A_COLOR_RESOLUTION_1080P;
            colorW = 1920;
            colorH = 1080;
            break;
        case AzureKinect::ColorMode::Color1440p:
            ak_colorMode = K4A_COLOR_RESOLUTION_1440P;
            colorW = 2560;
            colorH = 1440;
            break;
        case AzureKinect::ColorMode::Color1536p:
            ak_colorMode = K4A_COLOR_RESOLUTION_1536P;
            colorW = 2048;
            colorH = 1536;
            break;
        case AzureKinect::ColorMode::Color2160p:
            ak_colorMode = K4A_COLOR_RESOLUTION_2160P;
            colorW = 3840;
            colorH = 2160;
            break;
        case AzureKinect::ColorMode::Color3072p:
            if (ak_fps == K4A_FRAMES_PER_SECOND_30)
                ak_fps = K4A_FRAMES_PER_SECOND_15;
            ak_colorMode = K4A_COLOR_RESOLUTION_3072P;
            colorW = 4906;
            colorH = 3072;
            break;
        }

        k4a_depth_mode_t ak_depthMode = K4A_DEPTH_MODE_OFF;
        int depthW = 0, depthH = 0;
        switch (depthMode)
        {
        case AzureKinect::DepthMode::DepthOff:
            ak_depthMode = K4A_DEPTH_MODE_OFF;
            break;
        case AzureKinect::DepthMode::Depth320x288:
            ak_depthMode = K4A_DEPTH_MODE_NFOV_2X2BINNED;
            depthW = 320;
            depthH = 288;
            break;
        case AzureKinect::DepthMode::Depth640x576:
            ak_depthMode = K4A_DEPTH_MODE_NFOV_UNBINNED;
            depthW = 640;
            depthH = 576;
            break;
        case AzureKinect::DepthMode::Depth512x512:
            ak_depthMode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
            depthW = 512;
            depthH = 512;
            break;
        case AzureKinect::DepthMode::Depth1024x1024:
            if (ak_fps == K4A_FRAMES_PER_SECOND_30)
                ak_fps = K4A_FRAMES_PER_SECOND_15;
            ak_depthMode = K4A_DEPTH_MODE_WFOV_UNBINNED;
            depthW = 1024;
            depthH = 1024;
            break;
        case AzureKinect::DepthMode::DepthIROnly:
            ak_depthMode = K4A_DEPTH_MODE_PASSIVE_IR;
            depthW = 1024;
            depthH = 1024;
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

        m_spColorImagePool = Caustic::CreateImagePool(20, colorW, colorH, 32);
        m_spDepthImagePool = Caustic::CreateImagePool(20, depthW, depthH, 16);

        m_cameraStarted = true;
    }
    
    //**********************************************************************
    // Method: Nextframe
    // See <IDepthCameraDevice::NextFrame>
    //**********************************************************************
    bool CAzureKinectDevice::NextFrame(IImage** ppColorImage)
    {
        return NextFrame(ppColorImage, nullptr, nullptr);
    }
    
    //**********************************************************************
    // Method: ColorExtrinsics
    // See <IDepthCameraDevice::ColorExtrinsics>
    //**********************************************************************
    Matrix4x4 CAzureKinectDevice::ColorExtrinsics()
    {
        if (!m_cameraStarted)
            CT(E_FAIL); // Camera not started
        Matrix4x4 mat;
        mat[0][0] = m_calibration.color_camera_calibration.extrinsics.rotation[0];
        mat[0][1] = m_calibration.color_camera_calibration.extrinsics.rotation[1];
        mat[0][2] = m_calibration.color_camera_calibration.extrinsics.rotation[2];
        mat[0][3] = 0.0f;
        mat[1][0] = m_calibration.color_camera_calibration.extrinsics.rotation[3];
        mat[1][1] = m_calibration.color_camera_calibration.extrinsics.rotation[4];
        mat[1][2] = m_calibration.color_camera_calibration.extrinsics.rotation[5];
        mat[1][3] = 0.0f;
        mat[2][0] = m_calibration.color_camera_calibration.extrinsics.rotation[6];
        mat[2][1] = m_calibration.color_camera_calibration.extrinsics.rotation[7];
        mat[2][2] = m_calibration.color_camera_calibration.extrinsics.rotation[8];
        mat[2][3] = 0.0f;
        mat[3][0] = m_calibration.color_camera_calibration.extrinsics.translation[0];
        mat[3][1] = m_calibration.color_camera_calibration.extrinsics.translation[0];
        mat[3][2] = m_calibration.color_camera_calibration.extrinsics.translation[0];
        mat[3][3] = 1.0f;
        return mat;
    }
    //**********************************************************************
    // Method: ColorIntrinsics
    // See <IDepthCameraDevice::ColorIntrinsics>
    //**********************************************************************
    Matrix3x3 CAzureKinectDevice::ColorIntrinsics()
    {
        if (!m_cameraStarted)
            CT(E_FAIL); // Camera not started
        Matrix3x3 mat;
        mat[0][0] = m_calibration.color_camera_calibration.intrinsics.parameters.param.fx;
        mat[1][1] = m_calibration.color_camera_calibration.intrinsics.parameters.param.fy;
        mat[2][2] = 1.0f;
        mat[2][0] = m_calibration.color_camera_calibration.intrinsics.parameters.param.cx;
        mat[2][1] = m_calibration.color_camera_calibration.intrinsics.parameters.param.cy;
        mat[0][1] = 0.0f;
        mat[0][2] = 0.0f;
        mat[1][0] = 0.0f;
        mat[1][2] = 0.0f;
        return mat;
    }
    //**********************************************************************
    // Method: DepthExtrinsics
    // See <IDepthCameraDevice::DepthExtrinsics>
    //**********************************************************************
    Matrix4x4 CAzureKinectDevice::DepthExtrinsics()
    {
        if (!m_cameraStarted)
            CT(E_FAIL); // Camera not started
        Matrix4x4 mat;
        mat[0][0] = m_calibration.color_camera_calibration.extrinsics.rotation[0];
        mat[0][1] = m_calibration.color_camera_calibration.extrinsics.rotation[1];
        mat[0][2] = m_calibration.color_camera_calibration.extrinsics.rotation[2];
        mat[0][3] = 0.0f;
        mat[1][0] = m_calibration.color_camera_calibration.extrinsics.rotation[3];
        mat[1][1] = m_calibration.color_camera_calibration.extrinsics.rotation[4];
        mat[1][2] = m_calibration.color_camera_calibration.extrinsics.rotation[5];
        mat[1][3] = 0.0f;
        mat[2][0] = m_calibration.color_camera_calibration.extrinsics.rotation[6];
        mat[2][1] = m_calibration.color_camera_calibration.extrinsics.rotation[7];
        mat[2][2] = m_calibration.color_camera_calibration.extrinsics.rotation[8];
        mat[2][3] = 0.0f;
        mat[3][0] = m_calibration.color_camera_calibration.extrinsics.translation[0];
        mat[3][1] = m_calibration.color_camera_calibration.extrinsics.translation[0];
        mat[3][2] = m_calibration.color_camera_calibration.extrinsics.translation[0];
        mat[3][3] = 1.0f;
        return mat;
    }
    
    //**********************************************************************
    // Method: DepthIntrinsics
    // See <IDepthCameraDevice::DepthIntrinsics>
    //**********************************************************************
    Matrix3x3 CAzureKinectDevice::DepthIntrinsics()
    {
        if (!m_cameraStarted)
            CT(E_FAIL); // Camera not started
        Matrix3x3 mat;
        mat[0][0] = m_calibration.depth_camera_calibration.intrinsics.parameters.param.fx;
        mat[1][1] = m_calibration.depth_camera_calibration.intrinsics.parameters.param.fy;
        mat[2][2] = 1.0f;
        mat[2][0] = m_calibration.depth_camera_calibration.intrinsics.parameters.param.cx;
        mat[2][1] = m_calibration.depth_camera_calibration.intrinsics.parameters.param.cy;
        mat[0][1] = 0.0f;
        mat[0][2] = 0.0f;
        mat[1][0] = 0.0f;
        mat[1][2] = 0.0f;
        return mat;
    }

    bool CAzureKinectDevice::NextFrame(IImage** ppColorImage, IImage** ppDepthImage, IImage** ppIRImage)
    {
        if (ppColorImage)
            *ppColorImage = nullptr;
        if (ppDepthImage)
            *ppDepthImage = nullptr;
        bool captured = false;
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
                        int stride = k4a_image_get_stride_bytes(colorimage);
                        CRefObj<IImage> spImage = m_spColorImagePool->Acquire(w, h, 32);
                        uint8* pRow = spImage->GetData();
                        for (int y = 0; y < h; y++)
                        {
                            memcpy(pRow, &buffer[y*stride], spImage->GetWidth() * spImage->GetBytesPerPixel());
                            pRow += spImage->GetStride();
                        }
                        *ppColorImage = spImage.Detach();
                        captured = true;
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
                        CRefObj<IImage> spImage = m_spDepthImagePool->Acquire(w, h, 16);
                        memcpy(spImage->GetData(), buffer, spImage->GetHeight() * spImage->GetStride());
                        *ppDepthImage = spImage.Detach();
                        captured = true;
                    }
                    k4a_image_release(depthimage);
                }
            }
            k4a_capture_release(m_capture);
            m_capture = nullptr;
        }
        return captured;
    }
    
    CRefObj<IImage> CAzureKinectDevice::BuildRayMap(uint32 w, uint32 h)
    {
        CRefObj<IImage> spImage = CreateImage(w, h, 128);
        CImageIter128 row(spImage, 0, 0);
        int index = 0;
        for (uint32 iy = 0; iy < h; iy++)
        {
            CImageIter128 col = row;
            for (uint32 ix = 0; ix < w; ix++)
            {
                // Convert pixel coords+depth to 3D point
                k4a_float2_t pt;
                pt.xy.x = float(ix);
                pt.xy.y = float(iy);
                k4a_float3_t result;
                int valid;
                k4a_result_t err = k4a_calibration_2d_to_3d(&m_calibration, &pt, 1000.0f, k4a_calibration_type_t::K4A_CALIBRATION_TYPE_DEPTH,
                    k4a_calibration_type_t::K4A_CALIBRATION_TYPE_DEPTH, &result, &valid);
                if (err == K4A_RESULT_SUCCEEDED && valid)
                {
                    float len = sqrtf(result.xyz.x * result.xyz.x + result.xyz.y * result.xyz.y + result.xyz.z * result.xyz.z);
                    result.xyz.x /= len;
                    result.xyz.y /= len;
                    result.xyz.z /= len;
                    col.SetRed(result.xyz.x);
                    col.SetGreen(result.xyz.y);
                    col.SetBlue(result.xyz.z);
                }
                else
                {
                    col.SetRed(0.0f);
                    col.SetGreen(0.0f);
                    col.SetBlue(1.0f);
                }
                col.SetAlpha(1.0f);
                col.Step(CImageIter::Right);
            }
            row.Step(CImageIter::Down);
        }
        return spImage;
    }
    
    bool CAzureKinectDevice::NextFrame(IImage** ppColorImage, std::vector<Vector3>& pts, std::vector<Vector3>& normals, BBox3 &bbox)
    {
        CRefObj<IImage> spDepthImage;
        if (NextFrame(ppColorImage, &spDepthImage, nullptr))
        {
            pts.resize(spDepthImage->GetWidth() * spDepthImage->GetHeight());
            normals.resize(spDepthImage->GetWidth() * spDepthImage->GetHeight());
            int ptIndex = 0;
            int normIndex = 0;
            CImageIter16 srcRow(spDepthImage, 0, 0);
            int w = (int)spDepthImage->GetWidth();
            int h = (int)spDepthImage->GetHeight();
            for (int iy = 0; iy < h; iy++)
            {
                CImageIter16 srcCol = srcRow;
                for (int ix = 0; ix < w; ix++)
                {
                    uint16 depth = srcCol.GetGray();
                    if (depth == 0)
                    {
                        srcCol.Step(CImageIter::Right);
                        continue;
                    }
                    unsigned short neighborLeft = depth;
                    unsigned short neighborRight = depth;
                    unsigned short neighborUp = depth;
                    unsigned short neighborDown = depth;
                    if (ix > 0)
                    {
                        srcCol.Step(CImageIter::Left);
                        neighborLeft = srcCol.GetGray();
                        srcCol.Step(CImageIter::Right);
                    }
                    if (ix < (int)spDepthImage->GetWidth())
                    {
                        srcCol.Step(CImageIter::Right);
                        neighborRight = srcCol.GetGray();
                        srcCol.Step(CImageIter::Left);
                    }
                    if (iy > 0)
                    {
                        srcCol.Step(CImageIter::Up);
                        neighborUp = srcCol.GetGray();
                        srcCol.Step(CImageIter::Down);
                    }
                    if (iy < (int)spDepthImage->GetWidth())
                    {
                        srcCol.Step(CImageIter::Down);
                        neighborDown = srcCol.GetGray();
                        srcCol.Step(CImageIter::Up);
                    }
                    float dddx = (neighborRight - neighborLeft) / (2.0f * 8000.0f);
                    float dddy = (neighborDown - neighborUp) / (2.0f * 8000.0f);
                    srcCol.Step(CImageIter::Right);

                    // Convert pixel coords+depth to 3D point
                    k4a_float2_t pt;
                    pt.xy.x = float(ix);
                    pt.xy.y = float(iy);
                    k4a_float3_t result;
                    int valid;
                    k4a_result_t err = k4a_calibration_2d_to_3d(&m_calibration, &pt, (float)depth, k4a_calibration_type_t::K4A_CALIBRATION_TYPE_DEPTH,
                        k4a_calibration_type_t::K4A_CALIBRATION_TYPE_DEPTH, &result, &valid);
                    if (err == K4A_RESULT_SUCCEEDED && valid)
                    {
                        Vector3 pos(result.xyz.x, result.xyz.y, result.xyz.z);
                        pts[ptIndex++] = pos;
                        Vector3 norm(dddx, dddy, -1.0f);
                        norm.Normalize();
                        normals[normIndex] = norm;
                        bbox.AddPoint(pos);
                    }
                }
                srcRow.Step(CImageIter::Down);
            }
            pts.resize(ptIndex);
            normals.resize(normIndex);
            return true;
        }
        return false;
    }
}
