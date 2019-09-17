//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "MeshViewer.h"
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Rendering\RenderWindow\IRenderWindow.h"
#include "Rendering\Caustic\ICausticFactory.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include "Rendering\SceneGraph\ISceneFactory.h"
#include "Geometry\MeshImport\MeshImport.h"
#include <Windows.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100

using namespace Caustic;

CRefObj<IRenderWindow> spRenderWindow;
CRefObj<Caustic::ICausticFactory> spCausticFactory;
CRefObj<Caustic::ISceneFactory> spSceneFactory;

void AddPointLight(Vector3 &lightPos)
{
    CRefObj<IScenePointLightElem> spLightElem;
    spSceneFactory->CreatePointLightElem(&spLightElem);
    spLightElem->SetPosition(lightPos);
    Vector3 lightColor(1.0f, 1.0f, 1.0f);
    spLightElem->SetColor(lightColor);
    spRenderWindow->GetSceneGraph()->AddChild(spLightElem.p);
}

void InitializeCaustic(HWND hwnd)
{
	Caustic::CreateSceneFactory(&spSceneFactory);
	Caustic::CreateCausticFactory(&spCausticFactory);
    std::wstring shaderFolder(SHADERPATH);
    CreateRenderWindow(hwnd, shaderFolder, &spRenderWindow);
    Vector3 lightPos(10.0f, 10.0f, 0.0f);
    AddPointLight(lightPos);
    lightPos = Vector3(-10.0f, 10.0f, 0.0f);
    AddPointLight(lightPos);
}

// Global Variables:
HWND hWnd;
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
    LoadStringW(hInstance, IDC_MESHVIEWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MESHVIEWER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MESHVIEWER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MESHVIEWER);
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

   // Setup our renderer
   InitializeCaustic(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
            case IDM_LOADMESH:
                {
                    wchar_t fn[MAX_PATH + 1] = { 0 };
                    OPENFILENAME ofn;
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(OPENFILENAME);
                    ofn.hwndOwner = hWnd;
                    ofn.hInstance = hInst;
                    ofn.lpstrFilter = L"OBJ Files\0*.obj\0PLY Files\0*.ply\0All Files\0*.*\0\0\0";
                    ofn.lpstrFile = fn;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrTitle = L"Open Mesh";
                    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
                    ofn.lpstrDefExt = L"obj";
                    if (GetOpenFileName(&ofn))
                    {
                        wchar_t *ext = StrRChrW(fn, nullptr, L'.');
                        CRefObj<ISceneMeshElem> spElem;
                        spSceneFactory->CreateMeshElem(&spElem);
                        CRefObj<IMesh> spMesh = nullptr;
                        if (StrCmpW(ext, L".obj") == 0)
                            spMesh = Caustic::MeshImport::LoadObj(fn);
                        else if (StrCmpW(ext, L".ply") == 0)
                            spMesh = Caustic::MeshImport::LoadPLY(fn);
                        spElem->SetMesh(spMesh.p);

                        CRefObj<ISceneMaterialElem> spMaterialElem;
                        spSceneFactory->CreateMaterialElem(&spMaterialElem);

                        CRefObj<IMaterialAttrib> spMaterial;
                        spMaterialElem->GetMaterial(&spMaterial);

                        Vector3 ambient(0.2f, 0.2f, 0.2f);
                        Vector3 diffuse(0.4f, 0.4f, 0.4f);
                        spMaterial->SetAmbientColor(ambient);
                        spMaterial->SetDiffuseColor(diffuse);

                        CRefObj<IShader> spShader;
                        CShaderMgr::Instance()->FindShader(L"Default", &spShader);
                        spMaterialElem->SetPixelShader(spShader.p);

                        spRenderWindow->GetSceneGraph()->AddChild(spMaterialElem.p);
                        spMaterialElem->AddChild(spElem.p);
                    }
                }
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
        spRenderWindow->MouseDown((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
        break;
    case WM_LBUTTONUP:
        spRenderWindow->MouseUp((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
        break;
    case WM_MOUSEMOVE:
        spRenderWindow->MouseMove((int)LOWORD(lParam), (int)HIWORD(lParam), (uint32)wParam);
        break;
    case WM_MOUSEWHEEL:
        spRenderWindow->MouseWheel((int)wParam);
        break;
    case WM_KEYDOWN:
        spRenderWindow->MapKey((uint32)wParam, (uint32)lParam);
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
