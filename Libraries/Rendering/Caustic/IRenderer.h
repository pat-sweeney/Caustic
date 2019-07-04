//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "IRenderMesh.h"
#include <Windows.h>
#include <d3d12.h>
#include <functional>

namespace Caustic
{
	struct IPointLight;
	struct IRenderCtx;
	struct ICamera;
	struct IShaderMgr;
    struct ITextureMgr;

	//**********************************************************************
	// Interface: IRenderer
	// Defines the interface clients use to talk to the renderer
	//**********************************************************************
	struct IRenderer : public IRefCount
	{
		virtual DXGI_SAMPLE_DESC GetSampleDesc() = 0;
		virtual DXGI_FORMAT GetFormat() = 0;
		virtual CComPtr<ID3D12RootSignature> GetRootSignature() = 0;
		virtual CComPtr<ID3D12Device5> GetDevice() = 0;
		virtual CRefObj<ICamera> GetCamera() = 0;
        virtual CRefObj<ITextureMgr> GetTextureMgr() = 0;
		virtual uint32 GetFrameIndex() = 0;
		virtual CComPtr<ID3D12GraphicsCommandList4> GetCommandList() = 0;
		virtual void SetCamera(ICamera *pCamera) = 0;
		virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) = 0;
		virtual CRefObj<IShaderMgr> GetShaderMgr() = 0;
		virtual void BeginFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) = 0;
		virtual void EndFrame() = 0;
		virtual void RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) = 0;
		virtual void GetRenderCtx(IRenderCtx **ppCtx) = 0;
		virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) = 0;
		virtual void DrawMesh(IRenderMesh *pMesh) = 0;
	};
}
