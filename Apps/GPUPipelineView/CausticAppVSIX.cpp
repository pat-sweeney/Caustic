// CausticAppVSIX.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "CausticAppVSIX.h"
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Rendering\RenderWindow\IRenderWindow.h"
#include "Rendering\Caustic\ICausticFactory.h"
#include "Rendering\Caustic\IShader.h"
#include "Imaging\GPUImage\GPUPipeline.h"
#include "Cameras\AzureKinect\IAzureKinect.h"
#include <string>

#define MAX_LOADSTRING 100

using namespace Caustic;

struct CApp
{
    CRefObj<IRenderWindow> spRenderWindow;
    CRefObj<Caustic::ICausticFactory> spCausticFactory;
    CRefObj<IRenderer> spRenderer;
    CRefObj<IAzureKinect> spCamera;
    CRefObj<IGPUPipeline> spGPUPipeline;
    CRefObj<IGPUPipelineSourceNode> spSourceColorNode;
    CRefObj<IGPUPipelineSourceNode> spSourceDepthNode;
    CRefObj<IGPUPipelineNode> spNode;
    CRefObj<ITexture> spOutputTexture;
    CRefObj<IShader> spShader;
    CRefObj<IShader> spDepthSelShader;
    CRefObj<IImage> spLastColorImage;
    CRefObj<IImage> spLastDepthImage;
    CRefObj<IImage> spRayMap;
    CRefObj<ITexture> spRayTex;
    HBITMAP imgbitmap = nullptr;
    int imgwidth, imgheight;
    HWND hwnd;
    Matrix4x4 cameraExt;
    Matrix3x3 cameraInt;

    void InitializeCaustic(HWND hwnd);
};
CApp app;

void CApp::InitializeCaustic(HWND hwnd)
{
    app.hwnd = hwnd;
    spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    spRenderWindow = CreateRenderWindow(hwnd, shaderFolder,
        [](IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)
        {
            if (pass != Caustic::c_PassOpaque)
                return;
            CRefObj<IImage> spColorImage;
            CRefObj<IImage> spDepthImage;
            if (app.spCamera)
            {
                if (app.spCamera->NextFrame(&spColorImage, &spDepthImage, nullptr))
                {
                    if (spColorImage)
                        app.spLastColorImage = spColorImage;
                    if (spDepthImage)
                        app.spLastDepthImage = spDepthImage;
                }
                if (app.spSourceColorNode && app.spLastColorImage && app.spLastDepthImage)
                {
                    app.spSourceColorNode->SetSource(app.spGPUPipeline, app.spLastColorImage);
                    app.spSourceDepthNode->SetSource(app.spGPUPipeline, app.spLastDepthImage);
                    app.spNode->Process(app.spGPUPipeline);
                    auto spTexture = app.spNode->GetOutputTexture(app.spGPUPipeline);
                    if (spTexture)
                        app.spRenderer->DrawScreenQuad(0.0f, 0.0f, 1.0f, 1.0f, spTexture, nullptr);
                }
            }
        });
    app.spRenderer = app.spRenderWindow->GetRenderer();
    app.spRenderer->Freeze();
    
    app.spCamera = Caustic::AzureKinect::CreateAzureKinect(1, AzureKinect::Color1080p, AzureKinect::Depth1024x1024, AzureKinect::FPS30);
    app.cameraExt = app.spCamera->ColorExtrinsics();
    app.cameraInt = app.spCamera->ColorIntrinsics();

    app.spGPUPipeline = Caustic::CreateGPUPipeline(app.spRenderer);

    app.spSourceColorNode = app.spGPUPipeline->CreateSourceNode(nullptr, 1920, 1080, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spSourceDepthNode = app.spGPUPipeline->CreateSourceNode(nullptr, 1024, 1024, DXGI_FORMAT_R16_UINT);

    app.spShader = app.spRenderer->GetShaderMgr()->FindShader(L"RawCopy");
    auto spDownSample = app.spGPUPipeline->CreateNode(app.spShader, 1920 / 32, 1080 / 32, DXGI_FORMAT_B8G8R8A8_UNORM);
    spDownSample->SetInput(L"sourceTexture1", L"sourceSampler1", app.spSourceColorNode);

    app.spDepthSelShader = app.spRenderer->GetShaderMgr()->FindShader(L"DepthSel");

    app.spRayMap = app.spCamera->BuildRayMap(1024, 1024);
    app.spRayTex = Caustic::CreateTexture(app.spRenderer, 1024, 1024, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
    app.spRayTex->CopyFromImage(app.spRenderer, app.spRayMap);
    //app.cameraExt.Inverse();
    Matrix m(app.cameraExt);
    std::any mat = std::any(m);
    app.spDepthSelShader->SetPSParam(L"colorExt", mat);
    Matrix m1(app.cameraInt);
    std::any mat1 = std::any(m1);
    app.spDepthSelShader->SetPSParam(L"colorInt", mat1);
    
    auto ci = app.spCamera->GetAzureColorIntrinsics();
    std::any f1 = std::any(ci.codx);
    std::any f2 = std::any(ci.cody);
    std::any f3 = std::any(ci.cx);
    std::any f4 = std::any(ci.cy);
    std::any f5 = std::any(ci.fx);
    std::any f6 = std::any(ci.fy);
    std::any f7 = std::any(ci.k1);
    std::any f8 = std::any(ci.k2);
    std::any f9 = std::any(ci.k3);
    std::any f10 = std::any(ci.k4);
    std::any f11 = std::any(ci.k5);
    std::any f12 = std::any(ci.k6);
    std::any f13 = std::any(ci.metricRadius);
    std::any f14 = std::any(ci.p1);
    std::any f15 = std::any(ci.p2);
    app.spDepthSelShader->SetPSParam(L"codx", f1);
    app.spDepthSelShader->SetPSParam(L"cody", f2);
    app.spDepthSelShader->SetPSParam(L"cx", f3);
    app.spDepthSelShader->SetPSParam(L"cy", f4);
    app.spDepthSelShader->SetPSParam(L"fx", f5);
    app.spDepthSelShader->SetPSParam(L"fy", f6);
    app.spDepthSelShader->SetPSParam(L"k1", f7);
    app.spDepthSelShader->SetPSParam(L"k2", f8);
    app.spDepthSelShader->SetPSParam(L"k3", f9);
    app.spDepthSelShader->SetPSParam(L"k4", f10);
    app.spDepthSelShader->SetPSParam(L"k5", f11);
    app.spDepthSelShader->SetPSParam(L"k6", f12);
    app.spDepthSelShader->SetPSParam(L"metricRadius", f13);
    app.spDepthSelShader->SetPSParam(L"p1", f14);
    app.spDepthSelShader->SetPSParam(L"p2", f15);
    std::any v(Int(ci.type));
    app.spDepthSelShader->SetPSParam(L"type", v);

    std::any raymap = std::any(app.spRayTex);
    app.spDepthSelShader->SetPSParam(L"rayTexture", raymap);

    app.spNode = app.spGPUPipeline->CreateNode(app.spDepthSelShader, 1920, 1080, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spNode->SetInput(L"colorTex", L"colorSampler", app.spSourceColorNode);
    app.spNode->SetInput(L"blurTex", L"colorSampler", spDownSample);
    app.spNode->SetInput(L"depthTex", nullptr, app.spSourceDepthNode);
    app.spRenderer->Unfreeze();
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
    LoadStringW(hInstance, IDC_CAUSTICAPPVSIX, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAUSTICAPPVSIX));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAUSTICAPPVSIX));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CAUSTICAPPVSIX);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

void DrawImage(HDC hdc, HBITMAP img, int w, int h)
{
    if (img != nullptr)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HGDIOBJ oldBitmap = SelectObject(hdcMem, img);
        RECT rect;
        GetClientRect(app.hwnd, &rect);
        SetStretchBltMode(hdc, COLORONCOLOR);
        StretchBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, w, h, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteObject(hdcMem);
    }
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
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        app.spRenderWindow->MouseDown((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
        break;
    case WM_LBUTTONUP:
        app.spRenderWindow->MouseUp((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
        break;
    case WM_MOUSEMOVE:
        app.spRenderWindow->MouseMove((int)LOWORD(lParam), (int)HIWORD(lParam), (uint32)wParam);
        break;
    case WM_MOUSEWHEEL:
        app.spRenderWindow->MouseWheel((int)wParam);
        break;
    case WM_KEYDOWN:
        app.spRenderWindow->MapKey((uint32)wParam, (uint32)lParam);
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
