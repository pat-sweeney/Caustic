//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
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
#include <cinttypes>

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
        uint64_t m_timestamp;
        std::unique_ptr<byte> m_spBuffer;
        uint32_t m_bufferSize;
        friend class CVideo;
    public:
        CAudioSample(uint32_t bufferSize, uint32_t timestamp);
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IAudioSample
        //**********************************************************************
        virtual uint64_t Timestamp() override
        {
            return m_timestamp;
        }

        virtual uint8_t* GetData() override
        {
            return m_spBuffer.get();
        }

        virtual uint32_t GetDataSize() override
        {
            return m_bufferSize;
        }
    };

    //**********************************************************************
    class CVideoSample : public IVideoSample, public CRefCount
    {
        uint64_t m_timestamp;
        CRefObj<IImage> m_spImage;
        static CRefObj<IImagePool> sm_spImagePool;
        friend class CVideo;
    public:
        CVideoSample(uint32_t width, uint32_t height, uint64_t timestamp);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IAudioSample
        //**********************************************************************
        virtual uint64_t Timestamp() override
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
    // <uint32_t at Caustic::uint32_t> m_Width - width of video in pixels
    // <uint32_t at Caustic::uint32_t> m_Height - height of video in pixels
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
        uint32_t m_Width;
        uint32_t m_Height;
        CVideoFormat m_videoFormat;
        CAudioFormat m_audioFormat;
        CRefObj<IImage> m_spImage;
        bool m_eos;

        void VideoFormatFromMediaSource(IMFSourceReader* pSourceReader);
        void AudioFormatFromMediaSource(IMFSourceReader* pSourceReader);
        void FromMediaSource(IMFSourceReader* pSourceReader);
        void UpdateAudio();
    public:
        CVideo();
        ~CVideo();

        void LoadFromFile(const wchar_t* pFilename);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IVideo
        //**********************************************************************
        virtual void GetAudioFormat(CAudioFormat* pFormat) override;
        virtual void GetVideoFormat(CVideoFormat* pFormat) override;
        virtual CRefObj<IAudioSample> NextAudioSample() override;
        virtual CRefObj<IVideoSample> NextVideoSample() override;
        virtual bool EndOfStream() override;
        virtual void Restart() override;
    };
};
