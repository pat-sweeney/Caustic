//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Imaging\Image\Image.h"
#include "Rendering/Caustic/IRenderer.h"


//**********************************************************************
// File: ITexture.h
// This file defines the published interface for textures.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Interface: ITexture
    // Defines how clients interact with textures
    //
    // Header:
    // [Link:Rendering/Caustic/ITexture.h]
    //**********************************************************************
    struct ITexture : public IRefCount
    {
        //**********************************************************************
        // Method: GetWidth
        // Returns:
        // Returns the width of the texture in pixels
        //**********************************************************************
        virtual uint32 GetWidth() = 0;

        //**********************************************************************
        // Method: GetHeight
        // Returns:
        // Returns the height of the texture in pixels
        //**********************************************************************
        virtual uint32 GetHeight() = 0;

        //**********************************************************************
        // Method: GetFormat
        // Returns:
        // Returns the pixel format of the texture
        //**********************************************************************
        virtual DXGI_FORMAT GetFormat() = 0; // Returns the DirectX format of the texture

        //**********************************************************************
        // Method: Update
        // Updates the underlying graphics card texture. Primarily used for
        // video textures.
        //
        // Parameters:
        // pRenderer - graphics device
        //**********************************************************************
        virtual void Update(IRenderer *pRenderer) = 0; // Called to update texture

        //**********************************************************************
        // Method: GetD3DTexture
        // Returns:
        // Returns the underlying D3D texture
        //**********************************************************************
        virtual CComPtr<ID3D11Texture2D> GetD3DTexture() = 0;

        //**********************************************************************
        // Method: GetD3DTextureRV
        // Returns:
        // Returns the underlying D3D shader resource view associated with the texture
        //**********************************************************************
        virtual CComPtr<ID3D11ShaderResourceView> GetD3DTextureRV() = 0;

        //**********************************************************************
        // Method: GenerateMips
        // Generates a set of mipmaps for the texture
        //
        // Parameters:
        // pRenderer - graphics device
        //**********************************************************************
        virtual void GenerateMips(IRenderer *pRenderer) = 0;

        //**********************************************************************
        // Method: Render
        // Pushes the specified texture to the GPU
        //
        // Parameters:
        // pRenderer - graphics device
        // slot - texture slot to assign texture to
        // isPixelShader - is texture being assigned to a pixel or vertex shader
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) = 0;

        //**********************************************************************
        // Method: CopyFromImage
        // Copies an image into an existing ITexture
        //
        // Parameters:
        // pRenderer - graphics device
        // pImage - image to copy pixels from
        //**********************************************************************
        virtual void CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap = false) = 0;
    };

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
    //
    // Header:
    // [Link:Rendering/Caustic/ITexture.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags);
    
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
    //
    // Header:
    // [Link:Rendering/Caustic/ITexture.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, uint32 width, uint32 height, DXGI_FORMAT format);
    
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
    //
    // Header:
    // [Link:Rendering/Caustic/ITexture.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, IImage* pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags);
    
    //**********************************************************************
    // Function: CheckerboardTexture
    // Creates a black and white checkboard texture
    //
    // Parameters:
    // pRenderer - Renderer
    //
    // Header:
    // [Link:Rendering/Caustic/ITexture.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> CheckerboardTexture(IRenderer* pRenderer);

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
    //
    // Header:
    // [Link:Rendering/Caustic/ITexture.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> LoadTexture(const wchar_t* pFilename, IRenderer* pRenderer);

    //**********************************************************************
    // Function: LoadVideoTexture
    // LoadVideoTexture loads a video and uses it as the texture source
    //
    // Parameters:
    // pFilename - Name of file to load
    // pRenderer - Renderer
    //
    // Returns:
    // Returns the new texture
    //
    // Header:
    // [Link:Rendering/Caustic/ITexture.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> LoadVideoTexture(const wchar_t* pFilename, IRenderer* pRenderer);

    //**********************************************************************
    // Function: VideoTextureFromWebcam
    // VideoTextureFromWebcam loads a video texture using a webcam as its source
    //
    // Parameters:
    // pRenderer - Renderer
    //
    // Returns:
    // Returns the new texture
    //
    // Header:
    // [Link:Rendering/Caustic/ITexture.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ITexture> VideoTextureFromWebcam(IRenderer* pRenderer);
}
