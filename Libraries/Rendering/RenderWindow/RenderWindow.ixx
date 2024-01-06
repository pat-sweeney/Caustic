//**********************************************************************
// Copyright Patrick Sweeney 2018-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <functional>
#include <windows.h>
#include <vector>
#include <string>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

export module Rendering.RenderWindow.RenderWindow;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.RendererMarshaller.IRendererMarshaller;
import Rendering.Caustic.ITrackball;
import Rendering.Caustic.ICamera;
import Rendering.SceneGraph.ISceneGraph;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.RenderWindow.IRenderWindow;

export namespace Caustic
{
    class CBaseRenderWindow : public IRenderWindow, public CRefCount
    {
    protected:
        CRefObj<IRendererMarshaller> m_spMarshaller;
        CRefObj<ISceneFactory> m_spSceneFactory;
        CRefObj<ISceneGraph> m_spSceneGraph;
        CRefObj<IPointLight> m_spPointLight;
        CRefObj<ICamera> m_spCamera;
        CRefObj<ITrackball> m_spTrackball;
        bool m_tracking;
        Vector3 m_eye;
        Vector3 m_look;
        Vector3 m_up;
        Vector3 m_upAsPt;
        HWND m_hwnd;
        DirectX::XMMATRIX m_view;
        DirectX::XMMATRIX m_invview;
        int m_startx, m_starty;
        int m_winwidth, m_winheight;
        Vector3 m_snapPosHome;
        Vector3 m_snapPosX;
        Vector3 m_snapPosY;
        Vector3 m_snapPosZ;
    public:
        CBaseRenderWindow(HWND hwnd);
        ~CBaseRenderWindow();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderWindow
        //**********************************************************************
        virtual CRefObj<ICamera> GetCamera() override { return m_spCamera; }
        virtual void SetCamera(CRefObj<ICamera> spCamera) override
        {
            m_spCamera = spCamera;
            CRefObj<IRenderer> spRenderer = m_spMarshaller->GetRenderer();
            spRenderer->SetCamera(m_spCamera);
        }
        virtual void RecordEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override {}
        virtual void SetViewport(float x0, float y0, float x1, float y1) override;
        virtual void SetSnapPositions(const Vector3& home, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) override;
        virtual CRefObj<ISceneGraph> GetSceneGraph() override { return m_spSceneGraph; }
        virtual void SetSceneGraph(ISceneGraph* pSceneGraph) override { m_spSceneGraph = pSceneGraph; }
        virtual void MouseDown(int x, int y, uint32_t button, uint32_t flags) override;
        virtual void MouseMove(int x, int y, uint32_t flags) override;
        virtual void MouseUp(int x, int y, uint32_t button, uint32_t flags) override;
        virtual void MouseWheel(int factor) override;
        virtual void MapKey(uint32_t wParam, uint32_t lParam) override;
        virtual CRefObj<IRenderer> GetRenderer() { return m_spMarshaller->GetRenderer(); }
    };

    class CRenderWindow : public CBaseRenderWindow
    {
        std::function<void(Caustic::IRenderer*, Caustic::IRenderCtx*, int)> m_callback;
        std::function<void(Caustic::IRenderer*)> m_prePresentCallback;
    public:
        CRenderWindow(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder,
            std::function<void(Caustic::IRenderer*, Caustic::IRenderCtx*, int)> callback,
            std::function<void(Caustic::IRenderer*)> prePresentCallback,
            bool useRenderGraph = false, bool startFrozen = false, int desktopIndex = 0);
        ~CRenderWindow();
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderWindow
        //**********************************************************************
        virtual void RecordEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override {}
        virtual void SetViewport(float x0, float y0, float x1, float y1) override;
        virtual void SetSnapPositions(const Vector3& home, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) override;
        virtual CRefObj<ISceneGraph> GetSceneGraph() override { return m_spSceneGraph; }
        virtual void MouseDown(int x, int y, uint32_t button, uint32_t flags) override;
        virtual void MouseMove(int x, int y, uint32_t flags) override;
        virtual void MouseUp(int x, int y, uint32_t button, uint32_t flags) override;
        virtual void MouseWheel(int factor) override;
        virtual void MapKey(uint32_t wParam, uint32_t lParam) override;
        virtual CRefObj<IRenderer> GetRenderer() { return m_spMarshaller->GetRenderer(); }
    };

    struct ImGuiEvent
    {
        HWND hWnd;
        UINT msg;
        WPARAM wParam;
        LPARAM lParam;
    };

    class CImguiRenderWindow : public CBaseRenderWindow
    {
    protected:
        ImFont* m_pFont;
        HWND m_hwnd;
        CRITICAL_SECTION m_cs;
        std::vector<ImGuiEvent> m_events;
        CRefObj<ITexture> m_spFinalRT;
        std::function<void(Caustic::IRenderer*, ITexture*, ImFont*)> m_renderUI;

    public:
        CImguiRenderWindow(HWND hwnd, BBox2& viewport, std::wstring& shaderFolder,
            std::function<void(Caustic::IRenderer*, Caustic::IRenderCtx*)> postSceneRender,
            std::function<void(Caustic::IRenderer*, ITexture*, ImFont*)> renderUI,
            bool startFrozen = false, int desktopIndex = 0);
        ~CImguiRenderWindow() {}

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderWindow
        //**********************************************************************
        virtual void RecordEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

        virtual void SetViewport(float x0, float y0, float x1, float y1) override
        {
            CBaseRenderWindow::SetViewport(x0, y0, x1, y1);
        }

        virtual void SetSnapPositions(const Vector3& home, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) override
        {
            CBaseRenderWindow::SetSnapPositions(home, xAxis, yAxis, zAxis);
        }

        virtual CRefObj<ISceneGraph> GetSceneGraph() override { return m_spSceneGraph; }

        virtual CRefObj<IRenderer> GetRenderer() { return m_spMarshaller->GetRenderer(); }
    };
}
