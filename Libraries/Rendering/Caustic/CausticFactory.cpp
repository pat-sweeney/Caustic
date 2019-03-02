//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "Renderer.h"
#include "CausticFactory.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>

namespace Caustic
{
	CRefObj<ICausticFactory> CCausticFactory::factory;

	CAUSTICAPI void CreateCausticFactory(ICausticFactory **ppFactory)
	{
		CCausticFactory *pFactory = new CCausticFactory();
		*ppFactory = pFactory;
		(*ppFactory)->AddRef();
	}

	CCausticFactory::CCausticFactory()
	{
		static bool setup = false;
		if (!setup)
		{
			CoInitializeEx(0, COINIT_MULTITHREADED);
			CT(MFStartup(MF_VERSION, MFSTARTUP_FULL));
			setup = true;
		}
	}

	CCausticFactory::~CCausticFactory()
	{
		static bool shutdown = false;
		if (!shutdown)
		{
			MFShutdown();
			CoUninitialize();
			shutdown = true;
		}
	}

	CAUSTICAPI void CreateRenderer(HWND hwnd, IRenderer **ppRenderer);
	void CCausticFactory::CreateRenderer(HWND hwnd, IRenderer **ppRenderer)
	{
		Caustic::CreateRenderer(hwnd, ppRenderer);
	}
	
	CAUSTICAPI void CreateGraphics(HWND hwnd, IGraphics **ppGraphics);
	void CCausticFactory::CreateGraphics(HWND hwnd, IGraphics **ppGraphics)
	{
		Caustic::CreateGraphics(hwnd, ppGraphics);
	}

	CAUSTICAPI void CreatePointLight(Vector3 &pos, IPointLight **ppLight);
	void CCausticFactory::CreatePointLight(Vector3 &pos, IPointLight **ppLight)
	{
		Caustic::CreatePointLight(pos, ppLight);
	}

	CAUSTICAPI void CreateTrackball(ITrackball **ppTrackball);
	void CCausticFactory::CreateTrackball(ITrackball **ppTrackball)
	{
		Caustic::CreateTrackball(ppTrackball);
	}

	CAUSTICAPI void CreateRendererMarshaller(IRendererMarshaller **ppClientServer);
	void CCausticFactory::CreateRendererMarshaller(IRendererMarshaller **ppMarshaller)
	{
		Caustic::CreateRendererMarshaller(ppMarshaller);
	}

	CAUSTICAPI void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial);
	void CCausticFactory::CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial)
	{
		Caustic::CreateRenderMaterial(pGraphics, pMaterialAttrib, pShader, ppRenderMaterial);
	}

	CAUSTICAPI void CreateRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, IShader *pShader, IRenderable **ppRenderable);
	void CCausticFactory::CreateRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, IShader *pShader, IRenderable **ppRenderable)
	{
		Caustic::CreateRenderable(pGraphics, pSubMesh, pMaterial, pShader, ppRenderable);
	}

	CAUSTICAPI void CreateRenderable(IRenderable **ppRenderable);
	void CCausticFactory::CreateRenderable(IRenderable **ppRenderable)
	{
		Caustic::CreateRenderable(ppRenderable);
	}

	CAUSTICAPI void CreateRenderable(ID3D11Buffer *pVB, uint32 numVertices, ID3D11Buffer *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable);
	void CCausticFactory::CreateRenderable(ID3D11Buffer *pVB, uint32 numVertices, ID3D11Buffer *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable)
	{
		Caustic::CreateRenderable(pVB, numVertices, pIB, numIndices, pFrontMaterial, pBackMaterial, mat, ppRenderable);
	}


	CAUSTICAPI void CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler);
	void CCausticFactory::CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler)
	{
		Caustic::CreateSampler(pGraphics, pTexture, ppSampler);
	}

	CAUSTICAPI void CreateCamera(bool leftHanded, ICamera **ppCamera);
	void CCausticFactory::CreateCamera(bool leftHanded, ICamera **ppCamera)
	{
		Caustic::CreateCamera(leftHanded, ppCamera);
	}

	CAUSTICAPI void CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture);
	void CCausticFactory::CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture)
	{
		Caustic::CreateTexture(pGraphics, width, height, format, cpuFlags, bindFlags, ppTexture);
	}

	CAUSTICAPI CRefObj<ITexture> CheckerboardTexture(IGraphics *pGraphics);
	CRefObj<ITexture> CCausticFactory::CheckerboardTexture(IGraphics *pGraphics)
	{
		return Caustic::CheckerboardTexture(pGraphics);
	}

	CAUSTICAPI void LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);
	void CCausticFactory::LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture)
	{
		Caustic::LoadTexture(pFilename, pGraphics, ppTexture);
	}

	CAUSTICAPI void LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);
	void CCausticFactory::LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture)
	{
		Caustic::LoadVideoTexture(pFilename, pGraphics, ppTexture);
	}

	CAUSTICAPI void MeshToD3D(IGraphics *pGraphics, ISubMesh *pMesh,
		int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
		int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
		BBox3 * /*pBbox*/, uint32 *pVertexSize);
	void CCausticFactory::MeshToD3D(IGraphics *pGraphics, ISubMesh *pMesh,
		int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
		int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
		BBox3 *pBbox, uint32 *pVertexSize)
	{
		Caustic::MeshToD3D(pGraphics, pMesh, vertexVersion, ppVertexBuffer, pNumVerts,
			indexVersion, ppIndexBuffer, pNumIndices, pBbox, pVertexSize);
	}

	CAUSTICAPI void MeshToNormals(IGraphics *pGraphics, ISubMesh *pSubMesh, ID3D11Buffer **ppVB, uint32 *pNumVerts);
	void CCausticFactory::MeshToNormals(IGraphics *pGraphics, ISubMesh *pSubMesh,
		ID3D11Buffer **ppVB, uint32 *pNumVerts)
	{
		Caustic::MeshToNormals(pGraphics, pSubMesh, ppVB, pNumVerts);
	}

	CAUSTICAPI void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion);
	void CCausticFactory::StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion)
	{
		Caustic::StoreSubMeshRenderableDataToStream(pStream, pMesh, vertexVersion, indexVersion);
	}

	CAUSTICAPI void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion);
	void CCausticFactory::LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion)
	{
		Caustic::LoadSubMeshRenderableDataFromStream(pStream, pDevice, ppIndexBuffer, pNumIndices, ppVertexBuffer, pNumVertices, pVertexVersion, pIndexVersion);
	}
};
