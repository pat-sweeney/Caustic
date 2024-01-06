//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <Processing.NDI.Advanced.h>
#include <Processing.NDI.Lib.h>

export module Cameras.NDIStream.INDIStream;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Cameras.CameraBase.ICamera;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Interface: INDIStream
    // Defines the interface to a NDIStream used for writing video to NDI
    //
    // Module:
    // {Link:import Cameras.NDIStream.INDIStream;{Cameras/NDIStream/INDIStream.ixx}}
    //**********************************************************************
    struct INDIStream : public IRefCount
    {
        //**********************************************************************
        // Method: Initialize
        // Initiliazes the NDI system
        //
        // Module:
        // {Link:import Cameras.NDIStream.INDIStream;{Cameras/NDIStream/INDIStream.ixx}}
        //**********************************************************************
        virtual void Initialize(std::string streamName, int w, int h, int frameRate, int samplingRate, int bitsPerSample, int numChannels) = 0;

        //**********************************************************************
        // Method: Shutdown
        // Shuts down the NDI system
        //
        // Module:
        // {Link:import Cameras.NDIStream.INDIStream;{Cameras/NDIStream/INDIStream.ixx}}
        //**********************************************************************
        virtual void Shutdown() = 0;

        //**********************************************************************
        // Method: SendImage
        // Writes an image to the video stream
        //
        // Module:
        // {Link:import Cameras.NDIStream.INDIStream;{Cameras/NDIStream/INDIStream.ixx}}
        //**********************************************************************
        virtual void SendImage(IImage* pImage) = 0;

        //**********************************************************************
        // Method: SendAudioFrame
        // Writes a block of audio data to the audio stream
        //
        // Module:
        // {Link:import Cameras.NDIStream.INDIStream;{Cameras/NDIStream/INDIStream.ixx}}
        //**********************************************************************
        virtual void SendAudioFrame(uint8_t* pBuffer, uint32_t bufferLen) = 0;
    };

    //**********************************************************************
    // Function: CreateNDIStream
    // Creates a new instance of NDIStream
    //
    // Parameters:
    // outputStreamName - name of NDI stream to create
    // w - width in pixels of camera resolution
    // h - height in pixels of camera resolution
    // frameRate - requested frame rate
    // audioDeviceName - endpoint name for the audio device as returned by
    // <IWebCamera::GetAvailableVideoDevices>
    // samplingRate - sampling rate for audio
    // bitsPerSample - bits per sample for audio
    // numChannels - number of audio channels
    // 
    // Returns:
    // Returns the newly created NDI stream
    //
    // Module:
    // {Link:import Cameras.NDIStream.INDIStream;{Cameras/NDIStream/INDIStream.ixx}}
    //**********************************************************************
    CRefObj<INDIStream> CreateNDIStream();
}
