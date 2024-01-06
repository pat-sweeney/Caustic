//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <xaudio2.h>
#include <math.h>
#include <cinttypes>

module Audio.AudioPlayback.AudioPlayback;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;

namespace Caustic
{
    CAudioPlayback::CAudioPlayback(int samplingRate, int bitsPerSample, int numChannels)
    {
        m_samplingRate = samplingRate;
        m_bitsPerSample = bitsPerSample;
        m_numChannels = numChannels;

        m_hBufferPlaying = CreateEvent(nullptr, true, false, nullptr);

        CT(XAudio2Create(&m_spXAudio));
        CT(m_spXAudio->CreateMasteringVoice(&m_wpMasteringVoice));
        CT(m_spXAudio->StartEngine());

        WAVEFORMATEX waveFormat;
        ZeroMemory(&waveFormat, sizeof(waveFormat));
        waveFormat.wFormatTag = WAVE_FORMAT_PCM;
        waveFormat.nChannels = numChannels;
        waveFormat.nSamplesPerSec = samplingRate;
        waveFormat.nAvgBytesPerSec = samplingRate * numChannels * bitsPerSample / 8;
        waveFormat.nBlockAlign = (numChannels * bitsPerSample) / 8;
        waveFormat.wBitsPerSample = bitsPerSample;
        waveFormat.cbSize = 0;
        CT(m_spXAudio->CreateSourceVoice(&m_wpSourceVoice, &waveFormat, 0, 2.0f, this));

        CT(m_wpSourceVoice->Start());
    }
    
    CAudioPlayback::~CAudioPlayback()
    {
        WaitForSingleObject(m_hBufferPlaying, INFINITE);
        if (m_wpSourceVoice != nullptr)
            m_wpSourceVoice->Stop();
        if (m_spXAudio != nullptr)
            m_spXAudio->StopEngine();
    }

    void CAudioPlayback::PlayBuffer(uint8_t*pData, uint32_t len)
    {
        XAUDIO2_BUFFER buf;
        buf.Flags = 0;
        buf.AudioBytes = len;
        buf.pAudioData = pData;
        buf.PlayBegin = 0;
        buf.PlayLength = 0;
        buf.LoopBegin = 0;
        buf.LoopLength = 0;
        buf.LoopCount = 0;
        buf.pContext = NULL;
        CT(m_wpSourceVoice->SubmitSourceBuffer(&buf, NULL));
    }

    void CAudioPlayback::Play(IAudioFrame* pFrame)
    {
        PlayBuffer(pFrame->GetData(), pFrame->GetLength());
    }

    void CAudioPlayback::Play(uint8_t* pData, uint32_t dataLen)
    {
        PlayBuffer(pData, dataLen);
    }

    void CAudioPlayback::SetVolume(float volume)
    {
        if (m_wpSourceVoice != nullptr)
            m_wpSourceVoice->SetVolume(volume);
    }

    void CAudioPlayback::PlayTone(int frequency)
    {
        uint32_t bufLen = m_samplingRate * m_numChannels * m_bitsPerSample / 8;
        byte* pData = new byte[bufLen];
        uint16_t* pw = reinterpret_cast<uint16_t*>(pData);
        for (UINT i = 0; i < bufLen / 4; i++)
        {
            float a = 2 * 3.1415f * float(i) / (float(m_samplingRate) / 440.0f);
            float v = (float)sin(a);
            *pw++ = static_cast<uint16_t>(v * 32767);
            *pw++ = static_cast<uint16_t>(v * 32767);
        }
        PlayBuffer(pData, bufLen);
    }
};
