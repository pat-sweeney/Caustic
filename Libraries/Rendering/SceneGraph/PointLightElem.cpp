//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "PointLightElem.h"
#include "Rendering\Caustic\CausticFactory.h"
#include <string>

namespace Caustic
{
    CAUSTICAPI void CreatePointLightElem(IScenePointLightElem **ppElem)
    {
		CRefObj<IPointLight> spPointLight;
		Caustic::CCausticFactory::Instance()->CreatePointLight(Vector3(0.0f, 0.0f, 0.0f), &spPointLight);
		std::unique_ptr<CPointLightElem> spPointLightObj(new CPointLightElem(spPointLight));
        *ppElem = spPointLightObj.release();
        (*ppElem)->AddRef();
    }

    std::wstring &CPointLightElem::Name()
    {
        return CSceneElem::m_Name;
    }

    void CPointLightElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        if (m_prerenderCallback)
            m_prerenderCallback();
        pSceneCtx->m_lights.push_back(m_spPointLight);
        if (m_postrenderCallback)
            m_postrenderCallback();
    }

    void CPointLightElem::GetBBox(BBox3 *pBBox)
    {
    }

    void CPointLightElem::SetPosition(Caustic::Vector3 &pos)
    {
        m_spPointLight->SetPosition(pos);
    }

    void CPointLightElem::SetColor(Caustic::Vector3 &clr)
    {
        m_spPointLight->SetColor(clr);
    }

    void CPointLightElem::Store(IStream *pStream)
    {
        Vector3 clr = m_spPointLight->GetColor();
        Vector3 pos = m_spPointLight->GetPosition();
        ULONG bytesWritten;
        CT(pStream->Write(&clr, sizeof(clr), &bytesWritten));
        CT(pStream->Write(&pos, sizeof(pos), &bytesWritten));
    }

    void CPointLightElem::Load(IStream *pStream)
    {
        Vector3 clr;
        Vector3 pos;
        ULONG bytesRead;
        CT(pStream->Read(&clr, sizeof(clr), &bytesRead));
        CT(pStream->Read(&pos, sizeof(pos), &bytesRead));
        m_spPointLight->SetColor(clr);
        m_spPointLight->SetPosition(pos);
        SetFlags(GetFlags() | ESceneElemFlags::RenderableDirty | ESceneElemFlags::BBoxDirty);
    }
}
