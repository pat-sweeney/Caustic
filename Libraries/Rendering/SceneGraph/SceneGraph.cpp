//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <float.h>
#include <math.h>
#include <cstdlib>
#include <Windows.h>
#include <vector>

module Rendering.SceneGraph.SceneGraph;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.SceneGraph.ISceneGraph;
import Rendering.SceneGraph.ISceneElem;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;

namespace Caustic
{
    void CSceneGraph::PathTraceInternal(PathTraceCtx* pCtx, int depth, Ray3& ray, FRGBColor* pRadiance)
    {
        pRadiance->r = 0.0f;
        pRadiance->g = 0.0f;
        pRadiance->b = 0.0f;
        if (depth == pCtx->maxDepth)
        {
            // Perform Russian roulette to determine if we should stop this ray
            return;
        }
        FRGBColor radiance(0.0f, 0.0f, 0.0f);
        RayIntersect3 ri;
        CRefObj<IMaterialAttrib> spMaterial;
        if (m_spRoot->RayIntersect(ray, &ri, &spMaterial))
        {
            // Computer incoming radiance
            FRGBColor incomingRadiance;
            PathTraceInternal(pCtx, depth + 1, ray, &incomingRadiance);
            float dp = ray.dir.Dot(ri.hitNorm);
            FRGBColor clr = spMaterial->GetColor(L"diffuse");
            pRadiance->r = dp * incomingRadiance.r * clr.r;
            pRadiance->g = dp * incomingRadiance.g * clr.g;
            pRadiance->b = dp * incomingRadiance.b * clr.b;
        }
    }
    
    void CSceneGraph::PathTrace(IRenderer *pRenderer, PathTraceCtx* pCtx, IImage* pDest)
    {
        if (pRenderer == nullptr)
            return;
        auto spCamera = pRenderer->GetCamera();
        uint32_t w = pDest->GetWidth();
        uint32_t h = pDest->GetHeight();
        Caustic::CImageIter24 rowIter(pDest, 0, 0);
        for (uint32_t y = 0; y < h; y++)
        {
            Caustic::CImageIter24 colIter = rowIter;
            for (uint32_t x = 0; x < w; x++)
            {
                Ray3 ray;
                FRGBColor finalClr(0.0f, 0.0f, 0.0f);
                for (int i = 0; i < pCtx->spp; i++)
                {
                    Vector3 eye, look, up, u, v, n;
                    spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
                    ray.pos = eye;
                    float fov;
                    float aspectRatio;
                    float nearZ;
                    float farZ;
                    spCamera->GetParams(&fov, &aspectRatio, &nearZ, &farZ);
                    float focalLen = 1.0f / (float)tan(fov);
                    float jx = float(rand()) / float(RAND_MAX) - 0.5f;
                    float jy = float(rand()) / float(RAND_MAX) - 0.5f;
                    float pu = 2.0f * (float(x + jx) / float(w)) - 1.0f;
                    float pv = 2.0f * (float(y + jy) / float(h)) - 1.0f;
                    ray.dir = eye + pu * u + pv * v + focalLen * n;
                    ray.dir.Normalize();

                    FRGBColor radiance;
                    PathTraceInternal(pCtx, 0, ray, &radiance);
                    finalClr.r += radiance.r;
                    finalClr.g += radiance.g;
                    finalClr.b += radiance.b;
                }
                finalClr.r /= (float)pCtx->spp;
                finalClr.g /= (float)pCtx->spp;
                finalClr.b /= (float)pCtx->spp;
                colIter.SetRed(uint8_t(finalClr.r * 255.0f));
                colIter.SetGreen(uint8_t(finalClr.g * 255.0f));
                colIter.SetBlue(uint8_t(finalClr.b * 255.0f));
                colIter.Step(Caustic::CImageIter24::EStepDirection::Right);
            }
            rowIter.Step(Caustic::CImageIter24::EStepDirection::Down);
        }
    }

    void CSceneGraph::Lock()
    {
        EnterCriticalSection(&m_cs);
        m_locked = true;
    }

    void CSceneGraph::Unlock()
    {
        m_locked = false;
        LeaveCriticalSection(&m_cs);
    }

    uint32_t CSceneGraph::NumberChildren()
    {
        return m_spRoot->NumberChildren();
    }

    CRefObj<ISceneElem> CSceneGraph::GetChild(uint32_t index)
    {
        return m_spRoot->GetChild(index);
    }

    void CSceneGraph::Merge(ISceneGraph *pSceneGraph)
    {
        CSceneGraph *pGraph = (CSceneGraph*)pSceneGraph;
        AddChild(pGraph->m_spRoot);
        pGraph->m_spRoot = nullptr;
    }

    void CSceneGraph::ClearSelected()
    {
        for (auto pElem : m_Selected)
            pElem->SetFlags(pElem->GetFlags() & ~ESceneElemFlags::Selected);
        m_Selected.clear();
    }

    void CSceneGraph::SelectObject(ISceneElem* pElem)
    {
        pElem->SetFlags(pElem->GetFlags() | ESceneElemFlags::Selected);
        CRefObj<ISceneElem> obj(pElem);
        m_Selected.push_back(obj);
    }

    void CSceneGraph::DeselectObject(ISceneElem* pElem)
    {
        pElem->SetFlags(pElem->GetFlags() & ~ESceneElemFlags::Selected);
        for (size_t i = 0; i < m_Selected.size(); i++)
        {
            if (m_Selected[i] == pElem)
            {
                m_Selected.erase(m_Selected.begin() + i);
                break;
            }
        }
    }

    void CSceneGraph::AddChild(ISceneElem *pElem)
    {
        m_spRoot->AddChild(pElem);
        SetFlags(GetFlags() | ESceneElemFlags::BBoxDirty);
    }

    void CSceneGraph::InsertChild(ISceneElem *pElem, uint32_t index)
    {
        return m_spRoot->InsertChild(pElem, index);
    }

    Matrix4x4 CSceneGraph::GetTransform()
    {
        return m_spRoot->GetTransform();
    }

    void CSceneGraph::SetTransform(Matrix4x4 &mat)
    {
        return m_spRoot->SetTransform(mat);
    }

    void CSceneGraph::GetBBox(BBox3* pBBox)
    {
        m_spRoot->GetBBox(pBBox);
    }

    void CSceneGraph::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        if (!(m_passes & pRenderCtx->GetCurrentPass()))
            return;
        Lock();
#ifdef _DEBUG
        pRenderer->BeginMarker(L"SceneGraph::Render");
#endif
        if (m_prerenderCallback)
            if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
            {
                Unlock();
                return;
            }
        m_spRoot->Render(pRenderer, pRenderCtx, pSceneCtx);
        if (m_postrenderCallback)
            m_postrenderCallback(pRenderCtx->GetCurrentPass());
#ifdef _DEBUG
        pRenderer->EndMarker();
#endif
        Unlock();
    }

    void CSceneGraph::Store(IStream *pStream)
    {
        m_spRoot->Store(pStream);
    }

    void CSceneGraph::Load(IStream *pStream)
    {
        m_spRoot->Load(pStream);
        SetFlags(GetFlags() | ESceneElemFlags::RenderableDirty | ESceneElemFlags::BBoxDirty);
    }
};
