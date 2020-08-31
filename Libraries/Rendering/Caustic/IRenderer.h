//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Geometry\Mesh\IMesh.h"
#include <functional>

//**********************************************************************
// File: IRenderer.h
// This file defines the published interface for the renderer.
//**********************************************************************

namespace Caustic
{
    struct ISceneGraph;
    struct IRenderSubMesh;
    struct ITexture;
    struct ITexture;
    struct IRenderCtx;
    struct IPointLight;
    struct IShader;
    struct ICamera;

    //**********************************************************************
    // Struct: MeshData
    // Defines class for holding mesh data (i.e. vertices and indices)
    //
    // Members:
    // m_spVB - The vertex buffer
    // m_spIB - The index buffer (maybe null)
    // m_vertexSize - Size in bytes of each vertex
    // m_numVertices - Number of vertices
    // m_numIndices - Number of indices. Maybe 0
    //
    // Header:
    // [Link:Rendering/Caustic/IRenderer.h]
    //**********************************************************************
    struct MeshData
    {
        CComPtr<ID3D11Buffer> m_spVB;
        CComPtr<ID3D11Buffer> m_spIB;
        uint32 m_vertexSize;
        uint32 m_numVertices;
        uint32 m_numIndices;
        BBox3 m_bbox;

        MeshData() :
            m_vertexSize(0),
            m_numVertices(0),
            m_numIndices(0)
        {
        }
    };

    //**********************************************************************
    // Group: Pass Flags
    // c_PassFirst - first pass
    // c_PassObjID - renders pass writing object IDs to output texture
    // c_PassShadow - pass that renders shadow maps
    // c_PassOpaque - pass rendering the opaque objects
    // c_PassTransparent - pass rendering transparent objects
    // c_PassEnvironment - pass rendering environment maps
    // c_PassLast - last pass
    // c_PassAllPasses - combination of all the pass flags
    //
    // Group: Shadow map selection
    // c_HiResShadowMap - selects the hi-res shadow map
    // c_MidResShadowMap - selects the mid-res shadow map
    // c_LowResShadowMap - selects the low-res shadow map
    //
    // Header:
    // [Link:Rendering/Caustic/Renderer.h]
    //**********************************************************************
    const int c_PassFirst = 0;
    const int c_PassObjID = 0;
    const int c_PassShadow = 1;
    const int c_PassOpaque = 2;
    const int c_PassTransparent = 3;
    const int c_PassEnvironment = 4;
    const int c_PassLast = c_PassEnvironment;
    const int c_PassAllPasses = (1 << c_PassLast) - 1;

    // Each shadow map will support up to 16 lights in a single map.
    // For instance, the hires map is 8096x8096. We divide this map
    // into 16 2048x2048 shadow maps.
    const int c_MaxLights = 4;
    const int c_HiResShadowMap = 0;
    const int c_MidResShadowMap = 1;
    const int c_LowResShadowMap = 2;
    const int c_MaxShadowMaps = 3; // Hi-res, Mid-res, Low-res

    //**********************************************************************
    // Interface: IRenderer 
    // Defines our basic renderer. IRenderer handles all rendering commands.
    // It is generally expected that this object is running on its own thread.
    //
    // Header:
    // [Link:Rendering/Caustic/IRenderer.h]
    //**********************************************************************
    struct IRenderer : public IRefCount
    {
        //**********************************************************************
        // Method: GetDevice
        // Returns:
        // The underlying D3D11 device
        //**********************************************************************
        virtual CComPtr<ID3D11Device> GetDevice() = 0;

        //**********************************************************************
        // Method: GetContext
        // Returns:
        // The underlying D3D11 device context
        //**********************************************************************
        virtual CComPtr<ID3D11DeviceContext> GetContext() = 0;

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
        // shaderFolder - path to shader folder
        // createDebugDevice - True if application wants the debug D3D device. False otherwise.
        //**********************************************************************
        virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) = 0;

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
        //**********************************************************************
        virtual void RenderLoop(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) = 0;

        //**********************************************************************
        // Method: RenderFrame
        // Renders the next frame
        //
        // Parameters:
        // renderCallback - callback for rendering custom objects
        //**********************************************************************
        virtual void RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) = 0;

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
        // Method: PushShadowmapRT
        // Setups up our shadow map as the current render target
        //
        // Parameters:
        // whichShadowmap - constant indicating which shadow map to use (c_HiResShadow, ...)
        // lightPos - position of the light to render from
        // lightDir - direction of the light to render from
        //**********************************************************************
        virtual void PushShadowmapRT(int whichShadowmap, int lightMapIndex, Vector3& lightPos, Vector3& lightDir) = 0;

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
    };

    //**********************************************************************
    // Function: CreateRenderer
    // Creates a renderer
    //
    // Parameters:
    // hwnd - window to attach renderer to
    // shaderFolder - path to directory containing shaders
    //
    // Returns:
    // Returns the created renderer
    //
    // Header:
    // [Link:Rendering/Caustic/IRenderer.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<IRenderer> CreateRenderer(HWND hwnd, std::wstring& shaderFolder);
}
