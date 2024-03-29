//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
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

module Rendering.Caustic.VideoTexture;
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.IRenderer;

namespace Caustic
{
    //**********************************************************************
    CVideoTexture::CVideoTexture(IRenderer *pRenderer)
    {
        m_spTexture = CCausticFactory::Instance()->CreateTexture(pRenderer, 1, 1, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        m_eos = false;
    }

    //**********************************************************************
    CVideoTexture::~CVideoTexture()
    {
    }

    //**********************************************************************
    // Method: GetWidth
    // Returns the width of the texture
    //**********************************************************************
    uint32_t CVideoTexture::GetWidth()
    {
        return m_Width;
    }

    //**********************************************************************
    // Method: GetHeight
    // Returns the height of the texture
    //**********************************************************************
    uint32_t CVideoTexture::GetHeight()
    {
        return m_Height;
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
    void CVideoTexture::GetVideoFormat(CVideoFormat *pFormat)
    {
        CComPtr<IMFMediaType> spType;
        CT(m_spSourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &spType));
        uint32_t width, height;
        CT(MFGetAttributeSize(spType, MF_MT_FRAME_SIZE, &width, &height));
        LONG lStride = (LONG)MFGetAttributeUINT32(spType, MF_MT_DEFAULT_STRIDE, 1);
        pFormat->m_topDown = (lStride > 0);
        MFRatio par = { 0 , 0 };
        HRESULT hr = MFGetAttributeRatio(spType, MF_MT_PIXEL_ASPECT_RATIO, (uint32_t*)&par.Numerator, (uint32_t*)&par.Denominator);
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

    bool CVideoTexture::EndOfStream()
    {
        return m_eos;
    }

    void CVideoTexture::Restart()
    {
        PROPVARIANT v;
        v.vt = VT_I8;
        v.hVal.QuadPart = 0L;
        m_spSourceReader->SetCurrentPosition(GUID_NULL, v);
        m_eos = false;
    }

    //**********************************************************************
    // Method: Update
    // Updates the underlying texture
    //**********************************************************************
    void CVideoTexture::Update(IRenderer* pRenderer)
    {
        DWORD streamIndex;
        DWORD flags;
        LONGLONG timestamp;
        CComPtr<IMFSample> spSample;
        CT(m_spSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &timestamp, &spSample));
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            m_eos = true;
            return;
        }
        if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
        {
            GetVideoFormat(&m_format);
            m_Width = m_format.m_width;
            m_Height = m_format.m_height;
            m_spTexture = CCausticFactory::Instance()->CreateTexture(pRenderer, m_Width, m_Height, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        }
        if (spSample)
        {
            CComPtr<ID3D11Texture2D> spD3DTexture = m_spTexture->GetD3DTexture();
            D3D11_MAPPED_SUBRESOURCE ms;
            CT(pRenderer->GetContext()->Map(spD3DTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));

            uint32_t pitch = 4 * m_format.m_width;
            CComPtr<IMFMediaBuffer> spBuffer;
            CT(spSample->ConvertToContiguousBuffer(&spBuffer));
            BYTE *pBitmapData;
            DWORD cbBitmapData;
            CT(spBuffer->Lock(&pBitmapData, NULL, &cbBitmapData));
            _ASSERT(cbBitmapData == (pitch * m_format.m_height));

            BYTE *pDst = reinterpret_cast<BYTE*>(ms.pData);
            BYTE *pSrc = reinterpret_cast<BYTE*>(pBitmapData);
            for (UINT i = 0; i < m_format.m_height; i++)
            {
                memcpy(pDst, pSrc, pitch);
                pSrc += pitch;
                pDst += ms.RowPitch;
            }
            pRenderer->GetContext()->Unmap(spD3DTexture, 0);
            CT(spBuffer->Unlock());
        }

    }

    //**********************************************************************
    void CVideoTexture::Render(IRenderer* pRenderer, int slot, bool isPixelShader)
    {
        m_spTexture->Render(pRenderer, slot, isPixelShader);
    }

    //**********************************************************************
    static void CreateVideoCaptureDevice(IMFMediaSource** ppSource)
    {
        *ppSource = nullptr;

        CComPtr<IMFAttributes> spConfig;
        CT(MFCreateAttributes(&spConfig, 1));
        CT(spConfig->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));

        IMFActivate** ppDevices = nullptr;
        uint32_t count = 0;
        CT(MFEnumDeviceSources(spConfig, &ppDevices, &count));
        CT((count <= 0) ? MF_E_NOT_FOUND : S_OK);
        CT(ppDevices[0]->ActivateObject(IID_PPV_ARGS(ppSource)));
        for (DWORD i = 0; i < count; i++)
        {
            ppDevices[i]->Release();
        }
        CoTaskMemFree(ppDevices);
    }

    //**********************************************************************
    void CVideoTexture::CreateFromWebcam(IRenderer* pRenderer)
    {
        CComPtr<IMFAttributes> spAttributes;
        CT(MFCreateAttributes(&spAttributes, 1));
        CT(spAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE));
        CComPtr<IMFMediaSource> spMediaSource;
        CreateVideoCaptureDevice(&spMediaSource);
        CComPtr<IMFSourceReader> spSourceReader;
        MFCreateSourceReaderFromMediaSource(spMediaSource, spAttributes, &spSourceReader);
        FromMediaSource(spSourceReader, pRenderer);
    }

    //**********************************************************************
    void CVideoTexture::LoadFromFile(const wchar_t* pFilename, IRenderer* pRenderer)
    {
        CComPtr<IMFAttributes> spAttributes;
        CT(MFCreateAttributes(&spAttributes, 1));
        CT(spAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE));
        CComPtr<IMFSourceReader> spSourceReader;
        CT(MFCreateSourceReaderFromURL(pFilename, spAttributes, &spSourceReader));
        FromMediaSource(spSourceReader, pRenderer);
    }

    //**********************************************************************
    void CVideoTexture::CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap /* = false */)
    {
        CT(E_NOTIMPL);
    }

    //**********************************************************************
    CRefObj<IImage> CVideoTexture::CopyToImage(IRenderer* pRenderer)
    {
        CT(E_NOTIMPL);
        return CRefObj<IImage>(nullptr);
    }

    //**********************************************************************
    void CVideoTexture::Copy(IRenderer* pRenderer, ITexture *pDst)
    {
        CT(E_NOTIMPL);
    }

    //**********************************************************************
    void CVideoTexture::CopyToImage(IRenderer* pRenderer, IImage* pImage)
    {
        CT(E_NOTIMPL);
    }

    //**********************************************************************
    void CVideoTexture::FromMediaSource(IMFSourceReader *pSourceReader, IRenderer* pRenderer)
    {
        m_spSourceReader = pSourceReader;
        CComPtr<IMFMediaType> spType;
        CT(MFCreateMediaType(&spType));
        CT(spType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
        CT(spType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32));
        CT(m_spSourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, spType));
        CT(m_spSourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE));
        CComPtr<IMFMediaType> spCurMediaType;
        CT(m_spSourceReader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM), &spCurMediaType));
        uint32_t width, height;
        CT(MFGetAttributeSize(spCurMediaType, MF_MT_FRAME_SIZE, &width, &height));

        uint32_t stride;
        HRESULT hr = spCurMediaType->GetUINT32(MF_MT_DEFAULT_STRIDE, &stride);
        if (FAILED(hr))
        {
            // Attribute not set. Try to calculate the default stride.
            LONG stride;
            hr = MFGetStrideForBitmapInfoHeader(MFVideoFormat_ARGB32.Data1, width, &stride);
            if (SUCCEEDED(hr))
            {
                bool inverted = (stride < 0) ? true : false;
                stride = static_cast<uint32_t>((inverted) ? -stride : stride);
            }
        }

        GetVideoFormat(&m_format);
        m_Width = m_format.m_width;
        m_Height = m_format.m_height;
        m_spTexture = CCausticFactory::Instance()->CreateTexture(pRenderer, m_Width, m_Height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
    }
}
