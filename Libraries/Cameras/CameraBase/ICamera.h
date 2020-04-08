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
	// Interface: ICameraDevice
	// Base interface shared across all camera types
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
	};
}
