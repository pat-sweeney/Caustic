//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Geometry\Mesh\Mesh.h"
#include "Imaging\Image\Image.h"
#include "Rendering\Caustic\Caustic.h"
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <any>

//**********************************************************************
// File: ICausticFactory.h
// Primary interface through which clients talk to the Caustic factory.
//**********************************************************************

namespace Caustic
{
	//**********************************************************************
	// Interface: ICausticFactory
	// Defines the main factory for creating Caustic objects
	//
	// Header:
	// [Link:Rendering/Caustic/ICausticFactory.h]
	//**********************************************************************
	struct ICausticFactory : public IRefCount
	{
		//**********************************************************************
		// Method: CreateRenderer 
		// Creates our basic renderer object to be used by the client
		//
		// Parameters:
		// hwnd - HWND to attach D3D renderer to
		//
		// Returns:
		// Returns the created renderer
		//
		// CreateRenderer creates the renderer object that the client will use to
		// talk to the renderer. This object runs on the clients thread and acts
		// only as a proxy for marshalling commands+data over to the renderer thread.
		//**********************************************************************
		virtual CRefObj<IRenderer> CreateRenderer(HWND hwnd, std::wstring &shaderFolder) = 0;

		//**********************************************************************
		// Method: CreateGraphics
		// Creates a wrapper around our D3D device.
		//
		// Paramters:
		// hwnd - HWND to attach D3D renderer to
		//
		// Returns:
		// Returns the graphics device
		//**********************************************************************
		virtual CRefObj<IGraphics> CreateGraphics(HWND hwnd) = 0;

        //**********************************************************************
		// Method: CreateRenderMesh
		// Creates a render mesh object
		//
		// Returns:
		// Returns the created render mesh
		//**********************************************************************
		virtual CRefObj<IRenderMesh> CreateRenderMesh() = 0;

		//**********************************************************************
		// Method: CreateRenderSubMesh
		// Creates a render submesh object
		//
		// Returns:
		// Returns the created render submesh
		//**********************************************************************
		virtual CRefObj<IRenderSubMesh> CreateRenderSubMesh() = 0;

		//**********************************************************************
		// Method: CreatePointLight
		// Creates a point light source
		//
		// Parameters:
		// pos - position of light source in world coordinates
		//
		// Returns:
		// Returns the point light source
		//**********************************************************************
		virtual CRefObj<IPointLight> CreatePointLight(Vector3& pos, FRGBColor& color, float intensity) = 0;

		//**********************************************************************
		// Method: CreateSpotLight
		// Creates a spot light
		//
		// Parameters:
		// pos - position of light source in world coordinates
		// dir - direction light is pointing 
		// color - color of light
		// intensity - light's intensity
		// innerAngle - inner angle defining where light is full intensity
		// outerAngle - outer angle. Light falls off from full to no intensity between inner and outer angle
		// casts - does light participate in shadow mapping?
		//
		// Returns:
		// Returns the spotlight
		//**********************************************************************
		virtual CRefObj<ISpotLight> CreateSpotLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity = 1000.0f, float innerAngle = 30.0f, float outerAngle = 45.0f, bool casts = true) = 0;

		//**********************************************************************
		// Method: CreateDirectionalLight
		// Creates a directional light source
		//
		// Parameters:
		// pos - position of light source in world coordinates
		//
		// Returns:
		// Returns the point light source
		//**********************************************************************
		virtual CRefObj<IDirectionalLight> CreateDirectionalLight(Vector3& pos, FRGBColor& color, float intensity) = 0;

		//**********************************************************************
		// Method: CreateTrackball
		// Creates a new trackball object
		//
		// Returns:
		// Returns the created trackball
		//**********************************************************************
		virtual CRefObj<ITrackball> CreateTrackball() = 0;

		//**********************************************************************
		// Method: CreateRendererMarshaller
		// Creates the marshalled version of the renderer. This renderer marshals
		// all calls over to the render thread (i.e. can be called on any thread)
		//
		// Returns:
		// Returns the created marshalled renderer
		//**********************************************************************
		virtual CRefObj<IRendererMarshaller> CreateRendererMarshaller() = 0;

		//**********************************************************************
		// Method: CreateMaterialAttrib
		// Creates a MaterialAttrib object.
		//
		// Returns:
		// Returns the created MaterialAttrib object
		//**********************************************************************
		virtual CRefObj<IMaterialAttrib> CreateMaterialAttrib() = 0;

		//**********************************************************************
		// Method: CreateRenderMaterial
		// Creates a RenderMaterial object.
		//
		// Parameters:
		// pGraphics - Graphics device
		// pMaterialAttrib - Material definition
		// pShader - Vertex+Pixel shader
		//
		// Returns:
		// Returns the created RenderMaterial object
		//**********************************************************************
		virtual CRefObj<IRenderMaterial> CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader) = 0;
		
		//**********************************************************************
		// Method: CreateRenderable
		// Creates a renderable object. A Renderable is a mesh+material+shader.
		//
		// Parameters:
		// pSubMesh - mesh object
        // pFrontMaterial - Material for front faces
        // pBackMaterial - Material for back faces
        // mat - Matrix to apply
		//
		// Returns:
        // Returns the created Renderable object
		//**********************************************************************
        virtual CRefObj<IRenderable> CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat) = 0;

		//**********************************************************************
		// Method: CreateSampler
		// Creates a new sampler. Samplers defined how shaders read from textures.
		//
		// Parameters:
		// pGraphics - graphics device
		// pTexture - texture to associate with sampler
		//
		// Returns:
		// Returns the newly create sampler
		//**********************************************************************
		virtual CRefObj<ISampler> CreateSampler(IGraphics *pGraphics, ITexture *pTexture) = 0;

		//**********************************************************************
		// Function: CreateCamera
		// Creates a new camera object. This camera can then
		// be passed to IRenderer to be used for rendering.
		//
		// Parameters:
		// leftHanded - Defines whether we have a left or right handed coordinate system
		//
		// Returns:
		// Returns the newly created camera object
		//**********************************************************************
		virtual CRefObj<ICamera> CreateCamera(bool leftHanded) = 0;

		//**********************************************************************
		// Method: CreateTexture
		// Creates a new texture
		//
		// Parameters:
		// pGraphics - graphics device
		// width - width of texture in pixels
		// height - height of texture in pixels
		// format - pixel format for the texture
		// cpuFlags - DirectX cpu flags
		// bindFlags - DirectX bind flags
		//
		// Returns:
		// Returns the newly created texture
		//**********************************************************************
		virtual CRefObj<ITexture> CreateTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags) = 0;

		//**********************************************************************
		// Method: CreateTexture
		// Creates a new texture
		//
		// Parameters:
		// pGraphics - graphics device
		// pImage - image data to set texture to
		// cpuFlags - DirectX cpu flags
		// bindFlags - DirectX bind flags
		//
		// Returns:
		// Returns the newly created texture
		//**********************************************************************
		virtual CRefObj<ITexture> CreateTexture(IGraphics* pGraphics, IImage *pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags) = 0;

		//**********************************************************************
		// Method: CheckerboardTexture
		// Creates a texture with a checkerboard pattern
		//
		// Parameters:
		// pGraphics - graphics device
		//**********************************************************************
		virtual CRefObj<ITexture> CheckerboardTexture(IGraphics *pGraphics) = 0;

		//**********************************************************************
		// Method: LoadTexture
		// Loads a texture from disk
		//
		// Parameters:
		// pFilename - filename of the image file
		// pGraphics - graphics device
		//
		// Returns:
		// Returns the newly created texture
		//**********************************************************************
		virtual CRefObj<ITexture> LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics) = 0;

		//**********************************************************************
		// Method: LoadVideoTexture
		// Loads a video from disk
		//
		// Parameters:
		// pFilename - filename of the image file
		// pGraphics - graphics device
		//
		// Returns:
		// Returns the newly created texture
		//**********************************************************************
		virtual CRefObj<ITexture> LoadVideoTexture(const wchar_t* pFilename, IGraphics* pGraphics) = 0;

		//**********************************************************************
		// Method: VideoTextureFromWebcam
		// Loads a video texture using a webcam as the source
		//
		// Parameters:
		// pGraphics - graphics device
		//
		// Returns:
		// Returns the newly created texture
		//**********************************************************************
		virtual CRefObj<ITexture> VideoTextureFromWebcam(IGraphics* pGraphics) = 0;

		//**********************************************************************
        // Method: CreateShader
        // Creates a shader
        //
        // Parameters:
        // pRenderer - renderer
        // pShaderName - name of shader
        // pVertexShaderBlob - blob containing the compiled vertex shader
        // pPixelShaderBlob - blob containing the compiled pixel shader
        // pShaderInfo - info about the shader (from the .shi file produced by ParseShader)
		//
		// Returns:
        // Returns the created shader
        //**********************************************************************
        virtual CRefObj<IShader> CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
            ID3DBlob *pVertexShaderBlob, ID3DBlob* pPixelShaderBlob, ID3DBlob* pComputeShaderBlob,
			IShaderInfo *pShaderInfo) = 0;

        //**********************************************************************
        // Method: CreateShaderInfo
        // Loads the shader info associated with a shader
        //
        // Parameters:
        // pFilename - path to .shi file
		//
		// Returns:
        // Returns the loaded shader info
        //**********************************************************************
        virtual CRefObj<IShaderInfo> CreateShaderInfo(const wchar_t *pFilename) = 0;
    };

    //**********************************************************************
    // Function: CreateCausticFactory
    // Function used by clients to create a Caustic factory
	//
	// Header:
	// [Link:Rendering/Caustic/ICausticFactory.h]
	//**********************************************************************
    CAUSTICAPI CRefObj<ICausticFactory> CreateCausticFactory();
}
