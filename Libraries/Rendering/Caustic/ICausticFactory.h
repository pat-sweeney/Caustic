//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once

//**********************************************************************
// File: ICausticFactory.h
// Primary interface through which clients talk to the Caustic factory.
//**********************************************************************

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Geometry\Mesh\Mesh.h"
#include "Rendering\Caustic\Caustic.h"
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <any>

// Namespace: Caustic
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
		virtual void CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture) = 0;

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

        virtual void CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
            ID3DBlob *pVertexShaderBlob, ID3DBlob *pPixelShaderBlob, IShaderInfo *pShaderInfo,
            IShader **ppShader) = 0;
        virtual void CreateShaderInfo(const wchar_t *pFilename, IShaderInfo **ppShaderInfo) = 0;
    };

    //**********************************************************************
    // Function: CreateCausticFactory
    // Function used by clients to create a Caustic factory
    //**********************************************************************
    CAUSTICAPI void CreateCausticFactory(ICausticFactory **ppFactory);
}
