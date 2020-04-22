//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Fomit.h"
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Imaging\Image\ImageFilter.h"
#include "Rendering\RenderWindow\IRenderWindow.h"
#include "Rendering\Caustic\ICausticFactory.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Rendering\SceneGraph\ISceneFactory.h"
#include "Geometry\MeshImport\MeshImport.h"
#include "Cameras\AzureKinect\IAzureKinect.h"
#include "Rendering\Caustic\ConstructBuffers.h"
#include "Geometry\Mesh\IMeshConstructor.h"
#include <Windows.h>
#include <commdlg.h>

using namespace Caustic;
namespace Caustic {
    extern void CreatePointCloudSubMesh(IRenderer* pRenderer, IShader* pShader, IRenderMaterial* pFrontMaterial, IRenderMaterial* pBackMaterial, std::vector<CGeomVertex>& verts, IRenderSubMesh** ppSubMesh);
}

CRefObj<IRenderWindow> spRenderWindow;
CRefObj<Caustic::ICausticFactory> spCausticFactory;
CRefObj<Caustic::ISceneFactory> spSceneFactory;
CRefObj<IRenderMaterial> spFrontMaterial;
CRefObj<IRenderMaterial> spBackMaterial;
CRefObj<IShader> spShader;
CRefObj<IMaterialAttrib> spMatAttrib;
CRefObj<IRenderMaterial> spRenderMaterial;
CRefObj<IImage> spRayMap;
DirectX::XMMATRIX xm;
CRITICAL_SECTION cs;
std::vector<CGeomVertex> verts;
CRefObj<IAzureKinect> spCamera;

void AddPointLight(Vector3& lightPos)
{
    CRefObj<IScenePointLightElem> spLightElem;
    spSceneFactory->CreatePointLightElem(&spLightElem);
    spLightElem->SetPosition(lightPos);
    Vector3 lightColor(1.0f, 1.0f, 1.0f);
    spLightElem->SetColor(lightColor);
    spRenderWindow->GetSceneGraph()->AddChild(spLightElem);
}

CRefObj<IRenderable> spRenderable;
CRefObj<IMesh> spMesh;
CRefObj<ISceneMeshElem> spMeshElem;
CRefObj<IRenderMesh> spRenderMesh;
CRefObj<IRenderSubMesh> spRenderSubMesh;
CRefObj<IMaterialAttrib> spMaterial;
CRefObj<IImage> spOutputImage;
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

    InitializeCriticalSection(&cs);

    CreateCube(&spMesh);
    
    // Next create our scene graph mesh element
    spSceneFactory->CreateMeshElem(&spMeshElem);
    spMeshElem->SetMesh(spMesh);

    // Find our shader
    CRefObj<IRenderer> spRenderer = spRenderWindow->GetRenderer();
    CRefObj<IShaderMgr> spShaderMgr = spRenderer->GetShaderMgr();
    CRefObj<IShader> spComputeShader;
    spShaderMgr->FindShader(L"Brighten", &spComputeShader);
    CRefObj<IShader> spDefaultShader;
    spShaderMgr->FindShader(L"Textured", &spDefaultShader);

    // Assign a material to our mesh
    CRefObj<ISceneMaterialElem> spMaterialElem;
    spSceneFactory->CreateMaterialElem(&spMaterialElem);
    spMaterialElem->GetMaterial(&spMaterial);
    std::vector<CRefObj<IMaterialAttrib>> attribs;
    attribs.push_back(spMaterial);
    spMesh->SetMaterials(attribs);
    spMaterialElem->SetShader(spDefaultShader);
    spMaterialElem->AddChild(spMeshElem);
    spRenderWindow->GetSceneGraph()->AddChild(spMaterialElem);

    CRefObj<ISceneComputeShaderElem> spComputeElem;
    spSceneFactory->CreateComputeShaderElem(spComputeShader, &spComputeElem);
    IShader* pComputeShader = spComputeShader;
    ISceneComputeShaderElem* pComputeElem = spComputeElem;
    int counter = 0;
    spComputeElem->SetPreRenderCallback([pComputeShader, pComputeElem]() {
        if (spCamera != nullptr)
        {
            CRefObj<IImage> spColorImage;
            if (spCamera->NextFrame(&spColorImage, nullptr, nullptr))
            {
                if (spColorImage)
                {
                    pComputeElem->SetShaderParam(L"imageWidth", spColorImage->GetWidth());
                    pComputeElem->SetShaderParam(L"imageHeight", spColorImage->GetHeight());
                    pComputeElem->SetShaderParam(L"brightness", (uint32)10);
                    pComputeElem->SetInputBuffer(L"ColorBuffer", spColorImage->GetData(), spColorImage->GetStride() * spColorImage->GetHeight(), spColorImage->GetBytesPerPixel());
                    pComputeElem->SetInputThreads(spColorImage->GetWidth(), spColorImage->GetHeight());
                    CreateImage(spColorImage->GetWidth(), spColorImage->GetHeight(), 32, &spOutputImage);
                    pComputeElem->SetOutputBuffer(L"Brightened", (uint8*)spOutputImage->GetData(), (uint32)(spOutputImage->GetStride() * spOutputImage->GetHeight()), spOutputImage->GetBytesPerPixel());
                }
            }
        }
        });
    spComputeElem->SetPostRenderCallback([pComputeShader, pComputeElem]() {
        spMeshElem->SetFlags(spMeshElem->GetFlags() | ESceneElemFlags::MaterialDirty);
        spMaterial->SetTexture(L"diffuseTexture", spOutputImage, EShaderAccess::PixelShader);
        });
    spRenderWindow->GetSceneGraph()->AddChild(spComputeElem);
    CreateAzureKinect(0, AzureKinect::ColorMode::Color1080p, AzureKinect::Depth512x512, AzureKinect::FPS30, &spCamera);
    spCamera->BuildRayMap(512, 512, &spRayMap);
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
    LoadStringW(hInstance, IDC_FOMIT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FOMIT));

    MSG msg;

    // Main message loop:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOMIT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FOMIT);
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
   InitializeCaustic(hWnd);

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
