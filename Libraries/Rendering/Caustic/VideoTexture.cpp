//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Error.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "VideoTexture.h"
#include <memory>
#include <wincodec.h>
#include <objbase.h>
#include <assert.h>
#include <Mferror.h>

namespace Caustic
{
    //**********************************************************************
    CVideoTexture::CVideoTexture(IGraphics *pGraphics)
    {
	    CCausticFactory::Instance()->CreateTexture(pGraphics, 1, 1, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &m_spTexture);
    }

    //**********************************************************************
    CVideoTexture::~CVideoTexture()
    {
    }

    //**********************************************************************
    // Method: GetWidth
    // Returns the width of the texture
    //**********************************************************************
    uint32 CVideoTexture::GetWidth()
    {
        return m_Width;
    }

    //**********************************************************************
    // Method: GetHeight
    // Returns the height of the texture
    //**********************************************************************
    uint32 CVideoTexture::GetHeight()
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
    // Method: Update
    // Updates the underlying texture
    //**********************************************************************
    void CVideoTexture::Update(IGraphics *pGraphics)
    {
        DWORD streamIndex;
        DWORD flags;
        LONGLONG timestamp;
        CComPtr<IMFSample> spSample;
        CT(m_spSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &timestamp, &spSample));
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            return;
        }
        if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
        {
            GetVideoFormat(&m_format);
            m_spTexture = nullptr;
		    CCausticFactory::Instance()->CreateTexture(pGraphics, m_Width, m_Height, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &m_spTexture);
        }
        if (spSample)
        {
            CComPtr<ID3D11Texture2D> spD3DTexture = m_spTexture->GetD3DTexture();
            D3D11_MAPPED_SUBRESOURCE ms;
            CT(pGraphics->GetContext()->Map(spD3DTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));

            UINT32 pitch = 4 * m_format.m_width;
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
            pGraphics->GetContext()->Unmap(spD3DTexture, 0);
            CT(spBuffer->Unlock());
        }

    }

    //**********************************************************************
    void CVideoTexture::Render(IGraphics* pGraphics, int slot, bool isPixelShader)
    {
        m_spTexture->Render(pGraphics, slot, isPixelShader);
    }

    //**********************************************************************
    void CVideoTexture::LoadFromFile(const wchar_t *pFilename, IGraphics *pGraphics)
    {
        CComPtr<IMFAttributes> spAttributes;
        CT(MFCreateAttributes(&spAttributes, 1));
        CT(spAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE));
        CT(MFCreateSourceReaderFromURL(pFilename, spAttributes, &m_spSourceReader));
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

        GetVideoFormat(&m_format);
        m_Width = m_format.m_width;
        m_Height = m_format.m_height;
        m_spTexture = nullptr;
	    CCausticFactory::Instance()->CreateTexture(pGraphics, m_Width, m_Height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &m_spTexture);
    }

    //**********************************************************************
    // Function: LoadVideoTexture
    // LoadVideoTexture loads a video and uses it as the texture source
    //
    // Parameters:
    // pFilename - Name of file to load
    // pGraphics - Renderer
    // ppTexture - Returns the new texture
    //**********************************************************************
    CAUSTICAPI void LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture)
    {
        std::unique_ptr<CVideoTexture> spTexture(new CVideoTexture(pGraphics));
        spTexture->LoadFromFile(pFilename, pGraphics);
        *ppTexture = spTexture.release();
        (*ppTexture)->AddRef();
    }
}
