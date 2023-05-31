// SplitVideo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SplitVideo.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include <string>
#include <any>
#include <algorithm>

import Caustic.Base;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Imaging.Image.ImageFilter;
import Imaging.Image.ImageFilter.FaceLandmarks;
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
import Imaging.Video.IVideo;
import Parsers.Phonemes.IPhonemes;

#define MAX_LOADSTRING 100

using namespace Caustic;

class CApp
{
public:
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<Caustic::ICausticFactory> m_spCausticFactory;
    HWND m_hwnd;
    void InitializeCaustic(HWND hWnd);
    char VideoPath[1024];
    char VideoName[1024];
    CRefObj<IPhonemes> m_spPhonemes;
};
CApp app;

void CApp::InitializeCaustic(HWND hwnd)
{
    Caustic::SystemStartup();
    m_spPhonemes = Caustic::CreatePhonemes();
    m_spPhonemes->LoadDatabase();
    m_spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    m_spRenderWindow = CreateImguiRenderWindow(hwnd, viewport, shaderFolder,
        [](Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx)
        {
        },
        [](Caustic::IRenderer* pRenderer, ITexture* pFinalRT, ImFont* pFont)
        {
            ImGui::Begin("ProcessFile");
            ImGui::Text("VideoPath:");
            ImGui::SameLine();
            ImGui::InputText("##VideoPath", app.VideoPath, sizeof(app.VideoPath));
            ImGui::Text("VideoName:");
            ImGui::SameLine();
            ImGui::InputText("##VideoName", app.VideoName, sizeof(app.VideoName));
            if (ImGui::Button("Convert"))
            {
                CRefObj<IImageFilter> spFilter = CreateFaceLandmarksFilter();
                std::wstring videoPath = Caustic::str2wstr(app.VideoPath);
                std::wstring videoName = Caustic::str2wstr(app.VideoName);
                std::wstring videoFullPath = videoPath + L"\\" + videoName + L".mp4";
                CRefObj<IVideo> p0 = CreateVideo(videoFullPath.c_str());
                int frameIndex = 0;
                std::vector<std::string> phonemes;
                std::string word(app.VideoName);
                std::transform(word.begin(), word.end(), word.begin(),
                    [](unsigned char c) { return std::toupper(c); });
                app.m_spPhonemes->GetPhonemes(word.c_str(), phonemes);

                // Make the first pass across the stream to find all the face
                // landmarks.
                std::vector<BBox2> faceBbox;
                std::vector<std::vector<Vector2>> faceLandmarks;
                while (!p0->EndOfStream())
                {
                    auto spVideoSample = p0->NextVideoSample();
                    if (spVideoSample != nullptr)
                    {
                        auto spImage = spVideoSample->GetImage();
                        Caustic::ImageFilterParams params;
                        params.params.insert(std::make_pair("outputImage", std::any(true)));
                        auto marked = spFilter->Apply(spImage, &params);
                        {
                            wchar_t rectFN[1024];
                            swprintf_s(rectFN, L"%s\\%s_%d.txt", videoPath.c_str(), videoName.c_str(), frameIndex);
                            HANDLE f = CreateFile(rectFN, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
                            auto bb = std::any_cast<BBox2>(params.params["Face0"]);
                            faceBbox.push_back(bb);
                            char buf[1024];
                            sprintf_s(buf, "FaceBBox: %f,%f..%f,%f\n", bb.minPt.x, bb.minPt.y, bb.maxPt.x, bb.maxPt.y);
                            DWORD dw;
                            WriteFile(f, buf, strlen(buf), &dw, nullptr);
                            int numLandmarks = std::any_cast<size_t>(params.params["Face0_NumLandmarks"]);
                            std::vector<Vector2> landmarks;
                            for (int j = 0; j < numLandmarks; j++)
                            {
                                sprintf_s(buf, "Face0_Point%d", j);
                                Vector2 pt = std::any_cast<Vector2>(params.params[buf]);
                                landmarks.push_back(pt);
                                sprintf_s(buf, "Landmark%d: %f,%f\n", j, pt.x, pt.y);
                                WriteFile(f, buf, strlen(buf), &dw, nullptr);
                            }
                            faceLandmarks.push_back(landmarks);
                            CloseHandle(f);
                        }
                        wchar_t buf[1024];
                        swprintf_s(buf, L"%s\\%s_marked_%d.png", videoPath.c_str(), videoName.c_str(), frameIndex);
                        Caustic::StoreImage(buf, marked);
                        swprintf_s(buf, L"%s\\%s_orig_%d.png", videoPath.c_str(), videoName.c_str(), frameIndex);
                        Caustic::StoreImage(buf, spImage);
                        frameIndex++;
                    }
                }

                // Next I need to build an array per phoneme that indicates how the grid
                // deforms based on the landmark motion. 
                // 
                // Here I assume that the phonemes that make up each word are evenly
                // spaced across the sound duration. This is clearly incorrect, but
                // I don't have another solution.
                //
                // I also assume that all the landmarks will be centered around
                // points 28 and 31 (the bridge of the nose). This is used to normalize
                // across the frames.
                // I then lay a grid mesh across the image and place a gaussian distribution
                // at each landmark to determine what portions of the image should move
                // based on the delta of the landmarks across the phoneme.
                int curPhoneme = 0;
                int framesPerPhoneme = frameIndex / phonemes.size();
                int framePhonemeIndex = 0;
                for (int i = 0; i < frameIndex; i++)
                {
                    if (framePhonemeIndex == framesPerPhoneme)
                    {
                        curPhoneme++;
                        framePhonemeIndex = 0;
                    }
                    faceLandmarks[i][c_FaceLandmark_NoseBridge_Top];
                    faceLandmarks[i][c_FaceLandmark_NoseBridge_Bottom];
                    // First align the bridge of the nose with the first landmark
                }
            }
            ImGui::End();
        });

    // Load our shaders
    CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();
    spRenderer->Unfreeze();
}

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
    LoadStringW(hInstance, IDC_SPLITVIDEO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLITVIDEO));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLITVIDEO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLITVIDEO);
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
