//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"

//**********************************************************************
// File: ITexture.h
// This file defines the published interface for textures.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Interface: ITexture
    // Defines how clients interact with textures
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
        // pGraphics - graphics device
        //**********************************************************************
        virtual void Update(IGraphics *pGraphics) = 0; // Called to update texture

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
        // pGraphics - graphics device
        //**********************************************************************
        virtual void GenerateMips(IGraphics *pGraphics) = 0;

        //**********************************************************************
        // Method: Render
        // Pushes the specified texture to the GPU
        //
        // Parameters:
        // pGraphics - graphics device
        // slot - texture slot to assign texture to
        // isPixelShader - is texture being assigned to a pixel or vertex shader
        //**********************************************************************
        virtual void Render(IGraphics* pGraphics, int slot, bool isPixelShader) = 0;
    };
}
