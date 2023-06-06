//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <atlbase.h>
#include <d3d11.h>
#include <directxmath.h>
#include <dxgi1_6.h>
#include <string>

export module Rendering.Caustic.IRenderer;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Geometry.Mesh.IMesh;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.ITexture;

//**********************************************************************
// File: IRenderer.ixx
// This file defines the published interface for the renderer.
//**********************************************************************

export namespace Caustic
{
    struct ISceneGraph;
    struct IRenderSubMesh;
    struct IRenderCtx;
    struct IPointLight;
    struct IShader;
    struct ICamera;
    struct ISampler;
    struct IRenderable;
    struct IRenderMesh;
    struct IShaderMgr;
    struct ILight;

    //**********************************************************************
    // Interface: IRenderer
    // Defines our basic renderer. IRenderer handles all rendering commands.
    // It is generally expected that this object is running on its own thread.
    //
    // Module:
    // {Link:import Rendering.Caustic.IRenderer;{Rendering/Caustic/IRenderer.ixx}}
    //**********************************************************************
    struct IRenderer : public IRefCount
    {
        //**********************************************************************
        // Method: SetViewport
        // Specifies the viewport for the final render target
        // 
        // Parameters:
        // x0 - top left X coordinate from 0..1 indicating which portion of the output window is used
        // y0 - top left Y coordinate from 0..1 indicating which portion of the output window is used
        // x1 - bottom right X coordinate from 0..1 indicating which portion of the output window is used
        // y1 - bottom right Y coordinate from 0..1 indicating which portion of the output window is used
        //**********************************************************************
        virtual void SetViewport(float x0, float y0, float x1, float y1) = 0;
        
        //**********************************************************************
        // Method: GetRenderThreadID
        // Returns the thread ID of the render thread
        // 
        // Returns:
        // Thread ID of the render thread
        //**********************************************************************
        virtual DWORD GetRenderThreadID() = 0;
        
        //**********************************************************************
        // Method: ToRenderMesh
        // Converts a mesh to a render mesh
        // 
        // Parameters:
        // pMesh - mesh to convert
        // pShader - shader to use (for vertex definition)
        //**********************************************************************
        virtual CRefObj<IRenderMesh> ToRenderMesh(IMesh* pMesh, IShader* pShader) = 0;

        //**********************************************************************
        // Method: ToRenderMaterials
        // Converts a list of material definitions into a list of renderable materials
        // 
        // Parameters:
        // pMesh - mesh whose materials need to be generated
        // pShader - shader to use (for vertex definition)
        // pRenderMesh - renderable version of 'pMesh'
        // pDefaultMaterial - default material to use if mesh/submesh has no associated material
        //**********************************************************************
        virtual void ToRenderMaterials(IMesh* pMesh, IShader* pShader, IRenderMesh* pRenderMesh, IMaterialAttrib* pDefaultMaterial) = 0;

        //**********************************************************************
        // Method: IsRenderThread
        // Returns whether the current thread is the render thread
        // 
        // Returns:
        // true if we are on the render thread; false otherwise
        //**********************************************************************
        virtual bool IsRenderThread() = 0;

        //**********************************************************************
        // Method: RunOnRenderer
        // Runs the specified function on the render thread.
        // 
        // Parameters:
        // callback - function to call
        // wait - should we wait for completion before returning?
        //**********************************************************************
        virtual void RunOnRenderer(std::function<void(IRenderer*)> callback, bool wait = false) = 0;

#ifdef _DEBUG
        //**********************************************************************
        // Method: BeginMarker
        // Add marker to display in graphics debugger
        //
        // Parameters:
        // pLabel - label
        //**********************************************************************
        virtual void BeginMarker(const wchar_t* pLabel) = 0;

        //**********************************************************************
        // Method: EndMarker
        // Ends marker
        //**********************************************************************
        virtual void EndMarker() = 0;

#ifdef SUPPORT_GRAPHICS_CAPTURE
        virtual void BeginCapture() = 0;
        virtual void EndCapture() = 0;
#endif // SUPPORT_GRAPHICS_CAPTURE
#endif

        //**********************************************************************
        // Method: EnableDepthTest
        // Turns on or off depth testing
        // 
        // Parameters:
        // enable - indicates whether to turn on or off depth testing
        // 
        // Returns:
        // Previous state
        //**********************************************************************
        virtual bool EnableDepthTest(bool enable) = 0;
        
        //**********************************************************************
        // Method: GetDevice
        // Returns:
        // The underlying D3D11 device
        //**********************************************************************
        virtual CComPtr<ID3D11Device> GetDevice() = 0;

        //**********************************************************************
        // Method: GetDuplication
        // Returns:
        // The underlying windows duplication service
        //**********************************************************************
        virtual CComPtr<IDXGIOutputDuplication> GetDuplication() = 0;

        //**********************************************************************
        // Method: Freeze
        // Freezes the renderer
        //**********************************************************************
        virtual void Freeze() = 0;

        //**********************************************************************
        // Method: Unfreeze
        // Unfreezes the renderer
        //**********************************************************************
        virtual void Unfreeze() = 0;

        //**********************************************************************
        // Method: IsFrozen
        // Returns whether the renderer is currently frozen
        //**********************************************************************
        virtual bool IsFrozen() = 0;

        //**********************************************************************
        // Method: LoadShaders
        // Loads into our shader manager all the shaders in the specified folder
        //
        // Parameters:
        // pFolder - folder from which to load shaders
        //**********************************************************************
        virtual void LoadShaders(const wchar_t* pFolder) = 0;

        //**********************************************************************
        // Method: SetPostEffect
        // Assigns a GPUImaging pipeline that is to run as a series of post effects
        // on the final render buffer.
        //
        // Parameters:
        // pPipeline - defines a GPU pipeline to run on the final rendered image
        //**********************************************************************
        // virtual void SetPostEffect(IGPUPipeline* pPipeline) = 0;

        //**********************************************************************
        // Method: DrawScreenQuadWithCustomShader
        // Renders a quad on the display at the specified uv coordinates.
        // A full screen quad runs from 0.0,0.0 => 1.0,1.0
        //
        // Parameters:
        // pShader - shader to use. The method expects the shader to contain
        //        the following variables:
        //           tex - texture to use
        //           s - sampler
        //           minu - min UV coordinate where quad is to be drawn
        //           minv - min UV coordinate where quad is to be drawn
        //           maxu - max UV coordinate where quad is to be drawn
        //           maxv - max UV coordinate where quad is to be drawn
        // minU - minimum U
        // minV - minimum V
        // maxU - maximum U
        // maxV - maximum V
        // pTexture - Texture to render on quad
        // pSampler - Sampler to use. Maybe nullptr
        // disableDepth - disable depth mapping?
        //**********************************************************************
        virtual void DrawScreenQuadWithCustomShader(IShader* pShader, float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler* pSampler, bool disableDepth = false) = 0;

        //**********************************************************************
        // Method: DrawScreenQuad
        // Renders a quad on the display at the specified uv coordinates.
        // A full screen quad runs from 0.0,0.0 => 1.0,1.0
        //
        // Parameters:
        // minU - minimum U
        // minV - minimum V
        // maxU - maximum U
        // maxV - maximum V
        // pTexture - Texture to render on quad
        // pSampler - Sampler to use. Maybe nullptr
        // disableDepth - disable depth mapping?
        //**********************************************************************
        virtual void DrawScreenQuad(float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler *pSampler, bool disableDepth = false) = 0;

        //**********************************************************************
        // Method: GetContext
        // Returns:
        // The underlying D3D11 device context
        //**********************************************************************
        virtual CComPtr<ID3D11DeviceContext> GetContext() = 0;

        //**********************************************************************
        // Method: AddRenderable
        // Returns:
        // Adds a renderable object to the renderer
        //**********************************************************************
        virtual void AddRenderable(IRenderable* pRenderable) = 0;

        //**********************************************************************
        // Method: GetCamera
        // Returns:
        // The camera associated with this device
        //**********************************************************************
        virtual CRefObj<ICamera> GetCamera() = 0;

        //**********************************************************************
        // Method: SetCamera
        // Assigns a camera to the renderer
        //
        // Parameters:
        // pCamera - camera to use when rendering
        //**********************************************************************
        virtual void SetCamera(ICamera* pCamera) = 0;

        //**********************************************************************
        // Method: GetShaderMgr
        // Returns the shader manager being used by the renderer
        //**********************************************************************
        virtual CRefObj<IShaderMgr> GetShaderMgr() = 0;

        //**********************************************************************
        // Method: Setup
        // Initializes the renderer. Applications need to call this method at startup.
        //
        // Parameters:
        // hwnd - window to bind renderer to
        // viewport - viewport onto window for final render
        // shaderFolder - path to shader folder
        // createDebugDevice - True if application wants the debug D3D device. False otherwise.
        // startFrozen - start renderer in a frozen state.
        // desktopIndex - index indicating which desktop duplication service will use
        //**********************************************************************
        virtual void Setup(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder, bool createDebugDevice, bool startFrozen = false, int desktopIndex = 0) = 0;

        //**********************************************************************
        // Method: DeviceWindowResized
        // Called when device window has been resized
        //
        // Parameters:
        // width - width of output device in pixels
        // height - height of output device in pixels
        //**********************************************************************
        virtual void DeviceWindowResized(uint32 width, uint32 height) = 0;

        //**********************************************************************
        // Method: DrawMesh
        // Renders a mesh by adding it to the Renderable list that is drawn each time a frame is rendered
        //
        // TODO: This method needs to be refactored. There is no point in sending in both a pMaterial
        // and a pTexture (the material already should define the diffuse texture). Also, we should pass
        // in both a front and back face material
        //
        // Parameters:
        // pMesh - mesh to render
        // pMaterial - material for front faces
        // pTexture - texture for diffuse component
        // pShader - shader to use to draw mesh
        // mat - matrix to transform mesh by
        //**********************************************************************
        virtual void DrawMesh(IRenderSubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) = 0;

        //**********************************************************************
        // Method: RenderLoop
        // Defines the main entry point for our renderer
        //
        // Parameters:
        // renderCallback - render callback
        // prePresentCallback - callback called right before Present()
        //**********************************************************************
        virtual void RenderLoop(
            std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback
            ) = 0;

        //**********************************************************************
        // Method: RenderFrame
        // Renders the next frame
        //
        // Parameters:
        // renderCallback - callback for rendering custom objects
        // prePresentCallback - callback called right before Present()
        //**********************************************************************
        virtual void RenderFrame(
            std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback
            ) = 0;

        //**********************************************************************
        // Method: GetBackBufferWidth
        // Returns the width in pixels of the back buffer
        //**********************************************************************
        virtual uint32 GetBackBufferWidth() = 0;

        //**********************************************************************
        // Method: GetBackBufferHeight
        // Returns the height in pixels of the back buffer
        //**********************************************************************
        virtual uint32 GetBackBufferHeight() = 0;

        //**********************************************************************
        // Method: GetBackBuffer
        // Returns the texture that contains the back buffer
        //**********************************************************************
        virtual CComPtr<ID3D11Texture2D> GetBackBuffer() = 0;

        //**********************************************************************
        // Method: CopyFrameBackBuffer
        // Copies the back buffer into an IImage (CPU based image)
        //
        // Parameters:
        // pImage - Image to copy to
        //**********************************************************************
        virtual void CopyFrameBackBuffer(IImage* pImage) = 0;
        
        //**********************************************************************
        // Method: AddPointLight
        // Adds a point light which the renderer uses
        //
        // Parameters:
        // pLight - light to render meshes with
        //**********************************************************************
        virtual void AddPointLight(IPointLight *pLight) = 0;

        //**********************************************************************
        // Method: GetRenderCtx
        // Returns the current render context
        //**********************************************************************
        virtual CRefObj<IRenderCtx> GetRenderCtx() = 0;

        //**********************************************************************
        // Method: ClearDepth
        // Erases the depth buffer
        //**********************************************************************
        virtual void ClearDepth() = 0;

        //**********************************************************************
        // Method: DrawLine
        // Renders a line
        //
        // Parameters:
        // p1 - starting point of line
        // p2 - ending point of line
        // clr - color to use while rendering line
        //**********************************************************************
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) = 0;

        //**********************************************************************
        // Method: BeginShadowmapPass
        // Setups rendering for the shadow map pass
        //
        // Parameters:
        // whichShadowmap - constant indicating which shadow map to use (c_HiResShadow, ...)
        //**********************************************************************
        virtual void BeginShadowmapPass(int whichShadowmap) = 0;

        //**********************************************************************
        // Method: EndShadowmapPass
        // Finishes the shadow map pass
        //
        // Parameters:
        // whichShadowmap - constant indicating which shadow map to use (c_HiResShadow, ...)
        //**********************************************************************
        virtual void EndShadowmapPass(int whichShadowmap) = 0;

        //**********************************************************************
        // Method: PushShadowmapRT
        // Setups up our shadow map as the current render target
        //
        // Parameters:
        // whichShadowmap - constant indicating which shadow map to use (c_HiResShadow, ...)
        // lightPos - position of the light to render from
        // lightDir - direction of the light to render from
        //**********************************************************************
        virtual void PushShadowmapRT(int whichShadowmap, int lightMapIndex, const Vector3& lightPos, const Vector3& lightDir) = 0;

        //**********************************************************************
        // Method: PopShadowmapRT
        // Restores the default render target after PushShadowmapRT
        //**********************************************************************
        virtual void PopShadowmapRT() = 0;

        //**********************************************************************
        // Method: SelectShadowmap
        // Selects which shadow map to use during shading
        //
        // Parameters:
        // whichShadowMap - constant indicating which shadow map to use (c_HiResShadow, ...)
        // lightMapIndex - which light is this for?
        // pShader - shader to use selected shadow map
        //**********************************************************************
        virtual void SelectShadowmap(int whichShadowMap, int lightMapIndex, std::vector<CRefObj<ILight>>& lights, IShader *pShader) = 0;

        //**********************************************************************
        // Method: GetShadowmapTexture
        // Returns the shadow map as a texture
        //
        // Parameters:
        // whichShadowmap - constant indicating which shadow map to use (c_HiResShadow, ...)
        //**********************************************************************
        virtual CRefObj<ITexture> GetShadowmapTexture(int whichShadowMap) = 0;

        //**********************************************************************
        // Method: SetFinalRenderTarget
        // Sets the final render target. This allows for the final output to
        // be something other than the default window.
        //
        // Parameters:
        // pTexture - Texture to use as final render target
        //**********************************************************************
        virtual void SetFinalRenderTarget(ID3D11Texture2D* pTexture) = 0;

        //**********************************************************************
        // Method: SetFinalRenderTargetUsingSharedTexture
        // Sets the final render target. This method is only intended to be used
        // by the WPF interop layer. It will assign the final render target using
        // the shared textured that D3D11Image returns.
        //
        // Parameters:
        // pTexture - Shared texture from D3D11Image (WPF Interop layer) to use
        // as final render target
        //**********************************************************************
        virtual void SetFinalRenderTargetUsingSharedTexture(IUnknown* pTexture) = 0;
    };

    //**********************************************************************
    // Function: CreateRenderer
    // Creates a renderer
    //
    // Parameters:
    // hwnd - window to attach renderer to
    // shaderFolder - path to directory containing shaders
    // startFrozen - should renderer be started in a frozen state?
    // desktopIndex - index of desktop to use with duplication service
    //
    // Returns:
    // Returns the created renderer
    //
    // Module:
    // {Link:import Rendering.Caustic.IRenderer;{Rendering/Caustic/IRenderer.ixx}}
    //**********************************************************************
    CRefObj<IRenderer> CreateRenderer(HWND hwnd, BBox2 &viewport, std::wstring& shaderFolder, bool startFrozen = false, int desktopIndex = 0)
    {
        extern CRefObj<IRenderer> CreateRendererInternal(HWND hwnd, BBox2 &viewport, std::wstring & shaderFolder, bool startFrozen = false, int desktopIndex = 0);
        return CreateRendererInternal(hwnd, viewport, shaderFolder, startFrozen, desktopIndex);
    }
}
