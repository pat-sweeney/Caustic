//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "SceneGraph.h"
#include <functional>

namespace Caustic
{
	//**********************************************************************
	// Class: CSceneLightCollectionElem
	// Defines a group of lights that effect all its children
	//**********************************************************************
	class CSceneLightCollectionElem :
		public CSceneGroupElem,
		public ISceneLightCollectionElem
	{
		std::vector<CRefObj<ILight>> m_lights;
	public:
		CSceneLightCollectionElem()
		{
			m_passes = c_PassAllPasses;
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ISceneElem
		//**********************************************************************
		virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
		virtual ESceneElemType GetType() override { return ESceneElemType::LightCollection; }
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
		virtual void GetBBox(BBox3* pBBox) override;
		virtual uint32 GetFlags() override { return m_Flags; }
		virtual void SetFlags(uint32 flags) override { m_Flags = flags; }
		virtual void SetInPass(uint32 pass) override { CSceneElem::SetInPass(pass); }
		virtual uint32 GetInPass() override { return CSceneElem::GetInPass(); }

		//**********************************************************************
		// ISerialize
		//**********************************************************************
		virtual void Load(IStream* pStream) override
		{
		}

		virtual void Store(IStream* pStream) override
		{
		}

		//**********************************************************************
		// ISceneGroupElem
		//**********************************************************************
		virtual uint32 NumberChildren() override { return CSceneGroupElem::NumberChildren(); }
		virtual CRefObj<ISceneElem> GetChild(uint32 index) override { return CSceneGroupElem::GetChild(index); }
		virtual void AddChild(ISceneElem* pElem) override { return CSceneGroupElem::AddChild(pElem); }
		virtual void InsertChild(ISceneElem* pElem, uint32 index) override { return CSceneGroupElem::InsertChild(pElem, index); }
		virtual void SetTransform(Matrix4x4& transform) override { m_Transform = transform; }

		//**********************************************************************
		// ISceneLightCollectionElem
		//**********************************************************************
		virtual uint32 NumberLights() override;
		virtual CRefObj<ILight> GetLight(int index) override;
		virtual void AddLight(ILight* pLight) override;
		virtual void RemoveLight(ILight* pLight) override;
	};
}
