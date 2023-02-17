//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include <d3d11.h>
#include <atlbase.h>
#include "WebcamToNDI.h"
#include <Windows.h>
#include <commdlg.h>
#include <string>
#include <map>
#include <any>
#include <functional>
#include <format>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include <sstream>
#ifdef USE_NDI
#include <Processing.NDI.Advanced.h>
#include <Processing.NDI.Lib.h>
#endif

import Caustic.Base;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Geometry.Mesh.IMeshConstructor;
import Geometry.MeshImport;
import Geometry.Mesh.Mesh;
import Imaging.Color;
import Parsers.JSon.JSonParser;
import Imaging.Image.IImage;
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
import Cameras.WebCamera.IWebCamera;

#define MAX_LOADSTRING 100

using namespace Caustic;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class CApp
{
public:
    HWND m_hwnd;                                          // HWND bound to renderer
    HINSTANCE m_hInst;
    CRefObj<IRenderer> m_spRenderer;                      // Instance of our renderer
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<ICausticFactory> m_spCausticFactory;
    std::vector<CameraInfo> cameras;
    std::vector<std::string> cameraNames;
    std::vector< std::vector<std::string>> cameraResolutions;
    std::vector< std::vector<Point2>> cameraResPoints;
    std::vector< std::vector<int>> cameraFrameRates;
    bool startBroadcast;
    CRefObj<IWebCamera> m_spCamera;
    CRefObj<IImage> m_spStaticImage;
    int currentCamera;
    int currentResolution;
    CRefObj<IImage> m_spColorImage;
    CRefObj<ITexture> m_spTexture;
    ImVec2 imageWinSize;
#ifdef USE_NDI
    NDIlib_send_instance_t pNDI_send;
    NDIlib_video_frame_v2_t NDI_video_frame;
#endif
    char NDIStreamName[1024];
    char ImagePath[1024];

    CApp()
    {
        startBroadcast = false;
        currentCamera = 0;
        currentResolution = 0;
    }

    void InitializeCaustic(HWND hwnd);
};
CApp app;

ImVec2 BuildMenuBar(ImFont *pFont)
{
    ImVec2 menuSize;
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
        menuSize = ImGui::GetWindowSize();
        ImGui::EndMenuBar();
    }
    ImGui::PopFont();
    return menuSize;
}

void BuildPanels(ITexture *pFinalRT, ImFont *pFont)
{
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
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImVec2 menuSize = BuildMenuBar(pFont);
    
    RECT rect;
    GetClientRect(app.m_hwnd, &rect);

#pragma region BroadcastCamera
    ImGui::Begin("CameraSelect");
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, app.startBroadcast);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * ((app.startBroadcast) ? 0.5f : 1.0f));
    ImGui::Text("Device:");
    ImGui::SameLine();
    size_t s = app.cameraNames.size();
    if (s > 0)
    {
        const char* pName = app.cameraNames[app.currentCamera].c_str();
        if (ImGui::BeginCombo("##Device", pName))
        {
            for (size_t i = 0; i < app.cameras.size(); i++)
            {
                bool is_selected = (app.currentCamera == i);
                if (ImGui::Selectable(app.cameraNames[i].c_str(), is_selected))
                    app.currentCamera = (int)i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::Text("Resolution:");
    ImGui::SameLine();
    if (app.cameraResolutions.size() > 0)
    {
        s = app.cameraResolutions[app.currentCamera].size();
        if (s > 0)
        {
            const char* pName = app.cameraResolutions[app.currentCamera][app.currentResolution].c_str();
            if (ImGui::BeginCombo("##Resolution", pName))
            {
                for (size_t i = 0; i < app.cameraResolutions[app.currentCamera].size(); i++)
                {
                    bool is_selected = (app.currentResolution == i);
                    if (ImGui::Selectable(app.cameraResolutions[app.currentCamera][i].c_str(), is_selected))
                        app.currentResolution = (int)i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
    }
    ImGui::Text("NDI Stream Name:");
    ImGui::SameLine();
    ImGui::InputText("##NDIStream", app.NDIStreamName, sizeof(app.NDIStreamName));
    if (ImGui::Button("Broadcast") && !app.startBroadcast)
    {
        app.m_spCamera = Caustic::CreateWebCamera(
            app.cameras[app.currentCamera].symlink.c_str(),
            app.cameraResPoints[app.currentCamera][app.currentResolution].x,
            app.cameraResPoints[app.currentCamera][app.currentResolution].y,
            app.cameraFrameRates[app.currentCamera][app.currentResolution]);

#ifdef USE_NDI
        app.NDI_video_frame;
        app.NDI_video_frame.xres = app.cameraResPoints[app.currentCamera][app.currentResolution].x;
        app.NDI_video_frame.yres = app.cameraResPoints[app.currentCamera][app.currentResolution].y;
        app.NDI_video_frame.FourCC = NDIlib_FourCC_type_BGRX;
        app.NDI_video_frame.p_data = (uint8_t*)malloc(app.NDI_video_frame.xres * app.NDI_video_frame.yres * 4);

        // We create the NDI sender
        NDIlib_send_create_t t;
        t.p_ndi_name = app.NDIStreamName;
        t.p_groups = nullptr;
        t.clock_video = true;
        t.clock_audio = false;
        app.pNDI_send = NDIlib_send_create(&t);
        if (!app.pNDI_send)
            return;
#endif

        app.startBroadcast = true;
    }
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
    ImGui::End();
#pragma endregion
    
#pragma region BroadcastStaticImage
    ImGui::Begin("StaticImageSelect");
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, app.startBroadcast);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha* ((app.startBroadcast) ? 0.5f : 1.0f));
    ImGui::Text("Image:");
    ImGui::SameLine();
    ImGui::InputText("##ImagePath", app.ImagePath, sizeof(app.ImagePath));
    ImGui::Text("NDI Stream Name:");
    ImGui::SameLine();
    ImGui::InputText("##NDIStream", app.NDIStreamName, sizeof(app.NDIStreamName));
    if (ImGui::Button("Broadcast") && !app.startBroadcast)
    {
        app.m_spStaticImage = Caustic::LoadImageFile(Caustic::str2wstr(app.ImagePath).c_str());

#ifdef USE_NDI
        app.NDI_video_frame;
        app.NDI_video_frame.xres = app.m_spStaticImage->GetWidth();
        app.NDI_video_frame.yres = app.m_spStaticImage->GetHeight();
        app.NDI_video_frame.FourCC = NDIlib_FourCC_type_BGRX;
        app.NDI_video_frame.p_data = (uint8_t*)malloc(app.NDI_video_frame.xres * app.NDI_video_frame.yres * 4);

        // We create the NDI sender
        NDIlib_send_create_t t;
        t.p_ndi_name = app.NDIStreamName;
        t.p_groups = nullptr;
        t.clock_video = true;
        t.clock_audio = false;
        app.pNDI_send = NDIlib_send_create(&t);
        if (!app.pNDI_send)
            return;
#endif
        app.startBroadcast = true;
    }
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
    ImGui::End();
#pragma endregion

    if (pFinalRT != nullptr)
    {
        ImGui::Begin("FinalRender", nullptr);
        if (app.m_spTexture)
        {
            app.imageWinSize = ImGui::GetContentRegionAvail();
            ImGui::Image((void*)app.m_spTexture->GetD3DTextureRV(), app.imageWinSize);
        }
        ImGui::End();
    }
    ImGui::End();
}

void InitializeCaustic(HWND hwnd)
{
    app.m_spCausticFactory = Caustic::CreateCausticFactory();
    SetDefaultCausticShaderDirectory(L"");
    std::wstring shaderFolder = GetCausticShaderDirectory();
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    app.imageWinSize = ImVec2(128, 128);
    app.m_spRenderWindow = CreateImguiRenderWindow(hwnd, viewport, shaderFolder,
            [](Caustic::IRenderer* pRenderer, ITexture *pFinalRT, ImFont *pFont)
            {
                BuildPanels(pFinalRT, pFont);
            });
    app.m_spRenderer = app.m_spRenderWindow->GetRenderer();

#ifdef USE_NDI
    // Not required, but "correct" (see the SDK documentation).
    if (!NDIlib_initialize())
        return;
#endif
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
    LoadStringW(hInstance, IDC_WEBCAMTONDI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEBCAMTONDI));
    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        if (app.m_spStaticImage)
        {
            app.m_spTexture = app.m_spCausticFactory->CreateTexture(app.m_spRenderer, app.m_spStaticImage,
                D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);

#ifdef USE_NDI
            int w = app.m_spStaticImage->GetWidth();
            int h = app.m_spStaticImage->GetHeight();
            uint8* pSrc = app.m_spStaticImage->GetData();
            uint8* pDst = app.NDI_video_frame.p_data;
            uint32 stride = app.m_spStaticImage->GetStride();
            int th = (h > app.NDI_video_frame.yres) ? app.NDI_video_frame.yres : h;
            for (int y = 0; y < th; y++)
            {
                int bytesToCopy = app.NDI_video_frame.xres * 4;
                if (bytesToCopy > (int)stride)
                    bytesToCopy = (int)stride;
                memcpy(pDst, pSrc, bytesToCopy);
                pSrc += stride;
                pDst += app.NDI_video_frame.xres * 4;
            }
            NDIlib_send_send_video_v2(app.pNDI_send, &app.NDI_video_frame);
#endif
        }
        else if (app.m_spCamera)
        {
            if (app.m_spColorImage)
                app.m_spColorImage->Release();
            if (app.m_spCamera->NextFrame(&app.m_spColorImage.p) && app.m_spColorImage)
            {
                app.m_spTexture = app.m_spCausticFactory->CreateTexture(app.m_spRenderer, app.m_spColorImage,
                    D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);

#ifdef USE_NDI
                int w = app.m_spColorImage->GetWidth();
                int h = app.m_spColorImage->GetHeight();
                uint8* pSrc = app.m_spColorImage->GetData();
                uint8* pDst = app.NDI_video_frame.p_data;
                uint32 stride = app.m_spColorImage->GetStride();
                int th = (h > app.NDI_video_frame.yres) ? app.NDI_video_frame.yres : h;
                for (int y = 0; y < th; y++)
                {
                    int bytesToCopy = app.NDI_video_frame.xres * 4;
                    if (bytesToCopy > (int)stride)
                        bytesToCopy = (int)stride;
                    memcpy(pDst, pSrc, bytesToCopy);
                    pSrc += stride;
                    pDst += app.NDI_video_frame.xres * 4;
                }
                NDIlib_send_send_video_v2(app.pNDI_send, &app.NDI_video_frame);
#endif
            }
        }
        if (app.m_spCamera)
        {
            if (app.m_spColorImage)
                app.m_spColorImage->Release();
            if (app.m_spCamera->NextFrame(&app.m_spColorImage.p) && app.m_spColorImage)
            {
                app.m_spTexture = app.m_spCausticFactory->CreateTexture(app.m_spRenderer, app.m_spColorImage,
                    D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);

#ifdef USE_NDI
                int w = app.m_spColorImage->GetWidth();
                int h = app.m_spColorImage->GetHeight();
                uint8* pSrc = app.m_spColorImage->GetData();
                uint8* pDst = app.NDI_video_frame.p_data;
                uint32 stride = app.m_spColorImage->GetStride();
                int th = (h > app.NDI_video_frame.yres) ? app.NDI_video_frame.yres : h;
                for (int y = 0; y < th; y++)
                {
                    int bytesToCopy = app.NDI_video_frame.xres * 4;
                    if (bytesToCopy > (int)stride)
                        bytesToCopy = (int)stride;
                    memcpy(pDst, pSrc, bytesToCopy);
                    pSrc += stride;
                    pDst += app.NDI_video_frame.xres * 4;
                }
                NDIlib_send_send_video_v2(app.pNDI_send, &app.NDI_video_frame);
#endif
            }
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEBCAMTONDI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
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
   app.cameras = IWebCamera::GetAvailableDevices();

   for (size_t i = 0; i < app.cameras.size(); i++)
   {
       std::string name = Caustic::wstr2str(app.cameras[i].name);
       app.cameraNames.push_back(name);
       std::vector<std::string> resolutions;
       std::vector<Point2> resPoints;
       std::vector<int> rates;
       for (size_t j = 0; j < app.cameras[i].resolutions.size(); j++)
       {
           std::string finalStr;
           std::stringstream ss(finalStr);
           int framerate = (int)((float)app.cameras[i].framerates[j].x / (float)app.cameras[i].framerates[j].y);
           ss << app.cameras[i].resolutions[j].x << "x" << app.cameras[i].resolutions[j].y << "x" << framerate;
           resolutions.push_back(ss.str().c_str());
           Point2 res(app.cameras[i].resolutions[j].x, app.cameras[i].resolutions[j].y);
           resPoints.push_back(res);
           rates.push_back(framerate);
       }
       app.cameraResolutions.push_back(resolutions);
       app.cameraResPoints.push_back(resPoints);
       app.cameraFrameRates.push_back(rates);
   }
   ShowWindow(app.m_hwnd, nCmdShow);
   UpdateWindow(app.m_hwnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (app.m_spRenderWindow != nullptr && message != WM_NCDESTROY)
        app.m_spRenderWindow->RecordEvent(hWnd, message, wParam, lParam);
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

        // Free the video frame
#ifdef USE_NDI
        if (app.NDI_video_frame.p_data != nullptr)
            free(app.NDI_video_frame.p_data);
        if (app.pNDI_send != nullptr)
            NDIlib_send_destroy(app.pNDI_send);
        NDIlib_destroy();
#endif

        Caustic::SystemShutdown();
        break;
    case WM_SIZE:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
