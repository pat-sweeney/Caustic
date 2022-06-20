//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>

export module Rendering.SceneGraph.SceneMeshElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.IShader;
import Geometry.Mesh.IMesh;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneMeshElem;

export namespace Caustic
{
	//**********************************************************************
	// Class: CSceneMeshElem
	// Defines a mesh element in our scene graph
	//**********************************************************************
	class CSceneMeshElem :
        public CSceneElem,
        public ISceneMeshElem,
        public CRefCount
    {
        CRefObj<IMesh> m_spMesh;
        CRefObj<IRenderMesh> m_spRenderMesh;
        CRefObj<IShader> m_spShader;
    public:
		//**********************************************************************
		// Constructor: CSceneMeshElem
		// Default constructor
		//**********************************************************************
		CSceneMeshElem()
        {
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
            auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Mesh", nullptr);
            auto spBase = CSceneElem::AsJson(pPropertyName, pParser);
            spObj->AddElement(spBase);
            return spObj;
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override;
        virtual ESceneElemType GetType() override { return ESceneElemType::Mesh; }
        virtual std::wstring GetName() override { return CSceneElem::GetName(); }
        virtual void SetName(const wchar_t *name) override { return CSceneElem::SetName(name); }
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
        // ISceneMeshElem
        //**********************************************************************
        virtual void SetMesh(IMesh *pMesh) override;
        virtual void SetShader(IShader* pShader) override { m_spShader = pShader; }
    };
};
