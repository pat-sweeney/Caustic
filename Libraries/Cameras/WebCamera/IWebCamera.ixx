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
import Base.Math.Point;
import Imaging.Image.IImage;
import Cameras.CameraBase.ICamera;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Structure: CameraInfo
    // Contains information about a camera available on the system.
    // This structure is returned by <IWebCamera::GetAvailableVideoDevices>.
    //
    // Members:
    // name - friendly name for the device
    // symlink - Symbolic link name for the device. This is passed to
    // <CreateWebCamera>.
    // info - defines the media type returned by the camera
    // resolutions - list of supported resolutions
    //
    // Module:
    // {Link:import Cameras.WebCamera.IWebCamera;{Cameras/WebCamera/IWebCamera.ixx}}
    //**********************************************************************
    struct CameraInfo
    {
        std::wstring name;
        std::wstring symlink;
        MFT_REGISTER_TYPE_INFO info;
        std::vector<Point2> resolutions;
        std::vector<Point2> framerates;
    };

    //**********************************************************************
    // Structure: AudioInfo
    // Contains information about audio devices available on the system.
    // This structure is returned by <IWebCamera::GetAvailableAudioDevices>.
    //
    // Members:
    // name - friendly name for the device
    // symlink - Symbolic link name for the device. This is passed to
    // <CreateWebCamera>.
    // samplingRates - list of supported sampling rates
    // bitsPerSample - number of bits per sample
    // channels - number of audio channels
    //
    // Module:
    // {Link:import Cameras.WebCamera.IWebCamera;{Cameras/WebCamera/IWebCamera.ixx}}
    //**********************************************************************
    struct AudioInfo
    {
        std::wstring name;
        std::wstring symlink;
        std::vector<int> samplingRates;
        std::vector<int> bitsPerSample;
        std::vector<int> channels;
    };

    //**********************************************************************
    // Interface: IWebCamera
    // Defines the interface to a WebCamera.
    //
    // Module:
    // {Link:import Cameras.WebCamera.IWebCamera;{Cameras/WebCamera/IWebCamera.ixx}}
    //**********************************************************************
    struct IWebCamera : public ICameraDevice
    {
        //**********************************************************************
        // Method: GetAvailableVideoDevices
        // Returns a list of cameras available on the system
        //
        // Module:
        // {Link:import Cameras.WebCamera.IWebCamera;{Cameras/WebCamera/IWebCamera.ixx}}
        //**********************************************************************
        static std::vector<CameraInfo> GetAvailableVideoDevices();

        //**********************************************************************
        // Method: GetAvailableAudioDevices
        // Returns a list of audio devices available on the system
        //
        // Module:
        // {Link:import Cameras.WebCamera.IWebCamera;{Cameras/WebCamera/IWebCamera.ixx}}
        //**********************************************************************
        static std::vector<AudioInfo> GetAvailableAudioDevices();
    };

    //**********************************************************************
    // Function: CreateWebCamera
    // Creates a new instance of WebCamera.
    //
    // Parameters:
    // deviceName - name of device to create
    // w - required resolution in X
    // h - required resolution in Y
    // frameRate - requested frame rate
    // 
    // Returns:
    // Returns the newly created camera
    //
    // Module:
    // {Link:import Cameras.WebCamera.IWebCamera;{Cameras/WebCamera/IWebCamera.ixx}}
    //**********************************************************************
    CRefObj<IWebCamera> CreateWebCamera(std::wstring deviceName, int w = -1, int h = -1, int frameRate = 30);
}
