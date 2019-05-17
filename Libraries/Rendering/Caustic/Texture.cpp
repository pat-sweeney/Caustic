//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Error.h"
#include "Texture.h"
#include "CausticFactory.h"
#include <memory>
#include <wincodec.h>
#include <objbase.h>
#include <map>
#include <d3dx12.h>

namespace Caustic
{
    void CTexture::GenerateMips(IRenderer *pRenderer)
    {
#if 0
        CD3DX12_TEXTURE2D_DESC desc(m_Format, m_Width, m_Height);
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
		pRenderer->GetContext()->GenerateMips(m_spTextureRV);
#endif
    }

    //**********************************************************************
    CTexture::CTexture(IRenderer *pRenderer, uint32 width, uint32 height, DXGI_FORMAT format) :
        m_Format(format),
        m_Width(width),
        m_Height(height)
    {
		D3D12_RESOURCE_DESC texDesc = {};
		texDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
		texDesc.Alignment = D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT;
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.DepthOrArraySize = 1;
		texDesc.MipLevels = 1;
		texDesc.Format = format;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		texDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

		CComPtr<ID3D12Device> spDevice = pRenderer->GetDevice();

		// this function gets the size an upload buffer needs to be to upload a texture to the gpu.
		// each row must be 256 byte aligned except for the last row, which can just be the size in bytes of the row
		// eg. textureUploadBufferSize = ((((width * numBytesPerPixel) + 255) & ~255) * (height - 1)) + (width * numBytesPerPixel);
		uint32 imageBytesPerRow = width * 4;
		UINT64 textureUploadBufferSize = (((imageBytesPerRow + 255) & ~255) * (texDesc.Height - 1)) + imageBytesPerRow;
		D3D12_RESOURCE_ALLOCATION_INFO info = spDevice->GetResourceAllocationInfo(0, 0, &texDesc);
		spDevice->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);

		// now we create an upload heap to upload our texture to the GPU
		spDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
			D3D12_HEAP_FLAG_NONE, // no flags
			&CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize), // resource description for a buffer (storing the image data in this heap just to copy to the default heap)
			D3D12_RESOURCE_STATE_GENERIC_READ, // We will copy the contents from this heap to the default heap above
			nullptr,
			IID_PPV_ARGS(&m_spTexture));
		m_spTexture->SetName(L"Texture");
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

    //**********************************************************************
    // LoadTexture loads a texture from a file using WIC
    // pFilename Name of file to load
    // pGraphics Renderer
    // \param[out] ppTexture Returns the new texture
    //**********************************************************************
    CAUSTICAPI void LoadTexture(const wchar_t *pFilename, IRenderer *pGraphics, ITexture **ppTexture)
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
        Caustic::CCausticFactory::Instance()->CreateTexture(pGraphics, width, height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, &spTexture);
        CComPtr<ID3D12Resource> spD3DTexture = spTexture->GetD3DTexture();
		D3D12_RANGE range;
		range.Begin = 0;
		range.End = 0;
		BYTE *prDst;
		CT(spD3DTexture->Map(0, &range, (void**)&prDst));
        for (UINT i = 0; i < height; i++)
        {
            WICRect r;
            r.X = 0;
            r.Y = i;
            r.Width = width;
            r.Height = 1;
            CT(spConverter->CopyPixels(&r, width * 4, width * 4, prDst));
            prDst += width * 4;
        }
		spD3DTexture->Unmap(0, &range);

        spTexture->GenerateMips(pGraphics);
        
        (*ppTexture) = spTexture.Detach();
        CRefObj<ITexture> obj(*ppTexture);
        cache[pFilename] = obj;
    }
};
