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
import Parsers.JSon.IJSonParser;
import Rendering.SceneGraph.SceneCtx;

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
        uint32_t m_Flags;
        BBox3 m_BBox;
        uint32_t m_passes; // List of passes this element is rendered in
        std::function<bool(int pass)> m_prerenderCallback;
        std::function<void(int pass)> m_postrenderCallback;

        void DrawSelected(IRenderer* pRenderer, ISceneElem* pElem, SceneCtx* pSceneCtx);
    public:
        CSceneElem() :
            m_Flags(DepthTested),
            m_passes(c_PassOpaque | c_PassShadow | c_PassTransparent | c_PassEnvironment)
        {
        }

        //**********************************************************************
        // ISceneElem base implementation
        //**********************************************************************
        CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser)
        {
            return pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Base",
                pParser->CreateJSon("Type", "BaseSceneElem"),
                pParser->CreateJSon("Name", m_Name.c_str()),
                pParser->CreateJSon("Flags", (int)m_Flags),
                pParser->CreateJSonMap("BBox",
                    pParser->CreateJSonMap("MinPt",
                        pParser->CreateJSon("x", m_BBox.minPt.x),
                        pParser->CreateJSon("y", m_BBox.minPt.y),
                        pParser->CreateJSon("z", m_BBox.minPt.z),
                        nullptr),
                    pParser->CreateJSonMap("MaxPt",
                        pParser->CreateJSon("x", m_BBox.maxPt.x),
                        pParser->CreateJSon("y", m_BBox.maxPt.y),
                        pParser->CreateJSon("z", m_BBox.maxPt.z),
                        nullptr),
                    nullptr),
                pParser->CreateJSon("Passes", (int)m_passes),
                nullptr);
        }
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
        uint32_t GetFlags() { return m_Flags; }
        void SetFlags(uint32_t flags) { m_Flags = flags; }
        void SetInPass(uint32_t pass) { m_passes = pass; }
        uint32_t GetInPass() { return m_passes; }
    };
}
