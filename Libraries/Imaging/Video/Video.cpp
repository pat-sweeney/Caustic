//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <wincodec.h>
#include <objbase.h>
#include <assert.h>
#include <Mferror.h>
#include <d3d11.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>
#include <atlbase.h>

module Imaging.Video.Video;
import Base.Core.Core;
import Base.Core.Error;
import Imaging.Image.IImage;
import Imaging.Image.ImagePool;

namespace Caustic
{
    //**********************************************************************
    CVideo::CVideo() :
        m_Width(0),
        m_Height(0),
        m_eos(false)
    {
    }

    //**********************************************************************
    CVideo::~CVideo()
    {
    }

    //-----------------------------------------------------------------------------
    // CorrectAspectRatio
    //
    // Converts a rectangle from the source's pixel aspect ratio (PAR) to 1:1 PAR.
    // Returns the corrected rectangle.
    //
    // For example, a 720 x 486 rect with a PAR of 9:10, when converted to 1x1 PAR,  
    // is stretched to 720 x 540. 
    //-----------------------------------------------------------------------------
    static RECT CorrectAspectRatio(const RECT& src, const MFRatio& srcPAR)
    {
        // Start with a rectangle the same size as src, but offset to the origin (0,0).
        RECT rc = { 0, 0, src.right - src.left, src.bottom - src.top };

        if ((srcPAR.Numerator != 1) || (srcPAR.Denominator != 1))
        {
            // Correct for the source's PAR.

            if (srcPAR.Numerator > srcPAR.Denominator)
            {
                // The source has "wide" pixels, so stretch the width.
                rc.right = MulDiv(rc.right, srcPAR.Numerator, srcPAR.Denominator);
            }
            else if (srcPAR.Numerator < srcPAR.Denominator)
            {
                // The source has "tall" pixels, so stretch the height.
                rc.bottom = MulDiv(rc.bottom, srcPAR.Denominator, srcPAR.Numerator);
            }
            // else: PAR is 1:1, which is a no-op.
        }
        return rc;
    }

    //**********************************************************************
    // Method: GetVideoFormat
    // Retrieves the current image format from the video
    //**********************************************************************
    void CVideo::GetVideoFormat(CVideoFormat* pFormat)
    {
        CComPtr<IMFMediaType> spType;
        CT(m_spSourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &spType));
        UINT32 width, height;
        CT(MFGetAttributeSize(spType, MF_MT_FRAME_SIZE, &width, &height));
        LONG lStride = (LONG)MFGetAttributeUINT32(spType, MF_MT_DEFAULT_STRIDE, 1);
        pFormat->m_topDown = (lStride > 0);
        MFRatio par = { 0 , 0 };
        HRESULT hr = MFGetAttributeRatio(spType, MF_MT_PIXEL_ASPECT_RATIO, (UINT32*)&par.Numerator, (UINT32*)&par.Denominator);
        if (SUCCEEDED(hr) && (par.Denominator != par.Numerator))
        {
            RECT rcSrc = { 0, 0, (LONG)width, (LONG)height };
            pFormat->m_rect = CorrectAspectRatio(rcSrc, par);
        }
        else
        {
            // Either the PAR is not set (assume 1:1), or the PAR is set to 1:1.
            SetRect(&pFormat->m_rect, 0, 0, width, height);
        }
        pFormat->m_width = width;
        pFormat->m_height = height;
    }

    //**********************************************************************
    // Method: GetAudioFormat
    // Retrieves the current audio format from the video
    //**********************************************************************
    void CVideo::GetAudioFormat(CAudioFormat* pFormat)
    {
        CComPtr<IMFMediaType> spType;
        CT(m_spSourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &spType));
        pFormat->m_bitsPerSample = MFGetAttributeUINT32(spType, MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
        pFormat->m_samplesPerSec = MFGetAttributeUINT32(spType, MF_MT_AUDIO_SAMPLES_PER_SECOND, 48000);
        pFormat->m_numChannels = MFGetAttributeUINT32(spType, MF_MT_AUDIO_NUM_CHANNELS, 1);
    }

    bool CVideo::EndOfStream()
    {
        return m_eos;
    }

    void CVideo::Restart()
    {
        PROPVARIANT v;
        v.vt = VT_I8;
        v.hVal.QuadPart = 0L;
        m_spSourceReader->SetCurrentPosition(GUID_NULL, v);
        m_eos = false;
    }

    //**********************************************************************
    // Method: NextAudioSample
    // Returns the next audio sample from the video
    //**********************************************************************
    CRefObj<IAudioSample> CVideo::NextAudioSample()
    {
        DWORD streamIndex;
        DWORD flags;
        LONGLONG timestamp;
        CComPtr<IMFSample> spSample;
        CT(m_spSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &streamIndex, &flags, &timestamp, &spSample));
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            m_eos = true;
            return CRefObj<IAudioSample>(nullptr);
        }
        if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
        {
            GetAudioFormat(&m_audioFormat);
        }
        if (spSample)
        {
            CComPtr<IMFMediaBuffer> spBuffer;
            CT(spSample->ConvertToContiguousBuffer(&spBuffer));
            BYTE* pBitmapData;
            DWORD cbBitmapData;
            CT(spBuffer->Lock(&pBitmapData, NULL, &cbBitmapData));
            CRefObj<IAudioSample> spAudioSample = new CAudioSample((uint32)cbBitmapData, (uint32)timestamp);
            memcpy(spAudioSample->GetData(), pBitmapData, cbBitmapData);
            CT(spBuffer->Unlock());
            return spAudioSample;
        }
        return CRefObj<IAudioSample>(nullptr);
    }
    
    //**********************************************************************
    CVideoSample::CVideoSample(uint32 width, uint32 height, uint64 timestamp)
    {
        if (sm_spImagePool == nullptr)
        {
            sm_spImagePool = CreateImagePool(10, width, height, EImageType::BGRX_32bpp);
        }
        m_spImage = sm_spImagePool->Acquire();
        m_timestamp = timestamp;
    }

    CAudioSample::CAudioSample(uint32 bufferSize, uint32 timestamp)
    {
        m_spBuffer.reset(new byte[bufferSize]);
        m_timestamp = timestamp;
        m_bufferSize = bufferSize;
    }
    
    CRefObj<IImagePool> CVideoSample::sm_spImagePool;
    
    //**********************************************************************
    // Method: NextVideoSample
    // Returns the next video sample from the video
    //**********************************************************************
    CRefObj<IVideoSample> CVideo::NextVideoSample()
    {
        DWORD streamIndex;
        DWORD flags;
        LONGLONG timestamp;
        CComPtr<IMFSample> spSample;
        CT(m_spSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &timestamp, &spSample));
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            m_eos = true;
            return CRefObj<IVideoSample>(nullptr);
        }
        if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
        {
            GetVideoFormat(&m_videoFormat);
        }
        if (spSample)
        {
            CRefObj<IVideoSample> spVideoSample = new CVideoSample(m_videoFormat.m_width, m_videoFormat.m_height, timestamp);

            UINT32 pitch = 4 * m_videoFormat.m_width;
            CComPtr<IMFMediaBuffer> spBuffer;
            CT(spSample->ConvertToContiguousBuffer(&spBuffer));
            BYTE* pBitmapData;
            DWORD cbBitmapData;
            CT(spBuffer->Lock(&pBitmapData, NULL, &cbBitmapData));
            _ASSERT(cbBitmapData == (pitch * m_videoFormat.m_height));

            CRefObj<IImage> spImage = spVideoSample->GetImage();
            BYTE* pDst = reinterpret_cast<BYTE*>(spImage->GetData());
            BYTE* pSrc = reinterpret_cast<BYTE*>(pBitmapData);
            for (UINT i = 0; i < m_videoFormat.m_height; i++)
            {
                memcpy(pDst, pSrc, pitch);
                pSrc += pitch;
                pDst += spImage->GetStride();
            }
            CT(spBuffer->Unlock());
            return spVideoSample;
        }
        return CRefObj<IVideoSample>(nullptr);
    }

    //**********************************************************************
    static void CreateVideoCaptureDevice(IMFMediaSource** ppSource)
    {
        *ppSource = nullptr;

        CComPtr<IMFAttributes> spConfig;
        CT(MFCreateAttributes(&spConfig, 1));
        CT(spConfig->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));

        IMFActivate** ppDevices = nullptr;
        UINT32 count = 0;
        CT(MFEnumDeviceSources(spConfig, &ppDevices, &count));
        CT((count <= 0) ? MF_E_NOT_FOUND : S_OK);
        CT(ppDevices[0]->ActivateObject(IID_PPV_ARGS(ppSource)));
        for (DWORD i = 0; i < count; i++)
        {
            ppDevices[i]->Release();
        }
        CoTaskMemFree(ppDevices);
    }

    ////**********************************************************************
    //void CVideoTexture::CreateFromWebcam(IRenderer* pRenderer)
    //{
    //    CComPtr<IMFAttributes> spAttributes;
    //    CT(MFCreateAttributes(&spAttributes, 1));
    //    CT(spAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE));
    //    CComPtr<IMFMediaSource> spMediaSource;
    //    CreateVideoCaptureDevice(&spMediaSource);
    //    CComPtr<IMFSourceReader> spSourceReader;
    //    MFCreateSourceReaderFromMediaSource(spMediaSource, spAttributes, &spSourceReader);
    //    FromMediaSource(spSourceReader, pRenderer);
    //}

    //**********************************************************************
    void CVideo::LoadFromFile(const wchar_t* pFilename)
    {
        CComPtr<IMFAttributes> spAttributes;
        CT(MFCreateAttributes(&spAttributes, 1));
        CT(spAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE));
        CComPtr<IMFSourceReader> spSourceReader;
        CT(MFCreateSourceReaderFromURL(pFilename, spAttributes, &spSourceReader));
        FromMediaSource(spSourceReader);
    }

    void CVideo::VideoFormatFromMediaSource(IMFSourceReader* pSourceReader)
    {
        CComPtr<IMFMediaType> spType;
        CT(MFCreateMediaType(&spType));
        CT(spType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
        CT(spType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32));
        CT(m_spSourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, spType));
        CT(m_spSourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE));
        CComPtr<IMFMediaType> spCurMediaType;
        CT(m_spSourceReader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM), &spCurMediaType));
        UINT32 width, height;
        CT(MFGetAttributeSize(spCurMediaType, MF_MT_FRAME_SIZE, &width, &height));

        UINT32 stride;
        HRESULT hr = spCurMediaType->GetUINT32(MF_MT_DEFAULT_STRIDE, &stride);
        if (FAILED(hr))
        {
            // Attribute not set. Try to calculate the default stride.
            LONG stride;
            hr = MFGetStrideForBitmapInfoHeader(MFVideoFormat_ARGB32.Data1, width, &stride);
            if (SUCCEEDED(hr))
            {
                bool inverted = (stride < 0) ? true : false;
                stride = static_cast<UINT32>((inverted) ? -stride : stride);
            }
        }

        GetVideoFormat(&m_videoFormat);
    }

    void CVideo::AudioFormatFromMediaSource(IMFSourceReader* pSourceReader)
    {
        CComPtr<IMFMediaType> spType;
        CT(MFCreateMediaType(&spType));
        CT(spType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio));
        CT(spType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM));
        CT(m_spSourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, spType));
        CT(m_spSourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE));
        CComPtr<IMFMediaType> spCurMediaType;
        CT(m_spSourceReader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &spCurMediaType));
        m_audioFormat.m_bitsPerSample = MFGetAttributeUINT32(spCurMediaType, MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
        m_audioFormat.m_samplesPerSec = MFGetAttributeUINT32(spCurMediaType, MF_MT_AUDIO_SAMPLES_PER_SECOND, 48000);
        m_audioFormat.m_numChannels = MFGetAttributeUINT32(spCurMediaType, MF_MT_AUDIO_NUM_CHANNELS, 1);
    }

    //**********************************************************************
    void CVideo::FromMediaSource(IMFSourceReader* pSourceReader)
    {
        m_spSourceReader = pSourceReader;
        VideoFormatFromMediaSource(pSourceReader);
        AudioFormatFromMediaSource(pSourceReader);
    }
}
