//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <k4a/k4a.h>
#include <k4abt.h>
#include <vector>

export module Cameras.AzureKinect.AzureKinect;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Base.Math.BBox;
import Imaging.Image.IImage;
import Imaging.Image.IImagePool;
import Base.Math.Matrix;
import Cameras.AzureKinect.IAzureKinect;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Class: CAzureKinectDevice
    // Defines the implementation for the Azure Kinect.
    //
    // Members:
    // <CRefObj> < <IImagePool> > m_spColorImagePool - image pool to get color image from
    // <CRefObj> < <IImagePool> > m_spDepthImagePool - image pool to get depth image from
    // int m_deviceIndex - index of device to use
    // k4a_device_t m_device - Azure Kinect device
    // k4a_device_configuration_t m_config - Azure Kinect configuration
    // k4a_calibration_t m_calibration - Azure Kinect calibration
    // k4a_image_t m_colorImage - last color image read from camera
    // k4a_image_t m_depthImage - last depth image read from camera
    // k4a_capture_t m_capture - capture context
    // bool m_cameraStarted - Has the camera been started?
    //
    // Module:
    // {Link:import Cameras.AzureKinect.AzureKinect;{Cameras/AzureKinect/IAzureKinect.ixx}}
    //**********************************************************************
    class CAzureKinectDevice : public IAzureKinect, public CRefCount
    {
        CRefObj<IImagePool> m_spColorImagePool;
        CRefObj<IImagePool> m_spDepthImagePool;
        int m_deviceIndex;
        k4a_device_t m_device;
        k4a_device_configuration_t m_config;
        k4a_calibration_t m_calibration;
        k4abt_tracker_t m_tracker;
        k4a_image_t m_colorImage;
        k4a_image_t m_depthImage;
        k4a_capture_t m_capture;
        bool m_cameraStarted;
        bool m_captureBodies;
        k4abt_frame_t m_bodyFrame;

        CameraIntrinsics GetAzureIntrinsics(k4a_calibration_intrinsic_parameters_t& params);
        Matrix4x4 BuildExtrinsics(k4a_calibration_extrinsics_t ext);
        bool BodyTracking();
    public:
        CAzureKinectDevice();
        ~CAzureKinectDevice();

        void Startup(int deviceId, AzureKinect::ColorMode colorMode, AzureKinect::DepthMode depthMode, AzureKinect::FPSMode fpsMode, bool captureBodies = false);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ICameraDevice
        //**********************************************************************
        virtual bool NextFrame(IImage** ppColorImage) override;
        virtual uint32 GetColorWidth() override;
        virtual uint32 GetColorHeight() override;

        //**********************************************************************
        // IDepthCameraDevice
        //**********************************************************************
        virtual bool NextFrame(IImage** ppColorImage, IImage** ppDepthImage, IImage** ppIRImage) override;
        virtual Matrix4x4 ColorExtrinsics() override;
        virtual Matrix3x3 ColorIntrinsics() override;
        virtual Matrix4x4 DepthExtrinsics() override;
        virtual Matrix3x3 DepthIntrinsics() override;
        virtual uint32 GetDepthWidth() override;
        virtual uint32 GetDepthHeight() override;

        //**********************************************************************
        // IAzureKinect
        //**********************************************************************
        virtual bool NextFrame(IImage** ppColorImage, std::vector<Vector3>& pts, std::vector<Vector3>& normals, BBox3 &bbox) override;
        virtual CRefObj<IImage> BuildRayMap(uint32 w, uint32 h, bool forDepth = true) override;
        virtual CameraIntrinsics GetAzureColorIntrinsics() override;
        virtual CameraIntrinsics GetAzureDepthIntrinsics() override;
        virtual bool BodyTrackingOn() override { return m_captureBodies; }
        virtual int NumberBodiesDetected() override;
        virtual AzureKinect::Joint GetParentJoint(AzureKinect::Joint joint) override;
        virtual Matrix4x4 GetJointMatrix(int bodyIndex, AzureKinect::Joint joint) override;
    };
}
