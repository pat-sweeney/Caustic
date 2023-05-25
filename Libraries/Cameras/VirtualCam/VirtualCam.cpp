//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <string>
#include <atlbase.h>

module Cameras.VirtualCamera.VirtualCamera;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.RefCount;
import Base.Core.Error;
import Rendering.Caustic.ITexture;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Cameras.VirtualCamera.IVirtualCamera;

// Namespace: Caustic
namespace Caustic
{
#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080
    CVirtualCamera::CVirtualCamera()
    {
        m_hVideoDataProduced = CreateEvent(NULL, FALSE, FALSE, L"VideoDataProduced");
        m_hVideoDataConsumed = CreateEvent(NULL, FALSE, TRUE, L"VideoDataConsumed");
        ULARGE_INTEGER size;
        size.QuadPart = IMAGE_WIDTH * IMAGE_HEIGHT * 3 + sizeof(DWORD);
        m_hVideoMapping = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, size.HighPart, size.LowPart, L"FrameData");
        m_pVideoMemory = (BYTE*)MapViewOfFile(m_hVideoMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

        const int AUDIO_BITRATE = 48000; // samples per second
        const int AUDIO_SAMPLELEN = 1; // length of sample in seconds
        const int AUDIO_NUMCHANNELS = 1; // number of audio channels
        const int AUDIO_BITSPERSAMPLE = 16; // number of bits per sample
        const int AUDIO_BUFFERSIZE = (AUDIO_BITRATE * AUDIO_SAMPLELEN * AUDIO_NUMCHANNELS * AUDIO_BITSPERSAMPLE) / 8;

        m_hAudioDataProduced = CreateEvent(NULL, FALSE, FALSE, L"AudioDataProduced");
        m_hAudioDataConsumed = CreateEvent(NULL, FALSE, TRUE, L"AudioDataConsumed");
        size.QuadPart = AUDIO_BUFFERSIZE + sizeof(DWORD);
        m_hAudioMapping = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, size.HighPart, size.LowPart, L"AudioData");
        m_pAudioMemory = (BYTE*)MapViewOfFile(m_hAudioMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    }

    void CVirtualCamera::SendAudioFrame(uint8* pData, uint32 dataSize)
    {
        // create or open events and shared memory:
        if (WaitForSingleObject(m_hAudioDataConsumed, 1) == WAIT_OBJECT_0)
        {
            wchar_t buf[1024];
            static int sampleCount = 0;
            swprintf_s(buf, L"Audio-%d: Sending audio sample  size=%d\n", sampleCount++, dataSize);
            OutputDebugString(buf);
            DWORD* pDataLen = (DWORD*)m_pAudioMemory;
            *pDataLen = dataSize;
            byte* pData = (byte*)m_pAudioMemory;
            memcpy(m_pAudioMemory + sizeof(DWORD), pData, dataSize);
            SetEvent(m_hAudioDataProduced);
        }
    }

    void CVirtualCamera::SendVideoFrame(IImage *pImage)
    {
        // create or open events and shared memory:
        if (WaitForSingleObject(m_hVideoDataConsumed, 1) == WAIT_OBJECT_0)
        {
            wchar_t buf[1024];
            static int sampleCount = 0;
            swprintf_s(buf, L"Video-%d: Sending video sample  size=%d\n", sampleCount++, IMAGE_WIDTH * IMAGE_HEIGHT * 3);
            OutputDebugString(buf);
            DWORD* pDataLen = (DWORD*)m_pVideoMemory;
            *pDataLen = IMAGE_WIDTH * IMAGE_HEIGHT * 3;
            auto pDst = m_pVideoMemory + sizeof(DWORD);
            CImageIter32 rowSrc(pImage, 0, pImage->GetHeight() - 1);
            for (uint32 y = 0; y < IMAGE_HEIGHT; y++)
            {
                CImageIter32 colSrc = rowSrc;
                for (uint32 x = 0; x < IMAGE_WIDTH; x++)
                {
                    if (x < pImage->GetWidth() && y < pImage->GetHeight())
                    {
                        *pDst++ = colSrc.GetBlue();
                        *pDst++ = colSrc.GetGreen();
                        *pDst++ = colSrc.GetRed();
                        colSrc.Step(CImageIter::Right);
                    }
                    else
                    {
                        *pDst++ = 0;
                        *pDst++ = 0;
                        *pDst++ = 0;
                    }
                }
                if (y < pImage->GetHeight())
                    rowSrc.Step(CImageIter::Up);
            }
            SetEvent(m_hVideoDataProduced);
        }
    }
}
