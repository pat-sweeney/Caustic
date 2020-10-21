//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\IRenderMesh.h"
#include "IRenderable.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\CritSec.h"
#include "Base\Core\Event.h"
#include "Shader.h"
#include "PointCloud.h"
#include "IPointCloud.h"
#include "CausticFactory.h"
#include <memory>
#include <any>
#include <vector>
#include <atlbase.h>
#include <d3d11.h>

//**********************************************************************
// File: Renderable.h
// Contains the declaration for the <CRenderable> class.
//**********************************************************************

namespace Caustic
{
    void CPointCloud::SetRayMap(IRenderer* pRenderer, IImage* pImage)
    {
        m_spRayMap->CopyFromImage(pRenderer, pImage);
        m_spRenderMaterial->SetTexture(pRenderer, L"rayTexture", m_spRayMap, Caustic::EShaderAccess::VertexShader);
    }

    void CPointCloud::SetColorMap(IRenderer* pRenderer, IImage* pImage)
    {
        m_spColorMap->CopyFromImage(pRenderer, pImage);
        m_spRenderMaterial->SetTexture(pRenderer, L"colorTexture", m_spColorMap, Caustic::EShaderAccess::PixelShader);
    }

    void CPointCloud::SetDepthMap(IRenderer *pRenderer, IImage *pImage)
    {
        m_spDepthMap->CopyFromImage(pRenderer, pImage);
        m_spRenderMaterial->SetTexture(pRenderer, L"depthTexture", m_spDepthMap, Caustic::EShaderAccess::VertexShader);
    }

	CPointCloud::CPointCloud(IRenderer *pRenderer, uint32 width, uint32 height)
	{
        m_spShader = pRenderer->GetShaderMgr()->FindShader(L"PointCloud");
        m_spShader->SetVSParam(L"width", std::any(float(width)));
        m_spShader->SetVSParam(L"height", std::any(float(width)));

        auto inst = Caustic::CCausticFactory::Instance()->CreateMaterialAttrib();

        std::vector<CGeomVertex> verts;
        std::vector<CGeomFace> faces;
        float dx = 2.0f / float(width);
        float dy = 2.0f / float(height);
        float cx = -1.0f;
        float cy = -1.0f;
        Vector3 normal(0.0f, 1.0f, 0.0f);
        for (uint32 y = 0; y < height; y++)
        {
            for (uint32 x = 0; x < width; x++)
            {
                CGeomVertex vertex;
                vertex.pos = Vector3(cx, cy, 0.0f);
                vertex.norm = normal;
                vertex.uvs[0] = Vector2(float(x) / float(width - 1), float(y) / float(height - 1));
                verts.push_back(vertex);
                cx += dx;
                if (x > 0 && y > 0)
                {
                    CGeomFace face;
                    face.indices[0] = (y - 1) * width + (x - 1);
                    face.indices[1] = y * width + (x - 1);
                    face.indices[2] = (y - 1) * width + x;
                    faces.push_back(face);
                    face.indices[0] = y * width + x;
                    face.indices[1] = (y - 1) * width + x;
                    face.indices[2] = y * width + (x - 1);
                    faces.push_back(face);
                }
            }
            cy += dy;
        }

        auto spCausticFactory = Caustic::CCausticFactory::Instance();
        m_spMaterialAttrib = spCausticFactory->CreateMaterialAttrib();
        m_spMaterialAttrib->SetTexture(L"depthTexture", nullptr, Caustic::EShaderAccess::VertexShader);
        m_spMaterialAttrib->SetTexture(L"rayTexture", nullptr, Caustic::EShaderAccess::VertexShader);
        m_spMaterialAttrib->SetTexture(L"colorTexture", nullptr, Caustic::EShaderAccess::PixelShader);
        m_spRenderMaterial = spCausticFactory->CreateRenderMaterial(pRenderer, m_spMaterialAttrib, m_spShader);
        m_spRenderMesh = Caustic::BuildRenderSubMesh(pRenderer, faces, verts, m_spShader);

        m_spDepthMap = Caustic::CreateTexture(pRenderer, width, height, DXGI_FORMAT::DXGI_FORMAT_R16_UINT);
        m_spRayMap = Caustic::CreateTexture(pRenderer, width, height, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
        m_spShader->SetVSParam(L"rayTexture", std::any(m_spRayMap));
        m_spShader->SetVSParam(L"depthTexture", std::any(m_spDepthMap));

        m_spColorMap = Caustic::CreateTexture(pRenderer, 1920, 1080, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
        m_spShader->SetPSParam(L"colorTexture", std::any(m_spColorMap));
        m_spColorSampler = Caustic::CreateSampler(pRenderer, m_spColorMap);
        m_spShader->SetPSParam(L"colorSampler", std::any(m_spColorSampler));

        m_renderable.SetSubMesh(m_spRenderMesh);
        m_renderable.SetFrontMaterial(m_spRenderMaterial);
        m_renderable.SetBackMaterial(m_spRenderMaterial);
    }

    void CPointCloud::Render(IRenderer* pRenderer, std::vector<CRefObj<ILight>>& lights, IRenderCtx* pRenderCtx)
    {
        m_renderable.Render(pRenderer, lights, pRenderCtx);
    }

    CAUSTICAPI CRefObj<IPointCloud> CreatePointCloud(IRenderer* pRenderer, uint32 width, uint32 height)
    {
        std::unique_ptr<CPointCloud> spPointCloud(new CPointCloud(pRenderer, width, height));
        return CRefObj<IPointCloud>(spPointCloud.release());
    }
}
