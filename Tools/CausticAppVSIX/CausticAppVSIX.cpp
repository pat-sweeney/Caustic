// CausticAppVSIX.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "CausticAppVSIX.h"
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Rendering\RenderWindow\IRenderWindow.h"
#include "Rendering\Caustic\ICausticFactory.h"
#include "Rendering\SceneGraph\ISceneFactory.h"

#define MAX_LOADSTRING 100

using namespace Caustic;

struct CApp
{
    CRefObj<IRenderWindow> spRenderWindow;
    CRefObj<Caustic::ICausticFactory> spCausticFactory;
    CRefObj<Caustic::ISceneFactory> spSceneFactory;

    void InitializeCaustic(HWND hwnd);
};

void CApp::InitializeCaustic(HWND hwnd)
{
    spSceneFactory = Caustic::CreateSceneFactory();
    spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    spRenderWindow = CreateRenderWindow(hwnd, shaderFolder, [](IRenderer*, IRenderCtx*, int) {
        });
}

// Global Variables:
CApp app;
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

    // Create a simple scene
    CRefObj<ISceneGraph> spSceneGraph = app.spRenderWindow->GetSceneGraph();

    // Create the default object (a cube)
    auto spCube = Caustic::CreateCube();
    auto spMeshElem = app.spSceneFactory->CreateMeshElem();
    spMeshElem->SetMesh(spCube);

    CRefObj<IMaterialAttrib> spMaterial = app.spCausticFactory->CreateMaterialAttrib();
    FRGBColor ambient(0.2f, 0.2f, 0.2f);
    FRGBColor diffuse(0.4f, 0.4f, 0.4f);
    spMaterial->SetColor(L"ambientColor", ambient);
    spMaterial->SetColor(L"diffuseColor", diffuse);
    std::vector<CRefObj<IMaterialAttrib>> mats;
    mats.push_back(spMaterial);
    spCube->SetMaterials(mats);

    // Create the material to apply to the cube
    CRefObj<IShader> spShader = app.spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(L"Default");
    CRefObj<ISceneMaterialElem> spMaterialElem = app.spSceneFactory->CreateMaterialElem();
    spMaterialElem->SetMaterial(spMaterial);
    spMaterialElem->SetShader(spShader);
    spMaterialElem->AddChild(spMeshElem);

    // Create a set of lights to illuminate the object
    auto spLightCollectionElem = app.spSceneFactory->CreateLightCollectionElem();
    Vector3 lightPos(1000.0f, 1000.0f, 0.0f);
    FRGBColor lightColor(1.0f, 1.0f, 1.0f);
    CRefObj<ILight> spLight(app.spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));
    spLightCollectionElem->AddLight(spLight);

    Vector3 lightDir(-1.0f, -1.0f, -1.0f);
    spLight = app.spCausticFactory->CreateDirectionalLight(lightPos, lightDir, lightColor, 1.0f);
    spLight->SetCastsShadows(true);
    spLightCollectionElem->AddLight(spLight);
    spMaterialElem->AddChild(spMeshElem);
    spLightCollectionElem->AddChild(spMaterialElem);
    spSceneGraph->AddChild(spLightCollectionElem);
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
