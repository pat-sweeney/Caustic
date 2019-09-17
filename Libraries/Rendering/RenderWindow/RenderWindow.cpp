//**********************************************************************
// Copyright Patrick Sweeney 2018-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Rendering\RenderWindow\RenderWindow.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include "Rendering\SceneGraph\SceneFactory.h"
#include "Rendering\Caustic\CausticFactory.h"
#include <Windows.h>

namespace Caustic
{
    CAUSTICAPI void CreateRenderWindow(HWND hwnd, std::wstring &shaderFolder, IRenderWindow **ppRenderWindow)
    {
        std::unique_ptr<CRenderWindow> spRenderWindow(new CRenderWindow(hwnd, shaderFolder));
        *ppRenderWindow = spRenderWindow.release();
        (*ppRenderWindow)->AddRef();
    }

    CRenderWindow::CRenderWindow(HWND hwnd, std::wstring &shaderFolder)
    {
        Caustic::CCausticFactory::Instance()->CreateRendererMarshaller(&m_spMarshaller);
        m_spMarshaller->Initialize(hwnd, shaderFolder);
		CSceneFactory::Instance()->CreateSceneGraph(&m_spSceneGraph);
        m_spMarshaller->SetSceneGraph(m_spSceneGraph.p);
		Caustic::CCausticFactory::Instance()->CreateCamera(true, &m_spCamera);
        CRefObj<IRenderer> spRenderer;
        m_spMarshaller->GetRenderer(&spRenderer);
        spRenderer->SetCamera(m_spCamera.p);
		Caustic::CCausticFactory::Instance()->CreateTrackball(&m_spTrackball);
        m_hwnd = hwnd;
    }
    
    void CRenderWindow::MouseDown(int x, int y, uint32 button, uint32 flags)
    {
        if (button == c_LeftButton)
        {
            m_spCamera->GetPosition(&m_eye, &m_look, nullptr, nullptr, &m_up, nullptr);
            RECT rect;
            GetClientRect(m_hwnd, &rect);
            m_winwidth = rect.right - rect.left;
            m_winheight = rect.bottom - rect.top;
            if (flags & MK_CONTROL)
            {
                m_startx = x;
                m_starty = y;
            }
            else
            {
                m_upAsPt.x = m_eye.x + m_up.x;
                m_upAsPt.y = m_eye.y + m_up.y;
                m_upAsPt.z = m_eye.z + m_up.z;
                m_view = m_spCamera->GetView();
                m_invview = DirectX::XMMatrixInverse(nullptr, m_view);
                m_spTrackball->BeginTracking(x, y, m_winwidth, m_winheight);
            }
            m_tracking = true;
        }
    }

    void CRenderWindow::MouseMove(int x, int y, uint32 flags)
    {
        if (m_tracking)
        {
            if (flags & MK_CONTROL)
            {
                float dist = (m_eye - m_look).Length();
                int dx = x - m_startx;
                int dy = y - m_starty;
                float offsetX = -dist * (float)dx / (float)m_winwidth;
                float offsetY = dist * (float)dy / (float)m_winheight;
                Vector3 u, v;
                m_spCamera->GetUVN(&u, &v, nullptr);
                Vector3 nl = m_look;
                nl.x += u.x * offsetX + v.x * offsetY;
                nl.y += u.y * offsetX + v.y * offsetY;
                nl.z += u.z * offsetX + v.z * offsetY;
                Vector3 ne = m_eye;
                ne.x += u.x * offsetX + v.x * offsetY;
                ne.y += u.y * offsetX + v.y * offsetY;
                ne.z += u.z * offsetX + v.z * offsetY;
                m_spCamera->SetPosition(ne, nl, m_up);
            }
            else
            {
                DirectX::XMMATRIX mat;
                ETrackballConstraint constraint;
                if (flags & MK_SHIFT)
                    constraint = ETrackballConstraint::Constraint_XAxis;
                else if (flags & MK_ALT)
                    constraint = ETrackballConstraint::Constraint_YAxis;
                else
                    constraint = ETrackballConstraint::Constraint_None;
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
                    DirectX::XMVECTOR up = DirectX::XMVectorSet(m_upAsPt.x, m_upAsPt.y, m_upAsPt.z, 1.0f);
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
    }

    void CRenderWindow::MouseUp(int x, int y, uint32 button, uint32 flags)
    {
        if (button == c_LeftButton)
        {
            if (!(flags & MK_CONTROL))
                m_spTrackball->EndTracking();
            m_tracking = false;
        }
    }

    void CRenderWindow::MouseWheel(int factor)
    {
        Vector3 eye, look, up, n;
        //    float distance = factor / 120;
        m_spCamera->GetPosition(&eye, &look, &up, nullptr, nullptr, &n);
        float dist = (eye - look).Length() * 0.10f;
        Vector3 dirVec = (((factor < 0.0f) ? -1.0f : +1.0f) * n).Normalize() * dist;
        eye = eye + dirVec;
        m_spCamera->SetPosition(eye, look, up);
    }

    void CRenderWindow::MapKey(uint32 wParam, uint32 lParam)
    {
        switch (wParam)
        {
        case VK_HOME:
        {
            static int axis = 0;
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            up = Vector3(0.0f, 1.0f, 0.0f);
            eye = Vector3(0.0f, 0.0f, 0.0f);
            switch (axis)
            {
            case 0:
                look = Vector3(1.0f, 0.0f, 0.0f);
                break;
            case 1:
                look = Vector3(0.0f, 1.0f, 0.0f);
                up = Vector3(0.0f, 0.0f, 1.0f);
                break;
            case 2:
                look = Vector3(0.0f, 0.0f, 1.0f);
                break;
            }
            axis++; if (axis > 2)axis = 0;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case VK_UP:
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            eye.x += n.x; look.x += n.x;
            eye.y += n.y; look.y += n.y;
            eye.z += n.z; look.z += n.z;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case VK_DOWN:
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            eye.x -= n.x; look.x -= n.x;
            eye.y -= n.y; look.y -= n.y;
            eye.z -= n.z; look.z -= n.z;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case VK_RIGHT:
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            eye.x += u.x; look.x += u.x;
            eye.y += u.y; look.y += u.y;
            eye.z += u.z; look.z += u.z;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case VK_LEFT:
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            eye.x -= u.x; look.x -= u.x;
            eye.y -= u.y; look.y -= u.y;
            eye.z -= u.z; look.z -= u.z;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case VK_PRIOR:
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            eye.x += v.x; look.x += v.x;
            eye.y += v.y; look.y += v.y;
            eye.z += v.z; look.z += v.z;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case VK_NEXT:
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            eye.x -= v.x; look.x -= v.x;
            eye.y -= v.y; look.y -= v.y;
            eye.z -= v.z; look.z -= v.z;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case 'E':
        {
            Vector3 eye, look, up, n;
            m_spCamera->GetPosition(&eye, &look, &up, nullptr, nullptr, &n);
            eye += n;
            look += n;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case 'D':
        {
            Vector3 eye, look, up, n;
            m_spCamera->GetPosition(&eye, &look, &up, nullptr, nullptr, &n);
            eye -= n;
            look -= n;
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        }
    }
}
