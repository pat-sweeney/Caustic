//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <Processing.NDI.Advanced.h>
#include <Processing.NDI.Lib.h>
#include <Windows.h>
#include <memory>

module Cameras.NDIStream.NDIStream;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Cameras.CameraBase.ICamera;

// Namespace: Caustic
namespace Caustic
{
    void CNDIStream::Initialize(std::string streamName, int w, int h, int frameRate, int samplingRate, int bitsPerSample, int numChannels)
    {
        // Not required, but "correct" (see the SDK documentation).
        NDIlib_initialize();

        m_width = w;
        m_height = h;
        m_frameRate = frameRate;
        m_bitsPerSample = bitsPerSample;
        m_numChannels = numChannels;
        m_samplingRate = samplingRate;

        // We create the NDI sender
        NDIlib_send_create_t t;
        m_streamName = streamName;
        t.p_ndi_name = m_streamName.c_str();
        t.p_groups = nullptr;
        t.clock_video = true;
        t.clock_audio = true;
        m_pSendInstance = NDIlib_send_create(&t);
        CT((m_pSendInstance == nullptr) ? E_FAIL : S_OK);

        for (int i = 0; i < c_numVideoFrames; i++)
        {
            m_pVideoFrames[i].xres = w;
            m_pVideoFrames[i].yres = h;
            m_pVideoFrames[i].FourCC = NDIlib_FourCC_type_BGRX;
            m_pVideoFrames[i].p_data = (uint8_t*)malloc(m_pVideoFrames[i].xres * m_pVideoFrames[i].yres * 4);
        }
        m_videoFrameIndex = 0;
    }

    void CNDIStream::Shutdown()
    {
        for (int i = 0; i < c_numVideoFrames; i++)
        {
            if (m_pVideoFrames[i].p_data != nullptr)
                free(m_pVideoFrames[i].p_data);
        }
        if (m_pSendInstance != nullptr)
            NDIlib_send_destroy(m_pSendInstance);
        NDIlib_destroy();
    }

    void CNDIStream::SendImage(IImage* pImage)
    {
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        uint8_t* pSrc = pImage->GetData();
        uint8_t* pDst = m_pVideoFrames[m_videoFrameIndex].p_data;
        uint32_t stride = pImage->GetStride();
        int th = (h > m_pVideoFrames[m_videoFrameIndex].yres) ? m_pVideoFrames[m_videoFrameIndex].yres : h;
        for (int y = 0; y < th; y++)
        {
            int bytesToCopy = m_pVideoFrames[m_videoFrameIndex].xres * 4;
            if (bytesToCopy > (int)stride)
                bytesToCopy = (int)stride;
            memcpy(pDst, pSrc, bytesToCopy);
            pSrc += stride;
            pDst += m_pVideoFrames[m_videoFrameIndex].xres * 4;
        }
        NDIlib_send_send_video_v2(m_pSendInstance, &m_pVideoFrames[m_videoFrameIndex]);
        m_videoFrameIndex++;
        if (m_videoFrameIndex == c_numVideoFrames)
            m_videoFrameIndex = 0;
        return;
    }

    void CNDIStream::SendAudioFrame(uint8_t* pBuffer, uint32_t bufferLen)
    {
        NDIlib_audio_frame_interleaved_16s_t t;
        t.no_channels = m_numChannels;
        t.no_samples = bufferLen / (m_numChannels * m_bitsPerSample / 8);
        t.p_data = (int16_t*)pBuffer;
        t.sample_rate = m_samplingRate;
        t.timecode = 0;
        NDIlib_util_send_send_audio_interleaved_16s(m_pSendInstance, &t);
        return;
    }

    //**********************************************************************
    CRefObj<INDIStream> CreateNDIStreamInternal()
    {
        std::unique_ptr<CNDIStream> pStream(new CNDIStream());
        return CRefObj<INDIStream>(pStream.release());
    }
}
