//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include <d3d11.h>
#include <DirectXMath.h>

//**********************************************************************
// File: CausticFactory.h
// Contains the declaration for our <CCausticFactory> class.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Class: CCausticFactory
    // Implements <ICausticFactory>
    //
    // Members:
    // factory - The CausticFactory singleton
    //**********************************************************************
    class CCausticFactory : public ICausticFactory, public CRefCount
	{
		static CRefObj<ICausticFactory> factory;
	public:
		CCausticFactory();
		~CCausticFactory();

        //**********************************************************************
        // Method: Instance()
        // Returns the CCausticFactory singleton.
        //**********************************************************************
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
		virtual void CreateRenderer(HWND hwnd, IRenderer **ppRenderer) override;
		virtual void CreateGraphics(HWND hwnd, IGraphics **ppGraphics) override;
		virtual void CreatePointLight(Vector3 &pos, IPointLight **ppLight) override;
		virtual void CreateTrackball(ITrackball **ppTrackball) override;
		virtual void CreateRendererMarshaller(IRendererMarshaller **ppMarshaller) override;
		virtual void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial) override;
		virtual void CreateRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, IShader *pShader, IRenderable **ppRenderable) override;
		virtual void CreateRenderable(IRenderable **ppRenderable) override;
		virtual void CreateRenderable(ID3D11Buffer *pVB, uint32 numVertices, ID3D11Buffer *pIB, uint32 numIndices, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable) override;
		virtual void CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler) override;
		virtual void CreateCamera(bool leftHanded, ICamera **ppCamera) override;
		virtual void CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture) override;
		virtual CRefObj<ITexture> CheckerboardTexture(IGraphics *pGraphics) override;
		virtual void LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture) override;
		virtual void LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture) override;
		virtual void MeshToD3D(IGraphics *pGraphics, ISubMesh *pMesh,
			int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
			int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
			BBox3 *pBbox, uint32 *pVertexSize) override;
		virtual void MeshToNormals(IGraphics *pGraphics, ISubMesh *pSubMesh,
			ID3D11Buffer **ppVB, uint32 *pNumVerts) override;
		virtual void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion) override;
		virtual void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion) override;
        virtual void CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
            ID3DBlob *pVertexShaderBlob, ID3DBlob *pPixelShaderBlob, IShaderInfo *pShaderInfo,
            IShader **ppShader) override;
        virtual void CreateShaderInfo(const wchar_t *pFilename, IShaderInfo **ppShaderInfo) override;
    };
};
