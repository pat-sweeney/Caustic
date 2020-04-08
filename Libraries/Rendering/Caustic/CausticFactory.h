//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
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
        // Method: Instance
        // Returns:
        // Returns the CCausticFactory singleton.
        //**********************************************************************
        static CRefObj<ICausticFactory> Instance()
		{
			if (factory == nullptr)
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
		virtual void CreateGraphics(HWND hwnd, IGraphics **ppGraphics) override;
		virtual void CreateRenderMesh(IRenderMesh **ppRenderMesh) override;
		virtual void CreateRenderSubMesh(IRenderSubMesh **ppRenderSubMesh) override;
		virtual void CreatePointLight(Vector3 &pos, IPointLight **ppLight) override;
		virtual void CreateTrackball(ITrackball **ppTrackball) override;
		virtual void CreateRendererMarshaller(IRendererMarshaller **ppMarshaller) override;
		virtual void CreateMaterialAttrib(IMaterialAttrib** ppMaterialAttrib) override;
		virtual void CreateRenderMaterial(IGraphics *pGraphics, IMaterialAttrib *pMaterialAttrib, IShader *pShader, IRenderMaterial **ppRenderMaterial) override;
        virtual void CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat, IRenderable **ppRenderable) override;
        virtual void CreateSampler(IGraphics *pGraphics, ITexture *pTexture, ISampler **ppSampler) override;
		virtual void CreateCamera(bool leftHanded, ICamera **ppCamera) override;
		virtual void CreateTexture(IGraphics *pGraphics, uint32 width, uint32 height, DXGI_FORMAT format, uint32 cpuFlags, uint32 bindFlags, ITexture **ppTexture) override;
		virtual CRefObj<ITexture> CheckerboardTexture(IGraphics *pGraphics) override;
		virtual void LoadTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture) override;
		virtual void LoadVideoTexture(const wchar_t *pFilename, IGraphics *pGraphics, ITexture **ppTexture) override;
        virtual void CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
            ID3DBlob *pVertexShaderBlob, ID3DBlob *pPixelShaderBlob, IShaderInfo *pShaderInfo,
            IShader **ppShader) override;
        virtual void CreateShaderInfo(const wchar_t *pFilename, IShaderInfo **ppShaderInfo) override;
    };
};
