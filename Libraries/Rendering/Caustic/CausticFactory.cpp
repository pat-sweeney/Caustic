//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <DirectXMath.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>
#include <string>
#include <memory>

module Rendering.Caustic.CausticFactory;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Imaging.Image.IImage;
import Rendering.Caustic.Renderer;
import Rendering.Caustic.ShaderInfo;
import Rendering.Caustic.RenderMesh;
import Rendering.Caustic.Shader;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.VideoTexture;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.DesktopTexture;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.ICamera;
import Geometry.Mesh.MaterialAttrib;

//**********************************************************************
// File: CausticFactory.cpp
// Contains the methods used to implement the CCausticFactory object.
//**********************************************************************

namespace Caustic
{
    static bool setupRequired = true;

    //**********************************************************************
    // Method: CCausticFactory
    // Defines the implementation of <ICausticFactory>
    //**********************************************************************
    CCausticFactory::CCausticFactory()
	{
		if (setupRequired)
		{
			CoInitializeEx(0, COINIT_MULTITHREADED);
			CT(MFStartup(MF_VERSION, MFSTARTUP_FULL));
            setupRequired = false;
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
            setupRequired = true;
			shutdown = true;
		}
	}

    //**********************************************************************
    // Method: CreateRenderer
    // See <ICausticFactory::CreateRenderer>
    //**********************************************************************
    CRefObj<IRenderer> CCausticFactory::CreateRenderer(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder, bool startFrozen /* = false */, int desktopIndex /* = 0 */)
	{
		return Caustic::CreateRenderer(hwnd, viewport, shaderFolder, startFrozen, desktopIndex);
	}
	
    //**********************************************************************
    // Method: CreatePointCloud
    // See <ICausticFactory::CreatePointCloud>
    //**********************************************************************
    CRefObj<IPointCloud> CCausticFactory::CreatePointCloud(IRenderer* pRenderer, uint32 width, uint32 height)
    {
        return CRefObj<IPointCloud>(Caustic::CreatePointCloud(pRenderer, width, height));
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
    CRefObj<IPointLight> CCausticFactory::CreatePointLight(Vector3& pos, FRGBColor& color, float intensity)
    {
        return Caustic::CreatePointLight(pos, color, intensity);
    }

    //**********************************************************************
    // Method: CreateSpotLight
    // See <ICausticFactory::CreateSpotLight>
    //**********************************************************************
    CRefObj<ISpotLight> CCausticFactory::CreateSpotLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity, float innerAngle, float outerAngle, bool casts)
    {
        return CreateSpotLight(pos, dir, color, intensity, innerAngle, outerAngle, casts);
    }

    //**********************************************************************
    // Method: CreateDirectionalLight
    // See <ICausticFactory::CreateDirectionalLight>
    //**********************************************************************
    CRefObj<IDirectionalLight> CCausticFactory::CreateDirectionalLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity)
    {
        return Caustic::CreateDirectionalLight(pos, dir, color, intensity);
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
    CRefObj<IRenderMaterial> CCausticFactory::CreateRenderMaterial(IRenderer *pRenderer, IMaterialAttrib *pMaterialAttrib, IShader *pShader)
	{
		return Caustic::CreateRenderMaterial(pRenderer, pMaterialAttrib, pShader);
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
    CRefObj<ISampler> CCausticFactory::CreateSampler(IRenderer *pRenderer, ITexture *pTexture)
	{
		return Caustic::CreateSampler(pRenderer, pTexture);
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
    CRefObj<ITexture> CCausticFactory::CreateTexture(IRenderer* pRenderer, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags)
    {
        return Caustic::CreateTexture(pRenderer, width, height, format, cpuFlags, bindFlags);
    }

    //**********************************************************************
    // Method: CreateTexture
    // See <ICausticFactory::CreateTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::CreateTexture(IRenderer* pRenderer, IImage* pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags)
    {
        return Caustic::CreateTexture(pRenderer, pImage, cpuFlags, bindFlags);
    }

    //**********************************************************************
    // Method: CheckerboardTexture
    // See <ICausticFactory::CheckerboardTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::CheckerboardTexture(IRenderer* pRenderer)
    {
        return Caustic::CheckerboardTexture(pRenderer);
    }

    //**********************************************************************
    // Method: CreateDesktopTexture
    // See <ICausticFactory::CreateDesktopTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::CreateDesktopTexture(IRenderer* pRenderer)
    {
        return Caustic::CreateDesktopTexture(pRenderer);
    }
    
    //**********************************************************************
    // Method: LoadTexture
    // See <ICausticFactory::LoadTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::LoadTexture(const wchar_t *pFilename, IRenderer *pRenderer)
	{
		return Caustic::LoadTexture(pFilename, pRenderer);
	}

    //**********************************************************************
    // Method: LoadVideoTexture
    // See <ICausticFactory::LoadVideoTexture>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::LoadVideoTexture(const wchar_t* pFilename, IRenderer* pRenderer)
    {
        return Caustic::LoadVideoTexture(pFilename, pRenderer);
    }

    //**********************************************************************
    // Method: VideoTextureFromWebcam
    // See <ICausticFactory::VideoTextureFromWebcam>
    //**********************************************************************
    CRefObj<ITexture> CCausticFactory::VideoTextureFromWebcam(IRenderer* pRenderer)
    {
        return Caustic::VideoTextureFromWebcam(pRenderer);
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
