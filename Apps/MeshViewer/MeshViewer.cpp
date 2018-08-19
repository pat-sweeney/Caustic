//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "MeshViewer.h"
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include "Geometry\MeshImport\MeshImport.h"
#include <Windows.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100

using namespace Caustic;

class CCausticRenderer
{
public:
    CRefObj<IRendererMarshaller> m_spMarshaller;
    CRefObj<ISceneGraph> m_spSceneGraph;
    CRefObj<ICamera> m_spCamera;
    CRefObj<IRenderer> m_spRenderer;
    CRefObj<ITrackball> m_spTrackball;
    bool m_tracking;
    Vector3 m_eye;
    Vector3 m_look;
    Vector3 m_up;
    HWND m_hwnd;
    DirectX::XMMATRIX m_view;
    DirectX::XMMATRIX m_invview;
public:
    CCausticRenderer() : m_tracking(false)
    {
    }

    void Initialize(HWND hWnd);
    void MouseDown(int x, int y);
    void MouseMove(int x, int y, uint32 flags);
    void MouseUp(int x, int y);
    void MouseWheel(int factor);
    void AddPointLight(Vector3 &lightPos);
};

void CCausticRenderer::AddPointLight(Vector3 &lightPos)
{
    CRefObj<IScenePointLightElem> spLightElem;
    Scene::CreatePointLightElem(&spLightElem);
    spLightElem->SetPosition(lightPos);
    Vector3 lightColor(1.0f, 1.0f, 1.0f);
    spLightElem->SetColor(lightColor);
    m_spSceneGraph->AddChild(spLightElem.p);
}

void CCausticRenderer::Initialize(HWND hWnd)
{
    m_hwnd = hWnd;
    Caustic::CausticSetup();
    Caustic::CreateRendererMarshaller(&m_spMarshaller);
    m_spMarshaller->Initialize(hWnd);
    Scene::CreateSceneGraph(&m_spSceneGraph);
    m_spMarshaller->SetSceneGraph(m_spSceneGraph.p);
    CreateCamera(true, &m_spCamera);
    Vector3 lightPos(10.0f, 10.0f, 10.0f);
    AddPointLight(lightPos);
    lightPos = Vector3(-10.0f, 10.0f, 10.0f);
    AddPointLight(lightPos);
    lightPos = Vector3(-10.0f, -10.0f, 10.0f);
    AddPointLight(lightPos);
    lightPos = Vector3(-10.0f, 10.0f, 10.0f);
    AddPointLight(lightPos);
    lightPos = Vector3(10.0f, 10.0f, -10.0f);
    AddPointLight(lightPos);
    lightPos = Vector3(-10.0f, 10.0f, -10.0f);
    AddPointLight(lightPos);
    lightPos = Vector3(10.0f, -10.0f, -10.0f);
    AddPointLight(lightPos);
    lightPos = Vector3(-10.0f, -10.0f, -10.0f);
    AddPointLight(lightPos);
    m_spMarshaller->GetRenderer(&m_spRenderer);
    m_spRenderer->SetCamera(m_spCamera.p);
    CreateTrackball(&m_spTrackball);
}

void CCausticRenderer::MouseDown(int x, int y)
{
    Vector3 up;
    m_spCamera->GetPosition(&m_eye, &m_look, nullptr, nullptr, &up, nullptr);
    m_up.x = m_eye.x + up.x;
    m_up.y = m_eye.y + up.y;
    m_up.z = m_eye.z + up.z;
    RECT rect;
    GetClientRect(m_hwnd, &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    m_view = m_spCamera->GetView();
    m_invview = DirectX::XMMatrixInverse(nullptr, m_view);
    m_spTrackball->BeginTracking(x, y, w, h);
    m_tracking = true;
}

void CCausticRenderer::MouseMove(int x, int y, uint32 flags)
{
    if (m_tracking)
    {
        DirectX::XMMATRIX mat;
        ETrackballConstrain constraint;
        if (flags & MK_CONTROL)
            constraint = ETrackballConstrain::Constraint_XAxis;
        else if (flags & MK_SHIFT)
            constraint = ETrackballConstrain::Constraint_YAxis;
        else
            constraint = ETrackballConstrain::Constraint_None;
        if (m_spTrackball->UpdateTracking(x, y, constraint, &mat))
        {
            DirectX::XMVECTOR vLook = DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 1.0f);
            vLook = DirectX::XMVector3Transform(vLook, m_view);
            DirectX::XMMATRIX mtrans = DirectX::XMMatrixTranslation(-DirectX::XMVectorGetX(vLook), -DirectX::XMVectorGetY(vLook), -DirectX::XMVectorGetZ(vLook));
            DirectX::XMMATRIX minvtrans = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(vLook), DirectX::XMVectorGetY(vLook), DirectX::XMVectorGetZ(vLook));
            DirectX::XMMATRIX m = DirectX::XMMatrixMultiply(m_view,
                DirectX::XMMatrixMultiply(mtrans,
                    DirectX::XMMatrixMultiply(mat,
                        DirectX::XMMatrixMultiply(minvtrans, m_invview))));
            DirectX::XMVECTOR eye = DirectX::XMVectorSet(m_eye.x, m_eye.y, m_eye.z, 1.0f);
            DirectX::XMVECTOR up = DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 1.0f);
            eye = DirectX::XMVector4Transform(eye, m);
            up = DirectX::XMVector3Transform(up, m);
            Vector3 neye(DirectX::XMVectorGetX(eye), DirectX::XMVectorGetY(eye), DirectX::XMVectorGetZ(eye));
            Vector3 nup(DirectX::XMVectorGetX(up), DirectX::XMVectorGetY(up), DirectX::XMVectorGetZ(up));
            nup = nup - neye;
            nup.Normalize();
            m_spCamera->SetPosition(neye, m_look, nup);
        }
    }
}

void CCausticRenderer::MouseUp(int x, int y)
{
    m_spTrackball->EndTracking();
    m_tracking = false;
}

void CCausticRenderer::MouseWheel(int factor)
{
    Vector3 eye, look, up, n;
    //    float distance = factor / 120;
    m_spCamera->GetPosition(&eye, &look, &up, nullptr, nullptr, &n);
    Vector3 dirVec = ((factor < 0.0f) ? -1.0f : +1.0f) * n;
    eye = eye + dirVec;
    m_spCamera->SetPosition(eye, look, up);
}

// Global Variables:
CCausticRenderer *pRenderer;
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
   pRenderer = new CCausticRenderer();
   pRenderer->Initialize(hWnd);

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
                        if (StrCmpW(ext, L".ply") == 0)
                        {
                            CRefObj<ISceneMeshElem> spElem;
                            Scene::CreateMeshElem(&spElem);
                            CRefObj<IMesh> spMesh = Caustic::MeshImport::LoadPLY(fn);
                            spElem->SetMesh(spMesh.p);

                            CRefObj<ISceneMaterialElem> spMaterialElem;
                            Scene::CreateMaterialElem(&spMaterialElem);

                            CRefObj<IMaterialAttrib> spMaterial;
                            spMaterialElem->GetMaterial(&spMaterial);

                            Vector3 ambient(0.2f, 0.2f, 0.2f);
                            Vector3 diffuse(0.4f, 0.4f, 0.4f);
                            spMaterial->SetAmbientColor(ambient);
                            spMaterial->SetDiffuseColor(diffuse);

                            CRefObj<IShader> spShader;
                            CShaderMgr::GetInstance()->FindShader(L"Default", &spShader);
                            spMaterialElem->SetPixelShader(spShader.p);

                            pRenderer->m_spSceneGraph->AddChild(spMaterialElem.p);
                            spMaterialElem->AddChild(spElem.p);
                        }
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
        pRenderer->MouseDown((int)LOWORD(lParam), (int)HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
        pRenderer->MouseUp((int)LOWORD(lParam), (int)HIWORD(lParam));
        break;
    case WM_MOUSEMOVE:
        pRenderer->MouseMove((int)LOWORD(lParam), (int)HIWORD(lParam), (uint32)wParam);
        break;
    case WM_MOUSEWHEEL:
        pRenderer->MouseWheel((int)wParam);
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
