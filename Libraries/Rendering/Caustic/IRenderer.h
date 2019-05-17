//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include <Windows.h>
#include <d3d12.h>

namespace Caustic
{
	struct IPointLight;
	struct IRenderCtx;
	struct ICamera;
	struct IShaderMgr;

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
		virtual uint32 GetFrameIndex() = 0;
		virtual CComPtr<ID3D12GraphicsCommandList4> GetCommandList() = 0;
		virtual void SetCamera(ICamera *pCamera) = 0;
		virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) = 0;
		virtual CRefObj<IShaderMgr> GetShaderMgr() = 0;
		virtual void RenderFrame() = 0; // Have renderer draw and present next frame
		virtual void AddPointLight(IPointLight *pLight) = 0;
		virtual void GetRenderCtx(IRenderCtx **ppCtx) = 0;
		virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) = 0;
	};
}