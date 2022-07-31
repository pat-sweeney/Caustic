//**********************************************************************
// Copyright Patrick Sweeney 2018-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

module Rendering.RenderWindow.RenderWindow;
import Base.Core.Core;
import Base.Math.BBox;
import Base.Math.Vector;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.ICamera;
import Rendering.RendererMarshaller.IRendererMarshaller;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.RenderGraph.RenderGraph;
import Rendering.Caustic.RendererFlags;

namespace Caustic
{
    CBaseRenderWindow::CBaseRenderWindow(HWND hwnd)
    {
        m_snapPosHome = { 10.0f, 10.0f, 10.0f };
        m_snapPosX = { 1000.0f, 0.0f, 0.0f };
        m_snapPosY = { 0.0f, 1000.0f, 0.0f };
        m_snapPosZ = { 0.0f, 0.0f, 1000.0f };
        m_spMarshaller = CreateRendererMarshaller();
        m_spSceneFactory = Caustic::CreateSceneFactory();
        m_spSceneGraph = m_spSceneFactory->CreateSceneGraph();

        Caustic::Vector3 pos(0.0f, 0.0f, 0.0f);
        Caustic::FRGBColor clr(1.0f, 1.0f, 1.0f);
        m_spPointLight = Caustic::CCausticFactory::Instance()->CreatePointLight(pos, clr, 1.0f);

        m_spCamera = Caustic::CCausticFactory::Instance()->CreateCamera(true);
        m_spTrackball = Caustic::CCausticFactory::Instance()->CreateTrackball();
        m_hwnd = hwnd;
    }

    CBaseRenderWindow::~CBaseRenderWindow()
    {
        if (m_spMarshaller != nullptr)
            m_spMarshaller->Shutdown();
    }

    void CBaseRenderWindow::SetViewport(float x0, float y0, float x1, float y1)
    {
        m_spMarshaller->GetRenderer()->SetViewport(x0, y0, x1, y1);
    }

    CRenderWindow::CRenderWindow(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder,
        std::function<void(Caustic::IRenderer*, Caustic::IRenderCtx*, int)> callback,
        std::function<void(Caustic::IRenderer*)> prePresentCallback,
        bool useRenderGraph /* = false */, bool startFrozen /* = false */, int desktopIndex /* = 0 */) : CBaseRenderWindow(hwnd)
    {
        m_callback = callback;
        m_prePresentCallback = prePresentCallback;
        m_spMarshaller->Initialize(hwnd, viewport, shaderFolder,
            nullptr,
            [this](IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass) {
                if (m_callback)
                    m_callback(pRenderer, pRenderCtx, pass);
                SceneCtx sceneCtx;
                sceneCtx.m_CurrentPass = pass;
                sceneCtx.m_ShowProxyObjects = m_spSceneGraph->GetShowProxyObjects();
                m_spSceneGraph->Render(pRenderer, pRenderCtx, &sceneCtx);
            },
            [this](IRenderer* pRenderer) {
                m_prePresentCallback(pRenderer);
            }, startFrozen, desktopIndex);

        CRefObj<IRenderer> spRenderer = m_spMarshaller->GetRenderer();
        spRenderer->SetCamera(m_spCamera);
    }
    
    CRenderWindow::~CRenderWindow()
    {
        m_spMarshaller->Shutdown();
    }

    void CRenderWindow::SetViewport(float x0, float y0, float x1, float y1)
    {
        CBaseRenderWindow::SetViewport(x0, y0, x1, y1);
    }

    void CRenderWindow::MouseDown(int x, int y, uint32 button, uint32 flags)
    {
        CBaseRenderWindow::MouseDown(x, y, button, flags);
    }

    void CBaseRenderWindow::MouseDown(int x, int y, uint32 button, uint32 flags)
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
        CBaseRenderWindow::MouseMove(x, y, flags);
    }

    void CBaseRenderWindow::MouseMove(int x, int y, uint32 flags)
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
        CBaseRenderWindow::SetSnapPositions(home, xAxis, yAxis, zAxis);
    }

    void CBaseRenderWindow::SetSnapPositions(const Vector3& home, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
    {
        m_snapPosHome = home;
        m_snapPosX = xAxis;
        m_snapPosY = yAxis;
        m_snapPosZ = zAxis;
    }

    void CRenderWindow::MouseUp(int x, int y, uint32 button, uint32 flags)
    {
        CBaseRenderWindow::MouseUp(x, y, button, flags);
    }

    void CBaseRenderWindow::MouseUp(int x, int y, uint32 button, uint32 flags)
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
        CBaseRenderWindow::MouseWheel(factor);
    }

    void CBaseRenderWindow::MouseWheel(int factor)
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
        CBaseRenderWindow::MapKey(wParam, lParam);
    }

    void CBaseRenderWindow::MapKey(uint32 wParam, uint32 lParam)
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

    void CImguiRenderWindow::RecordEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        ImGuiEvent evt;
        evt.hWnd = hWnd;
        evt.lParam = lParam;
        evt.wParam = wParam;
        evt.msg = message;
        EnterCriticalSection(&m_cs);
        m_events.push_back(evt);
        LeaveCriticalSection(&m_cs);
    }

    CImguiRenderWindow::CImguiRenderWindow(HWND hwnd, BBox2& viewport, std::wstring& shaderFolder,
        std::function<void(Caustic::IRenderer*, ITexture*, ImFont*)> renderUI, bool startFrozen /*= false*/, int desktopIndex /*= 0*/)
        : CBaseRenderWindow(hwnd)
    {
        m_hwnd = hwnd;
        m_renderUI = renderUI;
        InitializeCriticalSection(&m_cs);
        m_spMarshaller->Initialize(hwnd, viewport, shaderFolder,
                [this](IRenderer* pRenderer)
                {
                    auto ctx = ImGui::CreateContext();
                    ImGui_ImplWin32_Init(m_hwnd);
                    ImGui_ImplDX11_Init(pRenderer->GetDevice(), pRenderer->GetContext());

    #pragma warning(push)
    #pragma warning(disable: 4996)
                    const char* pCausticPixel = std::getenv("CausticRoot");
    #pragma warning(pop)
                    if (pCausticPixel == nullptr)
                        pCausticPixel = "d:\\github\\Caustic";
                    std::string fontPath = std::string(pCausticPixel) + "\\" + "External\\imgui\\misc\\fonts\\DroidSans.ttf";
                    m_pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), 24);

                    m_spFinalRT = Caustic::CreateTexture(pRenderer, 1920, 1080, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
                        (D3D11_CPU_ACCESS_FLAG)0, (D3D11_BIND_FLAG)(D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE));
                    pRenderer->SetFinalRenderTarget(m_spFinalRT->GetD3DTexture());
                },
                [this](IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)
                {
                    SceneCtx sceneCtx;
                    sceneCtx.m_CurrentPass = pass;
                    sceneCtx.m_ShowProxyObjects = m_spSceneGraph->GetShowProxyObjects();
                    m_spSceneGraph->Render(pRenderer, pRenderCtx, &sceneCtx);
                },
                [&](IRenderer* pRenderer)
                {
                    auto io = ImGui::GetIO();
                    EnterCriticalSection(&m_cs);
                    for (size_t i = 0; i < m_events.size(); i++)
                    {
                        extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
                        if (io.WantCaptureMouse)
                            ImGui_ImplWin32_WndProcHandler(m_events[i].hWnd, m_events[i].msg, m_events[i].wParam, m_events[i].lParam);
                    }
                    m_events.resize(0);
                    LeaveCriticalSection(&m_cs);

                    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
                    ImGui_ImplDX11_NewFrame();
                    ImGui_ImplWin32_NewFrame();
                    ImGui::NewFrame();

                    if (m_renderUI)
                        m_renderUI(pRenderer, m_spFinalRT, m_pFont);
                    ImGui::Render();
                    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
                }, startFrozen, desktopIndex);

            CRefObj<IRenderer> spRenderer = m_spMarshaller->GetRenderer();
            spRenderer->SetCamera(m_spCamera);
    }
}
