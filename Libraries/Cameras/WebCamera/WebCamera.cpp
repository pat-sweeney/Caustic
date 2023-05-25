//**********************************************************************
// Copyright Patrick Sweeney 2015-2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <winerror.h>
#include <new>
#include <windows.h>
#include <windowsx.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <strsafe.h>
#include <assert.h>
#include <atlbase.h>
#include <vector>
#include <string>
#include <combaseapi.h>
#include <camerauicontrol.h>
#pragma comment(lib, "wmcodecdspuuid.lib")

module Cameras.WebCamera.WebCamera;
import Base.Core.Error;
import Base.Math.BBox;
import Base.Math.Quaternion;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Image.IImagePool;
import Imaging.Image.ImagePool;
import Cameras.WebCamera.IWebCamera;

namespace Caustic
{
    CComPtr<IMFMediaSource> CWebCamera::CreateVideoDevice(std::wstring videoDeviceName, int w, int h, int frameRate)
    {
        CComPtr<IMFAttributes> spAttr;
        CT(MFCreateAttributes(&spAttr, 2));
        CT(spAttr->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));
        CT(spAttr->SetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, videoDeviceName.c_str()));
        CComPtr<IMFMediaSource> spDevice;
        CT(MFCreateDeviceSource(spAttr, &spDevice));
        return spDevice;
    }

    CComPtr<IMFMediaSource> CWebCamera::CreateAudioDevice(std::wstring audioDeviceName, int samplingRate, int bitsPerSample, int numChannels)
    {
        CComPtr<IMFAttributes> spAttr;
        CT(MFCreateAttributes(&spAttr, 2));
        CT(spAttr->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID));
        CT(spAttr->SetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_ENDPOINT_ID, audioDeviceName.c_str()));
        CComPtr<IMFMediaSource> spDevice;
        CT(MFCreateDeviceSource(spAttr, &spDevice));
        return spDevice;
    }

    CComPtr<IMFMediaType> CWebCamera::FindVideoMediaType(int w, int h, int frameRate)
    {
        CComPtr<IMFMediaType> spMediaType;

        // Find the correct resolution
        int mediaIndex = 0;
        int maxWidth = 0;
        int maxHeight = 0;
        CComPtr<IMFMediaType> spMaxMediaType;
        HRESULT hr;
        while (true)
        {
            GUID majorType, subType;
            spMediaType = nullptr;
            hr = m_spReader->GetNativeMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, mediaIndex, &spMediaType);
            if (FAILED(hr))
                break;
            CT(spMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType));
            CT(spMediaType->GetGUID(MF_MT_SUBTYPE, &subType));
            if (majorType == MFMediaType_Video)
            {
                UINT curWidth, curHeight;
                CT(MFGetAttributeSize(spMediaType, MF_MT_FRAME_SIZE, &curWidth, &curHeight));
                UINT frameRateNum, frameRateDen;
                CT(MFGetAttributeSize(spMediaType, MF_MT_FRAME_RATE, &frameRateNum, &frameRateDen));
                int curframerate = (int)((float)frameRateNum / (float)frameRateDen);
                if (w == -1 && (int)curWidth > maxWidth && curframerate >= frameRate)
                {
                    maxWidth = curWidth;
                    maxHeight = curHeight;
                    spMaxMediaType = spMediaType;
                }
                else if (curWidth == w && curHeight == h && curframerate == frameRate)
                    break;
            }
            mediaIndex++;
        }
        if (w == -1)
        {
            spMediaType = spMaxMediaType;
        }
        else
            CT(hr);
        return spMediaType;
    }

    CComPtr<IMFMediaType> CWebCamera::FindAudioMediaType(int samplingRate, int bitsPerSample, int numChannels)
    {
        CComPtr<IMFMediaType> spMediaType;

        // Find the correct resolution
        int mediaIndex = 0;
        while (true)
        {
            GUID majorType, subType;
            spMediaType = nullptr;
            HRESULT hr = m_spReader->GetNativeMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, mediaIndex, &spMediaType);
            if (FAILED(hr))
                break;
            if (samplingRate == 0)
                break;
            CT(spMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType));
            CT(spMediaType->GetGUID(MF_MT_SUBTYPE, &subType));
            if (majorType == MFMediaType_Audio)
            {
                UINT curSamplingRate;
                CT(spMediaType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &curSamplingRate));
                UINT curNumChannels;
                CT(spMediaType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &curNumChannels));
                UINT curBitsPerSample;
                CT(spMediaType->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &curBitsPerSample));
                if (samplingRate == curSamplingRate && bitsPerSample == curBitsPerSample && numChannels == curNumChannels)
                    break;
            }
            mediaIndex++;
        }
        return spMediaType;
    }

    CWebCamera::CWebCamera(std::wstring videoDeviceName, int w, int h, int frameRate,
        std::wstring audioDeviceName, int samplingRate, int bitsPerSample, int numChannels)
    {
        CComPtr<IMFMediaSource> spVideoDevice = CreateVideoDevice(videoDeviceName, w, h, frameRate);
        CComPtr<IMFMediaSource> spAudioDevice = (audioDeviceName.empty()) ? nullptr : CreateAudioDevice(audioDeviceName, samplingRate, bitsPerSample, numChannels);
        CComPtr<IMFCollection> spCollection;
        CComPtr<IMFMediaSource> spMediaSource;
        CT(MFCreateCollection(&spCollection));
        CT(spCollection->AddElement(spVideoDevice));
        if (spAudioDevice != nullptr)
        {
            CT(spCollection->AddElement(spAudioDevice));
            m_hasAudio = true;
        }
        else
            m_hasAudio = false;
        CT(MFCreateAggregateSource(spCollection, &spMediaSource));
        
        CComPtr<IMFAttributes> spAttr;
        CT(MFCreateAttributes(&spAttr, 2));
        CT(spAttr->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, TRUE));
        CT(spAttr->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, FALSE));
        CT(MFCreateSourceReaderFromMediaSource(spMediaSource, spAttr, &m_spReader));

        CComPtr<IMFMediaType> spVideoMediaType = FindVideoMediaType(w, h, frameRate);
        CComPtr<IMFMediaType> spAudioMediaType = FindAudioMediaType(samplingRate, bitsPerSample, numChannels);

        CT(spVideoMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32));
        CT(m_spReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, spVideoMediaType));
        CT(m_spReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE));
        CComPtr<IMFMediaType> spCurMediaType;
        CT(m_spReader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM), &spCurMediaType));
        CT(MFGetAttributeSize(spCurMediaType, MF_MT_FRAME_SIZE, &m_width, &m_height));
        CT(MFGetAttributeSize(spCurMediaType, MF_MT_FRAME_RATE, &m_width, &m_height));

        HRESULT hr = spCurMediaType->GetUINT32(MF_MT_DEFAULT_STRIDE, (UINT32*)&m_stride);
        if (FAILED(hr))
        {
            // Attribute not set. Try to calculate the default stride.
            hr = MFGetStrideForBitmapInfoHeader(MFVideoFormat_ARGB32.Data1, m_width, &m_stride);
            if (SUCCEEDED(hr))
            {
                bool inverted = (m_stride < 0) ? true : false;
                m_stride = static_cast<UINT32>((inverted) ? -m_stride : m_stride);
            }
        }

        //CT(spVideoMediaType->SetGUID(MF_MT_SUBTYPE, MF));
        if (spAudioDevice != nullptr)
        {
            CT(m_spReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, spAudioMediaType));
            CT(m_spReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE));
        }
    }

    CWebCamera::~CWebCamera()
    {
        m_spReader = nullptr;
    }
    
    bool CWebCamera::NextVideoFrame(IImage** ppColorImage)
    {
        if (ppColorImage)
            *ppColorImage = nullptr;
        DWORD stream, flags;
        LONGLONG timestamp;
        CComPtr<IMFSample> spSample;
        HRESULT hr;
        hr = m_spReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &stream, &flags, &timestamp, &spSample);
        if (FAILED(hr))
            return false;
        if (flags & MF_SOURCE_READERF_STREAMTICK)
            return false;
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
            return false;
        CComPtr<IMFMediaType> spType;
        CT(m_spReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &spType));
        UINT width, height;
        CT(MFGetAttributeSize(spType, MF_MT_FRAME_SIZE, &width, &height));
        CComPtr<IMFMediaBuffer> spBuffer;
        CT(spSample->ConvertToContiguousBuffer(&spBuffer));
        BYTE* pbBuffer;
        DWORD maxLen, curLen;
        CT(spBuffer->Lock(&pbBuffer, &maxLen, &curLen));

        if (m_spColorImagePool == nullptr)
            m_spColorImagePool = Caustic::CreateImagePool(20, width, height, EImageType::BGRA_32bpp);

        uint32 stride = width * 4;
        CRefObj<IImage> spImage = m_spColorImagePool->Acquire();
        uint8* pRow = spImage->GetData();
        int imgWidth = spImage->GetWidth();
        int bpp = spImage->GetBPP() / 8;
        int bps = imgWidth * bpp;
        int imgStride = spImage->GetStride();
        for (int y = 0; y < (int)height; y++)
        {
            memcpy(pRow, &pbBuffer[y * stride], bps);
            pRow += imgStride;
        }
        CT(spBuffer->Unlock());
        *ppColorImage = spImage.Detach();
        return true;
    }

    bool CWebCamera::NextAudioFrame(IAudioFrame** ppAudioFrame)
    {
        if (!m_hasAudio || ppAudioFrame == nullptr)
            return false;
        *ppAudioFrame = nullptr;
        DWORD stream, flags;
        LONGLONG timestamp;
        CComPtr<IMFSample> spSample;
        HRESULT hr;
        hr = m_spReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &stream, &flags, &timestamp, &spSample);
        if (flags & MF_SOURCE_READERF_STREAMTICK)
            return false;
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
            return false;
        CComPtr<IMFMediaType> spMediaType;
        CT(m_spReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &spMediaType));
        GUID majorGUID;
        spMediaType->GetMajorType(&majorGUID);
        GUID subtypeGUID;
        spMediaType->GetGUID(MF_MT_SUBTYPE,&subtypeGUID);
        if (majorGUID == MFMediaType_Audio && subtypeGUID == MFAudioFormat_Float)
        {

        }
            //        UINT width, height;
//        CT(MFGetAttributeSize(spType, MF_MT_FRAME_SIZE, &width, &height));
//        CComPtr<IMFMediaBuffer> spBuffer;
//        CT(spSample->ConvertToContiguousBuffer(&spBuffer));
//        BYTE* pbBuffer;
//        DWORD maxLen, curLen;
//        CT(spBuffer->Lock(&pbBuffer, &maxLen, &curLen));
//
//        if (m_spColorImagePool == nullptr)
//            m_spColorImagePool = Caustic::CreateImagePool(20, width, height, EImageType::BGRA_32bpp);
//
//        *ppColorImage = spImage.Detach();
        return true;
    }
}
