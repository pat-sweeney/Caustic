//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <string>
#include <cinttypes>

export module Cameras.VirtualCamera.IVirtualCamera;
import Base.Core.Core;
import Base.Core.IRefCount;
import Imaging.Image.IImage;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Interface: IVirtualCamera
    // Defines the interface for a virtual camera
    //
    // Module:
    // {Link:import Cameras.VirtualCam.IVirtualCam;{Cameras/VirtualCam/IVirtualCam.ixx}}
    //**********************************************************************
    struct IVirtualCamera : public IRefCount
    {
        virtual void SendVideoFrame(IImage* pTexture) = 0;
        virtual void SendAudioFrame(uint8_t* pData, uint32_t dataSize) = 0;
    };
}
