//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Geometry\Mesh\Mesh.h"
#include "Rendering\Caustic\Graphics.h"
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <any>

//!**********************************************************************
//! This file defines the published interface for talking to the Caustic Renderer
//!**********************************************************************

namespace Caustic
{
    //**********************************************************************
    //! \interface ITexture
    //! \brief ITexture defines how clients interact with textures
    //**********************************************************************
    struct ITexture : public IRefCount
    {
        virtual uint32 GetWidth() = 0; //!< Returns the width of the texture
        virtual uint32 GetHeight() = 0; //!< Returns the height of the texture
        virtual DXGI_FORMAT GetFormat() = 0; //!< Returns the DirectX format of the texture
        virtual void Update(IGraphics *pGraphics) = 0; //!< Called to update texture
        virtual CComPtr<ID3D11Texture2D> GetD3DTexture() = 0;
        virtual CComPtr<ID3D11ShaderResourceView> GetD3DTextureRV() = 0;
        virtual void GenerateMips(IGraphics *pGraphics) = 0;
    };

    CAUSTICAPI void CausticSetup();
    CAUSTICAPI void CausticShutdown();
    CAUSTICAPI void CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture);
    CAUSTICAPI CRefObj<ITexture> CheckboardTexture(IGraphics *pGraphics);
    CAUSTICAPI void LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);
    CAUSTICAPI void LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);

    //**********************************************************************
    struct ISampler : public IRefCount
    {
        virtual D3D11_FILTER GetFilter() = 0;
        virtual void SetFilter(D3D11_FILTER filter) = 0;
        virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressU() = 0;
        virtual void SetAddressU(D3D11_TEXTURE_ADDRESS_MODE filter) = 0;
        virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressV() = 0;
        virtual void SetAddressV(D3D11_TEXTURE_ADDRESS_MODE filter) = 0;
        virtual void Render(IGraphics *pGraphics, int slot) = 0;
        virtual void GetTexture(ITexture **ppTexture) = 0;
    };
    CAUSTICAPI void CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler);
    
    //**********************************************************************
    // \brief ICamera defines our camera object
    //**********************************************************************
    struct ICamera : public ISerialize
    {
        virtual void SetParams(float fov, float aspectRatio, float nearZ, float farZ) = 0;
        virtual void SetPosition(Vector3 &eye, Vector3 &look, Vector3 &up) = 0;
        virtual void GetPosition(Vector3 *pEye, Vector3 *pLook, Vector3 *pUp, Vector3 *pU, Vector3 *pV, Vector3 *pN) = 0;
        virtual void SetOffset(Vector3 &offset) = 0;
        virtual void GetOffset(Vector3 &offset) = 0;
        virtual DirectX::XMMATRIX GetProjection() = 0;
        virtual DirectX::XMMATRIX GetView() = 0;
        virtual float GetNear() = 0;
        virtual float GetFar() = 0;
        virtual void GetUVN(Vector3 *u, Vector3 *v, Vector3 *n) = 0;
    };
    CAUSTICAPI void CreateCamera(bool leftHanded, ICamera **ppCamera);

    //**********************************************************************
    // \brief Defines available constraints for the trackball object
    //**********************************************************************
    enum ETrackballConstrain
    {
        Constraint_None,
        Constraint_XAxis,
        Constraint_YAxis,
        Constraint_ZAxis,
    };

    //**********************************************************************
    // \brief ITrackball defines the interface to our trackball implementation
    //**********************************************************************
    struct ITrackball : public IRefCount
    {
        virtual void BeginTracking(int screenX, int screenY, int screenW, int screenH) = 0;
        virtual bool UpdateTracking(int screenX, int screenY, ETrackballConstrain constraint, DirectX::XMMATRIX *pMatrix) = 0;
        virtual void EndTracking() = 0;
    };
    CAUSTICAPI void CreateTrackball(ITrackball **ppTrackball);

    struct IShader;

    struct IPointLight : public IRefCount
    {
        virtual void SetPosition(Vector3 &pos) = 0;
        virtual Vector3 GetPosition() = 0;
        virtual void SetColor(Vector3 &color) = 0;
        virtual Vector3 GetColor() = 0;
    };
    CAUSTICAPI void CreatePointLight(Vector3 &pos, IPointLight **ppLight);

    namespace RenderCtxFlags
    {
        const uint32 c_DisplayNormalsAsColors = 0x01;
        const uint32 c_DisplayNormalsAsLines = 0x02;
        const uint32 c_DisplayFaceNormals = 0x04;
        const uint32 c_DisplayWireframe = 0x08;
        const uint32 c_DisplayWorldAxis = 0x10;
        const uint32 c_DisplayUVsAsColors = 0x20;
        const uint32 c_DisplayLightDir = 0x40;
        const uint32 c_DisplayGrid = 0x80;
    }

    //**********************************************************************
    //! \brief Defines the main interface for setting various features in
    //! the renderer
    //**********************************************************************
    struct IRenderCtx : public IRefCount
    {
        virtual void SetDebugFlags(uint32 flags) = 0;
        virtual uint32 GetDebugFlags() = 0;
        virtual void SetNormalScale(float normalScale) = 0;
        virtual float GetNormalScale() = 0;
        virtual uint32 GetCurrentPass() = 0;
        virtual bool PassBlendable() = 0;
    };

    struct ISceneGraph;

    //**********************************************************************
    //! \brief IRenderer is the interface clients use to talk to the renderer
    //**********************************************************************
    struct IRenderer : public IGraphics
    {
        virtual void Setup(HWND hwnd, bool createDebugDevice) = 0;
        virtual void DrawMesh(ISubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) = 0; ///< Draws a mesh
        virtual void RenderLoop() = 0; ///< Renderer entry point
        virtual void RenderFrame() = 0; ///< Have renderer draw and present next frame
        virtual void SetCamera(ICamera *pCamera) = 0; ///< Sets camera
        virtual void AddPointLight(IPointLight *pLight) = 0;
        virtual void GetRenderCtx(IRenderCtx **ppCtx) = 0;
        virtual void SetSceneGraph(ISceneGraph *pSceneGraph) = 0;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) = 0;
        virtual void GetGraphics(IGraphics **ppGraphics) = 0;
    };
    
    //! \brief CreateRenderer creates a renderer
    CAUSTICAPI void CreateRenderer(HWND hwnd, IRenderer **ppRenderer);
    CAUSTICAPI void CreateGraphics(HWND hwnd, IGraphics **ppGraphics);
    
    //! \brief Creates a renderer that runs on its own thread and method calls
    //! are marshalled from the client to render thread.
    //! renderer is on a separate thread where the client is calling from)
    struct IRendererMarshaller : public IRefCount
    {
        virtual void Initialize(HWND hwnd) = 0;
        virtual void Shutdown() = 0;
        virtual void SetMaxCmdLength() = 0;
        virtual void GetRenderer(IRenderer **ppRenderer) = 0;
        virtual void LoadTexture(const wchar_t *pPath, ITexture **ppTexture) = 0;
        virtual void LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture) = 0;
        virtual void SetSceneGraph(ISceneGraph *pSceneGraph) = 0;
        virtual void SaveScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) = 0;
        virtual void LoadScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) = 0;
    };
    CAUSTICAPI void CreateRendererMarshaller(IRendererMarshaller **ppMarshaller);

    //! \brief MeshToD3D converts an IMesh into a renderable form
    CAUSTICAPI void MeshToD3D(IGraphics *pGraphics, ISubMesh *pMesh,
        int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
        int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
        BBox3 *pBbox, uint32 *pVertexSize);
    CAUSTICAPI void MeshToNormals(IGraphics *pGraphics, ISubMesh *pSubMesh,
        ID3D11Buffer **ppVB, uint32 *pNumVerts);
    CAUSTICAPI void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion);
    CAUSTICAPI void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion);

    //**********************************************************************
    //! \brief IShader defines the public interface for uisng CShader
    //**********************************************************************
    struct IShader : public IRefCount
    {
        virtual std::wstring &Name() = 0; ///< Returns the name of this shader
        virtual void BeginRender(IGraphics *pGraphics, DirectX::XMMATRIX *pWorld = nullptr) = 0; ///< Called before rendering. This method sets the device with the shader
        virtual void SetPSParam(std::wstring paramName, std::any &value) = 0; ///< Sets a pixel shader parameter
        virtual void SetVSParam(std::wstring paramName, std::any &value) = 0; ///< Sets a vertex shader parameter
        virtual void SetPSParam(std::wstring paramName, int index, std::any &value) = 0; ///< Sets a pixel shader array member parameter
        virtual void SetVSParam(std::wstring paramName, int index, std::any &value) = 0; ///< Sets a vertex shader array member parameter
        virtual void EndRender(IGraphics *pGraphics) = 0; ///< Called after the rendering using the shader has completed
    };

    //**********************************************************************
    //! \brief IRenderMaterial defines a material that has its associated
    //! render element (as opposed to an IMaterialAttrib which only defines
    //! the attributes of the material).
    //**********************************************************************
    struct IRenderMaterial : public IRefCount
    {
        virtual void SetShader(IShader *pShader) = 0;
        virtual void GetShader(IShader **ppShader) = 0;
        virtual void SetMaterial(IMaterialAttrib *pMaterial) = 0;
        virtual void GetMaterial(IMaterialAttrib **ppMaterial) = 0;
        virtual void SetDiffuseTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;
        virtual void SetSpecularTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;
        virtual void SetAmbientTexture(IGraphics *pGraphics, ITexture *pTexture) = 0;
        virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) = 0;
    };
    CAUSTICAPI void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial);

    ///**********************************************************************
    ///! \brief IShaderMgr manages shaders within the renderer
    ///**********************************************************************
    struct IShaderMgr : public IRefCount
    {
        virtual void FindShader(const wchar_t *pShaderName, IShader **ppShader) = 0;
        virtual void RegisterShader(const wchar_t *pShaderName, IShader *pShader) = 0;
    };

    struct IRenderable : public IRefCount
    {
        virtual Vector3 GetPos() = 0;
        virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx) = 0;
        virtual void SetTransform(DirectX::XMMATRIX &mat) = 0;
        virtual DirectX::XMMATRIX &GetTransform() = 0;
        virtual bool InPass(int pass) = 0;
    };
    CAUSTICAPI void CreateRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, IShader *pShader, IRenderable **ppRenderable);
    CAUSTICAPI void CreateRenderable(IRenderable **ppRenderable);
    CAUSTICAPI void CreateRenderable(ID3D11Buffer *pVB, uint32 numVertices, ID3D11Buffer *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable);
}
