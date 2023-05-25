//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>
#include <atlbase.h>
#include <memory>

export module Imaging.Video.Video;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Video.IVideo;
import Imaging.Image.IImage;
import Imaging.Image.IImagePool;

export namespace Caustic
{
    //**********************************************************************
    class CAudioSample : public IAudioSample, public CRefCount
    {
        uint64 m_timestamp;
        std::unique_ptr<byte> m_spBuffer;
        uint32 m_bufferSize;
        friend class CVideo;
    public:
        CAudioSample(uint32 bufferSize, uint32 timestamp);
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IAudioSample
        //**********************************************************************
        virtual uint64 Timestamp() override
        {
            return m_timestamp;
        }

        virtual uint8* GetData() override
        {
            return m_spBuffer.get();
        }

        virtual uint32 GetDataSize() override
        {
            return m_bufferSize;
        }
    };

    //**********************************************************************
    class CVideoSample : public IVideoSample, public CRefCount
    {
        uint64 m_timestamp;
        CRefObj<IImage> m_spImage;
        static CRefObj<IImagePool> sm_spImagePool;
        friend class CVideo;
    public:
        CVideoSample(uint32 width, uint32 height, uint64 timestamp);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IAudioSample
        //**********************************************************************
        virtual uint64 Timestamp() override
        {
            return m_timestamp;
        }

        virtual CRefObj<IImage> GetImage() override
        {
            return m_spImage;
        }
    };

    //**********************************************************************
    // Class: CVideo
    // Defines class for a video
    //
    // Members:
    // CComPtr<IMFSourceReader> m_spSourceReader - Media Foundation's source reader
    // <uint32 at Caustic::uint32> m_Width - width of video in pixels
    // <uint32 at Caustic::uint32> m_Height - height of video in pixels
    // <CVideoFormat> m_format - format of the video
    // <CRefObj> < <ITexture> > m_spTexture - texture for decoded video frame
    //
    // Module:
    // {Link:import Imaging.Video.Video;{Imaging/Video/Video.ixx}}
    //**********************************************************************
    class CVideo :
        public IVideo,
        public CRefCount
    {
    protected:
        CComPtr<IMFSourceReader> m_spSourceReader;
        uint32 m_Width;
        uint32 m_Height;
        CVideoFormat m_videoFormat;
        CAudioFormat m_audioFormat;
        CRefObj<IImage> m_spImage;
        bool m_eos;

        void VideoFormatFromMediaSource(IMFSourceReader* pSourceReader);
        void AudioFormatFromMediaSource(IMFSourceReader* pSourceReader);
        void FromMediaSource(IMFSourceReader* pSourceReader);
        void UpdateAudio();
        void GetVideoFormat(CVideoFormat* pFormat);
        void GetAudioFormat(CAudioFormat* pFormat);
    public:
        CVideo();
        ~CVideo();

        void LoadFromFile(const wchar_t* pFilename);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IVideo
        //**********************************************************************
        virtual CRefObj<IAudioSample> NextAudioSample() override;
        virtual CRefObj<IVideoSample> NextVideoSample() override;
        virtual bool EndOfStream() override;
        virtual void Restart() override;
    };
};
