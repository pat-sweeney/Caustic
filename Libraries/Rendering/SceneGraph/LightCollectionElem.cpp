//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "SceneGraph.h"
#include "SceneFactory.h"
#include "LightCollectionElem.h"
#include <string>
#include <functional>

namespace Caustic
{
    CAUSTICAPI CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem()
    {
        return CRefObj<ISceneLightCollectionElem>(new CSceneLightCollectionElem());
    }

    uint32 CSceneLightCollectionElem::NumberLights()
    {
        return (uint32)m_lights.size();
    }

    CRefObj<ILight> CSceneLightCollectionElem::GetLight(int index)
    {
        return m_lights[index];
    }

    void CSceneLightCollectionElem::AddLight(ILight* pLight)
    {
        m_lights.push_back(pLight);
    }

    void CSceneLightCollectionElem::RemoveLight(ILight* pLight)
    {
        for (int i = 0; i < (int)m_lights.size(); i++)
            if (m_lights[i] == pLight)
            {
                m_lights.erase(m_lights.begin() + i);
            }
    }

    void CSceneLightCollectionElem::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)
    {
        std::vector<CRefObj<ILight>> lights = pSceneCtx->m_lights;
        for (int i = 0; i < (int)m_lights.size(); i++)
            pSceneCtx->m_lights.push_back(m_lights[i]);
        if (pRenderCtx->GetCurrentPass() == Caustic::c_PassShadow)
        {
            for (int i = 0; i < (int)m_lights.size(); i++)
            {
                if (m_lights[i]->GetCastsShadows())
                {
                    pRenderer->PushShadowmapRT(c_HiResShadowMap, false, m_lights[i]->GetPosition());
                    CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
                    pRenderer->PopShadowmapRT();
                }
            }
        }
        else
        {
            CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
        }
        pSceneCtx->m_lights = lights;
    }

    void CSceneLightCollectionElem::GetBBox(BBox3* pBBox)
    {
        CSceneGroupElem::GetBBox(pBBox);
    }
}
