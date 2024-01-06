//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.SceneElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.RendererFlags;
import Rendering.SceneGraph.ISceneElem;

namespace Caustic
{
    void CSceneElem::DrawSelected(IRenderer* pRenderer, ISceneElem* pElem, SceneCtx* pSceneCtx)
    {
        // Draw manipulator around currently selected object
        if (pElem->GetFlags() & (uint32_t)ESceneElemFlags::Selected)
        {
            BBox3 bbox;
            pElem->GetBBox(&bbox);
            Vector4 minPt = bbox.minPt * pSceneCtx->m_Transform;
            Vector4 maxPt = bbox.maxPt * pSceneCtx->m_Transform;
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, minPt.z),
                Vector3(maxPt.x, minPt.y, minPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, maxPt.z),
                Vector3(maxPt.x, minPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, maxPt.y, minPt.z),
                Vector3(maxPt.x, maxPt.y, minPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, maxPt.y, maxPt.z),
                Vector3(maxPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, minPt.z),
                Vector3(minPt.x, maxPt.y, minPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, maxPt.z),
                Vector3(minPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(maxPt.x, minPt.y, minPt.z),
                Vector3(maxPt.x, maxPt.y, minPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(maxPt.x, minPt.y, maxPt.z),
                Vector3(maxPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, minPt.z),
                Vector3(minPt.x, minPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, maxPt.y, minPt.z),
                Vector3(minPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(maxPt.x, minPt.y, minPt.z),
                Vector3(maxPt.x, minPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(maxPt.x, maxPt.y, minPt.z),
                Vector3(maxPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        }
    }
}
