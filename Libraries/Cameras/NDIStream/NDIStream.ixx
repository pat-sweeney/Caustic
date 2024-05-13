//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include "Processing.NDI.Advanced.h"
#include "Processing.NDI.Lib.h"

export module Cameras.NDIStream.NDIStream;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Cameras.CameraBase.ICamera;
import Cameras.NDIStream.INDIStream;

// Namespace: Caustic
export namespace Caustic
{
    const int c_numVideoFrames = 25;

    //**********************************************************************
    // Class: CNDIStream
    // Implements INDIStream
    //
    // Module:
    // {Link:import Cameras.NDIStream.NDIStream;{Cameras/NDIStream/NDIStream.ixx}}
    //**********************************************************************
    class CNDIStream : public CRefCount, public INDIStream
    {
        std::string m_streamName;
        NDIlib_send_instance_t m_pSendInstance;
        NDIlib_video_frame_v2_t m_pVideoFrames[c_numVideoFrames];
        int m_videoFrameIndex;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_frameRate;
        uint32_t m_bitsPerSample;
        uint32_t m_numChannels;
        uint32_t m_samplingRate;
    public:
        CNDIStream() :
            m_videoFrameIndex(0)
        {
        }

        ~CNDIStream()
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // INDIStream
        //**********************************************************************
        virtual void Initialize(std::string streamName, int w, int h, int frameRate, int samplingRate, int bitsPerSample, int numChannels) override;
        virtual void Shutdown() override;
        virtual void SendImage(IImage* pImage) override;
        virtual void SendAudioFrame(uint8_t* pBuffer, uint32_t bufferLen) override;
    };

    //**********************************************************************
    CRefObj<INDIStream> CreateNDIStreamInternal();
}
