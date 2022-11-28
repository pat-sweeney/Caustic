//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "framework.h"
#include <d3d11.h>
#include <atlbase.h>
#include "CausticTemplate.h"
#include <Windows.h>
#include <commdlg.h>
#include <string>
#include <map>
#include <any>
#include <functional>
#include <format>
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
import Caustic.Base;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Imaging.Color;
import Parsers.JSon.JSonParser;
import Rendering.Caustic.ISpotLight;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.IRenderer;
import Rendering.GuiControls.Common;
import Rendering.RenderWindow.IRenderWindow;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.SceneGraph.ISceneCameraCollectionElem;
import Rendering.SceneImport.Collada;
import Rendering.RenderWindow.RenderWindow;

#define MAX_LOADSTRING 100

using namespace Caustic;

class CApp
{
public:
    HWND m_hwnd;                                          // HWND bound to renderer
    HINSTANCE m_hInst;                                    // HINSTANCE for app
    CRefObj<IRenderer> m_spRenderer;                      // Instance of our renderer
    CRefObj<IRenderWindow> m_spRenderWindow;              // Our render window
    CRefObj<ICausticFactory> m_spCausticFactory;          // Factory for creating core Caustic objects
    CRefObj<ICamera> m_spDefaultCamera;                   // Default camera to render with
    CRITICAL_SECTION m_cs;
    std::vector<ImGuiEvent> m_events;
    ImFont* m_pFont;
    bool m_initialized;

    CApp()
    {
        m_initialized = false;
    }

    void InitializeCaustic(HWND hwnd);
};
CApp app;

ImVec2 BuildMenuBar(ImFont* pFont)
{
    ImVec2 menuSize;
    if (pFont != nullptr)
        ImGui::PushFont(pFont);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
                DestroyWindow(app.m_hwnd);
            ImGui::EndMenu();
        }
        static bool isAboutOpen = false;
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                isAboutOpen = true;
            }
            ImGui::EndMenu();
        }
        menuSize = ImGui::GetWindowSize();
        ImGui::EndMenuBar();

        if (isAboutOpen)
        {
            ImGui::Begin("About", &isAboutOpen);
            ImGui::Text("CausticTemplate, Version 1.0");
            ImGui::Text("Copyright (c) 2022");
            ImGui::End();
        }
    }
    if (pFont != nullptr)
        ImGui::PopFont();
    return menuSize;
}

void InitializeCaustic(HWND hwnd)
{
    InitializeCriticalSection(&app.m_cs);
    app.m_spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    
    app.m_spRenderWindow = CreateRenderWindow(hwnd, viewport, shaderFolder,
        /*callback:*/ [](Caustic::IRenderer* pRenderer, IRenderCtx *pRenderCtx, int pass)
        {
        },
        /*prePresentCallback:*/ [](IRenderer* pRenderer)
        {
            if (!app.m_initialized)
            {
                auto ctx = ImGui::CreateContext();
                ImGui_ImplWin32_Init(app.m_hwnd);
                ImGui_ImplDX11_Init(pRenderer->GetDevice(), pRenderer->GetContext());

#pragma warning(push)
#pragma warning(disable: 4996)
                const char* pCausticPixel = std::getenv("CausticRoot");
#pragma warning(pop)
                if (pCausticPixel == nullptr)
                    pCausticPixel = "d:\\github\\Caustic";
                std::string fontPath = std::string(pCausticPixel) + "\\" + "External\\imgui\\misc\\fonts\\DroidSans.ttf";
                app.m_pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), 24);
                app.m_initialized = true;
            }
            auto io = ImGui::GetIO();
            EnterCriticalSection(&app.m_cs);
            for (size_t i = 0; i < app.m_events.size(); i++)
            {
                extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
                if (io.WantCaptureMouse)
                    ImGui_ImplWin32_WndProcHandler(app.m_events[i].hWnd, app.m_events[i].msg, app.m_events[i].wParam, app.m_events[i].lParam);
            }
            app.m_events.resize(0);
            LeaveCriticalSection(&app.m_cs);

            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", nullptr, window_flags);
            ImGui::PopStyleVar();
            ImGui::PopStyleVar(2);

            // Submit the DockSpace
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            ImVec2 menuSize = BuildMenuBar(nullptr);
            ImGui::End();
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }, /*startFrozen:*/ true, /*desktopIndex:*/ 0);
    app.m_spRenderer = app.m_spRenderWindow->GetRenderer();
    app.m_spDefaultCamera = app.m_spRenderWindow->GetCamera();

    app.m_spRenderer->Unfreeze();
}

// Global Variables:
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CAUSTICTEMPLATE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAUSTICTEMPLATE));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAUSTICTEMPLATE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    app.m_hInst = hInstance; // Store instance handle in our global variable

    app.m_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!app.m_hwnd)
    {
        return FALSE;
    }

    // Setup our renderer
    Caustic::SystemStartup();
    InitializeCaustic(app.m_hwnd);

    ShowWindow(app.m_hwnd, nCmdShow);
    UpdateWindow(app.m_hwnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Because we are using ImGui which doesn't process events in a normal way
    // we need to tell the render window to record the event so that it can be
    // handled by ImGui later.
    if (app.m_spRenderWindow != nullptr && message != WM_NCDESTROY)
    {
        ImGuiEvent evt;
        evt.hWnd = hWnd;
        evt.lParam = lParam;
        evt.wParam = wParam;
        evt.msg = message;
        EnterCriticalSection(&app.m_cs);
        app.m_events.push_back(evt);
        LeaveCriticalSection(&app.m_cs);
    }
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_QUIT:
        Caustic::SystemShutdown();
        break;
    case WM_SIZE:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
