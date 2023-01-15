//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <wincodec.h>
#include <objbase.h>
#include <map>
#include <d3d11.h>
#include <string>
#include <atlbase.h>

module Rendering.Caustic.Texture;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IRenderer;

namespace Caustic
{
    //**********************************************************************
    // Function: CopyFromImage
    // Copies an image's pixels into a texture
    //
    // Parameters:
    // pRenderer - Renderer
    // pImage - image to use
    //**********************************************************************
    void CTexture::CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap /* = false */)
    {
        pRenderer->RunOnRenderer(
            [&](IRenderer* pRenderer) {
                DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
                bool fastCopy = false;
                switch (pImage->GetBPP())
                {
                case 1:
                    format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM;
                    break;
                case 8:
                    format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM;
                    fastCopy = true;
                    break;
                case 16:
                    format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
                    fastCopy = true;
                    break;
                case 24:
                    format = (pImage->GetImageType() == EImageType::RGBA_32bpp) ?
                        DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM :
                        DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
                    break;
                case 32:
                    format = (pImage->GetImageType() == EImageType::RGBA_32bpp) ?
                        DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM :
                        DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
                    fastCopy = true;
                    break;
                case 128: // Image with 4 floats per pixel
                    format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
                    fastCopy = true;
                    break;
                }
                D3D11_MAPPED_SUBRESOURCE ms;
                auto ctx = pRenderer->GetContext();
                CT(ctx->Map(GetD3DTexture(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
                if (fastCopy)
                {
                    BYTE* pDst = reinterpret_cast<BYTE*>(ms.pData);
                    BYTE* pSrc = pImage->GetData();
                    int h = pImage->GetHeight();
                    int w = pImage->GetWidth();
                    int bps = w * pImage->GetBPP() / 8;
                    int stride = pImage->GetStride();
                    for (int y = 0; y < (int)h; y++)
                    {
                        memcpy(pDst, pSrc, bps);
                        pSrc += stride;
                        pDst += ms.RowPitch;
                    }
                }
                else
                {
                    CImageIterGeneric srcRow(pImage, 0, 0);
                    BYTE* pr = reinterpret_cast<BYTE*>(ms.pData);
                    for (int y = 0; y < (int)pImage->GetHeight(); y++)
                    {
                        CImageIterGeneric srcCol = srcRow;
                        BYTE* pc = pr;
                        for (int x = 0; x < (int)pImage->GetWidth(); x++)
                        {
                            switch (format)
                            {
                            case DXGI_FORMAT::DXGI_FORMAT_R8_UNORM:
                                pc[0] = srcCol.GetRed();
                                pc++;
                                break;
                            case DXGI_FORMAT::DXGI_FORMAT_R16_UINT:
                                ((uint16*)pc)[0] = (uint16)srcCol.GetGray();
                                pc += 2;
                                break;
                            case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
                                pc[0] = srcCol.GetRed();
                                pc[1] = srcCol.GetGreen();
                                pc[2] = srcCol.GetBlue();
                                pc[3] = srcCol.GetAlpha();
                                pc += 4;
                                break;
                            case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
                                pc[0] = srcCol.GetBlue();
                                pc[1] = srcCol.GetGreen();
                                pc[2] = srcCol.GetRed();
                                pc[3] = srcCol.GetAlpha();
                                pc += 4;
                                break;
                            case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
                                break;
                            }
                            srcCol.Step(CImageIter::Right);
                        }
                        pr += ms.RowPitch;
                        srcRow.Step(CImageIter::Down);
                    }
                }
                ctx->Unmap(GetD3DTexture(), 0);
                if (generateMipMap && format == DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
                    GenerateMips(pRenderer);
            }, true);
    }

    void CTexture::CopyToImage(IRenderer* pRenderer, IImage* pImage)
    {
        pRenderer->RunOnRenderer(
            [&](IRenderer* pRenderer) {
                int bpp = 32;
                switch (m_Format)
                {
                case DXGI_FORMAT::DXGI_FORMAT_R8_UNORM:
                    bpp = 8;
                    break;
                case DXGI_FORMAT::DXGI_FORMAT_R16_UINT:
                    bpp = 16;
                    break;
                case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
                case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
                    bpp = 32;
                    break;
                case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
                    bpp = 128;
                    break;
                }
                CT((pImage->GetBPP() == bpp) ? S_OK : E_FAIL);
                CT((pImage->GetWidth() == m_Width) ? S_OK : E_FAIL);
                CT((pImage->GetHeight() == m_Height) ? S_OK : E_FAIL);
                D3D11_MAPPED_SUBRESOURCE ms;
                auto ctx = pRenderer->GetContext();
                CComPtr<ID3D11Texture2D> spTex = GetD3DTexture();
                CT(ctx->Map(spTex, 0, D3D11_MAP_READ, 0, &ms));
                BYTE* pSrc = reinterpret_cast<BYTE*>(ms.pData);
                BYTE* pDst = pImage->GetData();
                int stride = pImage->GetStride();
                if (ms.RowPitch == stride)
                {
                    CopyMemory(pDst, pSrc, m_Height * stride);
                }
                else
                {
                    for (int y = 0; y < (int)m_Height; y++)
                    {
                        memcpy(pDst, pSrc, m_Width * bpp / 8);
                        pSrc += ms.RowPitch;
                        pDst += stride;
                    }
                }
                ctx->Unmap(spTex, 0);
            }, true);
    }
    
    //**********************************************************************
    // Function: CopyToImage
    // Copies an texture's pixels into an <IImage>
    //
    // Parameters:
    // pRenderer - Renderer
    //**********************************************************************
    CRefObj<IImage> CTexture::CopyToImage(IRenderer* pRenderer)
    {
        EImageType imageType;
        switch (m_Format)
        {
        case DXGI_FORMAT::DXGI_FORMAT_R8_UNORM:
            imageType = EImageType::Gray_8bpp;
            break;
        case DXGI_FORMAT::DXGI_FORMAT_R16_UINT:
            imageType = EImageType::Gray_16bpp;
            break;
        case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
            imageType = EImageType::RGBA_32bpp;
            break;
        case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
            imageType = EImageType::BGRA_32bpp;
            break;
        case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
            imageType = EImageType::Float4_128bpp;
            break;
        }
        CRefObj<IImage> spImage = Caustic::CreateImage(m_Width, m_Height, imageType);
        CopyToImage(pRenderer, spImage);
        return spImage;
    }

    //**********************************************************************
    // Function: GenerateMips
    // Generates the MIPMAP chain for a texture
    //
    // Parameters:
    // pRenderer - Renderer
    //**********************************************************************
    void CTexture::GenerateMips(IRenderer *pRenderer)
    {
        pRenderer->RunOnRenderer(
            [&](IRenderer* pRenderer) {
                CD3D11_TEXTURE2D_DESC desc(m_Format, m_Width, m_Height);
                desc.Usage = D3D11_USAGE_DEFAULT;
                desc.CPUAccessFlags = 0;
                desc.MipLevels = 0;
                desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
                desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
                CComPtr<ID3D11Texture2D> spTexture;
                CT(pRenderer->GetDevice()->CreateTexture2D(&desc, nullptr, &spTexture));

                pRenderer->GetContext()->CopySubresourceRegion(spTexture, 0, 0, 0, 0, m_spTexture, 0, NULL);

                m_spTexture = spTexture;

                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
                srvDesc.Format = desc.Format;
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = (UINT)-1;
                m_spTextureRV = nullptr;
                CT(pRenderer->GetDevice()->CreateShaderResourceView(m_spTexture, &srvDesc, &m_spTextureRV));
                pRenderer->GetContext()->GenerateMips(m_spTextureRV);
            }, true);
    }

    //**********************************************************************
    // Constructor: CTexture
    // Ctor for our texture wrapper object
    //
    // Parameters:
    // pRenderer - Graphics renderer
    // width - Width of image in pixels
    // height - Height of image in pixels
    // format - Defines the pixel format for the image
    // cpuFlags - Flags indicating allowed access to texture from CPU (see D3D11 documentation)
    // bindFlags - bind flags (see D3D11 documentation)
    //**********************************************************************
    CTexture::CTexture(IRenderer *pRenderer, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags) :
        m_Format(format),
        m_Width(width),
        m_Height(height)
    {
        CT(pRenderer->IsRenderThread() ? S_OK : E_FAIL);
        CD3D11_TEXTURE2D_DESC desc(format, width, height);
        if (cpuFlags & D3D11_CPU_ACCESS_READ)
            desc.Usage = D3D11_USAGE_STAGING;
        else if (cpuFlags & D3D11_CPU_ACCESS_WRITE)
            desc.Usage = D3D11_USAGE_DYNAMIC;
        else
            desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = cpuFlags;
        desc.MipLevels = 1;
        desc.BindFlags = bindFlags;
        CT(pRenderer->GetDevice()->CreateTexture2D(&desc, nullptr, &m_spTexture));

        if (desc.Usage == D3D11_USAGE_DYNAMIC || desc.Usage == D3D11_USAGE_DEFAULT)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;
            CT(pRenderer->GetDevice()->CreateShaderResourceView(m_spTexture, &srvDesc, &m_spTextureRV));
        }
    }
    
    CTexture::CTexture(ID3D11Texture2D* pD3DTexture, ID3D11ShaderResourceView* pD3DRV)
    {
        m_spTextureRV = pD3DRV;
        m_spTexture = pD3DTexture;
    }

    CTexture::~CTexture()
    {
    }
    
    uint32 CTexture::GetWidth()
    {
        return m_Width;
    }
    
    uint32 CTexture::GetHeight()
    {
        return m_Height;
    }
    
    DXGI_FORMAT CTexture::GetFormat()
    {
        return m_Format;
    }

    void CTexture::Render(IRenderer* pRenderer, int slot, bool isPixelShader)
    {
        pRenderer->RunOnRenderer(
            [&](IRenderer* pRenderer)
            {
                auto ctx = pRenderer->GetContext();
                if (m_spTexture != nullptr)
                {
                    CComPtr<ID3D11ShaderResourceView> spResource = m_spTextureRV;
                    if (isPixelShader)
                        ctx->PSSetShaderResources(slot, 1, &spResource.p);
                    else
                        ctx->VSSetShaderResources(slot, 1, &spResource.p);
                }
                else
                {
                    // No valid texture?! Checkerboard it
                    CRefObj<ITexture> spTexture = CheckerboardTexture(pRenderer);
                    CComPtr<ID3D11ShaderResourceView> spResource = spTexture->GetD3DTextureRV();
                    if (isPixelShader)
                        ctx->PSSetShaderResources(slot, 1, &spResource.p);
                    else
                        ctx->VSSetShaderResources(slot, 1, &spResource.p);
                }
            });
        return;
    }
};
