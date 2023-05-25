//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>
#include <atlbase.h>
#include <memory>

export module Imaging.Video.IVideo;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;

export namespace Caustic
{
    //**********************************************************************
    // Class: CVideoFormat
    // Defines the format for a video
    //
    // Members:
    // <uint32 at Caustic::uint32> m_width - width of video in pixels
    // <uint32 at Caustic::uint32> m_height - heightof video in pixels
    // bool m_topDown - is video oriented from top to bottom (pixel 0,0 in top left corner)
    // RECT m_rect - Video rect corrected for pixel aspect ratio
    //
    // Module:
    // {Link:import Imaging.Video.IVideo;{Imaging/Video/IVideo.ixx}}
    //**********************************************************************
    struct CVideoFormat
    {
        uint32 m_width;
        uint32 m_height;
        bool m_topDown;
        RECT m_rect;    // Corrected for pixel aspect ratio

        CVideoFormat() :
            m_width(0),
            m_height(0),
            m_topDown(false)
        {
            SetRectEmpty(&m_rect);
        }
    };

    //**********************************************************************
    // Class: CAudioFormat
    // Defines the audio format for a video
    //
    // Members:
    // <uint32 at Caustic::uint32> m_numChannels - number of channels
    // <uint32 at Caustic::uint32> m_bitsPerSample - bits per audio sample
    //
    // Module:
    // {Link:import Imaging.Video.IVideo;{Imaging/Video/IVideo.ixx}}
    //**********************************************************************
    struct CAudioFormat
    {
        uint32 m_numChannels;
        uint32 m_bitsPerSample;
        uint32 m_samplesPerSec;

        CAudioFormat() :
            m_numChannels(1),
            m_bitsPerSample(16),
            m_samplesPerSec(48000)
        {
        }
    };
    
    //**********************************************************************
    struct IAudioSample : public IRefCount
    {
        virtual uint64 Timestamp() = 0;
        virtual uint8* GetData() = 0;
        virtual uint32 GetDataSize() = 0;
    };

    //**********************************************************************
    struct IVideoSample : public IRefCount
    {
        virtual uint64 Timestamp() = 0;
        virtual CRefObj<IImage> GetImage() = 0;
    };

    //**********************************************************************
    // Interface: IVideo
    // Interface to a video
    //
    // Module:
    // {Link:import Imaging.Video.IVideo;{Imaging/Video/IVideo.ixx}}
    //**********************************************************************
    struct IVideo : public IRefCount
    {
        //**********************************************************************
        // Method: NextAudioSample
        // Returns the next audio sample
        //**********************************************************************
        virtual CRefObj<IAudioSample> NextAudioSample() = 0;

        //**********************************************************************
        // Method: NextVideoSample
        // Returns the next video sample
        //**********************************************************************
        virtual CRefObj<IVideoSample> NextVideoSample() = 0;

        virtual bool EndOfStream() = 0;
        virtual void Restart() = 0;
    };

    CRefObj<IVideo> CreateVideo(const wchar_t* pFilename);
};
