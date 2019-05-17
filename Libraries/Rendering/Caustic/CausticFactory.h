//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include <d3d12.h>
#include <DirectXMath.h>

namespace Caustic
{
	class CCausticFactory : public ICausticFactory, public CRefCount
	{
		static CRefObj<ICausticFactory> factory;
	public:
		CCausticFactory();
		~CCausticFactory();

		static CRefObj<ICausticFactory> Instance()
		{
			if (factory.p == nullptr)
			{
				factory = CRefObj<ICausticFactory>(new CCausticFactory);
			}
			return factory;
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ICausticFactory
		//**********************************************************************
		virtual void CreateRenderer(HWND hwnd, std::wstring &shaderFolder, IRenderer **ppRenderer) override;
		virtual void CreatePointLight(Vector3 &pos, IPointLight **ppLight) override;
		virtual void CreateTrackball(ITrackball **ppTrackball) override;
		virtual void CreateRenderMaterial(IRenderer *pRenderer, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial) override;
		virtual void CreateRenderable(ID3D12Resource *pVB, uint32 numVertices, ID3D12Resource *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable) override;
		virtual void CreateSampler(IRenderer *pRenderer, ITexture *pTexture, ISampler **ppSampler) override;
		virtual void CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName, ID3DBlob *pVertexShaderBlob, ID3DBlob *pPixelShaderBlob, IShaderInfo *pShaderInfo, IShader **ppShader) override;
		virtual void CreateShaderInfo(const wchar_t *pFilename, IShaderInfo **ppShaderInfo) override;
		virtual void CreateMaterial(Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, float specularExp, float alpha, IMaterialAttrib **ppMaterial) override;
		virtual void CreateMaterial(IMaterialAttrib **ppMaterial) override;
		virtual void CreateCamera(bool leftHanded, ICamera **ppCamera) override;
		virtual void CreateTexture(IRenderer *pRenderer, uint32 width, uint32 height, DXGI_FORMAT format, ITexture **ppTexture) override;
		virtual CRefObj<ITexture> CheckerboardTexture(IRenderer *pRenderer) override;
		virtual void LoadTexture(const wchar_t *pFilename, IRenderer *pRenderer, ITexture **ppTexture) override;
		virtual void LoadVideoTexture(const wchar_t *pFilename, IRenderer *pRenderer, ITexture **ppTexture) override;
//		virtual void MeshToD3D(IRenderer *pRenderer, ISubMesh *pMesh,
//			int vertexVersion, ID3D12Resource **ppVertexBuffer, uint32 *pNumVerts,
//			int indexVersion, ID3D12Resource **ppIndexBuffer, uint32 *pNumIndices,
//			BBox3 *pBbox, uint32 *pVertexSize) override;
//		virtual void MeshToNormals(IRenderer *pRenderer, ISubMesh *pSubMesh,
//			ID3D12Resource **ppVB, uint32 *pNumVerts) override;
//		virtual void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion) override;
//		virtual void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D12Device *pDevice, ID3D12Resource **ppIndexBuffer, uint32 *pNumIndices, ID3D12Resource **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion) override;
	};
};
