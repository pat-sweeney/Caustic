//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <any>
#include <vector>
#include <atlbase.h>
#include <d3d11.h>

module Rendering.Caustic.PointCloud;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.CritSec;
import Rendering.Caustic.Shader;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderable;
import Rendering.Caustic.IPointCloud;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.BuildRenderMesh;

//**********************************************************************
// File: PointCloud.cpp
// Contains the implementation for the <CPointCloud> class.
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

    void CPointCloud::SetColorExtrinsics(Matrix4x4& mat)
    {
        m_extrinsics = mat;
    }

    void CPointCloud::SetColorIntrinsics(Matrix3x3& mat)
    {
        m_intrinsics = mat;
    }

	CPointCloud::CPointCloud(IRenderer *pRenderer, uint32 width, uint32 height)
	{
        m_spShader = pRenderer->GetShaderMgr()->FindShader(L"PointCloud");
        m_spShader->SetVSParam(L"width", std::any(float(width)));
        m_spShader->SetVSParam(L"height", std::any(float(height)));
        m_spShader->SetPSParam(L"width", std::any(float(width)));
        m_spShader->SetPSParam(L"height", std::any(float(height)));

        auto spCausticFactory = Caustic::CCausticFactory::Instance();

        m_spRenderMesh = CreateDepthGridMesh(pRenderer, width, height, m_spShader);
        
        auto inst = spCausticFactory->CreateMaterialAttrib();
        m_spMaterialAttrib = spCausticFactory->CreateMaterialAttrib();
        m_spMaterialAttrib->SetTexture(L"depthTexture", nullptr, Caustic::EShaderAccess::VertexShader);
        m_spMaterialAttrib->SetTexture(L"rayTexture", nullptr, Caustic::EShaderAccess::VertexShader);
        m_spMaterialAttrib->SetTexture(L"colorTexture", nullptr, Caustic::EShaderAccess::PixelShader);
        m_spRenderMaterial = spCausticFactory->CreateRenderMaterial(pRenderer, m_spMaterialAttrib, m_spShader);

        m_spDepthMap = Caustic::CreateTexture(pRenderer, width, height, DXGI_FORMAT::DXGI_FORMAT_R16_UINT);
        m_spRayMap = Caustic::CreateTexture(pRenderer, width, height, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
        m_spShader->SetVSParam(L"rayTexture", std::any(m_spRayMap));
        m_spShader->SetVSParam(L"depthTexture", std::any(m_spDepthMap));
        m_spShader->SetPSParam(L"depthTexture", std::any(m_spDepthMap));
        m_spDepthSampler = Caustic::CreateSampler(pRenderer, m_spDepthMap);
        m_spShader->SetVSParam(L"depthSampler", std::any(m_spDepthSampler));
        m_spShader->SetPSParam(L"depthSampler", std::any(m_spDepthSampler));

        m_spColorMap = Caustic::CreateTexture(pRenderer, 1920, 1080, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
        m_spShader->SetPSParam(L"colorTexture", std::any(m_spColorMap));
        m_spColorSampler = Caustic::CreateSampler(pRenderer, m_spColorMap);
        m_spShader->SetPSParam(L"colorSampler", std::any(m_spColorSampler));

        m_renderable.SetSubMesh(m_spRenderMesh);
        m_renderable.SetFrontMaterial(m_spRenderMaterial);
        m_renderable.SetBackMaterial(m_spRenderMaterial);
    }

    void CPointCloud::Render(IRenderer* pRenderer, std::vector<CRefObj<ILight>>& lights, IRenderCtx* pRenderCtx)
    {
        Matrix m;
        memcpy(m.x, m_extrinsics.v, sizeof(float) * 16);
        m_spShader->SetPSParam(L"colorExt", std::any(m));
        Matrix_3x3 m1;
        memcpy(m1.x, m_intrinsics.v, sizeof(float) * 9);
        m_spShader->SetPSParam(L"colorInt", std::any(m1));

        m_renderable.Render(pRenderer, lights, pRenderCtx);
    }
}
