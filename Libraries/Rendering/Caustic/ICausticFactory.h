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
	//**********************************************************************
	struct ICausticFactory : public IRefCount
	{
		//**********************************************************************
		// Method: CreateRenderer 
		// Creates our basic renderer object to be used by the client
		//
		// Parameters:
		// hwnd - HWND to attach D3D renderer to
		// ppRenderer - Returns the created renderer
		//
		// CreateRenderer creates the renderer object that the client will use to
		// talk to the renderer. This object runs on the clients thread and acts
		// only as a proxy for marshalling commands+data over to the renderer thread.
		//**********************************************************************
		virtual void CreateRenderer(HWND hwnd, std::wstring &shaderFolder, IRenderer **ppRenderer) = 0;

		//**********************************************************************
		// Method: CreateGraphics
		// Creates a wrapper around our D3D device.
		//
		// Paramters:
		// hwnd - HWND to attach D3D renderer to
		// ppGraphics - Returns the graphics device
		//**********************************************************************
		virtual void CreateGraphics(HWND hwnd, IGraphics **ppGraphics) = 0;

        //**********************************************************************
		// Method: CreateRenderMesh
		// Creates a render mesh object
		//
		// Parameters:
		// ppRenderMesh - Returns the created render mesh
		//**********************************************************************
		virtual void CreateRenderMesh(IRenderMesh **ppRenderMesh) = 0;

		//**********************************************************************
		// Method: CreateRenderSubMesh
		// Creates a render submesh object
		//
		// Parameters:
		// ppRenderSubMesh - Returns the created render submesh
		//**********************************************************************
		virtual void CreateRenderSubMesh(IRenderSubMesh **ppRenderSubMesh) = 0;

		//**********************************************************************
		// Method: CreatePointLight
		// Creates a point light source
		//
		// Parameters:
		// pos - position of light source in world coordinates
		// ppLight - Returns the point light source
		//**********************************************************************
		virtual void CreatePointLight(Vector3 &pos, IPointLight **ppLight) = 0;

		//**********************************************************************
		// Method: CreateTrackball
		// Creates a new trackball object
		//
		// Parameters:
		// ppTrackball - Returns the created trackball
		//**********************************************************************
		virtual void CreateTrackball(ITrackball **ppTrackball) = 0;

		//**********************************************************************
		// Method: CreateRendererMarshaller
		// Creates the marshalled version of the renderer. This renderer marshals
		// all calls over to the render thread (i.e. can be called on any thread)
		//
		// Parameters:
		// ppMarshaller - Returns the created marshalled renderer
		//**********************************************************************
		virtual void CreateRendererMarshaller(IRendererMarshaller **ppMarshaller) = 0;

		//**********************************************************************
		// Method: CreateMaterialAttrib
		// Creates a MaterialAttrib object.
		//
		// Parameters:
		// ppMaterialAttrib - Returns the created MaterialAttrib object
		//**********************************************************************
		virtual void CreateMaterialAttrib(IMaterialAttrib** ppMaterialAttrib) = 0;

		//**********************************************************************
		// Method: CreateRenderMaterial
		// Creates a RenderMaterial object.
		//
		// Parameters:
		// pGraphics - Graphics device
		// pMaterialAttrib - Material definition
		// pShader - Vertex+Pixel shader
		// ppRenderMaterial - Returns the created RenderMaterial object
		//**********************************************************************
		virtual void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial) = 0;
		
		//**********************************************************************
		// Method: CreateRenderable
		// Creates a renderable object. A Renderable is a mesh+material+shader.
		//
		// Parameters:
		// pSubMesh - mesh object
        // pFrontMaterial - Material for front faces
        // pBackMaterial - Material for back faces
        // mat - Matrix to apply
        // ppRenderable - Returns the created Renderable object
		//**********************************************************************
        virtual void CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable) = 0;

		//**********************************************************************
		// Method: CreateSampler
		// Creates a new sampler. Samplers defined how shaders read from textures.
		//
		// Parameters:
		// pGraphics - graphics device
		// pTexture - texture to associate with sampler
		// ppSampler - Returns the newly create sampler
		//**********************************************************************
		virtual void CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler) = 0;

		//**********************************************************************
		// Function: CreateCamera
		// Creates a new camera object. This camera can then
		// be passed to IRenderer to be used for rendering.
		//
		// Parameters:
		// leftHanded - Defines whether we have a left or right handed coordinate system
		// ppCamera - Returns the newly created camera object
		//**********************************************************************
		virtual void CreateCamera(bool leftHanded, ICamera **ppCamera) = 0;

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
		// ppTexture - returns the newly created texture
		//**********************************************************************
		virtual void CreateTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags, ITexture** ppTexture) = 0;

		//**********************************************************************
		// Method: CreateTexture
		// Creates a new texture
		//
		// Parameters:
		// pGraphics - graphics device
		// pImage - image data to set texture to
		// cpuFlags - DirectX cpu flags
		// bindFlags - DirectX bind flags
		// ppTexture - returns the newly created texture
		//**********************************************************************
		virtual void CreateTexture(IGraphics* pGraphics, IImage *pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags, ITexture** ppTexture) = 0;

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
		// ppTexture - returns the newly created texture
		//**********************************************************************
		virtual void LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture) = 0;

		//**********************************************************************
		// Method: LoadVideoTexture
		// Loads a video from disk
		//
		// Parameters:
		// pFilename - filename of the image file
		// pGraphics - graphics device
		// ppTexture - returns the newly created texture
		//**********************************************************************
		virtual void LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture) = 0;

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
        // ppShader - Returns the created shader
        //**********************************************************************
        virtual void CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
            ID3DBlob *pVertexShaderBlob, ID3DBlob* pPixelShaderBlob, ID3DBlob* pComputeShaderBlob,
			IShaderInfo *pShaderInfo, IShader **ppShader) = 0;

        //**********************************************************************
        // Method: CreateShaderInfo
        // Loads the shader info associated with a shader
        //
        // Parameters:
        // pFilename - path to .shi file
        // ppShaderInfo - Returns the loaded shader info
        //**********************************************************************
        virtual void CreateShaderInfo(const wchar_t *pFilename, IShaderInfo **ppShaderInfo) = 0;
    };

    //**********************************************************************
    // Function: CreateCausticFactory
    // Function used by clients to create a Caustic factory
    //**********************************************************************
    CAUSTICAPI void CreateCausticFactory(ICausticFactory **ppFactory);
}
