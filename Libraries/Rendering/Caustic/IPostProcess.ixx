//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <string>

export module Rendering.Caustic.IPostProcess;
import Base.Core.Core;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderer;

//**********************************************************************
// File: IPostProcess.ixx
// Defines the interface for post-processing effects and the
// post-processing chain used by the renderer.
//**********************************************************************

export namespace Caustic
{
    struct IShader;
    struct ITexture;

    //**********************************************************************
    // Interface: IPostProcess
    // Represents a single post-processing effect that operates on
    // fullscreen render targets.
    //
    // Module:
    // {Link:import Rendering.Caustic.IPostProcess;{Rendering/Caustic/IPostProcess.ixx}}
    //**********************************************************************
    struct IPostProcess : public IRefCount
    {
        //**********************************************************************
        // Method: GetName
        // Returns the name of this post-process effect
        //**********************************************************************
        virtual const wchar_t* GetName() = 0;

        //**********************************************************************
        // Method: IsEnabled
        // Returns whether this effect is currently active
        //**********************************************************************
        virtual bool IsEnabled() = 0;

        //**********************************************************************
        // Method: SetEnabled
        // Enables or disables this effect
        //
        // Parameters:
        // enabled - true to enable, false to disable
        //**********************************************************************
        virtual void SetEnabled(bool enabled) = 0;

        //**********************************************************************
        // Method: Render
        // Executes the post-process effect. The effect reads from the source
        // render target and writes to the destination render target.
        //
        // Parameters:
        // pRenderer - the renderer
        // pSrcRT - source render target (scene or previous effect output)
        // pSrcDepth - depth buffer SRV (for effects like SSAO)
        // pDstRTV - destination render target view to write to
        // width - render target width in pixels
        // height - render target height in pixels
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, ID3D11ShaderResourceView* pSrcRT,
            ID3D11ShaderResourceView* pSrcDepth, ID3D11RenderTargetView* pDstRTV,
            uint32_t width, uint32_t height) = 0;
    };
}
