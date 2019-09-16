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
#include "Rendering\Caustic\IGraphics.h"
#include "Rendering\Caustic\Texture.h"
#include "Rendering\Caustic\ISampler.h"
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
		virtual void CreateRenderer(HWND hwnd, IRenderer **ppRenderer) = 0;

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
		// pGraphics - Graphics device
		// pSubMesh - mesh object
		// pMaterial - Material definition
		// pShader - Vertex+Pixel shader
		// ppRenderable - Returns the created Renderable object
		//**********************************************************************
		virtual void CreateRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, IShader *pShader, IRenderable **ppRenderable) = 0;

		//**********************************************************************
		// Method: CreateRenderable
		// Creates a renderable object. A Renderable is a mesh+material+shader.
		//
		// Parameters:
		// ppRenderable - Returns the created Renderable object
		//**********************************************************************
		virtual void CreateRenderable(IRenderable **ppRenderable) = 0;

		//**********************************************************************
		// Method: CreateRenderable
		// Creates a renderable object. A Renderable is a mesh+material+shader.
		//
		// Parameters:
		// pVB - vertex buffer
		// numVertices - number of vertices in vertex buffer
		// pIB - index buffer
		// numIndices - number of indices in index buffer
		// pFrontMaterial - Material to apply to front polygons
		// pBackMaterial - Material to apply to back polygons
		// mat - matrix to apply to mesh
		// ppRenderable - Returns the created Renderable object
		//**********************************************************************
		virtual void CreateRenderable(ID3D11Buffer *pVB, uint32 numVertices, ID3D11Buffer *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable) = 0;

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

		//**********************************************************************
		// Method: MeshToD3D
		// Converts an IMesh into a renderable form
		//
		// Parameters:
		// pGraphics - Graphics device to use
		// pMesh - Mesh to convert
		// vertexVersion - Version of vertex to use
		// ppVertexBuffer - Returns the created vertex buffer
		// indexVersion - Version of index buffer to use
		// ppIndexBuffer - Returns the created index buffer
		// pBbox - Returns the bounding box of the mesh
		// pVertexSize - Returns the size of each vertex in bytes
		//**********************************************************************
		virtual void MeshToD3D(IGraphics *pGraphics, ISubMesh *pMesh,
			int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
			int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
			BBox3 *pBbox, uint32 *pVertexSize) = 0;
		
		virtual void MeshToNormals(IGraphics *pGraphics, ISubMesh *pSubMesh,
			ID3D11Buffer **ppVB, uint32 *pNumVerts) = 0;

		//**********************************************************************
		// Method: StoreSubMeshRenderableDataToStream
		// This function converts an ISubMesh into data that can later be converted into
		// an IRenderable object. This data is then saved to the specified stream. Later
		// the data can be loaded via this function's complementary function (LoadSubMeshRenderableDataFromStream())
		// and then pass the data to BuildIndexBufferGPU/BuildVertexBufferGPU to create an ID3D11Buffer object
		// from which an IRenderable object can be created from.
		//
		// Parameters:
		// pStream - stream to save to
		// pMesh - mesh to store
		//**********************************************************************
		virtual void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion) = 0;
		virtual void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion) = 0;

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
