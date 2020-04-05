//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Imaging\Image\Image.h"

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Interface: ICamera
	// Base interface shared across all camera types
	//**********************************************************************
	struct ICamera : public IRefCount
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
}
