//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "SceneGraph.h"
#include "Rendering\Caustic\Caustic.h"
#include "GroupElem.h"

namespace Caustic
{
	//**********************************************************************
	// Class: CSceneMaterialElem
	// Defines a material that can be assigned to a scene element
	//**********************************************************************
	class CSceneMaterialElem :
        public CSceneGroupElem,
        public ISceneMaterialElem
    {
        CRefObj<IMaterialAttrib> m_spMaterial;
        CRefObj<IShader> m_spShader;
    public:
		//**********************************************************************
		// Constructor: CSceneMaterialElem
		// Default constructor
		//**********************************************************************
		CSceneMaterialElem()
        {
        }

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ISceneElem
		//**********************************************************************
		virtual ESceneElemType GetType() { return ESceneElemType::Material; }
        virtual std::wstring &Name() override;
		virtual void SetPreRenderCallback(std::function<void()> prerenderCallback) override
		{
			CSceneElem::SetPreRenderCallback(prerenderCallback);
		}
		virtual void SetPostRenderCallback(std::function<void()> postrenderCallback) override
		{
			CSceneElem::SetPostRenderCallback(postrenderCallback);
		}
		virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) override;
        virtual void GetBBox(BBox3 *pBBox) override {}
        virtual uint32 GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32 flags) override { m_Flags = flags; }
        
		//**********************************************************************
		// ISerialize
		//**********************************************************************
		virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;

		//**********************************************************************
		// ISceneGroupElem
		//**********************************************************************
		virtual uint32 NumberChildren() override { return CSceneGroupElem::NumberChildren(); }
        virtual CRefObj<ISceneElem> GetChild(uint32 index) override { return CSceneGroupElem::GetChild(index); }
        virtual void AddChild(ISceneElem *pElem) override { CSceneGroupElem::AddChild(pElem); }
        virtual void InsertChild(ISceneElem *pElem, uint32 index) override { CSceneGroupElem::InsertChild(pElem, index); }
        virtual void SetTransform(Matrix4x4 &transform) override { m_Transform = transform; }

		//**********************************************************************
		// ISceneMaterialElem
		//**********************************************************************
		virtual void SetShader(IShader *pShader) override
        {
            m_spShader = pShader;
        }

		virtual void GetMaterial(IMaterialAttrib **ppMaterial) override
		{
			*ppMaterial = m_spMaterial;
			if (m_spMaterial)
				m_spMaterial->AddRef();
		}

		virtual void SetMaterial(IMaterialAttrib *pMaterial) override
		{
			m_spMaterial = pMaterial;
		}
	};
};
