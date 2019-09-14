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

//**********************************************************************
// File: CausticFactory.cpp
// Contains the methods used to implement the CCausticFactory object.
//**********************************************************************

// Namespace: Caustic
namespace Caustic
{
    CAUSTICAPI void CreateRenderer(HWND hwnd, IRenderer **ppRenderer);
    CAUSTICAPI void CreateGraphics(HWND hwnd, IGraphics **ppGraphics);
    CAUSTICAPI void CreatePointLight(Vector3 &pos, IPointLight **ppLight);
    CAUSTICAPI void CreateTrackball(ITrackball **ppTrackball);
    CAUSTICAPI void CreateRendererMarshaller(IRendererMarshaller **ppClientServer);
    CAUSTICAPI void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial);
    CAUSTICAPI void CreateRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, IShader *pShader, IRenderable **ppRenderable);
    CAUSTICAPI void CreateRenderable(IRenderable **ppRenderable);
    CAUSTICAPI void CreateRenderable(ID3D11Buffer *pVB, uint32 numVertices, ID3D11Buffer *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable);
    CAUSTICAPI void CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler);
    CAUSTICAPI void CreateCamera(bool leftHanded, ICamera **ppCamera);
    CAUSTICAPI void CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture);
    CAUSTICAPI CRefObj<ITexture> CheckerboardTexture(IGraphics *pGraphics);
    CAUSTICAPI void LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);
    CAUSTICAPI void LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);
    CAUSTICAPI void MeshToD3D(IGraphics *pGraphics, ISubMesh *pMesh,
        int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
        int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
        BBox3 * /*pBbox*/, uint32 *pVertexSize);
    CAUSTICAPI void MeshToNormals(IGraphics *pGraphics, ISubMesh *pSubMesh, ID3D11Buffer **ppVB, uint32 *pNumVerts);
    CAUSTICAPI void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion);
    CAUSTICAPI void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion);
    
    CRefObj<ICausticFactory> CCausticFactory::factory;

    //**********************************************************************
    // Function: CreateCausticFactory
    // Global function for creating the main Caustic factory.
    //
    // Parameters:
    // ppFactory - Returns the newly created Caustic factory.
    //**********************************************************************
    CAUSTICAPI void CreateCausticFactory(ICausticFactory **ppFactory)
	{
		CCausticFactory *pFactory = new CCausticFactory();
		*ppFactory = pFactory;
		(*ppFactory)->AddRef();
	}

    //**********************************************************************
    // Method: CCausticFactory
    // Defines the implementation of <ICausticFactory>
    //**********************************************************************
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

    //**********************************************************************
    // Method: ~CCausticFactory
    // Implements the dtor for CCausticFactory
    //**********************************************************************
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

    //**********************************************************************
    // Method: CreateRenderer
    // See <ICausticFactory::CreateRenderer>
    //**********************************************************************
    void CCausticFactory::CreateRenderer(HWND hwnd, IRenderer **ppRenderer)
	{
		Caustic::CreateRenderer(hwnd, ppRenderer);
	}
	
    //**********************************************************************
    // Method: CreateGraphics
    // See <ICausticFactory::CreateGraphics>
    //**********************************************************************
    void CCausticFactory::CreateGraphics(HWND hwnd, IGraphics **ppGraphics)
	{
		Caustic::CreateGraphics(hwnd, ppGraphics);
	}

    //**********************************************************************
    // Method: CreatePointLight
    // See <ICausticFactory::CreatePointLight>
    //**********************************************************************
    void CCausticFactory::CreatePointLight(Vector3 &pos, IPointLight **ppLight)
	{
		Caustic::CreatePointLight(pos, ppLight);
	}

    //**********************************************************************
    // Method: CreateTrackball
    // See <ICausticFactory::CreateTrackball>
    //**********************************************************************
    void CCausticFactory::CreateTrackball(ITrackball **ppTrackball)
	{
		Caustic::CreateTrackball(ppTrackball);
	}

    //**********************************************************************
    // Method: CreateRendererMarshaller
    // See <ICausticFactory::CreateRendererMarshaller>
    //**********************************************************************
    void CCausticFactory::CreateRendererMarshaller(IRendererMarshaller **ppMarshaller)
	{
		Caustic::CreateRendererMarshaller(ppMarshaller);
	}

    //**********************************************************************
    // Method: CreateRenderMaterial
    // See <ICausticFactory::CreateRenderMaterial>
    //**********************************************************************
    void CCausticFactory::CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial)
	{
		Caustic::CreateRenderMaterial(pGraphics, pMaterialAttrib, pShader, ppRenderMaterial);
	}

    //**********************************************************************
    // Method: CreateRenderable
    // See <ICausticFactory::CreateRenderable>
    //**********************************************************************
    void CCausticFactory::CreateRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, IShader *pShader, IRenderable **ppRenderable)
	{
		Caustic::CreateRenderable(pGraphics, pSubMesh, pMaterial, pShader, ppRenderable);
	}

    //**********************************************************************
    // Method: CreateRenderable
    // See <ICausticFactory::CreateRenderable>
    //**********************************************************************
    void CCausticFactory::CreateRenderable(IRenderable **ppRenderable)
	{
		Caustic::CreateRenderable(ppRenderable);
	}

    //**********************************************************************
    // Method: CreateRenderable
    // See <ICausticFactory::CreateRenderable>
    //**********************************************************************
    void CCausticFactory::CreateRenderable(ID3D11Buffer *pVB, uint32 numVertices, ID3D11Buffer *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable)
	{
		Caustic::CreateRenderable(pVB, numVertices, pIB, numIndices, pFrontMaterial, pBackMaterial, mat, ppRenderable);
	}

    //**********************************************************************
    // Method: CreateSampler
    // See <ICausticFactory::CreateSampler>
    //**********************************************************************
    void CCausticFactory::CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler)
	{
		Caustic::CreateSampler(pGraphics, pTexture, ppSampler);
	}

    //**********************************************************************
    // Method: CreateCamera
    // See <ICausticFactory::CreateCamera>
    //**********************************************************************
    void CCausticFactory::CreateCamera(bool leftHanded, ICamera **ppCamera)
	{
		Caustic::CreateCamera(leftHanded, ppCamera);
	}

    //**********************************************************************
    // Method: CreateTexture
    // See <ICausticFactory::CreateTexture>
    //**********************************************************************
    void CCausticFactory::CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture)
	{
		Caustic::CreateTexture(pGraphics, width, height, format, cpuFlags, bindFlags, ppTexture);
	}

    //**********************************************************************
    // Method: CheckerboardTexture
    // See <ICausticFactory::CheckerboardTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::CheckerboardTexture(IGraphics *pGraphics)
	{
		return Caustic::CheckerboardTexture(pGraphics);
	}

    //**********************************************************************
    // Method: LoadTexture
    // See <ICausticFactory::LoadTexture>
    //**********************************************************************
    void CCausticFactory::LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture)
	{
		Caustic::LoadTexture(pFilename, pGraphics, ppTexture);
	}

    //**********************************************************************
    // Method: LoadVideoTexture
    // See <ICausticFactory::LoadVideoTexture>
    //**********************************************************************
    void CCausticFactory::LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture)
	{
		Caustic::LoadVideoTexture(pFilename, pGraphics, ppTexture);
	}

    //**********************************************************************
    // Method: MeshToD3D
    // See <ICausticFactory::MeshToD3D>
    //**********************************************************************
    void CCausticFactory::MeshToD3D(IGraphics *pGraphics, ISubMesh *pMesh,
		int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
		int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
		BBox3 *pBbox, uint32 *pVertexSize)
	{
		Caustic::MeshToD3D(pGraphics, pMesh, vertexVersion, ppVertexBuffer, pNumVerts,
			indexVersion, ppIndexBuffer, pNumIndices, pBbox, pVertexSize);
	}

    //**********************************************************************
    // Method: MeshToNormals
    // See <ICausticFactory::MeshToNormals>
    //**********************************************************************
    void CCausticFactory::MeshToNormals(IGraphics *pGraphics, ISubMesh *pSubMesh,
		ID3D11Buffer **ppVB, uint32 *pNumVerts)
	{
		Caustic::MeshToNormals(pGraphics, pSubMesh, ppVB, pNumVerts);
	}

    //**********************************************************************
    // Method: StoreSubMeshRenderableDataToStream
    // See <ICausticFactory::StoreSubMeshRenderableDataToStream>
    //**********************************************************************
    void CCausticFactory::StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion)
	{
		Caustic::StoreSubMeshRenderableDataToStream(pStream, pMesh, vertexVersion, indexVersion);
	}

    //**********************************************************************
    // Method: LoadSubMeshRenderableDataFromStream
    // See <ICausticFactory::LoadSubMeshRenderableDataFromStream>
    //**********************************************************************
    void CCausticFactory::LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion)
	{
		Caustic::LoadSubMeshRenderableDataFromStream(pStream, pDevice, ppIndexBuffer, pNumIndices, ppVertexBuffer, pNumVertices, pVertexVersion, pIndexVersion);
	}
};
