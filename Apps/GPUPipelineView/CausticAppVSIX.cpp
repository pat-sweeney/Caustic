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
#include "Imaging\GPUImage\GPUPipelineImpl.h"
#include "Cameras\AzureKinect\IAzureKinect.h"
#include "Rendering\SceneImport\Collada.h"
#include "Rendering\SceneGraph\ISceneFactory.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Geometry\MeshImport\MeshImport.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <string>
#include <windows.h>
#include <winuser.h>

#define MAX_LOADSTRING 100

using namespace Caustic;

class CApp
{
public:
    CRefObj<IImage> spBackgroundImage;
    CRefObj<ITexture> spBackgroundTexture;
    CRefObj<ISceneLightCollectionElem> m_spLightCollectionElem;
    CRefObj<IRenderWindow> spRenderWindow;
    CRefObj<Caustic::ICausticFactory> spCausticFactory;
    CRefObj<IRenderer> spRenderer;
    CRefObj<IAzureKinect> spCamera;
    CRefObj<IGPUPipeline> spGPUPipeline;
    CRefObj<IGPUPipelineSourceNode> spSourceColorNode;
    CRefObj<IGPUPipelineSourceNode> spSourceDepthNode;
    CRefObj<IGPUPipelineNode> spNode;
    CRefObj<IGPUPipelineNode> spNodeSmooth;
    CRefObj<ITexture> spOutputTexture;
    CRefObj<IShader> spShader;
    CRefObj<IShader> spBokehShader;
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
    float depth[4];
    bool colored;
    bool useBackground;
    bool renderModel;
    bool renderSkeleton;
    bool focusTracking;
    bool renderCOC;
    bool showInFocus;
    bool guiInited;
    float focusDistance;
    float focusMaxWidth;
    float focusWidth;
    float BokehRadius;
    bool smooth;

    CApp() :
        smooth(false),
        guiInited(false),
        colored(false),
        renderModel(false),
        renderSkeleton(false),
        useBackground(true),
        focusTracking(true),
        renderCOC(false),
        showInFocus(false),
        focusDistance(0.0f),
        focusMaxWidth(0.057f),
        focusWidth(0.0f),
        BokehRadius(10.0f)
    {
    }

    void InitializeCaustic(HWND hwnd);
    void Setup3DScene(IRenderWindow* pRenderWindow);
};
CApp app;

void CApp::Setup3DScene(IRenderWindow *pRenderWindow)
{
    auto spSceneFactory = Caustic::CreateSceneFactory();
    auto spSceneGraph = spRenderWindow->GetSceneGraph();
    auto spMesh = Caustic::MeshImport::LoadObj(L"j:\\Models\\newell_teaset\\teacup.obj");
    auto spMeshElem = spSceneFactory->CreateMeshElem();
    spMeshElem->SetMesh(spMesh);

    CRefObj<IShader> spShader = spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(L"Textured");
    CRefObj<ISceneMaterialElem> spMaterialElem = spSceneFactory->CreateMaterialElem();
    CRefObj<IMaterialAttrib> spMaterial = spCausticFactory->CreateMaterialAttrib();
    FRGBColor ambient(0.2f, 0.2f, 0.2f);
    FRGBColor diffuse(0.4f, 0.4f, 0.4f);
    spMaterial->SetColor(L"ambientColor", ambient);
    spMaterial->SetColor(L"diffuseColor", diffuse);
    spMaterialElem->SetMaterial(spMaterial);
    spMaterialElem->SetShader(spShader);

    app.m_spLightCollectionElem = spSceneFactory->CreateLightCollectionElem();

    Vector3 lightPos(1000.0f, 1000.0f, 0.0f);
    FRGBColor lightColor(1.0f, 1.0f, 1.0f);
    CRefObj<ILight> spLight(spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));
    app.m_spLightCollectionElem->AddLight(spLight);

    Vector3 lightDir(-1.0f, -1.0f, -1.0f);
    spLight = spCausticFactory->CreateDirectionalLight(lightPos, lightDir, lightColor, 1.0f);
    app.m_spLightCollectionElem->AddLight(spLight);
    spMaterialElem->AddChild(spMeshElem);
    app.m_spLightCollectionElem->AddChild(spMaterialElem);
    spSceneGraph->AddChild(app.m_spLightCollectionElem);

    auto spCamera = spCausticFactory->CreateCamera(true);
    Vector3 eye(app.cameraExt[3][0], app.cameraExt[3][1], app.cameraExt[3][2]);
    Vector3 look(
        app.cameraExt[0][0] + app.cameraExt[3][0],
        app.cameraExt[1][1] + app.cameraExt[3][1],
        app.cameraExt[2][2] + app.cameraExt[3][2]);
    Vector3 up(0.0f, 1.0f, 0.0f);
    spCamera->SetPosition(eye, look, up);
}

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
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            ImGui::Checkbox("UseDynamicBackground", &app.useBackground);
            ImGui::Checkbox("RenderCircleConfusion", &app.renderCOC);
            ImGui::Checkbox("ColorInFocus", &app.showInFocus);
            ImGui::Checkbox("RenderModel", &app.renderModel);
            ImGui::Checkbox("smooth", &app.smooth);
            ImGui::Checkbox("RenderSkeleton", &app.renderSkeleton);
            ImGui::Checkbox("UseFocusTracking", &app.focusTracking);
            ImGui::SliderFloat("FocusDistance", &app.focusDistance, 0.0f, 1.0f, "%f", 1.0f);
            ImGui::SliderFloat("FocusWidth", &app.focusWidth, 0.0f, 3.0f, "%f", 1.0f);
            ImGui::SliderFloat("FocusMaxWidth", &app.focusMaxWidth, 0.0f, 3.0f, "%f", 1.0f);
            ImGui::SliderFloat("BokehRadius", &app.BokehRadius, 0.0f, 30.0f, "%f", 1.0f);

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
                    app.spBackgroundTexture->Update(app.spRenderer);

                    std::any d3x(app.BokehRadius);
                    app.spBokehShader->SetPSParam(L"BokehRadius", d3x);
                    std::any bt(app.spBackgroundTexture);
                    app.spBokehShader->SetPSParam(L"backgroundTexture", bt);

                    std::any d1(app.focusDistance);
                    std::any d2(app.focusMaxWidth);
                    std::any d3(app.focusWidth);
                    //app.spDepthSelShader->SetPSParam(L"focusDistance", d1);
                    //app.spDepthSelShader->SetPSParam(L"focusMaxWidth", d2);
                    app.spBokehShader->SetPSParam(L"focusDistance", d1);
                    app.spBokehShader->SetPSParam(L"focusMaxWidth", d2);
                    app.spBokehShader->SetPSParam(L"focusWidth", d3);
                    std::any f((app.colored) ? 1.0f : 0.0f);
                    app.spBokehShader->SetPSParam(L"colored", f);
                    std::any f2((app.useBackground) ? 1.0f : 0.0f);
                    app.spBokehShader->SetPSParam(L"useBackground", f2);
                    std::any f3((app.renderCOC) ? 1.0f : 0.0f);
                    app.spBokehShader->SetPSParam(L"renderCOC", f3);
                    std::any f4((app.showInFocus) ? 1.0f : 0.0f);
                    app.spBokehShader->SetPSParam(L"showInFocus", f4);
                    app.spSourceColorNode->SetSource(app.spGPUPipeline, app.spLastColorImage);
                    app.spSourceDepthNode->SetSource(app.spGPUPipeline, app.spLastDepthImage);
                    ((app.smooth) ? app.spNodeSmooth : app.spNode)->Process(app.spGPUPipeline);
                    auto spTexture = ((app.smooth) ? app.spNodeSmooth : app.spNode)->GetOutputTexture(app.spGPUPipeline);
                    if (spTexture)
                        app.spRenderer->DrawScreenQuad(0.0f, 0.0f, 1.0f, 1.0f, spTexture, nullptr);

                    app.spRenderer->ClearDepth();
                }
                if (app.renderModel)
                    app.m_spLightCollectionElem->SetFlags(app.m_spLightCollectionElem->GetFlags() & ~ESceneElemFlags::Hidden);
                else
                    app.m_spLightCollectionElem->SetFlags(app.m_spLightCollectionElem->GetFlags() | ESceneElemFlags::Hidden);
                if (app.spCamera->BodyTrackingOn() && app.spCamera->NumberBodiesDetected() >= 1)
                {
                    if (app.focusTracking)
                    {
                        auto mat0 = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::Head);
                        app.focusDistance = mat0.v[3][2] / 8.0f;
                        auto mat1 = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::Neck);
                        app.focusWidth = fabs(mat1.v[3][2] - mat0.v[3][2]) / 8.0f;
                        app.focusWidth = 0.03f;
                    }

                    if (app.renderSkeleton)
                    {
                        for (int i = 0; i < (int)Caustic::AzureKinect::Joint::Count; i++)
                        {
                            auto parent = app.spCamera->GetParentJoint((Caustic::AzureKinect::Joint)i);
                            auto mat0 = app.spCamera->GetJointMatrix(0, parent);
                            auto mat1 = app.spCamera->GetJointMatrix(0, (Caustic::AzureKinect::Joint)i);
                            Vector3 p1(mat0.v[3][0], mat0.v[3][1], mat0.v[3][2]);
                            Vector3 p2(mat1.v[3][0], mat1.v[3][1], mat1.v[3][2]);
                            Vector4 clr(1.0f, 0.0f, 0.0f, 1.0f);
                            pRenderer->DrawLine(p1, p2, clr);
                        }
                    }
                    auto mat = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::HandLeft);
                    mat.v[3][0] *= 10.0f;
                    mat.v[3][1] *= 10.0f;
                    mat.v[3][2] *= 10.0f;
                    app.m_spLightCollectionElem->SetTransform(mat);
                }
            }
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        });
    app.spRenderer = app.spRenderWindow->GetRenderer();

    app.spRenderer->Freeze();

    app.spBackgroundTexture = Caustic::LoadVideoTexture(L"j:\\github\\caustic\\background.mp4", app.spRenderer);

    app.spCamera = Caustic::AzureKinect::CreateAzureKinect(1, AzureKinect::Color1080p, AzureKinect::Depth1024x1024, AzureKinect::FPS30, true);
    app.cameraExt = app.spCamera->ColorExtrinsics();
    app.cameraInt = app.spCamera->ColorIntrinsics();

    Setup3DScene(app.spRenderWindow);

    app.spGPUPipeline = Caustic::CreateGPUPipeline(app.spRenderer);

    //**********************************************************************
    // Overall our pipeline looks like:
    //    +-------------+      +-------------+
    //    | Depth Image | ---> | Depth2Color | ---+
    //    +-------------+      +-------------+    |    +----------+
    //    +-------------+      +-------+          +--> |          |
    //    | Color Image | -+-> | Bokeh |-------------> | DepthSel |
    //    +-------------+  |   +-------+          +--> |          |
    //                     |                      |    +----------+
    //                     +----------------------+
    // We have a color+depth image in. We then convert the depth to match
    // the color image dimensions. We then select between a blurred version
    // of the color image and pristine color image based on the depth.
    //**********************************************************************

    //**********************************************************************
    // Create source nodes for color/depth images from camera
    //**********************************************************************
    uint32 depthW = app.spCamera->GetDepthWidth();
    uint32 depthH = app.spCamera->GetDepthHeight();
    uint32 colorW = app.spCamera->GetColorWidth();
    uint32 colorH = app.spCamera->GetColorHeight();
    app.spSourceColorNode = app.spGPUPipeline->CreateSourceNode(nullptr, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spSourceDepthNode = app.spGPUPipeline->CreateSourceNode(nullptr, depthW, depthH, DXGI_FORMAT_R16_UINT);

    //**********************************************************************
    // Create downsampled depth using a median filter
    //**********************************************************************
    auto spsh = app.spRenderer->GetShaderMgr()->FindShader(L"MedianDepth");
    auto spMedian = app.spGPUPipeline->CreateNode(spsh, depthW / 2, depthH / 2, DXGI_FORMAT_R16_UINT);
    spMedian->SetInput(L"depthTex", nullptr, app.spSourceDepthNode);
    auto spMedian2 = app.spGPUPipeline->CreateNode(spsh, depthW / 4, depthH / 4, DXGI_FORMAT_R16_UINT);
    spMedian2->SetInput(L"depthTex", nullptr, spMedian);
    auto spMedian3 = app.spGPUPipeline->CreateNode(spsh, depthW / 8, depthH / 8, DXGI_FORMAT_R16_UINT);
    spMedian3->SetInput(L"depthTex", nullptr, spMedian2);
    auto spMedian4 = app.spGPUPipeline->CreateNode(spsh, depthW / 16, depthH / 16, DXGI_FORMAT_R16_UINT);
    spMedian4->SetInput(L"depthTex", nullptr, spMedian3);
    auto spMedian5 = app.spGPUPipeline->CreateNode(spsh, depthW / 32, depthH / 32, DXGI_FORMAT_R16_UINT);
    spMedian5->SetInput(L"depthTex", nullptr, spMedian4);
    auto spMedian6 = app.spGPUPipeline->CreateNode(spsh, depthW / 64, depthH / 64, DXGI_FORMAT_R16_UINT);
    spMedian6->SetInput(L"depthTex", nullptr, spMedian5);
    auto spMedian7 = app.spGPUPipeline->CreateNode(spsh, depthW / 128, depthH / 128, DXGI_FORMAT_R16_UINT);
    spMedian7->SetInput(L"depthTex", nullptr, spMedian6);
    auto spMedian8 = app.spGPUPipeline->CreateNode(spsh, depthW / 256, depthH / 256, DXGI_FORMAT_R16_UINT);
    spMedian8->SetInput(L"depthTex", nullptr, spMedian7);

    //**********************************************************************
    // Fill holes in the depth map
    //**********************************************************************
    auto spfh = app.spRenderer->GetShaderMgr()->FindShader(L"FillHoles");
    auto spFillHoles = app.spGPUPipeline->CreateNode(spfh, depthW, depthH, DXGI_FORMAT_R16_UINT);
    spFillHoles->SetInput(L"depthTex0", nullptr, app.spSourceDepthNode);
    spFillHoles->SetInput(L"depthTex1", nullptr, spMedian);
    spFillHoles->SetInput(L"depthTex2", nullptr, spMedian2);
    spFillHoles->SetInput(L"depthTex3", nullptr, spMedian3);
    spFillHoles->SetInput(L"depthTex4", nullptr, spMedian4);
    spFillHoles->SetInput(L"depthTex5", nullptr, spMedian5);
    spFillHoles->SetInput(L"depthTex6", nullptr, spMedian6);
    spFillHoles->SetInput(L"depthTex7", nullptr, spMedian7);
    spFillHoles->SetInput(L"depthTex8", nullptr, spMedian8);

    //**********************************************************************
    // Normalize the depth map to be in the range 0..1
    //**********************************************************************
    auto spnd = app.spRenderer->GetShaderMgr()->FindShader(L"NormDepth");
    auto spNormSmooth = app.spGPUPipeline->CreateNode(spnd, depthW, depthH, DXGI_FORMAT_R32_FLOAT);
    spNormSmooth->SetInput(L"depthTex", nullptr, spFillHoles);

    auto spNorm = app.spGPUPipeline->CreateNode(spnd, depthW, depthH, DXGI_FORMAT_R32_FLOAT);
    spNorm->SetInput(L"depthTex", nullptr, app.spSourceDepthNode);
    
    //**********************************************************************
    // Create texture that maps depth pixels into rays
    //**********************************************************************
    app.spRayMap = app.spCamera->BuildRayMap(depthW, depthH);
    app.spRayTex = Caustic::CreateTexture(app.spRenderer, depthW, depthH, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
    app.spRayTex->CopyFromImage(app.spRenderer, app.spRayMap);

    //**********************************************************************
    // Create node that generates depth texture matching color texture
    //**********************************************************************
    auto extrinsics = app.spCamera->ColorExtrinsics();
    auto intrinsics = app.spCamera->GetAzureColorIntrinsics();
    auto spDepthMeshNode = app.spGPUPipeline->CreatePredefinedNode(c_CustomNode_DepthMeshNode, (unsigned int)depthW, (unsigned int)depthH, (unsigned int)colorW, (unsigned int)colorH,
        app.spRayTex, extrinsics, intrinsics, 10.0, 8000.0,
        DXGI_FORMAT_R32_FLOAT);
    spDepthMeshNode->SetInput(L"depthTex", nullptr, app.spSourceDepthNode);

    auto spDepthMeshNodeSmooth = app.spGPUPipeline->CreatePredefinedNode(c_CustomNode_DepthMeshNode, depthW, depthH, colorW, colorH,
        app.spRayTex, extrinsics, intrinsics, 10.0, 8000.0,
        DXGI_FORMAT_R32_FLOAT);
    spDepthMeshNodeSmooth->SetInput(L"depthTex", nullptr, spFillHoles);

    //**********************************************************************
    // Create depth of field node
    //**********************************************************************
    app.spBokehShader = app.spRenderer->GetShaderMgr()->FindShader(L"Bokeh");
    auto bokeh = app.spGPUPipeline->CreateNode(app.spBokehShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    bokeh->SetInput(L"colorTex", L"colorSampler", app.spSourceColorNode);
    bokeh->SetInput(L"depthTex", nullptr, spDepthMeshNode);

    auto bokehSmooth = app.spGPUPipeline->CreateNode(app.spBokehShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    bokehSmooth->SetInput(L"colorTex", L"colorSampler", app.spSourceColorNode);
    bokehSmooth->SetInput(L"depthTex", nullptr, spDepthMeshNodeSmooth);

    app.spShader = app.spRenderer->GetShaderMgr()->FindShader(L"RawCopy");
    app.spNode = app.spGPUPipeline->CreateNode(app.spShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spNode->SetInput(L"sourceTexture1", L"sourceSampler1", spDepthMeshNode); //bokeh); //spDepthMeshNode);

    app.spNodeSmooth = app.spGPUPipeline->CreateNode(app.spShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spNodeSmooth->SetInput(L"sourceTexture1", L"sourceSampler1", spDepthMeshNodeSmooth);// bokehSmooth); //spDepthMeshNodeSmooth);
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
    auto ctx = ImGui::CreateContext();
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(app.spRenderer->GetDevice(), app.spRenderer->GetContext());
    app.guiInited = true;

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
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool captureKeyboard = false;
    static bool captureMouse = false;
    if (app.guiInited)
    {
        ImGui::CaptureKeyboardFromApp(&captureKeyboard);
        ImGui::CaptureMouseFromApp(&captureMouse);
        ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
    }
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
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
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        PostQuitMessage(0);
        break;
//    case WM_LBUTTONDOWN:
//        if (!captureMouse)
//            app.spRenderWindow->MouseDown((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
//        break;
//    case WM_LBUTTONUP:
//        if (!captureMouse)
//            app.spRenderWindow->MouseUp((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
//        break;
//    case WM_MOUSEMOVE:
//        if (!captureMouse)
//            app.spRenderWindow->MouseMove((int)LOWORD(lParam), (int)HIWORD(lParam), (uint32)wParam);
//        break;
//    case WM_MOUSEWHEEL:
//        if (!captureMouse)
//            app.spRenderWindow->MouseWheel((int)wParam);
//        break;
    case WM_KEYDOWN:
        if (!captureKeyboard)
            app.spRenderWindow->MapKey((uint32)wParam, (uint32)lParam);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
