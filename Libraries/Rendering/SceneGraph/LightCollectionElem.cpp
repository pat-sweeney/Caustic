//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "SceneGraph.h"
#include <string>
#include <functional>
#include <d3d11_4.h>
#include <atlbase.h>

module Rendering.SceneGraph.LightCollectionElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ILight;
import Rendering.SceneGraph.SceneFactory;

namespace Caustic
{
    CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem()
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
        if (!(m_passes & pRenderCtx->GetCurrentPass()))
            return;
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(pRenderer->GetContext()->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
        spCtx2->BeginEventInt(L"LightCollection", 0);
#endif
        std::vector<CRefObj<ILight>> lights = pSceneCtx->m_lights;
        for (int i = 0; i < (int)m_lights.size(); i++)
            pSceneCtx->m_lights.push_back(m_lights[i]);
        if (pRenderCtx->GetCurrentPass() == Caustic::c_PassShadow)
        {
            int totalLights = 0;
            for (int i = 0; i < (int)m_lights.size(); i++)
            {
                if (totalLights >= c_MaxLights)
                    break;
                if (m_lights[i]->GetCastsShadows() && m_lights[i]->GetType() == ELightType::DirectionalLight)
                {
                    pRenderer->PushShadowmapRT(c_HiResShadowMap, totalLights++, m_lights[i]->GetPosition(), m_lights[i]->GetDirection());
                    bool inShadowLightGroup = pSceneCtx->m_inShadowLightGroup;
                    pSceneCtx->m_inShadowLightGroup = true;
                    CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
                    pSceneCtx->m_inShadowLightGroup = inShadowLightGroup;
                    pRenderer->PopShadowmapRT();
                }
            }
        }
        else
        {
            CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
        }
        pSceneCtx->m_lights = lights;
#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }

    void CSceneLightCollectionElem::GetBBox(BBox3* pBBox)
    {
        CSceneGroupElem::GetBBox(pBBox);
    }
}
