// TestSamplers.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TestSamplers.h"
#include "Base\Math\FuncFilter.h"
#include "Base\Math\Vector.h"
#include "Imaging\Image\Image.h"
#include "Geometry\Rast\Bresenham.h"

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
    int totalSamples = 512;
    auto spImage = Caustic::CreateImage(totalSamples, 255, 32);
    std::vector<float> offsets;
    offsets.resize(totalSamples);
    for (int i = 0; i < totalSamples; i++)
        if (i == totalSamples / 2)
            offsets[i] = 30 + 10.0f * float(rand()) / float(RAND_MAX) - 5.0f;
        else
            offsets[i] = 10.0f * float(rand()) / float(RAND_MAX) - 5.0f;

    Caustic::uint8 clr[4];
    float ls = offsets[0] + 128;
    for (int i = 1; i < totalSamples; i++)
    {
        float s = ls + offsets[i];
        clr[0] = 255;
        clr[1] = 0;
        clr[2] = 0;
        clr[3] = 255;
        Caustic::Vector2 tv0 = Caustic::Vector2(float(i - 1), float(ls));
        Caustic::Vector2 tv1 = Caustic::Vector2(float(i), float(s));
        spImage->DrawLine(tv0, tv1, clr);
        ls = s;
    }

    Caustic::MovingAverage avg(10);
    ls = offsets[0] + 128;
    float sls = avg.Sample(ls);
    for (int i = 1; i < totalSamples; i++)
    {
        float s = ls + offsets[i];
        float ss = avg.Sample(s);
        clr[0] = 0;
        clr[1] = 255;
        clr[2] = 0;
        clr[3] = 255;
        Caustic::Vector2 tv0 = Caustic::Vector2(float(i - 1), float(sls));
        Caustic::Vector2 tv1 = Caustic::Vector2(float(i), float(ss));
        spImage->DrawLine(tv0, tv1, clr);
        ls = s;
        sls = ss;
    }

    Caustic::DoubleExponentialFilter avg2(0.06f, 0.06f);
    ls = offsets[0] + 128;
    sls = avg2.Sample(ls);
    for (int i = 1; i < totalSamples; i++)
    {
        float s = ls + offsets[i];
        float ss = avg2.Sample(s);
        clr[0] = 0;
        clr[1] = 0;
        clr[2] = 255;
        clr[3] = 255;
        Caustic::Vector2 tv0 = Caustic::Vector2(float(i - 1), float(sls));
        Caustic::Vector2 tv1 = Caustic::Vector2(float(i), float(ss));
        spImage->DrawLine(tv0, tv1, clr);
        ls = s;
        sls = ss;
    }

    Caustic::OneEulerFilter avg3(60.0f /* rate */, 1.0f /* mincutoff */, 0.001f /* beta */, 1.0f /* dcutoff */);
    ls = offsets[0] + 128;
    sls = avg3.Sample(ls);
    for (int i = 1; i < totalSamples; i++)
    {
        float s = ls + offsets[i];
        float ss = avg3.Sample(s);
        clr[0] = 255;
        clr[1] = 255;
        clr[2] = 0;
        clr[3] = 255;
        Caustic::Vector2 tv0 = Caustic::Vector2(float(i - 1), float(sls));
        Caustic::Vector2 tv1 = Caustic::Vector2(float(i), float(ss));
        spImage->DrawLine(tv0, tv1, clr);
        ls = s;
        sls = ss;
    }

    Caustic::StoreImage(L"g:\\output.png", spImage);
    
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTSAMPLERS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTSAMPLERS));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTSAMPLERS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTSAMPLERS);
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
