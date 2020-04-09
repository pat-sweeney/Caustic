//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Imaging\Image\Image.h"
#include "RenderMesh.h"
#include "ShaderInfo.h"
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

namespace Caustic
{
    CAUSTICAPI void CreateRenderer(HWND hwnd, std::wstring &shaderFolder, IRenderer **ppRenderer);
    CAUSTICAPI void CreateGraphics(HWND hwnd, IGraphics **ppGraphics);
    CAUSTICAPI void CreatePointLight(Vector3 &pos, IPointLight **ppLight);
    CAUSTICAPI void CreateTrackball(ITrackball **ppTrackball);
    CAUSTICAPI void CreateRendererMarshaller(IRendererMarshaller **ppClientServer);
    CAUSTICAPI void CreateMaterialAttrib(IMaterialAttrib** ppMaterialAttib);
    CAUSTICAPI void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial);
    CAUSTICAPI void CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable);
    CAUSTICAPI void CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler);
    CAUSTICAPI void CreateCamera(bool leftHanded, ICamera **ppCamera);
    CAUSTICAPI void CreateTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags, ITexture** ppTexture);
    CAUSTICAPI void CreateTexture(IGraphics* pGraphics, IImage *pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags, ITexture** ppTexture);
    CAUSTICAPI CRefObj<ITexture> CheckerboardTexture(IGraphics *pGraphics);
    CAUSTICAPI void LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);
    CAUSTICAPI void LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture);
    
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
    // Implements the dtor for <CCausticFactory>
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
    void CCausticFactory::CreateRenderer(HWND hwnd, std::wstring &shaderFolder, IRenderer **ppRenderer)
	{
		Caustic::CreateRenderer(hwnd, shaderFolder, ppRenderer);
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
    // Method: CreateRenderMesh
    // See <ICausticFactory::CreateRenderMesh>
    //**********************************************************************
    void CCausticFactory::CreateRenderMesh(IRenderMesh **ppRenderMesh)
	{
		_ASSERT(ppRenderMesh);
		*ppRenderMesh = new CRenderMesh();
		(*ppRenderMesh)->AddRef();
	}

    //**********************************************************************
    // Method: CreateRenderSubMesh
    // See <ICausticFactory::CreateRenderSubMesh>
    //**********************************************************************
    void CCausticFactory::CreateRenderSubMesh(IRenderSubMesh **ppRenderSubMesh)
	{
		_ASSERT(ppRenderSubMesh);
		*ppRenderSubMesh = new CRenderSubMesh();
		(*ppRenderSubMesh)->AddRef();
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
    // Method: CreateMaterialAttrib
    // See <ICausticFactory::CreateMaterialAttrib>
    //**********************************************************************
    void CCausticFactory::CreateMaterialAttrib(IMaterialAttrib** ppMaterialAttrib)
    {
        Caustic::CreateMaterialAttrib(ppMaterialAttrib);
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
    void CCausticFactory::CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable)
	{
        Caustic::CreateRenderable(pSubMesh, pFrontMaterial, pBackMaterial, mat, ppRenderable);
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
    void CCausticFactory::CreateTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags, ITexture** ppTexture)
    {
        Caustic::CreateTexture(pGraphics, width, height, format, cpuFlags, bindFlags, ppTexture);
    }

    //**********************************************************************
    // Method: CreateTexture
    // See <ICausticFactory::CreateTexture>
    //**********************************************************************
    void CCausticFactory::CreateTexture(IGraphics* pGraphics, IImage* pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags, ITexture** ppTexture)
    {
        Caustic::CreateTexture(pGraphics, pImage, cpuFlags, bindFlags, ppTexture);
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
    // Method: CreateShader
    // See <ICausticFactory::CreateShader>
    //**********************************************************************
    void CCausticFactory::CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
        ID3DBlob *pVertexShaderBlob, ID3DBlob *pPixelShaderBlob, IShaderInfo *pShaderInfo,
        IShader **ppShader)
    {
        std::unique_ptr<CShader> spShader(new CShader());
        spShader->Create(pRenderer, pShaderName, pShaderInfo, pPixelShaderBlob, pVertexShaderBlob);
        *ppShader = spShader.release();
        (*ppShader)->AddRef();
    }

    //**********************************************************************
    // Method: CreateShaderInfo
    // See <ICausticFactory::CreateShaderInfo>
    //**********************************************************************
    void CCausticFactory::CreateShaderInfo(const wchar_t *pFilename, IShaderInfo **ppShaderInfo)
    {
        CShaderInfo *pShaderInfo = new CShaderInfo();
        std::wstring fn(pFilename);
        pShaderInfo->LoadShaderDef(fn);
        *ppShaderInfo = pShaderInfo;
        (*ppShaderInfo)->AddRef();
    }
};
