//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base/Core/IRefCount.h"
#include "Base\Math\Vector.h"
#include <Windows.h>
#include <atlbase.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <any>

// Namespace: Caustic
namespace Caustic
{
	struct IRenderer;
	struct IPointLight;
	struct ITrackball;
	struct IMaterialAttrib;
	struct IRenderMaterial;
	struct ICamera;
	struct ITexture;
	struct IShader;
	struct IRenderable;
	struct ISampler;
	struct IShaderInfo;

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
		// shaderFolder - folder that contains our shaders
		// ppRenderer - Returns the created renderer
		//**********************************************************************
		virtual void CreateRenderer(HWND hwnd, std::wstring &shaderFolder, IRenderer **ppRenderer) = 0;

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
		// Method: CreateRenderMaterial
		// Creates a RenderMaterial object.
		//
		// Parameters:
		// pGraphics - Graphics device
		// pMaterialAttrib - Material definition
		// pShader - Vertex+Pixel shader
		// ppRenderMaterial - Returns the created RenderMaterial object
		//**********************************************************************
		virtual void CreateRenderMaterial(IRenderer *pRenderer, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial) = 0;
		
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
		virtual void CreateRenderable(ID3D12Resource *pVB, uint32 numVertices, ID3D12Resource *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable) = 0;

		//**********************************************************************
		// Method: CreateSampler
		// Creates a new sampler. Samplers defined how shaders read from textures.
		//
		// Parameters:
		// pRenderer - Rendering device
		// pTexture - texture to associate with sampler
		// ppSampler - Returns the newly create sampler
		//**********************************************************************
		virtual void CreateSampler(IRenderer *pRenderer, ITexture *pTexture, ISampler **ppSampler) = 0;

		//**********************************************************************
		// Method: CreateShader
		// Creates a new shader.
		//
		// Parameters:
		// pRenderer - Rendering device
		// pShaderName - name of shader
		// pVertexShaderBlob - binary defining out vertex shader
		// pPixelShaderBlob - binary defining out pixel shader
		// pShaderInfo - Information about the shader
		// ppShader - returns the newly created shader
		//**********************************************************************
		virtual void CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
			ID3DBlob *pVertexShaderBlob, ID3DBlob *pPixelShaderBlob, IShaderInfo *pShaderInfo,
			IShader **ppShader) = 0;
		
		virtual void CreateShaderInfo(const wchar_t *pFilename, IShaderInfo **ppShaderInfo) = 0;

		virtual void CreateMaterial(Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, float specularExp, float alpha, IMaterialAttrib **ppMaterial) = 0;
		virtual void CreateMaterial(IMaterialAttrib **ppMaterial) = 0;

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
		virtual void CreateTexture(IRenderer *pRenderer, uint32 width, uint32 height, DXGI_FORMAT format, ITexture **ppTexture) = 0;

		//**********************************************************************
		// Method: CheckerboardTexture
		// Creates a texture with a checkerboard pattern
		//
		// Parameters:
		// pGraphics - graphics device
		//**********************************************************************
		virtual CRefObj<ITexture> CheckerboardTexture(IRenderer *pRenderer) = 0;

		//**********************************************************************
		// Method: LoadTexture
		// Loads a texture from disk
		//
		// Parameters:
		// pFilename - filename of the image file
		// pGraphics - graphics device
		// ppTexture - returns the newly created texture
		//**********************************************************************
		virtual void LoadTexture(const wchar_t *pFilename, IRenderer *pRenderer, ITexture **ppTexture) = 0;

		//**********************************************************************
		// Method: LoadVideoTexture
		// Loads a video from disk
		//
		// Parameters:
		// pFilename - filename of the image file
		// pGraphics - graphics device
		// ppTexture - returns the newly created texture
		//**********************************************************************
		virtual void LoadVideoTexture(const wchar_t *pFilename, IRenderer *pRenderer, ITexture **ppTexture) = 0;

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
		//virtual void MeshToD3D(IRenderer *pRenderer, ISubMesh *pMesh,
		//	int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
		//	int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
		//	BBox3 *pBbox, uint32 *pVertexSize) = 0;
		//
		//virtual void MeshToNormals(IRenderer *pRenderer, ISubMesh *pSubMesh,
		//	ID3D11Buffer **ppVB, uint32 *pNumVerts) = 0;

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
		//virtual void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion) = 0;
		//virtual void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion) = 0;

	};
	
	CAUSTICAPI void CreateCausticFactory(ICausticFactory **ppFactory);
}
