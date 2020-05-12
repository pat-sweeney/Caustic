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
    CAUSTICAPI CRefObj<IRenderer> CreateRenderer(HWND hwnd, std::wstring &shaderFolder);
    CAUSTICAPI CRefObj<IGraphics> CreateGraphics(HWND hwnd);
    CAUSTICAPI CRefObj<IPointLight> CreatePointLight(Vector3 &pos, Vector3 &color);
    CAUSTICAPI CRefObj<ITrackball> CreateTrackball();
    CAUSTICAPI CRefObj<IRendererMarshaller> CreateRendererMarshaller();
    CAUSTICAPI CRefObj<IMaterialAttrib> CreateMaterialAttrib();
    CAUSTICAPI CRefObj<IRenderMaterial> CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader);
    CAUSTICAPI CRefObj<IRenderable > CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat);
    CAUSTICAPI CRefObj<ISampler> CreateSampler(IGraphics *pGraphics, ITexture *pTexture);
    CAUSTICAPI CRefObj<ICamera> CreateCamera(bool leftHanded);
    CAUSTICAPI CRefObj<ITexture> CreateTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags);
    CAUSTICAPI CRefObj<ITexture> CreateTexture(IGraphics* pGraphics, IImage *pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags);
    CAUSTICAPI CRefObj<ITexture> CheckerboardTexture(IGraphics* pGraphics);
    CAUSTICAPI CRefObj<ITexture> LoadTexture(const wchar_t *pFilename, IGraphics* pGraphics);
    CAUSTICAPI CRefObj<ITexture> LoadVideoTexture(const wchar_t *pFilename, IGraphics* pGraphics);
    
    CRefObj<ICausticFactory> CCausticFactory::factory;

    //**********************************************************************
    // Function: CreateCausticFactory
    // Global function for creating the main Caustic factory.
    //
    // Returns:
    // ppFactory - Returns the newly created Caustic factory.
    //**********************************************************************
    CAUSTICAPI CRefObj<ICausticFactory> CreateCausticFactory()
	{
		return CRefObj<ICausticFactory>(new CCausticFactory());
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
    CRefObj<IRenderer> CCausticFactory::CreateRenderer(HWND hwnd, std::wstring &shaderFolder)
	{
		return Caustic::CreateRenderer(hwnd, shaderFolder);
	}
	
    //**********************************************************************
    // Method: CreateGraphics
    // See <ICausticFactory::CreateGraphics>
    //**********************************************************************
    CRefObj<IGraphics> CCausticFactory::CreateGraphics(HWND hwnd)
	{
		return Caustic::CreateGraphics(hwnd);
	}

    //**********************************************************************
    // Method: CreateRenderMesh
    // See <ICausticFactory::CreateRenderMesh>
    //**********************************************************************
    CRefObj<IRenderMesh> CCausticFactory::CreateRenderMesh()
	{
		return CRefObj<IRenderMesh>(new CRenderMesh());
	}

    //**********************************************************************
    // Method: CreateRenderSubMesh
    // See <ICausticFactory::CreateRenderSubMesh>
    //**********************************************************************
    CRefObj<IRenderSubMesh> CCausticFactory::CreateRenderSubMesh()
	{
		return CRefObj<IRenderSubMesh>(new CRenderSubMesh());
	}

    //**********************************************************************
    // Method: CreatePointLight
    // See <ICausticFactory::CreatePointLight>
    //**********************************************************************
    CRefObj<IPointLight> CCausticFactory::CreatePointLight(Vector3 &pos, Vector3 &color)
	{
		return Caustic::CreatePointLight(pos, color);
	}

    //**********************************************************************
    // Method: CreateTrackball
    // See <ICausticFactory::CreateTrackball>
    //**********************************************************************
    CRefObj<ITrackball> CCausticFactory::CreateTrackball()
	{
		return Caustic::CreateTrackball();
	}

    //**********************************************************************
    // Method: CreateRendererMarshaller
    // See <ICausticFactory::CreateRendererMarshaller>
    //**********************************************************************
    CRefObj<IRendererMarshaller> CCausticFactory::CreateRendererMarshaller()
	{
		return Caustic::CreateRendererMarshaller();
	}

    //**********************************************************************
    // Method: CreateMaterialAttrib
    // See <ICausticFactory::CreateMaterialAttrib>
    //**********************************************************************
    CRefObj<IMaterialAttrib> CCausticFactory::CreateMaterialAttrib()
    {
        return Caustic::CreateMaterialAttrib();
    }

    //**********************************************************************
    // Method: CreateRenderMaterial
    // See <ICausticFactory::CreateRenderMaterial>
    //**********************************************************************
    CRefObj<IRenderMaterial> CCausticFactory::CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader)
	{
		return Caustic::CreateRenderMaterial(pGraphics, pMaterialAttrib, pShader);
	}

    //**********************************************************************
    // Method: CreateRenderable
    // See <ICausticFactory::CreateRenderable>
    //**********************************************************************
    CRefObj<IRenderable> CCausticFactory::CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat)
	{
        return Caustic::CreateRenderable(pSubMesh, pFrontMaterial, pBackMaterial, mat);
	}
    
    //**********************************************************************
    // Method: CreateSampler
    // See <ICausticFactory::CreateSampler>
    //**********************************************************************
    CRefObj<ISampler> CCausticFactory::CreateSampler(IGraphics *pGraphics, ITexture *pTexture)
	{
		return Caustic::CreateSampler(pGraphics, pTexture);
	}

    //**********************************************************************
    // Method: CreateCamera
    // See <ICausticFactory::CreateCamera>
    //**********************************************************************
    CRefObj<ICamera> CCausticFactory::CreateCamera(bool leftHanded)
	{
		return Caustic::CreateCamera(leftHanded);
	}

    //**********************************************************************
    // Method: CreateTexture
    // See <ICausticFactory::CreateTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::CreateTexture(IGraphics* pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags)
    {
        return Caustic::CreateTexture(pGraphics, width, height, format, cpuFlags, bindFlags);
    }

    //**********************************************************************
    // Method: CreateTexture
    // See <ICausticFactory::CreateTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::CreateTexture(IGraphics* pGraphics, IImage* pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags)
    {
        return Caustic::CreateTexture(pGraphics, pImage, cpuFlags, bindFlags);
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
    CRefObj<ITexture> CCausticFactory::LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics)
	{
		return Caustic::LoadTexture(pFilename, pGraphics);
	}

    //**********************************************************************
    // Method: LoadVideoTexture
    // See <ICausticFactory::LoadVideoTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics)
	{
		return Caustic::LoadVideoTexture(pFilename, pGraphics);
	}

    //**********************************************************************
    // Method: CreateShader
    // See <ICausticFactory::CreateShader>
    //**********************************************************************
    CRefObj<IShader> CCausticFactory::CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
        ID3DBlob *pVertexShaderBlob, ID3DBlob* pPixelShaderBlob, ID3DBlob* pComputeShaderBlob,
        IShaderInfo *pShaderInfo)
    {
        std::unique_ptr<CShader> spShader(new CShader());
        spShader->Create(pRenderer, pShaderName, pShaderInfo, pPixelShaderBlob, pVertexShaderBlob, pComputeShaderBlob);
        return CRefObj<IShader>(spShader.release());
    }

    //**********************************************************************
    // Method: CreateShaderInfo
    // See <ICausticFactory::CreateShaderInfo>
    //**********************************************************************
    CRefObj<IShaderInfo> CCausticFactory::CreateShaderInfo(const wchar_t *pFilename)
    {
        std::unique_ptr<CShaderInfo> spShaderInfo(new CShaderInfo());
        std::wstring fn(pFilename);
        spShaderInfo->LoadShaderDef(fn);
        return CRefObj<IShaderInfo>(spShaderInfo.release());
    }
};
