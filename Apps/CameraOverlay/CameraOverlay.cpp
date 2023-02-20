// CameraOverlay.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CameraOverlay.h"
#include <string>
import Caustic.Base;
import Base.Core.Core;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Cameras.WebCamera.IWebCamera;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;
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

void DrawImage(HDC hdc, HBITMAP img, int w, int h)
{
    if (img != nullptr)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HGDIOBJ oldBitmap = SelectObject(hdcMem, imgbitmap);
        RECT rect;
        GetClientRect(hWnd, &rect);
        SetStretchBltMode(hdc, COLORONCOLOR);
        StretchBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, w, h, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteObject(hdcMem);
    }
}

void SetDisplayImage(IImage *pImage)
{
    IImage* pDisplayImage = pImage;
    spSourceImage = pImage;
    imgwidth = pDisplayImage->GetWidth();
    imgheight = pDisplayImage->GetHeight();
    if (imgbitmap != nullptr)
        DeleteObject(imgbitmap);
    imgbitmap = CreateBitmap(imgwidth, imgheight, 1, 32, pDisplayImage->GetData());
    InvalidateRect(hWnd, nullptr, false);
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
    LoadStringW(hInstance, IDC_CAMERAOVERLAY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAMERAOVERLAY));

    MSG msg;

    // Main message loop:
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
        if (spWebCamera != nullptr)
        {
            CRefObj<IImage> spColorImage;
            if (spWebCamera->NextVideoFrame(&spColorImage.p) && spColorImage != nullptr)
            {
                SetDisplayImage(spColorImage);
                DrawImage(GetDC(hWnd), imgbitmap, imgwidth, imgheight);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAMERAOVERLAY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
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

   int screenW = GetSystemMetrics(SM_CXSCREEN);
   int screenH = GetSystemMetrics(SM_CYSCREEN) - 75;
   int windowW = 400;
   int windowH = (windowW * 1080) / 1920;
   hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_EX_TOPMOST | WS_POPUP,
       screenW - windowW, screenH - windowH, windowW, windowH, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }

   Caustic::SystemStartup();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   auto videoDevices = IWebCamera::GetAvailableVideoDevices();
   int i = 0;
   for (; i < (int)videoDevices.size(); i++)
   {
       std::wstring name = videoDevices[i].name;
       if (name.contains(L"LifeCam"))
           break;
   }
   auto audioDevices = IWebCamera::GetAvailableAudioDevices();
   int j = 0;
   for (; j < (int)audioDevices.size(); j++)
   {
       std::wstring name = audioDevices[j].name;
       if (name.contains(L"LifeCam"))
           break;
   }
   spWebCamera = CreateWebCamera(videoDevices[i].symlink.c_str(), 1920, 1080, 30,
       audioDevices[j].endpointID.c_str(), 48000, 32, 1);

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
            DrawImage(hdc, imgbitmap, imgwidth, imgheight);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
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
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_QUIT:
        Caustic::SystemShutdown();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return (INT_PTR)FALSE;
}
