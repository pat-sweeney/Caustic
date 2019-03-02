//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "SceneGraphImpl.h"
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
        CRefObj<IShader> m_spVertexShader;
        CRefObj<IShader> m_spPixelShader;
    public:
		//**********************************************************************
		// Constructor: CSceneMaterialElem
		// Default constructor
		//**********************************************************************
		CSceneMaterialElem()
        {
        }

	//======================================================================
	// IUnknown
	//======================================================================

		//**********************************************************************
		// Method: AddRef
		// Increments the reference count
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }

		//**********************************************************************
		// Method: Release
		// Decrements the reference count
		//**********************************************************************
		virtual uint32 Release() override { return CRefCount::Release(); }

	//======================================================================
	// ISceneElem
	//======================================================================
		virtual ESceneElemType GetType() { return ESceneElemType::Material; }
        virtual std::wstring &Name() override;
        virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) override;
        virtual void GetBBox(BBox3 *pBBox) override {}
        virtual uint32 GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32 flags) override { m_Flags = flags; }
        
	//======================================================================
	// ISerialize
	//======================================================================
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;

	//======================================================================
	// ISceneGroupElem
	//======================================================================
		virtual uint32 NumberChildren() override { return CSceneGroupElem::NumberChildren(); }
        virtual CRefObj<ISceneElem> GetChild(uint32 index) override { return CSceneGroupElem::GetChild(index); }
        virtual void AddChild(ISceneElem *pElem) override { CSceneGroupElem::AddChild(pElem); }
        virtual void InsertChild(ISceneElem *pElem, uint32 index) override { CSceneGroupElem::InsertChild(pElem, index); }
        virtual void SetTransform(Matrix4x4 &transform) override { m_Transform = transform; }

	//======================================================================
	// ISceneMaterialElem
	//======================================================================
		virtual void SetPixelShader(IShader *pShader) override
        {
            m_spPixelShader = pShader;
        }

        virtual void SetVertexShader(IShader *pShader) override
        {
            m_spVertexShader = pShader;
        }

		virtual void GetMaterial(IMaterialAttrib **ppMaterial) override
		{
			*ppMaterial = m_spMaterial.p;
			if (m_spMaterial.p)
				m_spMaterial->AddRef();
		}

		virtual void SetMaterial(IMaterialAttrib *pMaterial) override
		{
			m_spMaterial = pMaterial;
		}
	};
};
