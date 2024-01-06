//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <xaudio2.h>
#include <cinttypes>

export module Audio.AudioPlayback.IAudioPlayback;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;

//**********************************************************************
// File: IAudioPlayback.ixx
// This file defines the published interface for audio playback.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: IAudioFrame
    // Defines interface used to playback of audio
    //
    // Module:
    // {Link:import Audio.AudioPlay.IAudioPlayback;{Audio/AudioPlayback/IAudioPlayback.ixx}}
    //**********************************************************************
    struct IAudioFrame : public IRefCount
    {
        virtual uint8_t* GetData() = 0;
        virtual uint32_t GetLength() = 0;
    };

    //**********************************************************************
    // Interface: IAudioPlayback
    // Defines interface used to playback of audio
    //
    // Module:
    // {Link:import Audio.AudioPlayback.IAudioPlayback;{Audio/AudioPlayback/IAudioPlayback.ixx}}
    //**********************************************************************
    struct IAudioPlayback : public IRefCount
    {
        //**********************************************************************
        // Method: Play
        // Plays an audio buffer.
        //
        // Parameters:
        // pFrame - audio buffer to play
        //**********************************************************************
        virtual void Play(IAudioFrame* pFrame) = 0;

        //**********************************************************************
        // Method: Play
        // Plays an audio buffer.
        //
        // Parameters:
        // pData - PCM audio data
        // dataLen - length of 'pData' buffer in bytes
        //**********************************************************************
        virtual void Play(uint8_t* pData, uint32_t dataLen) = 0;

        //**********************************************************************
        // Method: PlayTone
        // Plays a tone at the specified frequency for 1 second.
        //
        // Parameters:
        // frequency - frequency of tone to play
        //**********************************************************************
        virtual void PlayTone(int frequency) = 0;

        //**********************************************************************
        // Method: SetVolume
        // Sets the volume level
        //
        // Parameters:
        // volume - volume level to play sound at
        //**********************************************************************
        virtual void SetVolume(float volume) = 0;
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
    CRefObj<IAudioPlayback> CreateAudioPlayback(int samplingRate, int bitsPerSample, int numChannels)
    {
        extern CRefObj<IAudioPlayback> CreateAudioPlaybackInternal(int samplingRate, int bitsPerSample, int numChannels);
        return CreateAudioPlaybackInternal(samplingRate, bitsPerSample, numChannels);
    }
}
