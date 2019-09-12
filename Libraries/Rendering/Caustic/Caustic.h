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

//**********************************************************************
// This file defines the published interface for talking to the Caustic Renderer
//**********************************************************************

// Namespace: Caustic
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
		// Returns the width of the texture in pixels
		//**********************************************************************
		virtual uint32 GetWidth() = 0;

		//**********************************************************************
		// Method: GetHeight
		// Returns the height of the texture in pixels
		//**********************************************************************
		virtual uint32 GetHeight() = 0;

		//**********************************************************************
		// Method: GetFormat
		// Returns the pixel format of the texture
		//**********************************************************************
		virtual DXGI_FORMAT GetFormat() = 0; //!< Returns the DirectX format of the texture

		//**********************************************************************
		// Method: Update
		// Updates the underlying graphics card texture. Primarily used for
		// video textures.
		//
		// Parameters:
		// pGraphics - graphics device
		//**********************************************************************
		virtual void Update(IGraphics *pGraphics) = 0; //!< Called to update texture

		//**********************************************************************
		// Method: GetD3DTexture
		// Returns the underlying D3D texture
		//**********************************************************************
		virtual CComPtr<ID3D11Texture2D> GetD3DTexture() = 0;

		//**********************************************************************
		// Method: GetD3DTextureRV
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
    };

	//**********************************************************************
	// Interface: ISampler
	// Defines a sampler. Samplers define how the underlying texture is accessed by shaders.
	//**********************************************************************
	struct ISampler : public IRefCount
    {
		//**********************************************************************
		// Method: GetFilter
		// Returns the current filter state
		//**********************************************************************
		virtual D3D11_FILTER GetFilter() = 0;

		//**********************************************************************
		// Method: SetFilter
		// Set the current filter state
		//
		// Parameters:
		// filter - filter state to set
		//**********************************************************************
		virtual void SetFilter(D3D11_FILTER filter) = 0;

		//**********************************************************************
		// Method: GetAddressU
		// Returns the current addressing mode in the U direction
		//**********************************************************************
		virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressU() = 0;

		//**********************************************************************
		// Method: SetAddressU
		// Set the current addressing mode in the U direction
		//
		// Parameters:
		// mode - addressing mode to set
		//**********************************************************************
		virtual void SetAddressU(D3D11_TEXTURE_ADDRESS_MODE mode) = 0;

		//**********************************************************************
		// Method: GetAddressV
		// Returns the current addressing mode in the V direction
		//**********************************************************************
		virtual D3D11_TEXTURE_ADDRESS_MODE GetAddressV() = 0;

		//**********************************************************************
		// Method: SetAddressU
		// Set the current addressing mode in the V direction
		//
		// Parameters:
		// mode - addressing mode to set
		//**********************************************************************
		virtual void SetAddressV(D3D11_TEXTURE_ADDRESS_MODE mode) = 0;

		//**********************************************************************
		// Method: Render
		// Sets up the sampler for the current rendering
		//
		// Parameters:
		// pGraphics - graphics device
		// slot - texture slot to which this sampler should be assigned
		//**********************************************************************
		virtual void Render(IGraphics *pGraphics, int slot) = 0;

		//**********************************************************************
		// Method: GetTexture
		// Returns the texture associated with this sampler
		//**********************************************************************
		virtual void GetTexture(ITexture **ppTexture) = 0;
    };
	
	//**********************************************************************
    // Interface: ICamera
	// The CCamera object defines our camera. We pass this to our IRenderer
	// to determine where to render from.
	//**********************************************************************
    struct ICamera : public ISerialize
    {
		//**********************************************************************
		// Method: SetParams
		// Sets the camera parameters
		//
		// Parameters:
		// fov - field of view
		// aspectRatio - Camera's aspect ratio
		// nearZ - distance to near plane
		// farZ - distance to far plane
		//**********************************************************************
		virtual void SetParams(float fov, float aspectRatio, float nearZ, float farZ) = 0;

		//**********************************************************************
		// Method: SetPosition
		// Sets the camera's position.
		//
		// Parameters:
		// eye - camera's world position
		// look - camera's look point. Used to define the camera's view direction
		// up - camera's up point. The eye point and up point define the up vector.
		//**********************************************************************
		virtual void SetPosition(Vector3 &eye, Vector3 &look, Vector3 &up) = 0;

		//**********************************************************************
		// Method: GetPosition
		// Gets the camera's position.
		//
		// Parameters:
		// eye - returns the camera's world position. Maybe nullptr.
		// look - returns the camera's look point. Maybe nullptr.
		// up - returns the camera's up point. Maybe nullptr.
		// pU - Returns the camera's X axis. Maybe nullptr.
		// pV - Returns the camera's Y axis. Maybe nullptr.
		// pN - Returns the camera's Z axis. Maybe nullptr.
		//**********************************************************************
		virtual void GetPosition(Vector3 *pEye, Vector3 *pLook, Vector3 *pUp, Vector3 *pU, Vector3 *pV, Vector3 *pN) = 0;

		//**********************************************************************
		// Method: SetOffset
		// Sets an offset that is applied to the camera's eye and look point.
		//
		// Parameters:
		// offset - offset to apply
		//**********************************************************************
		virtual void SetOffset(Vector3 &offset) = 0;

		//**********************************************************************
		// Method: GetOffset
		// Returns the current offset
		//
		// Parameters:
		// offset - Returns the offset that is being applied to the camera's eye and look point
		//**********************************************************************
		virtual void GetOffset(Vector3 &offset) = 0;

		//**********************************************************************
		// Method: GetProjection
		// Returns the camera's projection matrix
		//**********************************************************************
		virtual DirectX::XMMATRIX GetProjection() = 0;

		//**********************************************************************
		// Method: GetView
		// Returns the camera's view matrix
		//**********************************************************************
		virtual DirectX::XMMATRIX GetView() = 0;

		//**********************************************************************
		// Method: GetNear
		// Returns the camera's near plane distance
		//**********************************************************************
		virtual float GetNear() = 0;

		//**********************************************************************
		// Method: GetFar
		// Returns the camera's far plane distance
		//**********************************************************************
		virtual float GetFar() = 0;

		//**********************************************************************
		// Method: GetUVN
		// Returns the camera axis in world coordinates
		//
		// Parameters:
		// u - X axis
		// v - Y axis
		// n - Z axis
		//**********************************************************************
		virtual void GetUVN(Vector3 *u, Vector3 *v, Vector3 *n) = 0;
    };

	//**********************************************************************
	// Class: ETrackballConstraint
	// Defines available constraints for the trackball object
	//
	// Constraint_None - Don't constrain rotations
	// Constraint_XAxis - Constrain rotations to be around X axis
	// Constraint_YAxis - Constrain rotations to be around Y axis
	// Constraint_ZAxis - Constrain rotations to be around Z axis
	//**********************************************************************
	enum ETrackballConstraint
	{
		Constraint_None,
		Constraint_XAxis,
		Constraint_YAxis,
		Constraint_ZAxis,
	};

    //**********************************************************************
    // Interface: ITrackball
	// Defines the interface to our trackball implementation
    //**********************************************************************
    struct ITrackball : public IRefCount
    {
		//**********************************************************************
		// Method: BeginTracking
		// Called to start tracking
		//
		// Parameters:
		// screenX - mouse's X position where tracking starts in pixels
		// screenY - mouse's Y position where tracking starts in pixels
		// screenW - width of the window in pixels
		// screenH - height of the window in pixels
		//**********************************************************************
		virtual void BeginTracking(int screenX, int screenY, int screenW, int screenH) = 0;

		//**********************************************************************
		// Method: UpdateTracking
		// Called when mouse moves during trackball tracking
		//
		// Parameters:
		// screenX - current mouse's X position
		// screenY - current mouse's Y position
		// constraint - current constraint
		// pMatrix - Resulting trackball matrix
		//**********************************************************************
		virtual bool UpdateTracking(int screenX, int screenY, ETrackballConstraint constraint, DirectX::XMMATRIX *pMatrix) = 0;

		//**********************************************************************
		// Method: EndTracking
		// Called when mouse is released during trackball tracking
		//**********************************************************************
		virtual void EndTracking() = 0;
    };

    struct IShader;

	//**********************************************************************
	// Interface: IPointLight
	// Defines a point light
	//**********************************************************************
	struct IPointLight : public IRefCount
    {
        virtual void SetPosition(Vector3 &pos) = 0;
        virtual Vector3 GetPosition() = 0;
        virtual void SetColor(Vector3 &color) = 0;
        virtual Vector3 GetColor() = 0;
    };

	//**********************************************************************
	// Namespace: RenderCtxFlags
	// Defines flags for setting various render modes
	//**********************************************************************
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
    // Interface: IRenderCtx
	// Defines the main interface for setting various features in
    // the renderer
    //**********************************************************************
    struct IRenderCtx : public IRefCount
    {
		//**********************************************************************
		// Method: SetDebugFlags
		// Sets debug flags used for rendering
		//
		// Parameters:
		// flags - new debug flags
		//**********************************************************************
		virtual void SetDebugFlags(uint32 flags) = 0;

		//**********************************************************************
		// Method: GetDebugFlags
		// Returns the current debug flags used for rendering
		//**********************************************************************
		virtual uint32 GetDebugFlags() = 0;

		//**********************************************************************
		// Method: SetNormalScale
		// Sets the scale used to determine how large debug normal vectors are drawn
		//
		// Parameters:
		// normalScale - sets the scale factor for drawing debug normal vectors
		//**********************************************************************
		virtual void SetNormalScale(float normalScale) = 0;

		//**********************************************************************
		// Method: GetNormalScale
		// Returns the scale used for determining how large debug normal vectors are drawn
		//**********************************************************************
		virtual float GetNormalScale() = 0;

		//**********************************************************************
		// Method: GetCurrentPass
		// Returns the current render pass
		//**********************************************************************
		virtual uint32 GetCurrentPass() = 0;

		//**********************************************************************
		// Method: PassBlendable
		// Returns whether the current render pass supports alpha blending
		//**********************************************************************
		virtual bool PassBlendable() = 0;
    };

    struct ISceneGraph;

    //**********************************************************************
    // Interface: IRenderer 
	// Defines the interface clients use to talk to the renderer
    //**********************************************************************
    struct IRenderer : public IGraphics
    {
        virtual void Setup(HWND hwnd, bool createDebugDevice) = 0;
        virtual void DrawMesh(ISubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) = 0; // Draws a mesh
        virtual void RenderLoop() = 0; // Renderer entry point
        virtual void RenderFrame() = 0; // Have renderer draw and present next frame
        virtual void SetCamera(ICamera *pCamera) = 0; // Sets camera
        virtual void AddPointLight(IPointLight *pLight) = 0;
        virtual void GetRenderCtx(IRenderCtx **ppCtx) = 0;
        virtual void SetSceneGraph(ISceneGraph *pSceneGraph) = 0;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) = 0;
        virtual void GetGraphics(IGraphics **ppGraphics) = 0;
    };

	//**********************************************************************
	// Interface: IRendererMarshaller
    // Creates a renderer that runs on its own thread and method calls
    // are marshalled from the client to render thread.
    // renderer is on a separate thread where the client is calling from)
	//**********************************************************************
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

    //**********************************************************************
    // Interface: IShader
	// Defines the public interface for using CShader
    //**********************************************************************
    struct IShader : public IRefCount
    {
        virtual std::wstring &Name() = 0; // Returns the name of this shader
        virtual void BeginRender(IGraphics *pGraphics, DirectX::XMMATRIX *pWorld = nullptr) = 0; // Called before rendering. This method sets the device with the shader
        virtual void SetPSParam(std::wstring paramName, std::any &value) = 0; // Sets a pixel shader parameter
        virtual void SetVSParam(std::wstring paramName, std::any &value) = 0; // Sets a vertex shader parameter
        virtual void SetPSParam(std::wstring paramName, int index, std::any &value) = 0; // Sets a pixel shader array member parameter
        virtual void SetVSParam(std::wstring paramName, int index, std::any &value) = 0; // Sets a vertex shader array member parameter
        virtual void EndRender(IGraphics *pGraphics) = 0; // Called after the rendering using the shader has completed
    };

    //**********************************************************************
    // Interface: IRenderMaterial
	// Defines a material that has its associated
    // render element (as opposed to an IMaterialAttrib which only defines
    // the attributes of the material).
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

    //**********************************************************************
    // Interface: IShaderMgr
	// Manages shaders within the renderer
    //**********************************************************************
    struct IShaderMgr : public IRefCount
    {
		//**********************************************************************
		// Method: FindShader
		// Finds an existing shader by name
		//
		// Parameters:
		// pShaderName - Name of shader to find.
		// ppShader - Returns the found shader. Nullptr if no matching shader is found.
		//**********************************************************************
		virtual void FindShader(const wchar_t *pShaderName, IShader **ppShader) = 0;

		//**********************************************************************
		// Method: RegisterShader
		// Registers a new shader
		//
		// Parameters:
		// pShaderName - Name of shader
		// pShader - Shader to register
		//**********************************************************************
		virtual void RegisterShader(const wchar_t *pShaderName, IShader *pShader) = 0;
    };

	//**********************************************************************
	// Interface: IRenderable
	// Defines the main interface for rendered objects
	//**********************************************************************
	struct IRenderable : public IRefCount
    {
		//**********************************************************************
		// Method: GetPos
		// Returns the world position of the object
		//**********************************************************************
		virtual Vector3 GetPos() = 0;

		//**********************************************************************
		// Method: Render
		// Called by the renderer to render this object
		//
		// Parameters:
		// pGraphics - Graphics device
		// lights - List of current lights effecting this object
		// pRenderCtx - Current render context
		//**********************************************************************
		virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx) = 0;

		//**********************************************************************
		// Method: SetTransform
		// Sets the transform on the object
		//
		// Parameters:
		// mat - transform to set
		//**********************************************************************
		virtual void SetTransform(DirectX::XMMATRIX &mat) = 0;

		//**********************************************************************
		// Method: GetTransform
		// Returns the transforms associated with the object
		//**********************************************************************
		virtual DirectX::XMMATRIX &GetTransform() = 0;

		//**********************************************************************
		// Method: InPass
		// Returns whether the object is part of the specified pass
		//
		// Parameters:
		// pass - render pass
		//
		// Returns:
		// Returns True if object is part of the specified pass. False otherwise.
		//**********************************************************************
		virtual bool InPass(int pass) = 0;
    };
}
#include "Rendering\Caustic\ICausticFactory.h"
