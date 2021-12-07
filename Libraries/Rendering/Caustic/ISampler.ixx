//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>

export module Rendering.Caustic.ISampler;
import Base.Core.Core;
import Base.Core.IRefCount;
import Rendering.Caustic.ITexture;

//**********************************************************************
// File: ISampler.ixx
// This file defines the published interface for samplers
//**********************************************************************

export namespace Caustic
{
    struct IRenderer;

    //**********************************************************************
    // Interface: ISampler
    // Defines a sampler. Samplers define how the underlying texture is accessed by shaders.
    //
    // Module:
    // {Link:import Rendering.Caustic.ISampler;{Rendering/Caustic/ISampler.ixx}}
    //**********************************************************************
    struct ISampler : public IRefCount
    {
        //**********************************************************************
        // Method: GetFilter
        // Returns:
        // Returns the current filter state
        //**********************************************************************
        virtual D3D11_FILTER GetFilter() = 0;

        //**********************************************************************
        // Method: SetFilter
        // Set the current filter state
        //
        // Parameters:
        // pRenderer - Renderer
        // filter - filter state to set
        //**********************************************************************
        virtual void SetFilter(IRenderer* pRenderer, D3D11_FILTER filter) = 0;

        //**********************************************************************
        // Method: GetAddressU
        // Returns:
        // Returns the current addressing mode in the U direction
        //**********************************************************************
        virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressU() = 0;

        //**********************************************************************
        // Method: SetAddressU
        // Set the current addressing mode in the U direction
        //
        // Parameters:
        // pRenderer - Renderer
        // mode - addressing mode to set
        //**********************************************************************
        virtual void SetAddressU(IRenderer* pRenderer, D3D11_TEXTURE_ADDRESS_MODE mode) = 0;

        //**********************************************************************
        // Method: GetAddressV
        // Returns:
        // Returns the current addressing mode in the V direction
        //**********************************************************************
        virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressV() = 0;

        //**********************************************************************
        // Method: SetAddressU
        // Set the current addressing mode in the V direction
        //
        // Parameters:
        // pRenderer - Renderer
        // mode - addressing mode to set
        //**********************************************************************
        virtual void SetAddressV(IRenderer* pRenderer, D3D11_TEXTURE_ADDRESS_MODE mode) = 0;

        //**********************************************************************
        // Method: Render
        // Sets up the sampler for the current rendering
        //
        // Parameters:
        // pRenderer - graphics device
        // slot - texture slot to which this sampler should be assigned
        // isPixelShader - is this sampler for a pixel or vertex shader?
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, int slot, bool isPixelShader) = 0;

        //**********************************************************************
        // Method: GetTexture
        // Returns the texture associated with this sampler
        //
        // Returns:
        // Returns the texture
        //**********************************************************************
        virtual CRefObj<ITexture> GetTexture() = 0;
    };

    //**********************************************************************
    // Function: CreateSampler
    // Creates a sampler. See <ISampler>
    //
    // Parameters:
    // pRenderer - Renderer
    // pTexture - texture sampler is associated with
    //
    // Module:
    // {Link:import Rendering.Caustic.ISampler;{Rendering/Caustic/ISampler.ixx}}
    //**********************************************************************
    CRefObj<ISampler> CreateSampler(IRenderer* pRenderer, ITexture* pTexture);
}
