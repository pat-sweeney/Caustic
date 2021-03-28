//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "SceneGraph.h"
#include <d3d11.h>
#include <atlbase.h>

namespace Caustic
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
        std::function<float(Vector3&)> m_sdf;
        uint32 m_subdivisions;
        float* m_sdfData;

    public:
        //**********************************************************************
        // Constructor: CSceneMarchingCubesElem
        // Default ctor
        //
        // Parameters:
        // pComputeShader - underlying compute shader
        //**********************************************************************
        CSceneMarchingCubesElem(IRenderer* pRenderer, uint32 subdivisions, std::function<float(Vector3&)> sdf);

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
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
        virtual uint32 GetFlags() override { return CSceneElem::GetFlags(); }
        virtual void SetFlags(uint32 flags) override { CSceneElem::SetFlags(flags); }
        virtual void SetInPass(uint32 pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32 GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { CT(E_NOTIMPL); }
        virtual void Store(IStream* pStream) override { CT(E_NOTIMPL); }

        //**********************************************************************
        // ISceneMarchingCubesElem
        //**********************************************************************
        virtual CRefObj<IShader> GetShader() override { return CRefObj<IShader>(nullptr); }
        virtual void SetSubdivisions(uint32 numCells) override { m_subdivisions = numCells; }
        virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) override;
        virtual void SetShaderParam(const wchar_t* pParamName, float value) override;
        virtual void SetSignedDistanceFunction(std::function<float(Vector3& v)> fn) override { m_sdf = fn; };
    };
}
