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
    CWebCamera::CWebCamera(std::wstring deviceName, int w /* = -1*/, int h /* = -1*/, int frameRate /* = 30*/)
    {
        CComPtr<IMFAttributes> spAttr;
        CT(MFCreateAttributes(&spAttr, 2));
        CT(spAttr->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));
        CT(spAttr->SetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, deviceName.c_str()));
        CComPtr<IMFMediaSource> spDevice;
        CT(MFCreateDeviceSource(spAttr, &spDevice));

        spAttr = nullptr;
        CT(MFCreateAttributes(&spAttr, 2));
        CT(spAttr->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, TRUE));
        CT(spAttr->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, FALSE));
        CT(MFCreateSourceReaderFromMediaSource(spDevice, spAttr, &m_spReader));

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
        
        CT(spMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32));
        CT(m_spReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, spMediaType));
        CT(m_spReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE));
        CComPtr<IMFMediaType> spCurMediaType;
        CT(m_spReader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM), &spCurMediaType));
        CT(MFGetAttributeSize(spCurMediaType, MF_MT_FRAME_SIZE, &m_width, &m_height));
        CT(MFGetAttributeSize(spCurMediaType, MF_MT_FRAME_RATE, &m_width, &m_height));

        hr = spCurMediaType->GetUINT32(MF_MT_DEFAULT_STRIDE, (UINT32*)&m_stride);
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
    }

    bool CWebCamera::NextFrame(IImage** ppColorImage)
    {
        if (ppColorImage)
            *ppColorImage = nullptr;
        DWORD stream, flags;
        LONGLONG timestamp;
        CComPtr<IMFSample> spSample;
        HRESULT hr;
        hr = m_spReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &stream, &flags, &timestamp, &spSample);
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
}
