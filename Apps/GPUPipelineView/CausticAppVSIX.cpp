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
#include <d3d11.h>
#include <d3d11_4.h>
#include <d3dcommon.h>

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
    CRefObj<IGPUPipelineSinkNode> spCameraReadyNode;
    CRefObj<ITexture> spOutputTexture;
    CRefObj<IShader> spShader;
    CRefObj<IShader> spBokehShader;
    CRefObj<IShader> spDepthSelShader;
    CRefObj<IShader> spHoleFillShaderV;
    CRefObj<IShader> spHoleFillShaderH;
    CRefObj<IImage> spLastColorImage;
    CRefObj<IImage> spLastDepthImage;
    CRefObj<IImage> spRayMap;
    CRefObj<ITexture> spRayTex;
    CComPtr<ID3D11Buffer> spLineVB;                   // Vertex buffer used to draw lines
    CRefObj<IShader> spLineShader;
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
    bool showDepth;
    bool showHoles;
    bool guiInited;
    float focusDistance;
    float focusMaxWidth;
    float focusWidth;
    float BokehRadius;
    bool smooth;
    bool checkDepth;
    int holeSize;

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
        showDepth(false),
        showHoles(false),
        checkDepth(false),
        focusDistance(0.0f),
        focusMaxWidth(0.057f),
        focusWidth(0.0f),
        BokehRadius(10.0f),
        holeSize(16)
    {
    }

    void InitializeCaustic(HWND hwnd);
    void Setup3DScene(IRenderWindow* pRenderWindow);
    void DrawLine(Vector3 p1, Vector3 p2);
    void SetupLineDrawing(uint32 depthW, uint32 depthH, uint32 colorW, uint32 colorH);
};
CApp app;

struct CLineVertex
{
    float x, y, z;
};

void CApp::Setup3DScene(IRenderWindow *pRenderWindow)
{
    CD3D11_BUFFER_DESC bufdesc(sizeof(CLineVertex) * 2, D3D11_BIND_VERTEX_BUFFER);
    CLineVertex* pVertexBuffer = new CLineVertex[2];
    pVertexBuffer[0].x = 0.0f;
    pVertexBuffer[0].y = 0.0f;
    pVertexBuffer[0].z = 0.0f;
    pVertexBuffer[1].x = 1.0f;
    pVertexBuffer[1].y = 1.0f;
    pVertexBuffer[1].z = 1.0f;
    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = pVertexBuffer;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;
    app.spRenderer->GetDevice()->CreateBuffer(&bufdesc, &data, &spLineVB);
    app.spLineShader = app.spRenderer->GetShaderMgr()->FindShader(L"AzureLine");

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

void CApp::SetupLineDrawing(uint32 depthW, uint32 depthH, uint32 colorW, uint32 colorH)
{
    app.spRenderer->Freeze();
    auto intrinsics = app.spCamera->GetAzureColorIntrinsics();
    app.spLineShader->SetVSParamFloat(L"codx", intrinsics.codx);
    app.spLineShader->SetVSParamFloat(L"cody", intrinsics.cody);
    app.spLineShader->SetVSParamFloat(L"cx", intrinsics.cx);
    app.spLineShader->SetVSParamFloat(L"cy", intrinsics.cy);
    app.spLineShader->SetVSParamFloat(L"fx", intrinsics.fx);
    app.spLineShader->SetVSParamFloat(L"fy", intrinsics.fy);
    app.spLineShader->SetVSParamFloat(L"k1", intrinsics.k1);
    app.spLineShader->SetVSParamFloat(L"k2", intrinsics.k2);
    app.spLineShader->SetVSParamFloat(L"k3", intrinsics.k3);
    app.spLineShader->SetVSParamFloat(L"k4", intrinsics.k4);
    app.spLineShader->SetVSParamFloat(L"k5", intrinsics.k5);
    app.spLineShader->SetVSParamFloat(L"k6", intrinsics.k6);
    app.spLineShader->SetVSParamFloat(L"metricRadius", intrinsics.metricRadius);
    app.spLineShader->SetVSParamFloat(L"p1", intrinsics.p1);
    app.spLineShader->SetVSParamFloat(L"p2", intrinsics.p2);
    app.spLineShader->SetVSParamInt(L"type", intrinsics.type);
    Matrix m(app.cameraExt);
    std::any mat = std::any(m);
    app.spLineShader->SetVSParam(L"colorExt", mat);
    app.spLineShader->SetVSParamInt(L"colorWidth", colorW);
    app.spLineShader->SetVSParamInt(L"colorHeight", colorH);
    app.spLineShader->SetVSParamFloat(L"minDepth", 10.0f);
    app.spLineShader->SetPSParamFloat(L"minDepth", 10.0f);
    app.spLineShader->SetVSParamFloat(L"maxDepth", 8000.0f);
    app.spLineShader->SetPSParamFloat(L"maxDepth", 8000.0f);
    app.spRenderer->Unfreeze();
}

void CApp::DrawLine(Vector3 p1, Vector3 p2)
{
    CComPtr<ID3D11DeviceContext2> spCtx2;
    app.spRenderer->GetContext()->QueryInterface<ID3D11DeviceContext2>(&spCtx2);
    spCtx2->BeginEventInt(L"AzureDrawLine", 0);

    UINT offset = 0;
    UINT vertexSize = sizeof(CLineVertex);
    ID3D11DeviceContext* pContext = app.spRenderer->GetContext();
    pContext->IASetVertexBuffers(0, 1, &spLineVB.p, &vertexSize, &offset);
    Matrix m;
    m.x[0] = p2.x - p1.x;    m.x[4] = 0.0f;           m.x[8] = 0.0f;           m.x[12] = 0.0f;
    m.x[1] = 0.0f;           m.x[5] = p2.y - p1.y;    m.x[9] = 0.0f;           m.x[13] = 0.0f;
    m.x[2] = 0.0f;           m.x[6] = 0.0f;           m.x[10] = p2.z - p1.z;   m.x[14] = 0.0f;
    m.x[3] = p1.x;           m.x[7] = p1.y;           m.x[11] = p1.z;          m.x[15] = 1.0f;
    std::any am(m);
    app.spLineShader->SetVSParam(L"endpoints", am);
    Float4 color(1.0f, 0.0f, 0.0f, 1.0f);
    std::any clr(color);
    app.spLineShader->SetPSParam(L"color", clr);
    std::vector<CRefObj<ILight>> lights;
    app.spLineShader->BeginRender(app.spRenderer, nullptr, nullptr, lights, nullptr);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    pContext->Draw(2, 0);
    app.spLineShader->EndRender(app.spRenderer);

    spCtx2->EndEvent();
}

HANDLE hCanRead = nullptr;
HANDLE hCanWrite = nullptr;
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
             if (ImGui::IsItemHovered())
                 ImGui::SetTooltip("Display pixels that are full in focus in yellow");
             ImGui::Checkbox("ShowHoles", &app.showHoles);
             ImGui::Checkbox("ShowDepth", &app.showDepth);
             ImGui::Checkbox("CheckDepth", &app.checkDepth);
             if (ImGui::IsItemHovered())
                 ImGui::SetTooltip("Forces depth discontinuity to effect blur sampling");
             ImGui::Checkbox("RenderModel", &app.renderModel);
             ImGui::Checkbox("FillHoles", &app.smooth);
             if (ImGui::IsItemHovered())
                 ImGui::SetTooltip("Fill holes in depth map");
             ImGui::Checkbox("RenderSkeleton", &app.renderSkeleton);
             ImGui::Checkbox("UseFocusTracking", &app.focusTracking);
             if (ImGui::IsItemHovered())
                 ImGui::SetTooltip("Use head tracking to set focus distance");
             ImGui::SliderFloat("FocusDistance", &app.focusDistance, 0.0f, 1.0f, "%f", 1.0f);
             ImGui::SliderFloat("FocusWidth", &app.focusWidth, 0.0f, 3.0f, "%f", 1.0f);
             ImGui::SliderFloat("FocusMaxWidth", &app.focusMaxWidth, 0.0f, 3.0f, "%f", 1.0f);
             ImGui::SliderFloat("BokehRadius", &app.BokehRadius, 0.0f, 30.0f, "%f", 1.0f);
             ImGui::DragInt("HoleSize", &app.holeSize, 1.0f, 0, 128);

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

                     std::any holeSize((float)app.holeSize);
                     app.spHoleFillShaderH->SetPSParam(L"holeSize", holeSize);
                     app.spHoleFillShaderV->SetPSParam(L"holeSize", holeSize);

                     app.spBokehShader->SetPSParamFloat(L"BokehRadius", app.BokehRadius);
                     std::any bt(app.spBackgroundTexture);
                     app.spBokehShader->SetPSParam(L"backgroundTexture", bt);
                     app.spBokehShader->SetPSParamFloat(L"focusDistance", app.focusDistance);
                     app.spBokehShader->SetPSParamFloat(L"focusMaxWidth", app.focusMaxWidth);
                     app.spBokehShader->SetPSParamFloat(L"focusWidth", app.focusWidth);
                     app.spBokehShader->SetPSParamFloat(L"colored", (app.colored) ? 1.0f : 0.0f);
                     app.spBokehShader->SetPSParamFloat(L"useBackground", (app.useBackground) ? 1.0f : 0.0f);
                     app.spBokehShader->SetPSParamFloat(L"renderCOC", (app.renderCOC) ? 1.0f : 0.0f);
                     app.spBokehShader->SetPSParamFloat(L"showInFocus", (app.showInFocus) ? 1.0f : 0.0f);
                     app.spBokehShader->SetPSParamFloat(L"showDepth", (app.showDepth) ? 1.0f : 0.0f);
                     app.spBokehShader->SetPSParamFloat(L"showHoles", (app.showHoles) ? 1.0f : 0.0f);
                     app.spBokehShader->SetPSParamFloat(L"checkDepth", (app.checkDepth) ? 1.0f : 0.0f);
                     app.spSourceColorNode->SetSource(app.spGPUPipeline, app.spLastColorImage);
                     app.spSourceDepthNode->SetSource(app.spGPUPipeline, app.spLastDepthImage);
                     //app.spCameraReadyNode->Process(app.spGPUPipeline);
                     //((app.smooth) ? app.spNodeSmooth : app.spNode)->Process(app.spGPUPipeline);
                     auto spTexture = ((app.smooth) ? app.spNodeSmooth : app.spNode)->GetOutputTexture(app.spGPUPipeline);
                     if (spTexture)
                         app.spRenderer->DrawScreenQuad(0.0f, 0.0f, 1.0f, 1.0f, spTexture, nullptr);

                     auto finalTex = app.spCameraReadyNode->GetOutputTexture(app.spGPUPipeline);
                     auto spImage = finalTex->CopyToImage(app.spRenderer);
                     DWORD res = WaitForSingleObject(hCanWrite, 10);
                     if (res == WAIT_OBJECT_0)
                     {
                         ResetEvent(hCanWrite);
                         auto hMapFile = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, L"Global\\VirtualCameraImage");
                         if (hMapFile)
                         {
                             auto pBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, 1920 * 1080 * 4);
                             if (pBuf)
                             {
                                 CopyMemory((PVOID)pBuf, spImage->GetData(), 1920 * 1080 * 4);
                                 UnmapViewOfFile(pBuf);
                             }
                             CloseHandle(hMapFile);
                             SetEvent(hCanRead);
                         }
                     }

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
                             Vector3 p1(mat0.v[3][0] * 1000.0f, mat0.v[3][1] * 1000.0f, mat0.v[3][2] * 1000.0f);
                             Vector3 p2(mat1.v[3][0] * 1000.0f, mat1.v[3][1] * 1000.0f, mat1.v[3][2] * 1000.0f);
                             Vector4 clr(1.0f, 0.0f, 0.0f, 1.0f);
                             app.DrawLine(p1, p2);
                         }
                     }
                     auto mat = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::HandLeft);
                     mat.v[3][0] *= 1000.0f;
                     mat.v[3][1] *= 1000.0f;
                     mat.v[3][2] *= 1000.0f;
                     app.m_spLightCollectionElem->SetTransform(mat);
                 }
             }
             ImGui::Render();
             ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
         });
     app.spRenderer = app.spRenderWindow->GetRenderer();

    app.spRenderer->Freeze();

    app.spBackgroundTexture = Caustic::LoadVideoTexture(L"j:\\github\\caustic\\background.mp4", app.spRenderer);

    hCanRead = OpenEvent(SYNCHRONIZE |EVENT_MODIFY_STATE, TRUE, L"Global\\VirtualCameraMutexRead");
    hCanWrite = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, TRUE, L"Global\\VirtualCameraMutexWrite");
    SetEvent(hCanWrite);

    app.spCamera = Caustic::AzureKinect::CreateAzureKinect(1, AzureKinect::Color1080p, AzureKinect::Depth1024x1024, AzureKinect::FPS30, true);
    app.cameraExt = app.spCamera->ColorExtrinsics();
    app.cameraInt = app.spCamera->ColorIntrinsics();
    uint32 depthW = app.spCamera->GetDepthWidth();
    uint32 depthH = app.spCamera->GetDepthHeight();
    uint32 colorW = app.spCamera->GetColorWidth();
    uint32 colorH = app.spCamera->GetColorHeight();

    Setup3DScene(app.spRenderWindow);
    SetupLineDrawing(depthW, depthH, colorW, colorH);

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
    app.spSourceColorNode = app.spGPUPipeline->CreateSourceNode(nullptr, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spSourceDepthNode = app.spGPUPipeline->CreateSourceNode(nullptr, depthW, depthH, DXGI_FORMAT_R16_UINT);

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
        DXGI_FORMAT_R16_UINT);
    spDepthMeshNode->SetInput(L"depthTex", nullptr, app.spSourceDepthNode);

    //**********************************************************************
    // Create downsampled depth using a median filter
    //**********************************************************************
#ifdef USE_MEDIAN_HOLE_FILLING
    auto spsh = app.spRenderer->GetShaderMgr()->FindShader(L"MedianDepth");
    auto spMedian = app.spGPUPipeline->CreateNode(spsh, colorW, colorH, DXGI_FORMAT_R16_UINT);
    spMedian->SetInput(L"depthTex", nullptr, spDepthMeshNode);
    auto spMedian = app.spGPUPipeline->CreateNode(spsh, colorW / 2, colorH / 2, DXGI_FORMAT_R16_UINT);
    spMedian->SetInput(L"depthTex", nullptr, spDepthMeshNode);
    auto spMedian2 = app.spGPUPipeline->CreateNode(spsh, colorW / 4, colorH / 4, DXGI_FORMAT_R16_UINT);
    spMedian2->SetInput(L"depthTex", nullptr, spMedian);
    auto spMedian3 = app.spGPUPipeline->CreateNode(spsh, colorW / 8, colorH / 8, DXGI_FORMAT_R16_UINT);
    spMedian3->SetInput(L"depthTex", nullptr, spMedian2);
    auto spMedian4 = app.spGPUPipeline->CreateNode(spsh, colorW / 16, colorH / 16, DXGI_FORMAT_R16_UINT);
    spMedian4->SetInput(L"depthTex", nullptr, spMedian3);
    auto spMedian5 = app.spGPUPipeline->CreateNode(spsh, colorW / 32, colorH / 32, DXGI_FORMAT_R16_UINT);
    spMedian5->SetInput(L"depthTex", nullptr, spMedian4);
    auto spMedian6 = app.spGPUPipeline->CreateNode(spsh, colorW / 64, colorH / 64, DXGI_FORMAT_R16_UINT);
    spMedian6->SetInput(L"depthTex", nullptr, spMedian5);
    auto spMedian7 = app.spGPUPipeline->CreateNode(spsh, colorW / 128, colorH / 128, DXGI_FORMAT_R16_UINT);
    spMedian7->SetInput(L"depthTex", nullptr, spMedian6);
    auto spMedian8 = app.spGPUPipeline->CreateNode(spsh, colorW / 256, colorH / 256, DXGI_FORMAT_R16_UINT);
    spMedian8->SetInput(L"depthTex", nullptr, spMedian7);

    //**********************************************************************
    // Fill holes in the depth map
    //**********************************************************************
    auto spfh = app.spRenderer->GetShaderMgr()->FindShader(L"FillHoles");
    auto spFillHoles = app.spGPUPipeline->CreateNode(spfh, colorW, colorH, DXGI_FORMAT_R16_UINT);
    spFillHoles->SetInput(L"depthTex0", nullptr, spDepthMeshNode);
    spFillHoles->SetInput(L"depthTex1", nullptr, spMedian);
    spFillHoles->SetInput(L"depthTex2", nullptr, spMedian2);
    spFillHoles->SetInput(L"depthTex3", nullptr, spMedian3);
    spFillHoles->SetInput(L"depthTex4", nullptr, spMedian4);
    spFillHoles->SetInput(L"depthTex5", nullptr, spMedian5);
    spFillHoles->SetInput(L"depthTex6", nullptr, spMedian6);
    spFillHoles->SetInput(L"depthTex7", nullptr, spMedian7);
    spFillHoles->SetInput(L"depthTex8", nullptr, spMedian8);
#else
#ifdef SINGLE_PASS_HOLEFILLING
    auto spsh = app.spRenderer->GetShaderMgr()->FindShader(L"FillHolesSP");
    auto spFillHoles = app.spGPUPipeline->CreateNode(spsh, colorW, colorH, DXGI_FORMAT_R16_UINT);
    spFillHoles->SetInput(L"depthTex", nullptr, spDepthMeshNode);
#else
    app.spHoleFillShaderH = app.spRenderer->GetShaderMgr()->FindShader(L"FillHolesHP");
    app.spHoleFillShaderV = app.spRenderer->GetShaderMgr()->FindShader(L"FillHolesVP");
    auto spFillHolesH = app.spGPUPipeline->CreateNode(app.spHoleFillShaderH, colorW, colorH, DXGI_FORMAT_R16_UINT);
    spFillHolesH->SetInput(L"depthTex", nullptr, spDepthMeshNode);
    auto spFillHolesV = app.spGPUPipeline->CreateNode(app.spHoleFillShaderV, colorW, colorH, DXGI_FORMAT_R16_UINT);
    spFillHolesV->SetInput(L"depthTex", nullptr, spFillHolesH);
#endif
#endif

    //**********************************************************************
    // Normalize the depth map to be in the range 0..1
    //**********************************************************************
    auto spnd = app.spRenderer->GetShaderMgr()->FindShader(L"NormDepth");
    auto spNormSmooth = app.spGPUPipeline->CreateNode(spnd, colorW, colorH, DXGI_FORMAT_R32_FLOAT);
    spNormSmooth->SetInput(L"depthTex", nullptr, spFillHolesV);

    auto spNorm = app.spGPUPipeline->CreateNode(spnd, colorW, colorH, DXGI_FORMAT_R32_FLOAT);
    spNorm->SetInput(L"depthTex", nullptr, spDepthMeshNode);
    
    //**********************************************************************
    // Create depth of field node
    //**********************************************************************
    app.spBokehShader = app.spRenderer->GetShaderMgr()->FindShader(L"Bokeh");
    auto bokeh = app.spGPUPipeline->CreateNode(app.spBokehShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    bokeh->SetInput(L"colorTex", L"colorSampler", app.spSourceColorNode);
    bokeh->SetInput(L"depthTex", nullptr, spNorm);

    auto bokehSmooth = app.spGPUPipeline->CreateNode(app.spBokehShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    bokehSmooth->SetInput(L"colorTex", L"colorSampler", app.spSourceColorNode);
    bokehSmooth->SetInput(L"depthTex", nullptr, spNormSmooth);

    app.spShader = app.spRenderer->GetShaderMgr()->FindShader(L"RawCopy");
    app.spNode = app.spGPUPipeline->CreateNode(app.spShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spNode->SetInput(L"sourceTexture1", L"sourceSampler1", bokeh);

    app.spNodeSmooth = app.spGPUPipeline->CreateNode(app.spShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spNodeSmooth->SetInput(L"sourceTexture1", L"sourceSampler1", bokehSmooth);

    app.spCameraReadyNode = app.spGPUPipeline->CreateSinkNode(app.spShader, 1920, 1080, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
    app.spCameraReadyNode->SetInput(L"sourceTexture1", L"sourceSampler1", app.spNode);
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
