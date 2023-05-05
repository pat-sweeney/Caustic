//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "framework.h"
#include "Ditto.h"
#include <string>

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

using namespace Caustic;

class CApp
{
public:
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<Caustic::ICausticFactory> m_spCausticFactory;
    CRefObj<IVideoTexture> video_Listen;
    CRefObj<ITexture> video_FollowUp;
    CRefObj<ITexture> video_FollowUpLater;
    CRefObj<ITexture> video_FollowUpEmail;
    CRefObj<ITexture> video_ScratchFace;
    CRefObj<ITexture> defaultTex;
    CRefObj<ISampler> m_spSampler;
    std::vector<CRefObj<IVideoTexture>> m_videos;
    std::vector<CRefObj<ISampler>> m_samplers;
    int m_videoIndex;
    bool m_texLoaded;

    void InitializeCaustic(HWND hWnd);
};
CApp app;

struct Vertex
{
    float pos[3];
    float norm[3];
};

void CApp::InitializeCaustic(HWND hwnd)
{
    Caustic::SystemStartup();
    app.m_texLoaded = false;
    m_spCausticFactory = Caustic::CreateCausticFactory();
    // Next create our output window
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    m_spRenderWindow = CreateRenderWindow(hwnd, viewport, shaderFolder,
        [](IRenderer* pRenderer, IRenderCtx*, int) {
            if (!app.m_texLoaded)
            {
                app.m_texLoaded = true;
                CRefObj<IShaderMgr> spShaderMgr = pRenderer->GetShaderMgr();

                app.m_videoIndex = 0;
                CRefObj<IVideoTexture> p0 = app.m_spCausticFactory->LoadVideoTexture(L"d:\\DittoData\\Listen.mp4", pRenderer);
                CRefObj<ISampler> s0 = app.m_spCausticFactory->CreateSampler(pRenderer, p0);
                app.m_videos.push_back(p0);
                app.m_samplers.push_back(s0);

                CRefObj<IVideoTexture> p1 = app.m_spCausticFactory->LoadVideoTexture(L"d:\\DittoData\\FollowUp.mp4", pRenderer);
                CRefObj<ISampler> s1 = app.m_spCausticFactory->CreateSampler(pRenderer, p1);
                app.m_videos.push_back(p1);
                app.m_samplers.push_back(s1);

                CRefObj<IVideoTexture> p2 = app.m_spCausticFactory->LoadVideoTexture(L"d:\\DittoData\\FollowUpLater.mp4", pRenderer);
                CRefObj<ISampler> s2 = app.m_spCausticFactory->CreateSampler(pRenderer, p2);
                app.m_videos.push_back(p2);
                app.m_samplers.push_back(s2);

                CRefObj<IVideoTexture> p3 = app.m_spCausticFactory->LoadVideoTexture(L"d:\\DittoData\\FollowUpEmail.mp4", pRenderer);
                CRefObj<ISampler> s3 = app.m_spCausticFactory->CreateSampler(pRenderer, p3);
                app.m_videos.push_back(p3);
                app.m_samplers.push_back(s3);

                CRefObj<IVideoTexture> p4 = app.m_spCausticFactory->LoadVideoTexture(L"d:\\DittoData\\ScratchFace.mp4", pRenderer);
                CRefObj<ISampler> s4 = app.m_spCausticFactory->CreateSampler(pRenderer, p4);
                app.m_videos.push_back(p4);
                app.m_samplers.push_back(s4);

            }
            app.m_videos[app.m_videoIndex]->Update(pRenderer);
            if (app.m_videos[app.m_videoIndex]->EndOfStream())
            {
                app.m_videos[app.m_videoIndex]->Restart();
                if (app.m_videoIndex == 0)
                {
                    app.m_videoIndex = (rand() % 4) + 1;
                }
                else
                {
                    app.m_videoIndex = 0;
                }
            }
            pRenderer->DrawScreenQuad(0.0f, 0.0f, 1.0f, 1.0f, app.m_videos[app.m_videoIndex], app.m_samplers[app.m_videoIndex]);
        },
        [](IRenderer*) {}, true, 0);

    // Load our shaders
    CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();
    spRenderer->Unfreeze();
}

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // Setup our renderer
   app.InitializeCaustic(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
    case WM_LBUTTONDOWN:
        app.m_spRenderWindow->MouseDown((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
        break;
    case WM_LBUTTONUP:
        app.m_spRenderWindow->MouseUp((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
        break;
    case WM_MOUSEMOVE:
        app.m_spRenderWindow->MouseMove((int)LOWORD(lParam), (int)HIWORD(lParam), (uint32)wParam);
        break;
    case WM_MOUSEWHEEL:
        app.m_spRenderWindow->MouseWheel((int)wParam);
        break;
    case WM_KEYDOWN:
        app.m_spRenderWindow->MapKey((uint32)wParam, (uint32)lParam);
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
