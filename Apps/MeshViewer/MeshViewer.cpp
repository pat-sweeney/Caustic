//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include <d3d11.h>
#include <atlbase.h>
#include "MeshViewer.h"
#include <Windows.h>
#include <commdlg.h>
#include <string>
#include <map>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.IMeshConstructor;
import Geometry.MeshImport;
import Geometry.Mesh.Mesh;
import Imaging.Color;
import Rendering.Caustic.ISpotLight;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.IRenderer;
import Rendering.RenderWindow.IRenderWindow;
import Rendering.SceneGraph.ISceneFactory;

#define MAX_LOADSTRING 100

using namespace Caustic;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct ImGuiEvent
{
    HWND hWnd;
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
};

class CApp
{
public:
    HWND m_hwnd;                                          // HWND bound to renderer
    HINSTANCE m_hInst;
    CRefObj<IRenderer> m_spRenderer;                      // Instance of our renderer
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<ICausticFactory> m_spCausticFactory;
    CRefObj<ISceneFactory> m_spSceneFactory;
    CRITICAL_SECTION m_cs;
    std::vector<ImGuiEvent> m_events;
    bool m_ImGuiInitialized;

    CApp() : m_ImGuiInitialized(false)
    {
        InitializeCriticalSection(&m_cs);
    }

    void InitializeCaustic(HWND hwnd);
};
CApp app;

void InitializeCaustic(HWND hwnd)
{
    app.m_spSceneFactory = Caustic::CreateSceneFactory();
    app.m_spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    app.m_spRenderWindow = CreateRenderWindow(hwnd, shaderFolder, 
        [](IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass) {
            if (app.m_ImGuiInitialized && pass == c_PassOpaque)
            {
                EnterCriticalSection(&app.m_cs);
                for (size_t i = 0; i < app.m_events.size(); i++)
                    ImGui_ImplWin32_WndProcHandler(app.m_events[i].hWnd, app.m_events[i].msg, app.m_events[i].wParam, app.m_events[i].lParam);
                app.m_events.resize(0);
                LeaveCriticalSection(&app.m_cs);

                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
        },
        [](IRenderer*) {
            if (app.m_ImGuiInitialized)
            {
                ImGui::Render();
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            }
        });
    app.m_spRenderer = app.m_spRenderWindow->GetRenderer();
}

// Global Variables:
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
   app.m_hInst = hInstance; // Store instance handle in our global variable

   app.m_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!app.m_hwnd)
   {
      return FALSE;
   }

   // Setup our renderer
   InitializeCaustic(app.m_hwnd);

    app.m_spRenderer->RunOnRenderer([](IRenderer* pRenderer) {
           auto ctx = ImGui::CreateContext();
           ImGui_ImplWin32_Init(app.m_hwnd);
           ImGui_ImplDX11_Init(pRenderer->GetDevice(), pRenderer->GetContext());
           app.m_ImGuiInitialized = true;
           }, true);

   ShowWindow(app.m_hwnd, nCmdShow);
   UpdateWindow(app.m_hwnd);

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
    ImGuiEvent evt;
    evt.hWnd = hWnd;
    evt.lParam = lParam;
    evt.wParam = wParam;
    evt.msg = message;
    EnterCriticalSection(&app.m_cs);
    app.m_events.push_back(evt);
    LeaveCriticalSection(&app.m_cs);
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(app.m_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_PDF_SPHERE:
            {
                CRefObj<IMeshConstructor> spMeshConstructor = IMeshConstructor::Create();
                auto spherePDF = [](Vector3& v)->float
                {
                    static Vector3 center(0.5f, 0.5f, 0.5f);
                    Vector3 q = v - center;
                    return static_cast<float>(max(fabs(q.x), max(fabs(q.y), fabs(q.z)))) - 0.5f;
                    //return (v - center).Length() - 0.5f;
                };
                CRefObj<ISceneMeshElem> spMeshElem = app.m_spSceneFactory->CreateMeshElem();
                CRefObj<IMesh> spMesh = spMeshConstructor->MeshFromDensityFunction(16, spherePDF);
                spMeshElem->SetMesh(spMesh);
                CRefObj<ISceneGraph> spSceneGraph = app.m_spRenderWindow->GetSceneGraph();
                CRefObj<IRenderer> spRenderer = app.m_spRenderWindow->GetRenderer();
                CRefObj<IShader> spShader = app.m_spRenderer->GetShaderMgr()->FindShader(L"Default");
                CRefObj<ISceneMaterialElem> spMaterialElem = app.m_spSceneFactory->CreateMaterialElem();
                CRefObj<IMaterialAttrib> spMaterial = app.m_spCausticFactory->CreateMaterialAttrib();
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

                auto spMC = app.m_spSceneFactory->CreateMarchingCubesElem(spRenderer, 16, spherePDF, false);

                auto spLightCollectionElem = app.m_spSceneFactory->CreateLightCollectionElem();
                spMaterialElem->AddChild(spMC);

                Vector3 lightPos(1000.0f, 1000.0f, 0.0f);
                FRGBColor lightColor(1.0f, 1.0f, 1.0f);
                CRefObj<ILight> spLight(app.m_spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));
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
                    ofn.hInstance = app.m_hInst;
                    ofn.lpstrFilter = L"OBJ Files\0*.obj\0PLY Files\0*.ply\0All Files\0*.*\0\0\0";
                    ofn.lpstrFile = fn;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrTitle = L"Open Mesh";
                    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
                    ofn.lpstrDefExt = L"obj";
                    if (GetOpenFileName(&ofn))
                    {
                        wchar_t *ext = StrRChrW(fn, nullptr, L'.');
                        CRefObj<ISceneMeshElem> spMeshElem = app.m_spSceneFactory->CreateMeshElem();
                        CRefObj<IMesh> spMesh = nullptr;
                        const wchar_t *pShaderName = L"Textured";
                        if (StrCmpW(ext, L".obj") == 0)
                        {
                       //     std::map<std::wstring, std::any> defaultMaterials = { { L"ambientColor", std::any(FRGBAColor(1.0f, 0.0f, 0.0f, 1.0f)) } };
                            spMesh = Caustic::MeshImport::LoadObj(fn, nullptr);
                            pShaderName = L"ObjShader";
                        }
                        else if (StrCmpW(ext, L".ply") == 0)
                        {
                            spMesh = Caustic::MeshImport::LoadPLY(fn);
                            pShaderName = L"Textured";
                        }
                        spMeshElem->SetMesh(spMesh);
                        CRefObj<ISceneGraph> spSceneGraph = app.m_spRenderWindow->GetSceneGraph();
                        CRefObj<IShader> spShader = app.m_spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(pShaderName);
                        CRefObj<ISceneMaterialElem> spMaterialElem = app.m_spSceneFactory->CreateMaterialElem();
                        CRefObj<IMaterialAttrib> spMaterial = app.m_spCausticFactory->CreateMaterialAttrib();
                        FRGBColor ambient(0.2f, 0.2f, 0.2f);
                        FRGBColor diffuse(0.4f, 0.4f, 0.4f);
                        spMaterial->SetColor(L"ambientColor", ambient);
                        spMaterial->SetColor(L"diffuseColor", diffuse);
                        spMaterialElem->SetMaterial(spMaterial);
                        spMaterialElem->SetShader(spShader);

                        auto spLightCollectionElem = app.m_spSceneFactory->CreateLightCollectionElem();
                        
                        Vector3 lightPos(1000.0f, 1000.0f, 0.0f);
                        FRGBColor lightColor(1.0f, 1.0f, 1.0f);
                        CRefObj<ILight> spLight(app.m_spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));
                        spLightCollectionElem->AddLight(spLight);

                        Vector3 lightDir(-1.0f, -1.0f, -1.0f);
                        spLight = app.m_spCausticFactory->CreateDirectionalLight(lightPos, lightDir, lightColor, 1.0f);
                        spLight->SetCastsShadows(true);
                        spLightCollectionElem->AddLight(spLight);
                        spMaterialElem->AddChild(spMeshElem);
                        spLightCollectionElem->AddChild(spMaterialElem);
                        spSceneGraph->AddChild(spLightCollectionElem);

                        // Add plane to scene
                        spMeshElem = app.m_spSceneFactory->CreateMeshElem();
                        spMeshElem->SetMesh(CreateGrid(20));
                        spMeshElem->SetName(L"GridMesh");
                        spShader = app.m_spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(L"TexturedWithShadow");
                        spMaterialElem = app.m_spSceneFactory->CreateMaterialElem();
                        spMaterial = app.m_spCausticFactory->CreateMaterialAttrib();
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
                        spOverlay = app.m_spSceneFactory->CreateOverlay2DElem();
                        auto spTexture = app.m_spCausticFactory->LoadTexture(L"c:\\users\\patricsw\\Pictures\\Capture.PNG", app.m_spRenderWindow->GetRenderer());
                        BBox2 bb;
                        bb.minPt = Vector2(0.0f, 0.0f);
                        bb.maxPt = Vector2(0.1f, 0.1f);
                        spOverlay->SetRect(bb);
                        spOverlay->SetTexture(spTexture);
                        spSceneGraph->AddChild(spOverlay);

                        spOverlay = app.m_spSceneFactory->CreateOverlay2DElem();
                        bb.minPt = Vector2(0.1f, 0.0f);
                        bb.maxPt = Vector2(0.2f, 0.1f);
                        spOverlay->SetRect(bb);
                        spOverlay->SetTexture(app.m_spRenderWindow->GetRenderer()->GetShadowmapTexture(c_HiResShadowMap));
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
