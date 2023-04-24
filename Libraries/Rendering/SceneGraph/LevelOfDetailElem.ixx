//**********************************************************************
// Copyright Patrick Sweeney 2022-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <string>

export module Rendering.SceneGraph.SceneLevelOfDetailElem;
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
import Rendering.SceneGraph.ISceneLevelOfDetailElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneLevelOfDetailElem
    // Defines a scene graph element for handling geometry-base
    // level of detail rendering
    //**********************************************************************
    class CSceneLevelOfDetailElem :
        public CSceneElem,
        public ISceneLevelOfDetailElem,
        public CRefCount
    {
    protected:
        struct LODChild
        {
            CRefObj<ISceneElem> m_spChild;
            float m_minRenderDistance;
            float m_maxRenderDistance;
        };
        std::vector<LODChild> m_children;
    public:
        CSceneLevelOfDetailElem() {}

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override;
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** ppMaterial) override;
        virtual ESceneElemType GetType() override { return ESceneElemType::LevelOfDetail; }
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
        virtual void Load(IStream* pStream) override;
        virtual void Store(IStream* pStream) override;

        //**********************************************************************
        // ISceneLevelOfDetailElem
        //**********************************************************************
        virtual uint32 NumberChildren() override { return (uint32)m_children.size(); }
        virtual CRefObj<ISceneElem> GetChild(uint32 index) override { return m_children[index].m_spChild; }
        virtual void AddChild(ISceneElem* pElem, float minRenderDistance, float maxRenderDistance) override;
        virtual void SetMinRenderDistance(int childIndex, float dist) { m_children[childIndex].m_minRenderDistance = dist; }
        virtual void SetMaxRenderDistance(int childIndex, float dist) { m_children[childIndex].m_maxRenderDistance = dist; }
        virtual float GetMinRenderDistance(int childIndex) { return m_children[childIndex].m_minRenderDistance; }
        virtual float GetMaxRenderDistance(int childIndex) { return m_children[childIndex].m_maxRenderDistance; }
    };
}
