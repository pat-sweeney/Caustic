//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "SimpleImageViewer.h"
#include <Windows.h>
#include <commdlg.h>
#include <any>
#include <string>
import Base.Core.Core;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Imaging.Image.AllImageFilters;
import Cameras.AzureKinect.IAzureKinect;
import Cameras.WebCamera.IWebCamera;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;
HBITMAP imgbitmap = nullptr;
int imgwidth, imgheight;
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
        GetClientRect(hWnd, &rect);
        SetStretchBltMode(hdc, COLORONCOLOR);
        StretchBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, w, h, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteObject(hdcMem);
    }
}

void SetDisplayImage(IImage *pImage, ImageFilterParams *pParams = nullptr)
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
        if (spWebCamera != nullptr)
        {
            CRefObj<IImage> spColorImage;
            if (spWebCamera->NextFrame(&spColorImage.p) && spColorImage != nullptr)
            {
                SetDisplayImage(spColorImage);
                DrawImage(GetDC(hWnd), imgbitmap, imgwidth, imgheight);
            }
        }
        else if (spAzure != nullptr)
        {
            CRefObj<IImage> spColorImage;
            CRefObj<IImage> spDepthImage;
            spAzure->NextFrame(&spColorImage.p, &spDepthImage.p, nullptr);
            if ((spDepthImage != nullptr && g_ShowDepth) || (spColorImage != nullptr && !g_ShowDepth))
            {
                if (g_ShowDepth)
                {
                    spFilter = CreateColorize();
                    SetDisplayImage(spDepthImage);
                }
                else
                {
                    SetDisplayImage(spColorImage);
                }
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
                /*
                                    //                    CRefObj<IImageFilter> spFilter;
                    //                    float weights[] = {
                    //                        1.0f, 1.0f, 1.0f,
                    //                        0.0f, 0.0f, 0.0f,
                    //                        -1.0f, -1.0f, -1.0f
                    //                    };
                    //                    CreateCustomFilter(3, 3, weights, &spFilter);
                    //                    CRefObj<IImage> spFiltered;
                    //                    spFilter->Apply(spColorImage, nullptr, &spFiltered);
*/
            case ID_FILTER_NONE:
                spFilter = nullptr;
                SetDisplayImage(spSourceImage);
                break;
            case ID_FILTER_SOBEL:
                spFilter = CreateSobelFilter(true);
                SetDisplayImage(spSourceImage);
                break;
            case ID_FILTER_SHARPEN:
                spFilter = CreateSharpenFilter();
                SetDisplayImage(spSourceImage);
                break;
            case ID_FILTER_UNSHARPMASK:
                spFilter = CreateUnsharpMaskFilter();
                SetDisplayImage(spSourceImage);
                break;
            case ID_FILTER_EQUALIZE:
            {
                spFilter = CreateEqualizeFilter();
                ImageFilterParams params;
                params.params["UseLuminance"] = std::any((bool)true);
                SetDisplayImage(spSourceImage, &params);
            }
            break;
            case ID_FILTER_MOSAIC:
            {
                spFilter = CreateMosaicFilter();
                ImageFilterParams params;
                params.params["BlockWidth"] = std::any((int)15);
                params.params["BlockHeight"] = std::any((int)15);
                SetDisplayImage(spSourceImage, &params);
            }
            break;
            case ID_FILTER_MOTIONBLUR:
            {
                spFilter = CreateMotionBlurFilter();
                ImageFilterParams params;
                params.params["DeltaX"] = std::any((int)20);
                params.params["DeltaY"] = std::any((int)20);
                SetDisplayImage(spSourceImage, &params);
            }
            break;
            case ID_FILTER_GAUSSIANBLUR:
                spFilter = CreateGaussianBlur();
                SetDisplayImage(spSourceImage);
                break;
            case ID_FILTER_MEDIANFILTER:
                spFilter = CreateMedian();
                SetDisplayImage(spSourceImage);
                break;
            case ID_FILTER_BOXFILTER3X3:
                spFilter = CreateBoxFilter(3);
                SetDisplayImage(spSourceImage);
                break;
            case ID_FILTER_BOXFILTER9X9:
                spFilter = CreateBoxFilter(9);
                SetDisplayImage(spSourceImage);
                break;
            case ID_FILTER_QUANTIZE:
                spFilter = CreateQuantizeFilter();
                SetDisplayImage(spSourceImage);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_FILE_LIVEWEBCAMERA:
                {
                    auto x = IWebCamera::GetAvailableDevices();
                    int i = 0;
                    for (; i < (int)x.size(); i++)
                        if (x[i].name.contains(L"LifeCam"))
                            break;
                    spWebCamera = CreateWebCamera(x[i].symlink.c_str());
                }
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
                        Caustic::CRefObj<Caustic::IImage> spImage = Caustic::LoadImageFile(ofn.lpstrFile);
                        spSourceImage = spImage;
                        if (spFilter)
                            spImage = spFilter->Apply(spImage, nullptr);
                        imgwidth = spImage->GetWidth();
                        imgheight = spImage->GetHeight();
                        imgbitmap = CreateBitmap(imgwidth, imgheight, 1, 32, spImage->GetData());
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
    }
    return (INT_PTR)FALSE;
}
