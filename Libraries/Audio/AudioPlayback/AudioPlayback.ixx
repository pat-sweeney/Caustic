//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <xaudio2.h>
#include <atlbase.h>
#include <memory>

export module Audio.AudioPlayback.AudioPlayback;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Audio.AudioPlayback.IAudioPlayback;

//**********************************************************************
// File: IAudioPlayback.ixx
// This file defines the published interface for audio playback.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CAudioPlayback
    // Defines implementation for IAudioPlayback
    //
    // Module:
    // {Link:import Audio.AudioPlayback.AudioPlayback;{Audio/AudioPlayback/AudioPlayback.ixx}}
    //**********************************************************************
    class CAudioPlayback : public CRefCount, public IAudioPlayback, public IXAudio2VoiceCallback
    {
        CComPtr<IXAudio2> m_spXAudio;
        IXAudio2MasteringVoice *m_wpMasteringVoice;
        IXAudio2SourceVoice *m_wpSourceVoice;
        int m_samplingRate;
        int m_bitsPerSample;
        int m_numChannels;
        HANDLE m_hBufferPlaying;

        void PlayBuffer(uint8* pData, uint32 len);
    public:
        CAudioPlayback(int samplingRate, int bitsPerSample, int numChannels);
        ~CAudioPlayback();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IAudioPlayback
        //**********************************************************************
        virtual void Play(IAudioFrame* pFrame) override;
        virtual void PlayTone(int frequency) override;
        virtual void SetVolume(float volume) override;

        //**********************************************************************
        // IXAudio2VoiceCallback
        //**********************************************************************
        virtual void OnBufferEnd(void* pBufferContext) override { SetEvent(m_hBufferPlaying); }
        virtual void OnBufferStart(void* pBufferContext) override { ResetEvent(m_hBufferPlaying); }
        virtual void OnLoopEnd(void* pBufferContext) override {}
        virtual void OnStreamEnd() override {}
        virtual void OnVoiceError(void* pBufferContext, HRESULT error) override {}
        virtual void OnVoiceProcessingPassEnd() override {}
        virtual void OnVoiceProcessingPassStart(UINT32 BytesRequired) override {}
    };

    //**********************************************************************
    // Function: CreateAudioPlayback
    // Creates a XAudio device for playing back audio
    //
    // Parameters:
    // samplingRate - sampling rate for audio
    // bitsPerSample - bits per audio sample
    // numChannels - number of audio channels
    //
    // Returns:
    // Returns the created XAudio playback device
    //
    // Module:
    // {Link:import Audio.AudioPlayback.IAudioPlayback;{Audio/AudioPlayback/IAudioPlayback.ixx}}
    //**********************************************************************
    CRefObj<IAudioPlayback> CreateAudioPlaybackInternal(int samplingRate, int bitsPerSample, int numChannels)
    {
        std::unique_ptr<CAudioPlayback> spAudioPlayback(new CAudioPlayback(samplingRate, bitsPerSample, numChannels));
        return CRefObj<IAudioPlayback>(spAudioPlayback.release());
    }
}
