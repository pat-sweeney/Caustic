//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Error.h"
#include "Texture.h"
#include <memory>
#include <wincodec.h>
#include <objbase.h>
#include <map>

namespace Caustic
{
    //**********************************************************************
    //! \brief Generates all the MIP levels for the texture
    //! \param[in] pGraphics Graphics renderer
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
        CT(pGraphics->GetDevice()->CreateTexture2D(&desc, nullptr, &spTexture.p));

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
    //! \brief Ctor for our texture wrapper object
    //! \param[in] pGraphics Graphics renderer
    //! \param[in] width Width in pixels of image
    //! \param[in] height Height in pixels of image
    //! \param[in format Defines the pixel format for the image
    //**********************************************************************
    CTexture::CTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags) :
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
        CT(pGraphics->GetDevice()->CreateTexture2D(&desc, nullptr, &m_spTexture.p));

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
    
    //**********************************************************************
    //! \brief Returns the width of the texture
    //**********************************************************************
    uint32 CTexture::GetWidth()
    {
        return m_Width;
    }
    
    //**********************************************************************
    //! \brief Returns the height of the texture
    //**********************************************************************
    uint32 CTexture::GetHeight()
    {
        return m_Height;
    }
    
    //**********************************************************************
    //! \brief Returns the format of the texture
    //**********************************************************************
    DXGI_FORMAT CTexture::GetFormat()
    {
        return m_Format;
    }

    //**********************************************************************
    //! \brief Creates a new empty texture
    //! \param[in] pGraphics Renderer
    //! \param[in] width Width of texture in pixels
    //! \param[in] height Height of texture in pixels
    //! \param[in] format Format of pixel
    //! \param[out] ppTexture Returns the newly created texture
    //**********************************************************************
    CAUSTICAPI void CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture)
    {
        std::unique_ptr<CTexture> spTexture(new CTexture(pGraphics, width, height, format, cpuFlags, bindFlags));
        *ppTexture = spTexture.release();
        (*ppTexture)->AddRef();
    }
    
    //**********************************************************************
    //! \brief Creates a static texture that looks like a checkerboard
    //! \param[in] pGraphics Renderer
    //! \returns Checkerboard texture
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> CheckboardTexture(IGraphics *pGraphics)
    {
        static CRefObj<ITexture> s_spCheckerBoard;
        if (s_spCheckerBoard == nullptr)
        {
            CreateTexture(pGraphics, 32, 32, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &s_spCheckerBoard);
            CComPtr<ID3D11Texture2D> spTexture = s_spCheckerBoard->GetD3DTexture();
            D3D11_MAPPED_SUBRESOURCE ms;
            CT(pGraphics->GetContext()->Map(spTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
            BYTE *pr = reinterpret_cast<BYTE*>(ms.pData);
            for (int i = 0; i < 32; i++)
            {
                BYTE *pc = pr;
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
    //! \brief LoadTexture loads a texture from a file using WIC
    //! \param[in] pFilename Name of file to load
    //! \param[in] pGraphics Renderer
    //! \param[out] ppTexture Returns the new texture
    //**********************************************************************
    CAUSTICAPI void LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture)
    {
        static std::map<std::wstring, CRefObj<ITexture>> cache;
        std::map<std::wstring, CRefObj<ITexture>>::iterator it;
        if ((it = cache.find(pFilename)) != cache.end())
        {
            *ppTexture = it->second.p;
            if (*ppTexture)
                (*ppTexture)->AddRef();
            return;
        }
        const wchar_t *pExt = &pFilename[wcslen(pFilename) - 1];
        while (pExt > pFilename && *pExt != '.')
            pExt--;
        if (*pExt == L'.')
        {
            // if (_wcsnicmp(pExt, L".tga", 4) == 0)
            // {
            //     extern     void LoadTarga(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);
            //     LoadTarga(pFilename, pGraphics, ppTexture);
            //     CRefObj<ITexture> obj(*ppTexture);
            //     cache[pFilename] = obj;
            //     return;
            // }
        }
        static CComPtr<IWICImagingFactory> s_spFactory;
        if (s_spFactory == nullptr)
        {
            CT(CoInitialize(nullptr));
            CT(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void**)&s_spFactory.p));
        }
        
        CComPtr<IWICBitmapDecoder> spDecoder;
        CT(s_spFactory->CreateDecoderFromFilename(pFilename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &spDecoder));
        
        CComPtr<IWICBitmapFrameDecode> spFrame;
        CT(spDecoder->GetFrame(0, &spFrame));
        
        UINT width, height;
        CT(spFrame->GetSize(&width, &height));
        
        CComPtr<IWICFormatConverter> spConverter;
        CT(s_spFactory->CreateFormatConverter(&spConverter));
        CT(spConverter->Initialize(spFrame, GUID_WICPixelFormat32bppRGBA,
            WICBitmapDitherType::WICBitmapDitherTypeNone, nullptr, 0.0f, 
            WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut));

        CRefObj<ITexture> spTexture;
        CreateTexture(pGraphics, width, height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &spTexture);
        CComPtr<ID3D11Texture2D> spD3DTexture = spTexture->GetD3DTexture();
        D3D11_MAPPED_SUBRESOURCE ms;
        CT(pGraphics->GetContext()->Map(spD3DTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        BYTE *prDst = reinterpret_cast<BYTE*>(ms.pData);
        for (UINT i = 0; i < height; i++)
        {
            WICRect r;
            r.X = 0;
            r.Y = i;
            r.Width = width;
            r.Height = 1;
            CT(spConverter->CopyPixels(&r, width * 4, width * 4, prDst));
            prDst += ms.RowPitch;
        }
        pGraphics->GetContext()->Unmap(spD3DTexture, 0);

        spTexture->GenerateMips(pGraphics);
        
        (*ppTexture) = spTexture.Detach();
        CRefObj<ITexture> obj(*ppTexture);
        cache[pFilename] = obj;
    }
};
