//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <functional>
#include <windows.h>

export module Rendering.SceneGraph.SceneElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Base.Math.Ray;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.RendererFlags;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneElem
    // Defines the base class for all our scene graph elements
    //**********************************************************************
    class CSceneElem
    {
    protected:
        std::wstring m_Name; // User provided name for this element
        uint32 m_Flags;
        BBox3 m_BBox;
        uint32 m_passes; // List of passes this element is rendered in
        std::function<bool(int pass)> m_prerenderCallback;
        std::function<void(int pass)> m_postrenderCallback;

        void DrawSelected(IRenderer* pRenderer, ISceneElem* pElem, SceneCtx* pSceneCtx);
    public:
        CSceneElem() :
            m_Flags(DepthTested),
            m_passes(c_PassOpaque)
        {
        }

        //**********************************************************************
        // ISceneElem base implementation
        //**********************************************************************
        ESceneElemType GetType() { return ESceneElemType::Unknown; }
        std::wstring GetName() { return m_Name; }
        void SetName(const wchar_t* name) { m_Name = name; }
        void SetPreRenderCallback(std::function<bool(int pass)> prerenderCallback)
        {
            m_prerenderCallback = prerenderCallback;
        }
        void SetPostRenderCallback(std::function<void(int pass)> postrenderCallback)
        {
            m_postrenderCallback = postrenderCallback;
        }
        void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)
        {
            CT(E_UNEXPECTED);
        }
        void GetBBox(BBox3* pBBox) { *pBBox = m_BBox; }
        uint32 GetFlags() { return m_Flags; }
        void SetFlags(uint32 flags) { m_Flags = flags; }
        void SetInPass(uint32 pass) { m_passes = pass; }
        uint32 GetInPass() { return m_passes; }
    };
}
