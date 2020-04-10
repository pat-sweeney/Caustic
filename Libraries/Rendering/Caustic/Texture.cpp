//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Error.h"
#include "Imaging\Image\Image.h"
#include "Imaging\Image\ImageIter.h"
#include "Texture.h"
#include <memory>
#include <wincodec.h>
#include <objbase.h>
#include <map>

namespace Caustic
{
    //**********************************************************************
    // Function: CreateTexture
    // Creates an empty texture.
    //
    // Parameters:
    // pGraphics - Renderer
    // width - width of texture in pixels
    // height - height of texture in pixels
    // format - image format
    // ppTexture - Returns the created texture
    //**********************************************************************
    CAUSTICAPI void CreateTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags, ITexture** ppTexture)
    {
        std::unique_ptr<CTexture> spTexture(new CTexture(pGraphics, width, height, format, cpuFlags, bindFlags));
        *ppTexture = spTexture.release();
        (*ppTexture)->AddRef();
    }

    //**********************************************************************
    // Function: CreateTexture
    // Creates an empty texture.
    //
    // Parameters:
    // pGraphics - Renderer
    // width - width of texture in pixels
    // height - height of texture in pixels
    // format - image format
    // ppTexture - Returns the created texture
    //**********************************************************************
    CAUSTICAPI void CreateTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, ITexture** ppTexture)
    {
        CreateTexture(pGraphics, width, height, format, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, ppTexture);
    }

    //**********************************************************************
    // Function: CreateTexture
    // Creates a texture from an <IImage at Caustic::IImage>.
    //
    // Parameters:
    // pGraphics - Renderer
    // pImage - image to use
    // cpuFlags - Flags indicating allowed access to texture from CPU (see D3D11 documentation)
    // bindFlags - bind flags (see D3D11 documentation)
    // ppTexture - Returns the created texture
    //**********************************************************************
    CAUSTICAPI void CreateTexture(IGraphics* pGraphics, IImage* pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags, ITexture** ppTexture)
    {
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
            format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case 32:
            format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
            fastCopy = true;
            break;
        case 128: // Image with 4 floats per pixel
            format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
            fastCopy = true;
            break;
        }
        CRefObj<ITexture> spTexture;
        CreateTexture(pGraphics, pImage->GetWidth(), pImage->GetHeight(), format, cpuFlags, bindFlags, &spTexture);
        D3D11_MAPPED_SUBRESOURCE ms;
        CT(pGraphics->GetContext()->Map(spTexture->GetD3DTexture(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        if (fastCopy)
        {
            BYTE* pDst = reinterpret_cast<BYTE*>(ms.pData);
            BYTE* pSrc = pImage->GetData();
            for (int y = 0; y < (int)pImage->GetHeight(); y++)
            {
                memcpy(pDst, pSrc, pImage->GetWidth() * pImage->GetBPP() / 8);
                pSrc += pImage->GetStride();
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
                    case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
                        break;
                    }
                    srcCol.Step(CImageIter::Right);
                }
                pr += ms.RowPitch;
                srcRow.Step(CImageIter::Down);
            }
        }
        pGraphics->GetContext()->Unmap(spTexture->GetD3DTexture(), 0);
        if (format == DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
            spTexture->GenerateMips(pGraphics);
        (*ppTexture) = spTexture.Detach();
    }

    //**********************************************************************
    // Function: CheckerboardTexture
    // Creates a black and white checkboard texture
    //
    // Parameters:
    // pGraphics - Renderer
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> CheckerboardTexture(IGraphics* pGraphics)
    {
        static CRefObj<ITexture> s_spCheckerBoard;
        if (s_spCheckerBoard == nullptr)
        {
            CreateTexture(pGraphics, 32, 32, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &s_spCheckerBoard);
            CComPtr<ID3D11Texture2D> spTexture = s_spCheckerBoard->GetD3DTexture();
            D3D11_MAPPED_SUBRESOURCE ms;
            CT(pGraphics->GetContext()->Map(spTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
            BYTE* pr = reinterpret_cast<BYTE*>(ms.pData);
            for (int i = 0; i < 32; i++)
            {
                BYTE* pc = pr;
                for (int j = 0; j < 32; j++)
                {
                    if ((i & 1) == (j & 1))
                    {
                        pc[0] = 0x00;
                        pc[1] = 0x00;
                        pc[2] = 0x00;
                        pc[3] = 0xff;
                    }
                    else
                    {
                        pc[0] = 0xff;
                        pc[1] = 0xff;
                        pc[2] = 0xff;
                        pc[3] = 0xff;
                    }
                    pc += 4;
                }
                pr += ms.RowPitch;
            }
            pGraphics->GetContext()->Unmap(spTexture, 0);
        }
        return s_spCheckerBoard;
    }

    //**********************************************************************
    // Function: LoadTexture
    // LoadTexture loads a texture from a file using WIC
    //
    // Parameters:
    // pFilename - Name of file to load
    // pGraphics - Renderer
    // ppTexture - Returns the new texture
    //**********************************************************************
    CAUSTICAPI void LoadTexture(const wchar_t* pFilename, IGraphics* pGraphics, ITexture** ppTexture)
    {
        static std::map<std::wstring, CRefObj<ITexture>> cache;
        std::map<std::wstring, CRefObj<ITexture>>::iterator it;
        if ((it = cache.find(pFilename)) != cache.end())
        {
            *ppTexture = it->second;
            if (*ppTexture)
                (*ppTexture)->AddRef();
            return;
        }
        CRefObj<IImage> spImage;
        LoadImage(pFilename, &spImage);
        CreateTexture(pGraphics, spImage, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, ppTexture);
        CRefObj<ITexture> obj(*ppTexture);
        cache[pFilename] = obj;
    }

    //**********************************************************************
    // Function: GenerateMips
    // Generates the MIPMAP chain for a texture
    //
    // Parameters:
    // pGraphics - Renderer
    //**********************************************************************
    void CTexture::GenerateMips(IGraphics *pGraphics)
    {
        CD3D11_TEXTURE2D_DESC desc(m_Format, m_Width, m_Height);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MipLevels = 0;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
        CComPtr<ID3D11Texture2D> spTexture;
        CT(pGraphics->GetDevice()->CreateTexture2D(&desc, nullptr, &spTexture));

        pGraphics->GetContext()->CopySubresourceRegion(spTexture, 0, 0, 0, 0, m_spTexture, 0, NULL);

        m_spTexture = spTexture;

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Format = desc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = (UINT)-1;
        m_spTextureRV = nullptr;
        CT(pGraphics->GetDevice()->CreateShaderResourceView(m_spTexture, &srvDesc, &m_spTextureRV));
        pGraphics->GetContext()->GenerateMips(m_spTextureRV);
    }

    //**********************************************************************
    // Constructor: CTexture
    // Ctor for our texture wrapper object
    //
    // Parameters:
    // pGraphics - Graphics renderer
    // width - Width of image in pixels
    // height - Height of image in pixels
    // format - Defines the pixel format for the image
    // cpuFlags - Flags indicating allowed access to texture from CPU (see D3D11 documentation)
    // bindFlags - bind flags (see D3D11 documentation)
    //**********************************************************************
    CTexture::CTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags) :
        m_Format(format),
        m_Width(width),
        m_Height(height)
    {
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
        CT(pGraphics->GetDevice()->CreateTexture2D(&desc, nullptr, &m_spTexture));

        if (desc.Usage == D3D11_USAGE_DYNAMIC || desc.Usage == D3D11_USAGE_DEFAULT)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;
            CT(pGraphics->GetDevice()->CreateShaderResourceView(m_spTexture, &srvDesc, &m_spTextureRV));
        }
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

    void CTexture::Render(IGraphics* pGraphics, int slot, bool isPixelShader)
    {
        auto ctx = pGraphics->GetContext();
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
            CRefObj<ITexture> spTexture = CheckerboardTexture(pGraphics);
            CComPtr<ID3D11ShaderResourceView> spResource = spTexture->GetD3DTextureRV();
            if (isPixelShader)
                ctx->PSSetShaderResources(slot, 1, &spResource.p);
            else
                ctx->VSSetShaderResources(slot, 1, &spResource.p);
        }
    }
};
