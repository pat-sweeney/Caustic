//**********************************************
#include "framework.h"
#include "Ditto.h"
#include <string>
#include <chrono>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

import Caustic.Base;
import Base.Core.Core;
import Base.Core.IRefCount;
import Imaging.Image.ImageFilter;
import Geometry.Mesh.IMeshConstructor;
import Geometry.MeshImport;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IVideoTexture;
import Rendering.Caustic.ICausticFactory;
import Rendering.RenderWindow.IRenderWindow;
import Cameras.WebCamera.WebCamera;
import Cameras.WebCamera.IWebCamera;
import Cameras.VirtualCamera.IVirtualCamera;
import Cameras.VirtualCamera.VirtualCamera;
import Imaging.Video.IVideo;

using namespace Caustic;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class CApp
{
public:
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<Caustic::ICausticFactory> m_spCausticFactory;
    CRefObj<ITexture> defaultTex;
    CRefObj<ISampler> m_spSampler;
    CRefObj<IImage> m_spLastFrame;
    CRefObj<ITexture> m_spLastTex;
    std::vector<CRefObj<IVideo>> m_videos;
    int m_curVideoIndex;
    int m_nextVideoIndex;
    bool m_texLoaded;
    HWND m_hwnd;
    std::chrono::time_point<std::chrono::system_clock> m_prevRenderTime;
    CRefObj<IVirtualCamera> m_spVirtualCam;
    CRefObj<IVideo> m_spVideo;
    void InitializeCaustic(HWND hWnd);
    void LiveWebCam();
    void BuildUI(ITexture* pFinalRT, ImFont* pFont);
};
CApp app;

struct Vertex
{
    float pos[3];
    float norm[3];
};

void CApp::LiveWebCam()
{
    auto videoDevices = IWebCamera::GetAvailableVideoDevices();
    int i = 0;
    for (; i < (int)videoDevices.size(); i++)
        if (videoDevices[i].name.contains(L"LifeCam"))
            break;
    auto audioDevices = IWebCamera::GetAvailableAudioDevices();

    std::wstring audioDeviceName;
    int samplingRate = 0;
    int bitsPerSample = 0;
    int numChannels = 0;

    if (audioDevices.size() > 0)
    {
        int j = 0;
        for (; j < (int)audioDevices.size(); j++)
            if (audioDevices[j].name.contains(L"LifeCam"))
                break;
        audioDeviceName = audioDevices[j].endpointID;
        samplingRate = audioDevices[j].samplingRates[0];
        bitsPerSample = audioDevices[j].bitsPerSample[0];
        numChannels = audioDevices[j].channels[0];
    }
    auto spWebCamera = CreateWebCamera(
        videoDevices[i].symlink.c_str(), -1, -1, 30, audioDeviceName, samplingRate, bitsPerSample, numChannels);
}

ImVec2 BuildMenuBar(ImFont* pFont)
{
    ImVec2 menuSize;
    ImGui::PushFont(pFont);
    if (ImGui::BeginMenuBar())
    {
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
            ImGui::Text("MeshViewer, Version 1.0");
            ImGui::Text("Copyright (c) 2022");
            ImGui::End();
        }
    }
    ImGui::PopFont();
    return menuSize;
}

void CApp::BuildUI(ITexture* pFinalRT, ImFont* pFont)
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

    //ImGui::ShowDemoWindow();

    ImGui::Begin("Actions", nullptr);
    if (ImGui::Button("Listening"))
        app.m_nextVideoIndex = 0;
    if (ImGui::Button("FollowUp"))
        app.m_nextVideoIndex = 1;
    if (ImGui::Button("FollowUpLater"))
        app.m_nextVideoIndex = 2;
    if (ImGui::Button("FollowUpEmail"))
        app.m_nextVideoIndex = 3;
    if (ImGui::Button("ScratchHead"))
        app.m_nextVideoIndex = 4;
    if (ImGui::Button("ScratchFace"))
        app.m_nextVideoIndex = 5;
    ImGui::End();

    ImVec2 menuSize = BuildMenuBar(pFont);

    RECT rect;
    GetClientRect(app.m_hwnd, &rect);

    if (pFinalRT != nullptr)
    {
        ImGui::Begin("FinalRender", nullptr);
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        ImVec2 vSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
        auto x = ImGui::GetCursorPos();
        ImGui::Image((void*)pFinalRT->GetD3DTextureRV(), vSize);
        ImGui::End();
    }
    ImGui::End();
}

void CApp::InitializeCaustic(HWND hwnd)
{
    Caustic::SystemStartup();
    app.m_texLoaded = false;
    m_spCausticFactory = Caustic::CreateCausticFactory();
    // Next create our output window
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    app.m_spVirtualCam = Caustic::CreateVirtualCamera();
    m_spRenderWindow = CreateImguiRenderWindow(hwnd, viewport, shaderFolder,
        [](Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx)
        {
            if (!app.m_texLoaded)
            {
                app.m_texLoaded = true;
                CRefObj<IShaderMgr> spShaderMgr = pRenderer->GetShaderMgr();

                app.m_curVideoIndex = 0;
                app.m_nextVideoIndex = 0;
                CRefObj<IVideo> p0 = CreateVideo(L"d:\\DittoData\\Listening.mp4");
                app.m_videos.push_back(p0);

                CRefObj<IVideo> p1 = CreateVideo(L"d:\\DittoData\\FollowUp.mp4");
                app.m_videos.push_back(p1);

                CRefObj<IVideo> p2 = CreateVideo(L"d:\\DittoData\\FollowUpLater.mp4");
                app.m_videos.push_back(p2);

                CRefObj<IVideo> p3 = CreateVideo(L"d:\\DittoData\\FollowUpEmail.mp4");
                app.m_videos.push_back(p3);

                CRefObj<IVideo> p4 = CreateVideo(L"d:\\DittoData\\ScratchFace.mp4");
                app.m_videos.push_back(p4);

                CRefObj<IVideo> p5 = CreateVideo(L"d:\\DittoData\\ScratchFace.mp4");
                app.m_videos.push_back(p5);
            }
            if (app.m_curVideoIndex != app.m_nextVideoIndex)
            {
                app.m_videos[app.m_curVideoIndex]->Restart();
                app.m_curVideoIndex = app.m_nextVideoIndex;
            }
            else if (app.m_videos[app.m_curVideoIndex]->EndOfStream())
            {
                app.m_videos[app.m_curVideoIndex]->Restart();
                app.m_curVideoIndex = app.m_nextVideoIndex = 0;
            }
            else
            {
                auto curRenderTime = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = curRenderTime - app.m_prevRenderTime;
                if (elapsed_seconds.count() * 1000.0f > 20)
                {
                    app.m_prevRenderTime = std::chrono::system_clock::now();
                    auto spVideoSample = app.m_videos[app.m_curVideoIndex]->NextVideoSample();
                    if (spVideoSample != nullptr)
                    {
                        app.m_spLastFrame = spVideoSample->GetImage();
                        app.m_spVirtualCam->SendVideoFrame(app.m_spLastFrame);
                        app.m_spLastTex = app.m_spCausticFactory->CreateTexture(pRenderer, app.m_spLastFrame, D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
                        app.m_spSampler = app.m_spCausticFactory->CreateSampler(pRenderer, app.m_spLastTex);
                    }
                }
                auto spAudioSample = app.m_videos[app.m_curVideoIndex]->NextAudioSample();
                if (spAudioSample != nullptr)
                    app.m_spVirtualCam->SendAudioFrame(spAudioSample->GetData(), spAudioSample->GetDataSize());
            }

            pRenderer->DrawScreenQuad(0.0f, 0.0f, 1.0f, 1.0f, app.m_spLastTex, app.m_spSampler);
        },
        [](Caustic::IRenderer* pRenderer, ITexture* pFinalRT, ImFont* pFont)
        {
            app.BuildUI(pFinalRT, pFont);
        });

    // Load our shaders
    CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();
    spRenderer->Unfreeze();
}

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HBITMAP imgbitmap = nullptr;
int imgwidth, imgheight;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


using namespace Caustic;
CRefObj<IWebCamera> spWebCamera;
CRefObj<IImage> spSourceImage;
CRefObj<IImage> spFilteredSourceImage;
CRefObj<IImageFilter> spFilter;

void DrawImage(HDC hdc, HBITMAP img, int w, int h)
{
    if (img != nullptr)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HGDIOBJ oldBitmap = SelectObject(hdcMem, img);
        RECT rect;
        GetClientRect(app.m_hwnd, &rect);
        SetStretchBltMode(hdc, COLORONCOLOR);
        StretchBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, w, h, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteObject(hdcMem);
    }
}

void SetDisplayImage(IImage* pImage, ImageFilterParams* pParams = nullptr)
{
    IImage* pDisplayImage = pImage;
    if (spFilter)
    {
        spFilteredSourceImage = spFilter->Apply(pImage, pParams);
        pDisplayImage = spFilteredSourceImage;
    }
    spSourceImage = pImage;
    imgwidth = pDisplayImage->GetWidth();
    imgheight = pDisplayImage->GetHeight();
    if (imgbitmap != nullptr)
        DeleteObject(imgbitmap);
    imgbitmap = CreateBitmap(imgwidth, imgheight, 1, 32, pDisplayImage->GetData());
    InvalidateRect(app.m_hwnd, nullptr, false);
}

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
    LoadStringW(hInstance, IDC_DITTO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DITTO));

    MSG msg;

    // Main message loop:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        if (spWebCamera != nullptr)
        {
            CRefObj<IImage> spColorImage;
            if (spWebCamera->NextVideoFrame(&spColorImage.p) && spColorImage != nullptr)
            {
                //SetDisplayImage(spColorImage);
                //DrawImage(GetDC(app.m_hwnd), imgbitmap, imgwidth, imgheight);
                //m_spVirtualCam->SendFrame(spColorImage);
            }
            CRefObj<IAudioFrame> spAudioFrame;
            if (spWebCamera->NextAudioFrame(&spAudioFrame))
            {
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DITTO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DITTO);
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
   hInst = hInstance; // Store instance handle in our global variable

   app.m_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!app.m_hwnd)
   {
      return FALSE;
   }

   // Setup our renderer
   app.InitializeCaustic(app.m_hwnd);

   ShowWindow(app.m_hwnd, nCmdShow);
   UpdateWindow(app.m_hwnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
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
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
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
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
