//**********************************************************************
// Copyright Patrick Sweeney 2015-2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <mfobjects.h>

export module Cameras.WebCamera.IWebCamera;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Cameras.CameraBase.ICamera;

// Namespace: Caustic
export namespace Caustic
{
    struct CameraInfo
    {
        std::wstring name;
        std::wstring symlink;
        MFT_REGISTER_TYPE_INFO info;
    };

    struct IWebCamera : public ICameraDevice
    {
        static std::vector<CameraInfo> GetAvailableDevices();
    };

    //**********************************************************************
    // Function: CreateWebCamera
    // Creates a new instance of WebCamera.
    //
    // Parameters:
    // deviceName - name of device to create
    // 
    // Returns:
    // Returns the newly created camera
    //
    // Module:
    // {Link:import Cameras.WebCamera.IWebCamera;{Cameras/WebCamera/IWebCamera.ixx}}
    //**********************************************************************
    CRefObj<IWebCamera> CreateWebCamera(std::wstring deviceName);
}
