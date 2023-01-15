//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <map>
#include <string>
#include <atlbase.h>
#include <memory>

module Rendering.Caustic.ITexture;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.Sampler;
import Rendering.Caustic.Texture;

namespace Caustic
{
    //**********************************************************************
    // Function: CreateTexture
    // Creates an empty texture.
    //
    // Parameters:
    // pRenderer - Renderer
    // width - width of texture in pixels
    // height - height of texture in pixels
    // format - image format
    // cpuFlags - D3D cpu flags to use
    // bindFlags - D3D bind flags to use
    //
    // Returns:
    // Returns the created texture
    //**********************************************************************
    CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags)
    {
        CRefObj<ITexture> spTexture;
        pRenderer->RunOnRenderer(
            [width, height, format, cpuFlags, bindFlags, &spTexture](IRenderer* pRenderer)
            {
                spTexture = CRefObj<ITexture>(new CTexture(pRenderer, width, height, format, cpuFlags, bindFlags));
            }, true);
        return spTexture;
    }

    //**********************************************************************
    // Function: CreateTexture
    // Creates an empty texture.
    //
    // Parameters:
    // pRenderer - Renderer
    // width - width of texture in pixels
    // height - height of texture in pixels
    // format - image format
    //
    // Returns:
    // Returns the created texture
    //**********************************************************************
    CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, uint32 width, uint32 height, DXGI_FORMAT format)
    {
        return CreateTexture(pRenderer, width, height, format, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
    }

    //**********************************************************************
    // Function: CreateTexture
    // Creates a texture from an <IImage at Caustic::IImage>.
    //
    // Parameters:
    // pRenderer - Renderer
    // pImage - image to use
    // cpuFlags - Flags indicating allowed access to texture from CPU (see D3D11 documentation)
    // bindFlags - bind flags (see D3D11 documentation)
    //
    // Returns:
    // Returns the created texture
    //**********************************************************************
    CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, IImage* pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags)
    {
        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
        switch (pImage->GetBPP())
        {
        case 1: format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM; break;
        case 8: format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM; break;
        case 16: format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT; break;
        case 24: format = (pImage->GetImageType() == EImageType::RGBA_32bpp) ? DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM; break;
        case 32: format = (pImage->GetImageType() == EImageType::RGBA_32bpp) ? DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM; break;
        case 128: format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT; break;
        }
        CRefObj<ITexture> spTexture = CreateTexture(pRenderer, pImage->GetWidth(), pImage->GetHeight(), format, cpuFlags, bindFlags);
        spTexture->CopyFromImage(pRenderer, pImage);
        return spTexture;
    }

    //**********************************************************************
    // Function: CheckerboardTexture
    // Creates a black and white checkboard texture
    //
    // Parameters:
    // pRenderer - Renderer
    //**********************************************************************
    CRefObj<ITexture> CheckerboardTexture(IRenderer* pRenderer, int w /* = 32 */, int h /* = 32 */, int blocksize /* = 4 */)
    {
        static CRefObj<ITexture> s_spCheckerBoard;
        if (s_spCheckerBoard == nullptr)
        {
            s_spCheckerBoard = CreateTexture(pRenderer, w, h, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
            CComPtr<ID3D11Texture2D> spTexture = s_spCheckerBoard->GetD3DTexture();
            D3D11_MAPPED_SUBRESOURCE ms;
            pRenderer->RunOnRenderer(
                [&](IRenderer* pRenderer) {
                    CT(pRenderer->GetContext()->Map(spTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
                    BYTE* pr = reinterpret_cast<BYTE*>(ms.pData);
                    for (int i = 0; i < h; i++)
                    {
                        BYTE* pc = pr;
                        for (int j = 0; j < w; j++)
                        {
                            int bx = j / blocksize;
                            int by = i / blocksize;
                            if ((bx & 1) == (by & 1))
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
                    pRenderer->GetContext()->Unmap(spTexture, 0);
                }, true);
        }
        return s_spCheckerBoard;
    }

    //**********************************************************************
    // Function: LoadTexture
    // LoadTexture loads a texture from a file using WIC
    //
    // Parameters:
    // pFilename - Name of file to load
    // pRenderer - Renderer
    //
    // Returns:
    // Returns the new texture
    //**********************************************************************
    CRefObj<ITexture> LoadTexture(const wchar_t* pFilename, IRenderer* pRenderer)
    {
        static std::map<std::wstring, CRefObj<ITexture>> cache;
        std::map<std::wstring, CRefObj<ITexture>>::iterator it;
        if ((it = cache.find(pFilename)) != cache.end())
            return CRefObj<ITexture>(it->second);
        CRefObj<IImage> spImage = Caustic::LoadImageFile(pFilename);
        CRefObj<ITexture> spTexture = CreateTexture(pRenderer, spImage, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        cache[pFilename] = spTexture;
        return spTexture;
    }
}
