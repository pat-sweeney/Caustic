//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Imaging\Image\Image.h"
#include "Base\Math\Matrix.h"

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Struct: CameraIntrinsics
	// Defines intrinsics for a camera
	//
	// Header:
	// [Link:Cameras/CameraBase/ICamera.h]
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
	// [Link:Cameras/CameraBase/ICamera.h]
	//**********************************************************************
	struct ICameraDevice : public IRefCount
	{
		//**********************************************************************
		// Method: NextFrame
		// Retrieves the next color/depth/ir image from the camera
		// Parameters:
		// ppColorImage - returns the next color image retrieved from the camera. Maybe nullptr.
		//**********************************************************************
		virtual bool NextFrame(IImage** ppColorImage) = 0;
		
		//**********************************************************************
		// Method: GetColorWidth
		// Returns the width in pixels of the color image returned by the camera
		//
		// Returns:
		// Width of the color image in pixels
		//**********************************************************************
		virtual uint32 GetColorWidth() = 0;

		//**********************************************************************
		// Method: GetColorHeight
		// Returns the height in pixels of the color image returned by the camera
		//
		// Returns:
		// Height of the color image in pixels
		//**********************************************************************
		virtual uint32 GetColorHeight() = 0;
	};

	//**********************************************************************
	// Interface: IDepthCameraDevice
	// Interface for depth cameras
	//**********************************************************************
	struct IDepthCameraDevice : public ICameraDevice
	{
		using ICameraDevice::NextFrame;

		//**********************************************************************
		// Method: NextFrame
		// Retrieves the next color/depth/ir image from the camera
		//
		// Parameters:
		// ppColorImage - returns the next color image retrieved from the camera. Maybe nullptr.
		// ppDepthImage - returns the next depth image retrieved from the camera. Maybe nullptr.
		// ppIRImage - returns the next infrared image retrieved from the camera. Maybe nullptr.
		//**********************************************************************
		virtual bool NextFrame(IImage** ppColorImage, IImage** ppDepthImage, IImage** ppIRImage) = 0;

		//**********************************************************************
		// Method: GetDepthWidth
		// Returns the width in pixels of the depth image returned by the camera
		//
		// Returns:
		// Width of the depth image in pixels
		//**********************************************************************
		virtual uint32 GetDepthWidth() = 0;

		//**********************************************************************
		// Method: GetDepthHeight
		// Returns the height in pixels of the depth image returned by the camera
		//
		// Returns:
		// Height of the depthimage in pixels
		//**********************************************************************
		virtual uint32 GetDepthHeight() = 0;

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
