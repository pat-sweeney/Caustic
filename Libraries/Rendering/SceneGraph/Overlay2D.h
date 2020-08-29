//**********************************************************************
// Copyright Patrick Sweeney 2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "SceneGraph.h"
#include <functional>

namespace Caustic
{
	//**********************************************************************
	// Class: CSceneOverlay2DElem
	// Defines a simple quad draw in screen coordinates
	//**********************************************************************
	class CSceneOverlay2DElem :
		public CSceneElem,
		public ISceneOverlay2DElem,
		public CRefCount
	{
	protected:
		BBox2 m_bounds;
		CRefObj<ITexture> m_spTexture;
		CComPtr<ID3D11Buffer> m_spQuadVB;
		CComPtr<ID3D11Buffer> m_spQuadIB;
		CRefObj<IShader> m_spShader;
		CRefObj<ISampler> m_spSampler;
		bool m_initialized;

		void CreateQuad(IRenderer* pRenderer);
	public:
		CSceneOverlay2DElem(IShader* pShader) : m_initialized(false) { m_spShader = pShader; }

		//**********************************************************************
		// ISerialize
		//**********************************************************************
		virtual void Load(IStream* pStream) override {}
		virtual void Store(IStream* pStream) override {}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ISceneElem
		//**********************************************************************
		virtual ESceneElemType GetType() override { return ESceneElemType::Overlay2D; }
		virtual std::wstring GetName() override { return CSceneElem::GetName(); }
		virtual void SetName(const wchar_t* name) override { return CSceneElem::SetName(name); }
		virtual void SetPreRenderCallback(std::function<bool(int pass)> prerenderCallback) override
		{
			CSceneElem::SetPreRenderCallback(prerenderCallback);
		}
		virtual void SetPostRenderCallback(std::function<void(int pass)> postrenderCallback) override
		{
			CSceneElem::SetPostRenderCallback(postrenderCallback);
		}
		virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx) override;
		virtual void GetBBox(BBox3* pBBox) override {}
		virtual uint32 GetFlags() override { return m_Flags; }
		virtual void SetFlags(uint32 flags) override { m_Flags = flags; }

		//**********************************************************************
		// ISceneOverlay2DElem
		//**********************************************************************
		virtual void SetRect(BBox2& bbox) override { m_bounds = bbox; }
		virtual void SetTexture(ITexture* pTexture) override;
		virtual void SetShader(IShader* pShader) override
		{
			m_spShader = pShader;
		}
	};
}
