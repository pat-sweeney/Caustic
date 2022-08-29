//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <functional>
#include <d3d11_4.h>
#include <atlbase.h>
#include <cstdlib>

module Rendering.SceneGraph.SceneCameraCollectionElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Base.Math.BBox;
import Geometry.MeshImport;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ICamera;
import Rendering.SceneGraph.SceneFactory;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.SceneGraph.ISceneCameraCollectionElem;
import Rendering.Caustic.IShaderMgr;
import Rendering.SceneGraph.SceneCubeElem;

namespace Caustic
{
    uint32 CSceneCameraCollectionElem::NumberCameras()
    {
        return (uint32)m_Cameras.size();
    }

    CRefObj<ICamera> CSceneCameraCollectionElem::GetCamera(int index)
    {
        return m_Cameras[index];
    }

    void CSceneCameraCollectionElem::AddCamera(ICamera* pCamera)
    {
        m_Cameras.push_back(pCamera);
    }

    void CSceneCameraCollectionElem::RemoveCamera(ICamera* pCamera)
    {
        for (int i = 0; i < (int)m_Cameras.size(); i++)
            if (m_Cameras[i] == pCamera)
            {
                m_Cameras.erase(m_Cameras.begin() + i);
            }
    }

    bool CSceneCameraCollectionElem::sm_proxiesCreated = false;
    CRefObj<ISceneMaterialElem> CSceneCameraCollectionElem::sm_spCameraProxyMaterialElem;
    CRefObj<ISceneMaterialElem> CSceneCameraCollectionElem::LoadCameraProxies(IRenderer* pRenderer)
    {
        if (!sm_proxiesCreated)
        {
#pragma warning(push)
#pragma warning(disable: 4996)
            const char* pModelDir = std::getenv("AppModelDir");
#pragma warning(pop)
            if (pModelDir == nullptr)
                pModelDir = "D:\\Models";
            std::string fn = std::string(pModelDir) + "\\CameraBulb.obj";
            std::wstring wfn = Caustic::str2wstr(fn);
            CRefObj<ISceneFactory> spSceneFactory = CSceneFactory::Instance();
            CRefObj<ISceneMeshElem> spMeshElem = spSceneFactory->CreateMeshElem();
            const wchar_t* pShaderName = L"ObjShader";
            CRefObj<IMesh> spMesh = Caustic::MeshImport::LoadObj(wfn.c_str(), nullptr);
            spMeshElem->SetMesh(spMesh);
            CRefObj<IShader> spShader = pRenderer->GetShaderMgr()->FindShader(pShaderName);
            sm_spCameraProxyMaterialElem = spSceneFactory->CreateMaterialElem();
            auto spCausticFactory = Caustic::CreateCausticFactory();
            CRefObj<IMaterialAttrib> spMaterial = spCausticFactory->CreateMaterialAttrib();
            FRGBColor ambient(0.2f, 0.2f, 0.2f);
            FRGBColor diffuse(0.4f, 0.4f, 0.4f);
            spMaterial->SetColor(L"ambientColor", ambient);
            spMaterial->SetColor(L"diffuseColor", diffuse);
            sm_spCameraProxyMaterialElem->SetMaterial(spMaterial);
            sm_spCameraProxyMaterialElem->SetShader(spShader);
            sm_spCameraProxyMaterialElem->AddChild(spMeshElem);
            sm_proxiesCreated = true;
        }
        return sm_spCameraProxyMaterialElem;
    }

    void CSceneCameraCollectionElem::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)
    {
        if (!(m_passes & pRenderCtx->GetCurrentPass()))
            return;
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(pRenderer->GetContext()->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
        spCtx2->BeginEventInt(L"CameraCollection", 0);
#endif
        if (pSceneCtx->m_ShowProxyObjects)
        {
            CRefObj<IShader> spOldShader = pSceneCtx->m_spCurrentShader;
            pSceneCtx->m_spCurrentShader = pRenderer->GetShaderMgr()->FindShader(L"default");
            for (int i = 0; i < (int)m_Cameras.size(); i++)
            {
                Vector3 eye, look, up;
                m_Cameras[i]->GetPosition(&eye, &look, &up, nullptr, nullptr, nullptr);
                auto spCameraProxyMaterialElem = LoadCameraProxies(pRenderer);
                auto mat = Matrix4x4::TranslationMatrix(eye.x, eye.y, eye.z);
                spCameraProxyMaterialElem->SetTransform(mat);
                spCameraProxyMaterialElem->Render(pRenderer, pRenderCtx, pSceneCtx);
            }
            pSceneCtx->m_spCurrentShader = spOldShader;
        }
#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }
}
