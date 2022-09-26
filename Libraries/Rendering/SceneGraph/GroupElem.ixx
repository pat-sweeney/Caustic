//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>

export module Rendering.SceneGraph.SceneGroupElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.Vector;
import Base.Math.Ray;
import Base.Math.Vector;
import Base.Math.BBox;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneGroupElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneGroupElem
	// Defines a simple group of scene graph elements
    //**********************************************************************
    class CSceneGroupElem : 
        public CSceneElem, 
        public ISceneGroupElem, 
        public CRefCount
    {
    protected:
		std::vector<CRefObj<ISceneElem>> m_Children;
		Matrix4x4 m_Transform;
    public:
		CSceneGroupElem() {}
		//**********************************************************************
		// Method: Load
		// Loads a scene element from a stream
		//**********************************************************************
		virtual void Load(IStream *pStream, std::function<void(ISceneElem *pElem)> func);

		//**********************************************************************
		// Method: Store
		// Saves a scene element to a stream
		//**********************************************************************
		virtual void Store(IStream *pStream, std::function<void(ISceneElem *pElem)> func);

		//**********************************************************************
		// IRefCount
		//**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ISceneElem
		//**********************************************************************
		virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
		{
			auto spGroup = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Group", nullptr);
			auto spBase = CSceneElem::AsJson(nullptr, pParser);
			spGroup->AddElement(spBase);
			for (size_t i = 0; i < m_Children.size(); i++)
			{
				auto spChild = m_Children[i]->AsJson(nullptr, pParser);
				spGroup->AddElement(spChild);
			}
			return spGroup;
		}

		virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** ppMaterial) override
		{
			bool found = false;
			for (size_t i = 0; i < m_Children.size(); i++)
			{
				RayIntersect3 ri;
				if (m_Children[i]->RayIntersect(ray, &ri, ppMaterial))
				{
					if (ri.hitTime < pIntersection->hitTime)
					{
						found = true;
						*pIntersection = ri;
					}
				}
			}
			return found;
		}
		virtual ESceneElemType GetType() override { return ESceneElemType::Group; }
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
		virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) override;
		virtual void GetBBox(BBox3 *pBBox) override;
		virtual uint32 GetFlags() override { return m_Flags; }
		virtual void SetFlags(uint32 flags) override { m_Flags = flags; }
		virtual void SetInPass(uint32 pass) override { CSceneElem::SetInPass(pass); }
		virtual uint32 GetInPass() override { return CSceneElem::GetInPass(); }

		//**********************************************************************
		// ISerialize
		//**********************************************************************
		virtual void Load(IStream *pStream) override;
		virtual void Store(IStream *pStream) override;

		//**********************************************************************
		// ISceneGroupElem
		//**********************************************************************
		virtual uint32 NumberChildren() override;
		virtual CRefObj<ISceneElem> GetChild(uint32 index) override;
		virtual void AddChild(ISceneElem *pElem) override;
		virtual void InsertChild(ISceneElem *pElem, uint32 index) override;
		virtual Matrix4x4 GetTransform() override { return m_Transform; }
		virtual void SetTransform(Matrix4x4 &transform) override
		{
			m_Transform = transform;
			SetFlags(GetFlags() | ESceneElemFlags::BBoxDirty);
		}
    };
}
