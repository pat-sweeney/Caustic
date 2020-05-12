//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "SimpleImageViewer.h"
#include <Windows.h>
#include <commdlg.h>
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Imaging\Image\Image.h"
#include "Imaging\Image\ImageFilter.h"
#include "Cameras\AzureKinect\IAzureKinect.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;
HBITMAP imgbitmap = nullptr;
bool g_ShowDepth = false;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

using namespace Caustic;
CRefObj<IAzureKinect> spAzure;

void DrawImage(HDC hdc)
{
    if (imgbitmap != nullptr)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HGDIOBJ oldBitmap = SelectObject(hdcMem, imgbitmap);
        BITMAP bitmap;
        GetObject(imgbitmap, sizeof(bitmap), &bitmap);
        RECT rect;
        GetClientRect(hWnd, &rect);
        StretchBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteObject(hdcMem);
    }
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
    LoadStringW(hInstance, IDC_SIMPLEIMAGEVIEWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLEIMAGEVIEWER));

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
        if (spAzure != nullptr)
        {
            CRefObj<IImage> spColorImage;
            CRefObj<IImage> spDepthImage;
            spAzure->NextFrame(&spColorImage.p, &spDepthImage.p, nullptr);
            if ((spDepthImage != nullptr && g_ShowDepth) || (spColorImage != nullptr && !g_ShowDepth))
            {
                if (imgbitmap != nullptr)
                    DeleteObject(imgbitmap);
                if (g_ShowDepth)
                {
                    CRefObj<IImage> spColoredDepthImage;
                    CRefObj<IImageFilter> spFilter;
                    CreateColorize(&spFilter);
                    spFilter->Apply(spDepthImage, nullptr, &spColoredDepthImage);
                    imgbitmap = CreateBitmap(spColoredDepthImage->GetWidth(), spColoredDepthImage->GetHeight(), 1, 32, spColoredDepthImage->GetData());
                }
                else
                {
//                    CRefObj<IImageFilter> spFilter;
//                    float weights[] = {
//                        1.0f, 1.0f, 1.0f,
//                        0.0f, 0.0f, 0.0f,
//                        -1.0f, -1.0f, -1.0f
//                    };
//                    CreateCustomFilter(3, 3, weights, &spFilter);
//                    CRefObj<IImage> spFiltered;
//                    spFilter->Apply(spColorImage, nullptr, &spFiltered);
                    imgbitmap = CreateBitmap(spColorImage->GetWidth(), spColorImage->GetHeight(), 1, 32, spColorImage->GetData());
                }
                DrawImage(GetDC(hWnd));
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLEIMAGEVIEWER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SIMPLEIMAGEVIEWER);
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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

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
            case ID_FILE_LIVECAMERA:
                spAzure = Caustic::AzureKinect::CreateAzureKinect(0, Caustic::AzureKinect::Color1080p, Caustic::AzureKinect::Depth512x512, Caustic::AzureKinect::FPS30);
                break;
            case ID_VIEW_SHOWDEPTH:
                g_ShowDepth = !g_ShowDepth;
                break;
            case ID_FILE_LOADIMAGE:
                {
                    OPENFILENAME ofn;
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.hInstance = hInst;
                    ofn.lpstrFilter = L"All Files\0*.*\0JPEG Files\0*.jpg\0PNG Files\0*.png\0\0\0";
                    wchar_t fn[_MAX_PATH];
                    ZeroMemory(fn, sizeof(fn));
                    ofn.lpstrFile = fn;
                    ofn.nMaxFile = _MAX_PATH;
                    ofn.lpstrFileTitle = nullptr;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = L"c:\\";
                    ofn.lpstrTitle = L"Open Image";
                    ofn.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
                    if (GetOpenFileName(&ofn))
                    {
                        Caustic::CRefObj<Caustic::IImage> spImage;
                        Caustic::LoadImage(ofn.lpstrFile, &spImage);
                        imgbitmap = CreateBitmap(spImage->GetWidth(), spImage->GetHeight(), 1, 32, spImage->GetData());
                        InvalidateRect(hWnd, nullptr, true);
                    }
                }
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
            DrawImage(hdc);
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
    }
    return (INT_PTR)FALSE;
}
