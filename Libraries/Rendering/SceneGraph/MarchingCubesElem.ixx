//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <functional>
#include <string>

export module Rendering.SceneGraph.SceneMarchingCubesElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.Vector;
import Base.Math.Ray;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneMarchingCubesElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneMarchingCubesElem
    // Defines a node that creates and renders a mesh from a signed distance
    // function using the marching cubes algorithm running on the GPU.
    //**********************************************************************
    class CSceneMarchingCubesElem :
        public CSceneElem,
        public ISceneMarchingCubesElem,
        public CRefCount
    {
        CRefObj<IShader> m_spMCCountVertsShader;
        CRefObj<IShader> m_spMCAllocVertsShader;
        CRefObj<IShader> m_spMCEmitVertsShader;
        CRefObj<IShader> m_spMCShader;
        std::function<float(Vector3&)> m_sdf;
        uint32_t m_subdivisions;          // Original voxel grid size request by client
        uint32_t m_xRoundedSubdivisions;  // Voxel grid size rounded up to nearest multiple of thread group size
        uint32_t m_yRoundedSubdivisions;  // Voxel grid size rounded up to nearest multiple of thread group size
        uint32_t m_zRoundedSubdivisions;  // Voxel grid size rounded up to nearest multiple of thread group size
        uint32_t m_numGroupsX;            // Number of groups in X direction for Dispatch
        uint32_t m_numGroupsY;            // Number of groups in Y direction for Dispatch
        uint32_t m_numGroupsZ;            // Number of groups in Z direction for Dispatch
        float* m_sdfData;
        bool m_initialized;
        bool m_drawIndexed;

    public:
        //**********************************************************************
        // Constructor: CSceneMarchingCubesElem
        // Default ctor
        //
        // Parameters:
        // pComputeShader - underlying compute shader
        //**********************************************************************
        CSceneMarchingCubesElem(IRenderer* pRenderer, uint32_t subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed = false);

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
            auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "MarchingCubes", nullptr);
            auto spBase = CSceneElem::AsJson("Base", pParser);
            spObj->AddElement(spBase);
//            spObj->AddElement(m_spMCCountVertsShader->AsJson("CountVertsShader", pParser));
//            spObj->AddElement(m_spMCAllocVertsShader->AsJson("AllocVertsShader", pParser));
//            spObj->AddElement(m_spMCEmitVertsShader->AsJson("EmitVertsShader", pParser));
//            spObj->AddElement(m_spMCShader->AsJson("Shader", pParser));
            spObj->AddElement(pParser->CreateJSon("Subdivisions", (int)m_subdivisions));
            spObj->AddElement(pParser->CreateJSon("xRoundedSubdivisions", (int)m_xRoundedSubdivisions));
            spObj->AddElement(pParser->CreateJSon("yRoundedSubdivisions", (int)m_yRoundedSubdivisions));
            spObj->AddElement(pParser->CreateJSon("zRoundedSubdivisions", (int)m_zRoundedSubdivisions));
            spObj->AddElement(pParser->CreateJSon("numGroupsX", (int)m_numGroupsX));
            spObj->AddElement(pParser->CreateJSon("numGroupsY", (int)m_numGroupsY));
            spObj->AddElement(pParser->CreateJSon("numGroupsZ", (int)m_numGroupsZ));
            return spObj;
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::ComputeShaderElem; }
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
        virtual void GetBBox(BBox3* pBBox) { CSceneElem::GetBBox(pBBox); }
        virtual uint32_t GetFlags() override { return CSceneElem::GetFlags(); }
        virtual void SetFlags(uint32_t flags) override { CSceneElem::SetFlags(flags); }
        virtual void SetInPass(uint32_t pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32_t GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { CT(E_NOTIMPL); }
        virtual void Store(IStream* pStream) override { CT(E_NOTIMPL); }

        //**********************************************************************
        // ISceneMarchingCubesElem
        //**********************************************************************
        virtual CRefObj<IShader> GetShader() override { return CRefObj<IShader>(nullptr); }
        virtual void SetSubdivisions(uint32_t numCells) override { m_subdivisions = numCells; }
        virtual void SetShaderParam(const wchar_t* pParamName, uint32_t value) override;
        virtual void SetShaderParam(const wchar_t* pParamName, float value) override;
        virtual void SetSignedDistanceFunction(std::function<float(Vector3& v)> fn) override { m_sdf = fn; };
    };
}
