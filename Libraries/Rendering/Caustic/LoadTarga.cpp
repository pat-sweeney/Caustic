//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
// For details on the TGA format see:
//     https://en.wikipedia.org/wiki/Truevision_TGA
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Error.h"
#include "Texture.h"

namespace Caustic {
    struct TargaHeader
    {
        uint8 IDLength;
        uint8 ColorMapType;
        uint8 ImageType;
        uint16 ColorMapFirstIndex;
        uint16 ColorMapLength;
        uint8 ColorMapBitsPerPixel;
        uint16 XOrigin;
        uint16 YOrigin;
        uint16 Width;
        uint16 Height;
        uint8 BitsPerPixel;
        uint8 ImageDescriptor;
    };

    void LoadTarga(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture)
    {
        HANDLE h = ::CreateFile(pFilename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        if (h == INVALID_HANDLE_VALUE)
        {
            return; // Ignore missing files
        }
        DWORD dwBytesRead;
        TargaHeader header;
        CTWIN32(::ReadFile(h, &header.IDLength, sizeof(header.IDLength), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.ColorMapType, sizeof(header.ColorMapType), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.ImageType, sizeof(header.ImageType), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.ColorMapFirstIndex, sizeof(header.ColorMapFirstIndex), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.ColorMapLength, sizeof(header.ColorMapLength), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.ColorMapBitsPerPixel, sizeof(header.ColorMapBitsPerPixel), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.XOrigin, sizeof(header.XOrigin), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.YOrigin, sizeof(header.YOrigin), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.Width, sizeof(header.Width), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.Height, sizeof(header.Height), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.BitsPerPixel, sizeof(header.BitsPerPixel), &dwBytesRead, nullptr));
        CTWIN32(::ReadFile(h, &header.ImageDescriptor, sizeof(header.ImageDescriptor), &dwBytesRead, nullptr));
        if (!(header.IDLength == 0 &&
            (header.BitsPerPixel == 24 || header.BitsPerPixel == 32) &&
            header.ImageType == 2))
        {
            CT(E_FAIL);
        }
        uint32 pitch = (uint32)header.Width * (uint32)header.BitsPerPixel;
        uint32 imageSize = (uint32)header.Height * pitch;
        std::unique_ptr<BYTE> data(new BYTE[imageSize]);
        CTWIN32(::ReadFile(h, data.get(), imageSize, &dwBytesRead, nullptr));
        ::CloseHandle(h);
        CRefObj<ITexture> spTexture;
        CreateTexture(pGraphics, (uint32)header.Width, (uint32)header.Height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UINT, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &spTexture);
        CComPtr<ID3D11Texture2D> spD3DTexture = spTexture->GetD3DTexture();
        D3D11_MAPPED_SUBRESOURCE ms;
        CT(pGraphics->GetContext()->Map(spD3DTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        BYTE *pSrc = data.get();
        BYTE *pDst = reinterpret_cast<BYTE*>(ms.pData);
        if (header.BitsPerPixel == 24)
        {
            for (uint32 i = 0; i < (uint32)header.Height; i++)
            {
                BYTE *pSrcCol = pSrc;
                BYTE *pDstCol = pDst;
                for (uint32 j = 0; j < (uint32)header.Width; j++)
                {
                    pDstCol[0] = pSrcCol[2];
                    pDstCol[1] = pSrcCol[1];
                    pDstCol[2] = pSrcCol[0];
                    pDstCol[3] = 0xff;
                    pDstCol += 4;
                    pSrcCol += 3;
                }
                pSrc += pitch;
                pDst += ms.RowPitch;
            }
        }
        else
        {
            for (uint32 i = 0; i < (uint32)header.Height; i++)
            {
                memcpy(pDst, pSrc, pitch);
                pSrc += pitch;
                pDst += ms.RowPitch;
            }
        }
        pGraphics->GetContext()->Unmap(spD3DTexture, 0);
        (*ppTexture) = spTexture.Detach();
    }
}
