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
#include <d3d12.h>
#include <DirectXMath.h>
#include "Renderer.h"
#include "RendererMarshaller.h"
#include "CausticFactory.h"
#include "PointLight.h"
#include "RenderMaterial.h"
#include "Trackball.h"
#include "Sampler.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "ShaderInfo.h"
#include "Material.h"
#include "Renderable.h"
#include "RenderMesh.h"
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

	void CCausticFactory::CreateRendererMarshaller(HWND hwnd, std::wstring &shaderFolder, IRendererMarshaller **ppRenderer)
	{
		_ASSERT(ppRenderer);
		std::unique_ptr<CRendererMarshaller> spRenderer(new CRendererMarshaller());
		*ppRenderer = spRenderer.release();
		(*ppRenderer)->AddRef();
	}

	void CCausticFactory::CreateRenderer(HWND hwnd, std::wstring &shaderFolder, IRenderer **ppRenderer)
	{
		_ASSERT(ppRenderer);
		std::unique_ptr<CRenderer> spRenderer(new CRenderer());
		spRenderer->Setup(hwnd, shaderFolder, true);

		CRefObj<ICamera> spCamera;
		CCausticFactory::Instance()->CreateCamera(true, &spCamera);
		spRenderer->SetCamera(spCamera);

		*ppRenderer = spRenderer.release();
		(*ppRenderer)->AddRef();
	}

    void CCausticFactory::CreateRenderMesh(IRenderMesh **ppRenderMesh)
	{
		_ASSERT(ppRenderMesh);
		*ppRenderMesh = new CRenderMesh();
		(*ppRenderMesh)->AddRef();
	}

	void CCausticFactory::CreateRenderSubMesh(IRenderSubMesh **ppRenderSubMesh)
	{
		_ASSERT(ppRenderSubMesh);
		*ppRenderSubMesh = new CRenderSubMesh();
		(*ppRenderSubMesh)->AddRef();
	}

	void CCausticFactory::CreatePointLight(Vector3 &pos, IPointLight **ppLight)
	{
		std::unique_ptr<CPointLight> spPointLight(new CPointLight());
		spPointLight->SetPosition(pos);
		*ppLight = spPointLight.release();
		(*ppLight)->AddRef();
	}

	//**********************************************************************
	// Creates a new IMaterialAttrib object
	// ambientColor - Normalized [0..1] ambient color value
	// diffuseColor - Normalized [0..1] diffse color value
	// specularColor - Normalized [0..1] specular color value
	// specularExp - Specular power value
	// alpha - Alpha value
	// ppMaterial - Returns the created material object
	//**********************************************************************
	void CCausticFactory::CreateMaterial(Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, float specularExp, float alpha, IMaterialAttrib **ppMaterial)
	{
		std::unique_ptr<CMaterial> spMaterial(new CMaterial());
		spMaterial->AmbientColor = ambientColor;
		spMaterial->DiffuseColor = diffuseColor;
		spMaterial->SpecularColor = specularColor;
		spMaterial->SpecularExp = specularExp;
		spMaterial->Alpha = alpha;
		*ppMaterial = spMaterial.release();
		(*ppMaterial)->AddRef();
	}
	
	//**********************************************************************
	// Creates a new empty IMaterial object
	// ppMaterial - Returns the created material object
	//**********************************************************************
	void CCausticFactory::CreateMaterial(IMaterialAttrib **ppMaterial)
	{
		std::unique_ptr<CMaterial> spMaterial(new CMaterial());
		*ppMaterial = spMaterial.release();
		(*ppMaterial)->AddRef();
	}

	void CCausticFactory::CreateTrackball(ITrackball **ppTrackball)
	{
		std::unique_ptr<CTrackball> spTrackball(new CTrackball());
		*ppTrackball = spTrackball.release();
		(*ppTrackball)->AddRef();
	}

	void CCausticFactory::CreateRenderMaterial(IRenderer *pRenderer, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial)
	{
        CRefObj<ICausticFactory> spFactory = Caustic::CCausticFactory::Instance();
        std::unique_ptr<CRenderMaterial> spRenderMaterial(new CRenderMaterial(pMaterialAttrib, pShader));
		if (pMaterialAttrib)
		{
            std::string fnDiffuse = pMaterialAttrib->GetDiffuseTexture();
			if (!fnDiffuse.empty())
			{
                CRefObj<ITexture> spDiffuseTexture;
                std::wstring wfn(fnDiffuse.begin(), fnDiffuse.end());
                spFactory->LoadTexture(wfn.c_str(), pRenderer, &spDiffuseTexture);
                spRenderMaterial->SetDiffuseTexture(pRenderer, spDiffuseTexture);
            }

			std::string fnSpecular = pMaterialAttrib->GetSpecularTexture();
			if (!fnSpecular.empty())
			{
                CRefObj<ITexture> spSpecularTexture;
                std::wstring wfn(fnSpecular.begin(), fnSpecular.end());
				spFactory->LoadTexture(wfn.c_str(), pRenderer, &spSpecularTexture);
                spRenderMaterial->SetSpecularTexture(pRenderer, spSpecularTexture);
            }

			std::string fnAmbient = pMaterialAttrib->GetAmbientTexture();
			if (!fnAmbient.empty())
			{
                CRefObj<ITexture> spAmbientTexture;
                std::wstring wfn(fnAmbient.begin(), fnAmbient.end());
				spFactory->LoadTexture(wfn.c_str(), pRenderer, &spAmbientTexture);
                spRenderMaterial->SetAmbientTexture(pRenderer, spAmbientTexture);
            }
        }
        *ppRenderMaterial = spRenderMaterial.release();
		(*ppRenderMaterial)->AddRef();
	}

	void CCausticFactory::CreateRenderable(ID3D12Resource *pVB, uint32 numVertices,
		ID3D12Resource *pIB, uint32 numIndices,
		IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial,
		DirectX::XMMATRIX &mat, IRenderable **ppRenderable)
	{
		CRefObj<IShader> spShader;
		pFrontMaterial->GetShader(&spShader);
		CVertexBuffer vb;
		vb.m_numIndices = numVertices;
		vb.m_spVB = pVB;
		vb.m_numIndices = numIndices;
		vb.m_spIB = pIB;
		vb.m_vertexSize = spShader->GetShaderInfo()->GetVertexSize();
		std::unique_ptr<CRenderable> spRenderable(new CRenderable(&vb, nullptr, pFrontMaterial, pBackMaterial, mat));
		*ppRenderable = spRenderable.release();
		(*ppRenderable)->AddRef();
	}

	void CCausticFactory::CreateSampler(IRenderer *pRenderer, ITexture *pTexture, ISampler **ppSampler)
	{
		std::unique_ptr<CSampler> spSampler(new CSampler(pRenderer, pTexture));
		*ppSampler = spSampler.release();
		(*ppSampler)->AddRef();
	}

    void CCausticFactory::CreateCamera(bool leftHanded, ICamera **ppCamera)
    {
        std::unique_ptr<CCamera> pCamera(new CCamera(leftHanded));
        *ppCamera = pCamera.release();
        (*ppCamera)->AddRef();
    }

    void CCausticFactory::CreateTextureMgr(IRenderer *pRenderer, uint32 numTexUploads, uint64 textureMemSize, ITextureMgr **ppTextureMgr)
    {
        std::unique_ptr<CTextureMgr> pTextureMgr(new CTextureMgr(pRenderer, numTexUploads, textureMemSize));
        *ppTextureMgr = pTextureMgr.release();
        (*ppTextureMgr)->AddRef();
    }

    void CCausticFactory::CreateTexture(IRenderer *pRenderer, uint32 width, uint32 height, DXGI_FORMAT format, ITexture **ppTexture)
	{
		std::unique_ptr<CTexture> spTexture(new CTexture(pRenderer, width, height, format));
		*ppTexture = spTexture.release();
		(*ppTexture)->AddRef();
	}

	CRefObj<ITexture> CCausticFactory::CheckerboardTexture(IRenderer *pRenderer)
	{
		static CRefObj<ITexture> s_spCheckerBoard;
#if 0
		if (s_spCheckerBoard == nullptr)
		{
			CreateTexture(pRenderer, 32, 32, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &s_spCheckerBoard);
			CComPtr<ID3D12Resource> spTexture = s_spCheckerBoard->GetD3DTexture();
			D3D12_MAPPED_SUBRESOURCE ms;
			CT(pRenderer->GetContext()->Map(spTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
			BYTE *pr = reinterpret_cast<BYTE*>(ms.pData);
			for (int i = 0; i < 32; i++)
			{
				BYTE *pc = pr;
				for (int j = 0; j < 32; j++)
				{
					if ((i & 1) == (j & 1))
					{
						pc[0] = 0x00;
						pc[1] = 0x00;
						pc[2] = 0x00;
						pc[3] = 0xff;
					}
					else
					{
						pc[0] = 0xff;
						pc[1] = 0xff;
						pc[2] = 0xff;
						pc[3] = 0xff;
					}
					pc += 4;
				}
				pr += ms.RowPitch;
			}
			pRenderer->GetContext()->Unmap(spTexture, 0);
		}
#endif
		return s_spCheckerBoard;
	}

	void CCausticFactory::LoadTexture(const wchar_t *pFilename, IRenderer *pRenderer, ITexture **ppTexture)
	{
		CTexture::LoadTexture(pFilename, pRenderer, ppTexture);
	}

	CAUSTICAPI void LoadVideoTexture(const wchar_t *pFilename, IRenderer *pRenderer, ITexture **ppTexture);
	void CCausticFactory::LoadVideoTexture(const wchar_t *pFilename, IRenderer *pRenderer, ITexture **ppTexture)
	{
		Caustic::LoadVideoTexture(pFilename, pRenderer, ppTexture);
	}

//	CAUSTICAPI void MeshToD3D(IRenderer *pRenderer, ISubMesh *pMesh,
//		int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
//		int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
//		BBox3 * /*pBbox*/, uint32 *pVertexSize);
//	void CCausticFactory::MeshToD3D(IRenderer *pRenderer, ISubMesh *pMesh,
//		int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
//		int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
//		BBox3 *pBbox, uint32 *pVertexSize)
//	{
//		Caustic::MeshToD3D(pRenderer, pMesh, vertexVersion, ppVertexBuffer, pNumVerts,
//			indexVersion, ppIndexBuffer, pNumIndices, pBbox, pVertexSize);
//	}

//	CAUSTICAPI void MeshToNormals(IRenderer *pRenderer, ISubMesh *pSubMesh, ID3D11Buffer **ppVB, uint32 *pNumVerts);
//	void CCausticFactory::MeshToNormals(IRenderer *pRenderer, ISubMesh *pSubMesh,
//		ID3D11Buffer **ppVB, uint32 *pNumVerts)
//	{
//		Caustic::MeshToNormals(pRenderer, pSubMesh, ppVB, pNumVerts);
//	}

// CAUSTICAPI void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion);
// void CCausticFactory::StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion)
// {
// 	Caustic::StoreSubMeshRenderableDataToStream(pStream, pMesh, vertexVersion, indexVersion);
// }
// 
// CAUSTICAPI void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion);
// void CCausticFactory::LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion)
// {
// 	Caustic::LoadSubMeshRenderableDataFromStream(pStream, pDevice, ppIndexBuffer, pNumIndices, ppVertexBuffer, pNumVertices, pVertexVersion, pIndexVersion);
// }

	void CCausticFactory::CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
		ID3DBlob *pVertexShaderBlob, ID3DBlob *pPixelShaderBlob, IShaderInfo *pShaderInfo,
		IShader **ppShader)
	{
		std::unique_ptr<CShader> spShader(new CShader());
		spShader->Create(pRenderer, pShaderName, pShaderInfo, pPixelShaderBlob, pVertexShaderBlob);
		*ppShader = spShader.release();
		(*ppShader)->AddRef();
	}
	
	void CCausticFactory::CreateShaderInfo(const wchar_t *pFilename, IShaderInfo **ppShaderInfo)
	{
		CShaderInfo *pShaderInfo = new CShaderInfo();
		std::wstring fn(pFilename);
		pShaderInfo->LoadShaderDef(fn);
		*ppShaderInfo = pShaderInfo;
		(*ppShaderInfo)->AddRef();
	}
}
