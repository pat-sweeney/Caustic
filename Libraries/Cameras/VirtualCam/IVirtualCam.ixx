//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <string>

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
        virtual void SendAudioFrame(uint8* pData, uint32 dataSize) = 0;
    };
}
