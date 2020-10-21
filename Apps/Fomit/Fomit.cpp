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
#include "Rendering\RenderGraph\IRenderGraph.h"
#include "Geometry\MeshImport\MeshImport.h"
#include "Cameras\AzureKinect\IAzureKinect.h"
#include "Rendering\Caustic\ConstructBuffers.h"
#include "Geometry\Mesh\IMeshConstructor.h"
#include "Rendering\RenderGraph\IRenderGraph.h"
#include "Rendering\RenderGraph\IRenderGraphFactory.h"
#include <Windows.h>
#include <commdlg.h>

using namespace Caustic;
namespace Caustic {
    extern void CreatePointCloudSubMesh(IRenderer* pRenderer, IShader* pShader, IRenderMaterial* pFrontMaterial, IRenderMaterial* pBackMaterial, std::vector<CGeomVertex>& verts, IRenderSubMesh** ppSubMesh);
}

class CApp
{
public:
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<Caustic::ICausticFactory> m_spCausticFactory;
    CRefObj<IRenderGraphFactory> m_spRenderGraphFactory;
    CRefObj<IRenderGraph> m_spRenderGraph;
    CRefObj<IAzureKinect> m_spCamera;
    CRefObj<IImage> m_spRayMap;

    void InitializeCaustic(HWND hWnd);
};
CApp app;

struct Vertex
{
    float pos[3];
    float norm[3];
};

void CApp::InitializeCaustic(HWND hwnd)
{
#ifdef SUPPORT_RENDERGRAPH
    // First create our underlying Caustic factory.
    // I have not yet made counter parts to all the Caustic objects
    // in the render graph (and probably won't since they are unnecessary
    // and would only act as wrappers. Wrapping for its own sake is bad design IMHO).
    m_spCausticFactory = Caustic::CreateCausticFactory();

    // Next create our output window
    std::wstring shaderFolder(SHADERPATH);
    m_spRenderWindow = CreateRenderWindow(hwnd, shaderFolder, [](IRenderer*, IRenderCtx*, int) {});

    m_spCamera = CreateAzureKinect(0, AzureKinect::ColorMode::Color1080p, AzureKinect::Depth512x512, AzureKinect::FPS30);

    // Load our shaders
    CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();
    CRefObj<IShaderMgr> spShaderMgr = spRenderer->GetShaderMgr();
    CRefObj<IShader> spDepth2PointsShader = spShaderMgr->FindShader(L"Depth2Points");
    CRefObj<IShader> spDefaultShader = spShaderMgr->FindShader(L"Textured");

    m_spRenderGraphFactory = Caustic::CreateRenderGraphFactory();
    m_spRenderGraph = m_spRenderWindow->GetRenderGraph();

    m_spRayMap = m_spCamera->BuildRayMap(512, 512);
    CRefObj<IRenderGraphNode_ImageToTexture> spImageNode = m_spRenderGraphFactory->CreateImageToTextureNode();
    spImageNode->SetName("Ray2Texture");
    spImageNode->FindInputPin("image")->SetDefaultValue(std::any(m_spRayMap));

    // The render graph we are building looks like:
    //
    //                        +---------------------------------------+
    // +--------+             |                 Group                 |
    // | lights |-----> o-----| +--------+                            |
    // +--------+      lights | |  Mesh  |                            |
    //                        | | (cube) |                            |
    // +----------+           | +--------+                            |
    // | material |---> 0-----| +----------------+     +----------+   |
    // +----------+  frontMat | |  Depth2Points  |---->|   Mesh   |   |
    //                        | | Compute Shader |     | (Points) |   |
    //                        | +----------------+     +----------+   |
    //                        |                                       |
    //                        +---------------------------------------+
    //

    CRefObj<IRenderGraphNode_LightCollection> spLightsNode = m_spRenderGraphFactory->CreateLightCollectionNode();
    spLightsNode->SetName("LightsCollection");
    m_spRenderGraph->AddChild(spLightsNode);

    // Add light @ 10,10,0
    Vector3 lightPos(10.0f, 10.0f, 0.0f);
    FRGBColor lightColor(1.0f, 1.0f, 1.0f);
    CRefObj<IPointLight> spPointLight = m_spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f);
    spLightsNode->AddLight(spPointLight);

    // Add light @ -10,10,0
    Vector3 lightPos2(-10.0f, 10.0f, 0.0f);
    CRefObj<IPointLight> spPointLight2 = m_spCausticFactory->CreatePointLight(lightPos2, lightColor, 1.0f);
    spLightsNode->AddLight(spPointLight2);

    CRefObj<IRenderGraphNode_Group> spGroupNode = m_spRenderGraphFactory->CreateGroupNode();
    spGroupNode->SetName("TopGroup");
    m_spRenderGraph->AddChild(spGroupNode);

    // Create our mesh (a cube). Our scene will have two objects: 1) a cube at the origin
    // that will display the camera's color image and 2) a point cloud displaying the depth image
    CRefObj<IMesh> spMesh = Caustic::CreateCube();
    CRefObj<IRenderGraphNode_Mesh> spCubeMeshNode = m_spRenderGraphFactory->CreateMeshNode();
    spCubeMeshNode->SetName("Cube");
    spGroupNode->AddChild(spCubeMeshNode);
    spCubeMeshNode->FindInputPin("mesh")->SetDefaultValue(std::any(spMesh));

    // Create a material node to attach to the cube
    CRefObj<IRenderGraphNode_PhongMaterial> spMaterialNode = m_spRenderGraphFactory->CreatePhongMaterialNode();
    spMaterialNode->SetName("FrontMaterial");
    m_spRenderGraph->AddChild(spMaterialNode);

    CRefObj<IRenderGraphNode_DepthCameraSource> spDepthCameraNode = m_spRenderGraphFactory->CreateDepthCameraSource(m_spCamera);
    spDepthCameraNode->SetName("DepthCamera");
    m_spRenderGraph->AddChild(spDepthCameraNode);
    spDepthCameraNode->FindOutputPin("colorImage")->LinkTo(spMaterialNode->FindInputPin("diffuseTexture"));
    CRefObj<IRenderGraphPin> spGroupMaterialPin = spGroupNode->FindInputPin("frontMaterial");
    spMaterialNode->FindOutputPin("material")->LinkTo(spGroupMaterialPin);

    // Create our Depth2Points compute shader
    CRefObj<IRenderGraphNode_Compute> spComputeNode = m_spRenderGraphFactory->CreateComputeNode(spDepth2PointsShader);
    spComputeNode->SetName("Depth2Points");

    CRefObj<IRenderGraphPin> spGroupLightsPin = spGroupNode->FindInputPin("lights");
    spLightsNode->FindOutputPin("lights")->LinkTo(spGroupLightsPin);

    //                   uint32 depthWidth2 = spDepthImage->GetWidth() / 2; // We need half the width since the shader does 2 depth points at a time
 //                   pComputeElem->SetShaderParam(L"depthImageWidth", depthWidth2);
 //                   pComputeElem->SetInputBuffer(L"DepthBuffer", spDepthImage->GetData(), spDepthImage->GetStride() * spDepthImage->GetHeight(), spDepthImage->GetBytesPerPixel() * 2);
 //                   pComputeElem->SetInputBuffer(L"RayBuffer", spRayMap->GetData(), spRayMap->GetStride() * spRayMap->GetHeight(), spRayMap->GetBytesPerPixel());
 //                   pComputeElem->SetInputThreads(depthWidth2, spDepthImage->GetHeight());
 //                   pComputeElem->SetOutputBuffer(L"Points", (uint8*)vertices.get(), (uint32)(sizeof(Vertex) * numVertices), (uint32)sizeof(Vertex));
   // spComputeElem->SetPostRenderCallback([pComputeShader, pComputeElem](int pass) {
   //     //spMeshElem->SetFlags(spMeshElem->GetFlags() | ESceneElemFlags::MaterialDirty);
   //     //spMaterial->SetTexture(L"diffuseTexture", spOutputImage, EShaderAccess::PixelShader);
   //     });
#endif // SUPPORT_RENDERGRAPH
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
   app.InitializeCaustic(hWnd);

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
