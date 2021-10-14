//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "MeshViewer.h"
import Base.Core.Core;
import Base.Core.IRefCount;
#include "Rendering\RenderWindow\IRenderWindow.h"
#include "Rendering\Caustic\ICausticFactory.h"
#include "Rendering\SceneGraph\ISceneFactory.h"
#include "Geometry\MeshImport\MeshImport.h"
#include "Geometry\Mesh\Mesh.h"
#include "Geometry\Mesh\IMeshConstructor.h"
#include <Windows.h>
#include <commdlg.h>
#include <string>

#define MAX_LOADSTRING 100

using namespace Caustic;

CRefObj<IRenderWindow> spRenderWindow;
CRefObj<Caustic::ICausticFactory> spCausticFactory;
CRefObj<Caustic::ISceneFactory> spSceneFactory;

void InitializeCaustic(HWND hwnd)
{
    spSceneFactory = Caustic::CreateSceneFactory();
    spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    spRenderWindow = CreateRenderWindow(hwnd, shaderFolder, [](IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass) {
        ////CRefObj<ISceneGraph> spSceneGraph = spRenderWindow->GetSceneGraph();
        ////if (spSceneGraph && spRenderWindow->GetRenderer())
        ////{
        ////    PathTraceCtx ctx;
        ////    ctx.spp = 4;
        ////    ctx.maxDepth = 10;
        ////    CRefObj<IImage> spImage = Caustic::CreateImage(1024, 1024, 24);
        ////    spSceneGraph->PathTrace(pRenderer, &ctx, spImage);
        ////    Caustic::StoreImage(L"c:\\temp\\florp.png", spImage);
        ////}
        },
        [](IRenderer*) {
        });
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
            case ID_PDF_SPHERE:
            {
                CRefObj<IMeshConstructor> spMeshConstructor = Caustic::CreateMeshConstructor();
                auto spherePDF = [](Vector3& v)->float
                {
                    static Vector3 center(0.5f, 0.5f, 0.5f);
                    Vector3 q = v - center;
                    return max(fabs(q.x), max(fabs(q.y), fabs(q.z))) - 0.5f;
                    return (v - center).Length() - 0.5f;
                };
                CRefObj<ISceneMeshElem> spMeshElem = spSceneFactory->CreateMeshElem();
                CRefObj<IMesh> spMesh = spMeshConstructor->MeshFromDensityFunction(16, spherePDF);
                spMeshElem->SetMesh(spMesh);
                CRefObj<ISceneGraph> spSceneGraph = spRenderWindow->GetSceneGraph();
                CRefObj<IRenderer> spRenderer = spRenderWindow->GetRenderer();
                CRefObj<IShader> spShader = spRenderer->GetShaderMgr()->FindShader(L"Default");
                CRefObj<ISceneMaterialElem> spMaterialElem = spSceneFactory->CreateMaterialElem();
                CRefObj<IMaterialAttrib> spMaterial = spCausticFactory->CreateMaterialAttrib();
                FRGBColor ambient(0.2f, 0.2f, 0.2f);
                FRGBColor diffuse(0.4f, 0.4f, 0.4f);
                spMaterial->SetColor(L"ambientColor", ambient);
                spMaterial->SetColor(L"diffuseColor", diffuse);
                spMaterial->SetFillMode(D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
                spMaterial->SetCullMode(D3D11_CULL_NONE);
                spMaterialElem->SetMaterial(spMaterial);
                spMaterialElem->SetShader(spShader);
                Matrix4x4 xm = Matrix4x4::TranslationMatrix(1.0f, 0.0f, 0.0f);
                spMaterialElem->SetTransform(xm);
                spMaterialElem->AddChild(spMeshElem);

                auto spMC = spSceneFactory->CreateMarchingCubesElem(spRenderer, 16, spherePDF, false);

                auto spLightCollectionElem = spSceneFactory->CreateLightCollectionElem();
                spMaterialElem->AddChild(spMC);

                Vector3 lightPos(1000.0f, 1000.0f, 0.0f);
                FRGBColor lightColor(1.0f, 1.0f, 1.0f);
                CRefObj<ILight> spLight(spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));
                spLightCollectionElem->AddLight(spLight);
                spLightCollectionElem->AddChild(spMaterialElem);

                spSceneGraph->AddChild(spLightCollectionElem);
            }
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
                        CRefObj<ISceneMeshElem> spMeshElem = spSceneFactory->CreateMeshElem();
                        CRefObj<IMesh> spMesh = nullptr;
                        const wchar_t *pShaderName = L"Textured";
                        if (StrCmpW(ext, L".obj") == 0)
                        {
                            spMesh = Caustic::MeshImport::LoadObj(fn);
                            pShaderName = L"ObjShader";
                        }
                        else if (StrCmpW(ext, L".ply") == 0)
                        {
                            spMesh = Caustic::MeshImport::LoadPLY(fn);
                            pShaderName = L"Textured";
                        }
                        spMeshElem->SetMesh(spMesh);
                        CRefObj<ISceneGraph> spSceneGraph = spRenderWindow->GetSceneGraph();
                        CRefObj<IShader> spShader = spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(pShaderName);
                        CRefObj<ISceneMaterialElem> spMaterialElem = spSceneFactory->CreateMaterialElem();
                        CRefObj<IMaterialAttrib> spMaterial = spCausticFactory->CreateMaterialAttrib();
                        FRGBColor ambient(0.2f, 0.2f, 0.2f);
                        FRGBColor diffuse(0.4f, 0.4f, 0.4f);
                        spMaterial->SetColor(L"ambientColor", ambient);
                        spMaterial->SetColor(L"diffuseColor", diffuse);
                        spMaterialElem->SetMaterial(spMaterial);
                        spMaterialElem->SetShader(spShader);

                        auto spLightCollectionElem = spSceneFactory->CreateLightCollectionElem();
                        
                        Vector3 lightPos(1000.0f, 1000.0f, 0.0f);
                        FRGBColor lightColor(1.0f, 1.0f, 1.0f);
                        CRefObj<ILight> spLight(spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));
                        spLightCollectionElem->AddLight(spLight);

                        Vector3 lightDir(-1.0f, -1.0f, -1.0f);
                        spLight = spCausticFactory->CreateDirectionalLight(lightPos, lightDir, lightColor, 1.0f);
                        spLight->SetCastsShadows(true);
                        spLightCollectionElem->AddLight(spLight);
                        spMaterialElem->AddChild(spMeshElem);
                        spLightCollectionElem->AddChild(spMaterialElem);
                        spSceneGraph->AddChild(spLightCollectionElem);

                        // Add plane to scene
                        spMeshElem = spSceneFactory->CreateMeshElem();
                        spMeshElem->SetMesh(CreateGrid(20));
                        spMeshElem->SetName(L"GridMesh");
                        spShader = spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(L"TexturedWithShadow");
                        spMaterialElem = spSceneFactory->CreateMaterialElem();
                        spMaterial = spCausticFactory->CreateMaterialAttrib();
                        ambient = FRGBColor(0.2f, 0.2f, 0.2f);
                        diffuse = FRGBColor(0.4f, 0.4f, 0.4f);
                        spMaterial->SetColor(L"ambientColor", ambient);
                        spMaterial->SetColor(L"diffuseColor", diffuse);
                        spMaterialElem->SetMaterial(spMaterial);
                        spMaterialElem->SetShader(spShader);
                        spMaterialElem->AddChild(spMeshElem);
                        Matrix4x4 mat = Matrix4x4::RotationMatrix(
                            Caustic::DegreesToRadians(90.0f), 0.0f, 0.0f) * 
                            Matrix4x4::ScalingMatrix(250.0f, 250.0f, 250.0f) * 
                            Matrix4x4::TranslationMatrix(0.0f, -10.0f, 0.0f);
                        spMaterialElem->SetTransform(mat);
                        spSceneGraph->AddChild(spMaterialElem);

                        CRefObj<ISceneOverlay2DElem> spOverlay;
                        spOverlay = spSceneFactory->CreateOverlay2DElem();
                        auto spTexture = spCausticFactory->LoadTexture(L"c:\\users\\patricsw\\Pictures\\Capture.PNG", spRenderWindow->GetRenderer());
                        BBox2 bb;
                        bb.minPt = Vector2(0.0f, 0.0f);
                        bb.maxPt = Vector2(0.1f, 0.1f);
                        spOverlay->SetRect(bb);
                        spOverlay->SetTexture(spTexture);
                        spSceneGraph->AddChild(spOverlay);

                        spOverlay = spSceneFactory->CreateOverlay2DElem();
                        bb.minPt = Vector2(0.1f, 0.0f);
                        bb.maxPt = Vector2(0.2f, 0.1f);
                        spOverlay->SetRect(bb);
                        spOverlay->SetTexture(spRenderWindow->GetRenderer()->GetShadowmapTexture(c_HiResShadowMap));
                        spSceneGraph->AddChild(spOverlay);
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
