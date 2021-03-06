//**********************************************************************
// Copyright Patrick Sweeney 2018-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Rendering\RenderWindow\RenderWindow.h"
#include "Rendering\RenderGraph\RenderGraph.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include <Windows.h>

namespace Caustic
{
    CAUSTICAPI CRefObj<IRenderWindow> CreateRenderWindow(HWND hwnd, std::wstring &shaderFolder,
        std::function<void(IRenderer*,IRenderCtx*,int)> callback,
        std::function<void(IRenderer*)> prePresentCallback,
        bool startFrozen /* = false */, int desktopIndex /* = 0 */)
    {
        return CRefObj<IRenderWindow>(new CRenderWindow(hwnd, shaderFolder, callback, prePresentCallback, false, startFrozen, desktopIndex));
    }

    CRenderWindow::CRenderWindow(HWND hwnd, std::wstring &shaderFolder,
        std::function<void(Caustic::IRenderer*, Caustic::IRenderCtx*, int)> callback,
        std::function<void(Caustic::IRenderer*)> prePresentCallback,
        bool useRenderGraph /* = false */, bool startFrozen /* = false */, int desktopIndex /* = 0 */)
    {
        m_snapPosHome = { 10.0f, 10.0f, 10.0f };
        m_snapPosX = { 1000.0f, 0.0f, 0.0f };
        m_snapPosY = { 0.0f, 1000.0f, 0.0f };
        m_snapPosZ = { 0.0f, 0.0f, 1000.0f };
        m_callback = callback;
        m_prePresentCallback = prePresentCallback;
        m_useRenderGraph = useRenderGraph;
        m_spMarshaller = Caustic::CCausticFactory::Instance()->CreateRendererMarshaller();
        if (useRenderGraph)
        {
            m_spRenderGraphFactory = Caustic::CreateRenderGraphFactory();
            m_spRenderGraph = m_spRenderGraphFactory->CreateRenderGraph();
        }
        else
        {
            m_spSceneFactory = Caustic::CreateSceneFactory();
            m_spSceneGraph = m_spSceneFactory->CreateSceneGraph();
        }
        m_spMarshaller->Initialize(hwnd, shaderFolder,
            [this](IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass) {
                if (m_callback)
                    m_callback(pRenderer, pRenderCtx, pass);
                if (m_useRenderGraph)
                    m_spRenderGraph->Render(pRenderer, pRenderCtx);
                else
                {
                    SceneCtx sceneCtx;
                    sceneCtx.m_CurrentPass = pass;
                    m_spSceneGraph->Render(pRenderer, pRenderCtx, &sceneCtx);
                }
            },
            [this](IRenderer* pRenderer) {
                m_prePresentCallback(pRenderer);
            }, startFrozen, desktopIndex);

        Caustic::Vector3 pos(0.0f, 0.0f, 0.0f);
        Caustic::FRGBColor clr(1.0f, 1.0f, 1.0f);
        m_spPointLight = Caustic::CCausticFactory::Instance()->CreatePointLight(pos, clr, 1.0f);

        m_spCamera = Caustic::CCausticFactory::Instance()->CreateCamera(true);
        CRefObj<IRenderer> spRenderer = m_spMarshaller->GetRenderer();
        spRenderer->SetCamera(m_spCamera);
        m_spTrackball = Caustic::CCausticFactory::Instance()->CreateTrackball();
        m_hwnd = hwnd;
    }
    
    CRenderWindow::~CRenderWindow()
    {
        m_spMarshaller->Shutdown();
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
                    m_spPointLight->SetPosition(neye);
                }
            }
        }
    }

    void CRenderWindow::SetSnapPositions(const Vector3& home, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
    {
        m_snapPosHome = home;
        m_snapPosX = xAxis;
        m_snapPosY = yAxis;
        m_snapPosZ = zAxis;
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
        case 'X':
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            up = Vector3(0.0f, 1.0f, 0.0f);
            eye = m_snapPosX;
            look = Vector3(0.0f, 0.0f, 0.0f);
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case 'Y':
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            up = Vector3(1.0f, 0.0f, 0.0f);
            eye = m_snapPosY;
            look = Vector3(0.0f, 0.0f, 0.0f);
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case 'Z':
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            up = Vector3(0.0f, 1.0f, 0.0f);
            eye = m_snapPosZ;
            look = Vector3(0.0f, 0.0f, 0.0f);
            m_spCamera->SetPosition(eye, look, up);
        }
        break;
        case VK_HOME:
        {
            Vector3 eye, look, up, u, v, n;
            m_spCamera->GetPosition(&eye, &look, &up, &u, &v, &n);
            up = Vector3(0.0f, 1.0f, 0.0f);
            eye = m_snapPosHome;
            look = Vector3(0.0f, 0.0f, 0.0f);
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
