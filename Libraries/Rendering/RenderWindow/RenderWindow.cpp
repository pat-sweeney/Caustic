//**********************************************************************
// Copyright Patrick Sweeney 2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Rendering\RenderWindow\RenderWindow.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include <Windows.h>

namespace Caustic
{
    CAUSTICAPI void CreateRenderWindow(HWND hwnd, IRenderWindow **ppRenderWindow)
    {
        std::unique_ptr<CRenderWindow> spRenderWindow(new CRenderWindow(hwnd));
        *ppRenderWindow = spRenderWindow.release();
        (*ppRenderWindow)->AddRef();
    }

    CRenderWindow::CRenderWindow(HWND hwnd)
    {
        Caustic::CreateRendererMarshaller(&m_spMarshaller);
        m_spMarshaller->Initialize(hwnd);
        Scene::CreateSceneGraph(&m_spSceneGraph);
        m_spMarshaller->SetSceneGraph(m_spSceneGraph.p);
        CreateCamera(true, &m_spCamera);
        CRefObj<IRenderer> spRenderer;
        m_spMarshaller->GetRenderer(&spRenderer);
        spRenderer->SetCamera(m_spCamera.p);
        CreateTrackball(&m_spTrackball);
        m_hwnd = hwnd;
    }
    
    void CRenderWindow::MouseDown(int x, int y)
    {
        Vector3 up;
        m_spCamera->GetPosition(&m_eye, &m_look, nullptr, nullptr, &up, nullptr);
        m_up.x = m_eye.x + up.x;
        m_up.y = m_eye.y + up.y;
        m_up.z = m_eye.z + up.z;
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        int w = rect.right - rect.left;
        int h = rect.bottom - rect.top;
        m_view = m_spCamera->GetView();
        m_invview = DirectX::XMMatrixInverse(nullptr, m_view);
        m_spTrackball->BeginTracking(x, y, w, h);
        m_tracking = true;
    }

    void CRenderWindow::MouseMove(int x, int y, uint32 flags)
    {
        if (m_tracking)
        {
            DirectX::XMMATRIX mat;
            ETrackballConstrain constraint;
            if (flags & MK_CONTROL)
                constraint = ETrackballConstrain::Constraint_XAxis;
            else if (flags & MK_SHIFT)
                constraint = ETrackballConstrain::Constraint_YAxis;
            else
                constraint = ETrackballConstrain::Constraint_None;
            if (m_spTrackball->UpdateTracking(x, y, constraint, &mat))
            {
                DirectX::XMVECTOR vLook = DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 1.0f);
                vLook = DirectX::XMVector3Transform(vLook, m_view);
                DirectX::XMMATRIX mtrans = DirectX::XMMatrixTranslation(-DirectX::XMVectorGetX(vLook), -DirectX::XMVectorGetY(vLook), -DirectX::XMVectorGetZ(vLook));
                DirectX::XMMATRIX minvtrans = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(vLook), DirectX::XMVectorGetY(vLook), DirectX::XMVectorGetZ(vLook));
                DirectX::XMMATRIX m = DirectX::XMMatrixMultiply(m_view,
                    DirectX::XMMatrixMultiply(mtrans,
                        DirectX::XMMatrixMultiply(mat,
                            DirectX::XMMatrixMultiply(minvtrans, m_invview))));
                DirectX::XMVECTOR eye = DirectX::XMVectorSet(m_eye.x, m_eye.y, m_eye.z, 1.0f);
                DirectX::XMVECTOR up = DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 1.0f);
                eye = DirectX::XMVector4Transform(eye, m);
                up = DirectX::XMVector3Transform(up, m);
                Vector3 neye(DirectX::XMVectorGetX(eye), DirectX::XMVectorGetY(eye), DirectX::XMVectorGetZ(eye));
                Vector3 nup(DirectX::XMVectorGetX(up), DirectX::XMVectorGetY(up), DirectX::XMVectorGetZ(up));
                nup = nup - neye;
                nup.Normalize();
                m_spCamera->SetPosition(neye, m_look, nup);
            }
        }
    }

    void CRenderWindow::MouseUp(int x, int y)
    {
        m_spTrackball->EndTracking();
        m_tracking = false;
    }

    void CRenderWindow::MouseWheel(int factor)
    {
        Vector3 eye, look, up, n;
        //    float distance = factor / 120;
        m_spCamera->GetPosition(&eye, &look, &up, nullptr, nullptr, &n);
        Vector3 dirVec = ((factor < 0.0f) ? -1.0f : +1.0f) * n;
        eye = eye + dirVec;
        m_spCamera->SetPosition(eye, look, up);
    }
}
