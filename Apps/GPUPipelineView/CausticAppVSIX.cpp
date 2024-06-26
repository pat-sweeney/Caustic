// CausticAppVSIX.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include <string>
#include <windows.h>
#include <winuser.h>
#include <d3d11.h>
#include <d3d11_4.h>
#include <d3dcommon.h>
#include <atlbase.h>
#include <any>
#include "CausticAppVSIX.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Base.Math.BBox;
import Imaging.Image.GPUPipeline;
import Imaging.Image.IGPUPipeline;
import Rendering.SceneImport.Collada;
import Geometry.MeshImport;
import Geometry.Mesh.MeshFuncs;
import Cameras.AzureKinect.IAzureKinect;
import Rendering.Caustic.Shader;
import Rendering.Caustic.VideoTexture;
import Rendering.Caustic.DesktopTexture;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IRenderer;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.SceneGraph.ISceneGraph;
import Rendering.SceneGraph.ISceneLightCollectionElem;
import Rendering.SceneGraph.ISceneMaterialElem;
import Rendering.SceneGraph.ISceneMeshElem;
import Rendering.RenderWindow.IRenderWindow;
import Imaging.Image.IImage;
import Geometry.Mesh.IMesh;

#define MAX_LOADSTRING 100

using namespace Caustic;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class CApp
{
public:
    CRefObj<ITexture> spBackgroundTexture;              // Background image
    CRefObj<ISceneLightCollectionElem> m_spLightCollectionElem; // List of 3D objects in scene effected by the light
    CRefObj<IRenderWindow> spRenderWindow;              // Window that renderer draws to
    CRefObj<Caustic::ICausticFactory> spCausticFactory; // Factory used to create Caustic objects
    CRefObj<IRenderer> spRenderer;                      // Instance of our renderer
    CRefObj<IAzureKinect> spCamera;                     // Azure Kinect camera
    CRefObj<IGPUPipeline> spGPUPipeline;                // GPU Pipeline used to process images from Azure Kinect camera
    CRefObj<IGPUPipelineSourceNode> spSourceColorNode;  // Source node in GPU pipeline that feeds color image from camera to pipeline
    CRefObj<IGPUPipelineSourceNode> spSourceDepthNode;  // Source node in GPU pipeline that feeds depth image from camera to pipeline
    CRefObj<IGPUPipelineNode> spNormNode;               // Contains normal map derived from depth map
    CRefObj<IGPUPipelineNode> spNormDepth;              // Normalized depth node
    CRefObj<IGPUPipelineNode> spDepthOfField;           // Node returning the color source blurred using depth of field
    CRefObj<IGPUPipelineNode> spDepthOfFieldFilled;     // Node returning the color source blurred using depth of field (using hole filled depth)
    CRefObj<IGPUPipelineNode> spColor2Depth;
    CRefObj<IShader> spRawCopyShader;                   // Shader used to copy image to resized/format changed image
    CRefObj<IShader> spBokehShader;                     // Shader used for computing depth of field
    CRefObj<IShader> spHoleFillShaderV;                 // Shader used for hole filling (vertical pass)
    CRefObj<IShader> spHoleFillShaderH;                 // Shader used for hole filling (horizontal pass)
    CRefObj<IImage> spLastColorImage;                   // Last color image read from Azure Kinect
    CRefObj<IImage> spLastDepthImage;                   // Last depth image read from Azure Kinect
    CRefObj<IImage> spRayMap;                           // Depth pixel to ray map
    CRefObj<ITexture> spRayTex;                         // Depth pixel to ray texture
    CComPtr<ID3D11Buffer> spLineVB;                     // Vertex buffer used to draw lines
    CRefObj<IShader> spLineShader;                      // Shader used for drawing lines
    CRefObj<ILight> spPointLight;                       // Point light source used to illuminate 3D models
    HANDLE hMapFile;                                    // Shared memory block for sending frames to virtual camera
    LPTSTR pMemBuf;                                     // Pointer to shared memory block
    HWND hwnd;                                          // HWND bound to renderer
    Matrix4x4 cameraExt;                                // Color extrinsics for Azure Kinect
    Matrix3x3 cameraInt;                                // Color intrinsics for Azure Kinect
    CRefObj<IImage> pBackImage;                         // Back buffer (as an IImage)
    CRefObj<ITexture> spDesktopTexture;
    CRefObj<IShader> spZPrePassShader;
    CRefObj<ISceneMaterialElem> spPlaneMaterialElem;
    Vector3 planeScale;
    Vector3 planeXlate;
    Vector3 planeRot;
    CRefObj<ISceneMeshElem> spLightBulbElem;
    CRefObj<ISceneMaterialElem> spLightBulbMaterialElem;
    Vector3 lightPos;
    Vector3 lightClr;
    CRefObj<IShader> spLightShader;
    float depth[4];
    bool displayNorms;
    bool displayColorImage;
    bool displayDepthImage;
    bool displayColor2DepthImage;
    bool colored;
    bool useBackground;
    bool sendToCamera;
    bool sendOrigToCamera;
    bool renderModel;
    bool renderDesktopView;
    bool renderSkeleton;
    bool detectHandRaised;
    bool focusTracking;
    bool renderCOC;
    bool showInFocus;
    bool showDepth;
    bool showHoles;
    float focusDistance;
    float focusMaxWidth;
    float focusWidth;
    float BokehRadius;
    bool smooth;
    bool checkDepth;
    int holeSize;
    float modelScale;
    Caustic::BBox3 modelBbox;
    CRefObj<IShader> spModelShader;
    CRefObj<IShader> spDesktopShader;
    CRefObj<ISceneMeshElem> spCubeElem;
    CRefObj<ISceneMaterialElem> spCubeMaterialElem;
    CRefObj<ISceneMeshElem> spPlaneElem;
    CRefObj<IMesh> spJointSphere[Caustic::AzureKinect::Joint::Count];
    CRefObj<ISceneMeshElem> spJointElem[Caustic::AzureKinect::Joint::Count];
    CRefObj<ISceneMaterialElem> spJointMaterialElem[Caustic::AzureKinect::Joint::Count];
    HANDLE hCanRead[2];
    HANDLE hCanWrite[2];
    int currentFrame;

    CApp() :
        smooth(false),
        displayNorms(false),
        displayColorImage(false),
        displayDepthImage(false),
        displayColor2DepthImage(false),
        colored(false),
        renderModel(false),
        renderDesktopView(true),
        renderSkeleton(false),
        detectHandRaised(false),
        useBackground(true),
        sendToCamera(true),
        sendOrigToCamera(false),
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
        holeSize(16),
        modelScale(1.0f),
        currentFrame(0)
    {
        hCanRead[0] = nullptr;
        hCanRead[1] = nullptr;
        hCanWrite[0] = nullptr;
        hCanWrite[1] = nullptr;
        planeScale = Vector3(300.0f, 300.0f, 300.0f);
        planeXlate = Vector3(0.0f, 0.0f, 1200.0f);
        planeRot = Vector3(0.0f, Caustic::DegreesToRadians(180.0f), 0.0f);
    }

    void InitializeCaustic(HWND hwnd);
    void Setup3DScene(IRenderWindow* pRenderWindow);
    void DrawLine(Vector3 p1, Vector3 p2);
    void SetupAzureCameraParameters(IShader *pShader, uint32_t depthW, uint32_t depthH, uint32_t colorW, uint32_t colorH);
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
//    auto spMesh = Caustic::MeshImport::LoadObj(L"g:\\models\\bugatti\\bugatti.obj");
//    auto spMeshElem = spSceneFactory->CreateMeshElem();
//    spMeshElem->SetMesh(spMesh);

    app.spModelShader = spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(L"ModelMesh");
    app.spDesktopShader = spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(L"Desktop");
    
    //**********************************************************************
    // Create cube. This model will be displayed when the user raises their left hand
    auto spCube = Caustic::CreateCube();
    spCube->GetBBox(&app.modelBbox);
    app.spCubeElem = spSceneFactory->CreateMeshElem();
    app.spCubeElem->SetMesh(spCube);
    app.spCubeElem->SetName(L"Cube");
    app.spCubeElem->SetShader(app.spModelShader);

    // Create material for the cube
    app.spCubeMaterialElem = spSceneFactory->CreateMaterialElem();
    CRefObj<IMaterialAttrib> spCubeMaterial = spCausticFactory->CreateMaterialAttrib();
    FRGBColor ambient(0.2f, 0.2f, 0.2f);
    FRGBColor diffuse(0.4f, 0.4f, 0.4f);
    spCubeMaterial->SetColor(L"ambientColor", ambient);
    spCubeMaterial->SetColor(L"diffuseColor", diffuse);
    app.spCubeMaterialElem->SetMaterial(spCubeMaterial);
    app.spCubeMaterialElem->SetShader(app.spModelShader);
    app.spCubeMaterialElem->AddChild(app.spCubeElem);
    //**********************************************************************

    //**********************************************************************
    // Create plane for displaying desktop
    auto spPlane = Caustic::CreateGrid(2, 2);
    app.spPlaneElem = spSceneFactory->CreateMeshElem();
    app.spPlaneElem->SetName(L"DesktopPlane");
    app.spPlaneElem->SetMesh(spPlane);
    app.spPlaneElem->SetShader(app.spDesktopShader);

    // Create plane material
    app.spPlaneMaterialElem = spSceneFactory->CreateMaterialElem();
    app.spPlaneMaterialElem->SetName(L"DesktopPlaneMaterial");
    app.spDesktopTexture = Caustic::CreateDesktopTexture(app.spRenderer);
    CRefObj<IMaterialAttrib> spPlaneMaterial = spCausticFactory->CreateMaterialAttrib();
    spPlaneMaterial->SetColor(L"ambientColor", ambient);
    spPlaneMaterial->SetColor(L"diffuseColor", diffuse);
    spPlaneMaterial->SetCullMode(D3D11_CULL_NONE);
    app.spPlaneMaterialElem->SetMaterial(spPlaneMaterial);
    app.spPlaneMaterialElem->SetShader(app.spDesktopShader);
    app.spPlaneMaterialElem->AddChild(app.spPlaneElem);
    //**********************************************************************

    //**********************************************************************
    // Create light node to hold all our objects
    app.m_spLightCollectionElem = spSceneFactory->CreateLightCollectionElem();

    // Load our model for our lightbulb
    CRefObj<IMesh> spLightBulbMesh = Caustic::MeshImport::LoadObj(L"g:\\models\\LightBulb\\Lightbulb_General_Poly_OBJ.obj");
    app.spLightShader = spRenderWindow->GetRenderer()->GetShaderMgr()->FindShader(L"ModelMesh");
    app.spLightBulbElem = spSceneFactory->CreateMeshElem(); 
    app.spLightBulbElem->SetMesh(spLightBulbMesh);
    app.spLightBulbElem->SetShader(app.spLightShader);
    app.spLightBulbElem->SetName(L"LightBulb");
    app.spLightBulbElem->SetInPass(c_PassOpaque);
    app.spLightBulbMaterialElem = spSceneFactory->CreateMaterialElem();
    app.spLightBulbMaterialElem->SetName(L"LightBulbMaterialElem");
    app.spLightBulbMaterialElem->SetShader(app.spLightShader);
    app.spLightBulbMaterialElem->AddChild(app.spLightBulbElem);
    app.m_spLightCollectionElem->AddChild(app.spLightBulbMaterialElem);

    Vector3 lightPos(1000.0f, 1000.0f, 0.0f);
    FRGBColor lightColor(1.0f, 1.0f, 1.0f);
    CRefObj<ILight> spLight(spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));
    app.m_spLightCollectionElem->AddLight(spLight);

    Vector3 lightDir(-1.0f, -1.0f, -1.0f);
    spPointLight = spCausticFactory->CreateDirectionalLight(lightPos, lightDir, lightColor, 1.0f);
    app.m_spLightCollectionElem->AddLight(spPointLight);
    
    // Create set of spheres used for rendering skeleton
    auto spJointMaterialElem = spSceneFactory->CreateMaterialElem();
    CRefObj<IMaterialAttrib> spJointMaterial = spCausticFactory->CreateMaterialAttrib();
    ambient = FRGBColor(0.2f, 0.2f, 0.2f);
    diffuse = FRGBColor(0.4f, 0.4f, 0.4f);
    spJointMaterial->SetColor(L"ambientColor", ambient);
    spJointMaterial->SetColor(L"diffuseColor", diffuse);
    spJointMaterialElem->SetMaterial(spJointMaterial);
    spJointMaterialElem->SetShader(app.spModelShader);
    spJointMaterialElem->SetName(L"JointRoot");
    spJointMaterialElem->SetFlags(spJointMaterialElem->GetFlags() & ~ESceneElemFlags::DepthTested);
    for (int i = 0; i < Caustic::AzureKinect::Joint::Count; i++)
    {
        app.spJointMaterialElem[i] = spSceneFactory->CreateMaterialElem();
        CRefObj<IMaterialAttrib> spMaterial = spCausticFactory->CreateMaterialAttrib();
        FRGBColor ambient(0.2f, 0.2f, 0.2f);
        FRGBColor diffuse(0.4f, 0.4f, 0.4f);
        spMaterial->SetColor(L"ambientColor", ambient);
        spMaterial->SetColor(L"diffuseColor", diffuse);
        app.spJointMaterialElem[i]->SetMaterial(spMaterial);
        app.spJointMaterialElem[i]->SetShader(app.spModelShader);

        app.spJointElem[i] = spSceneFactory->CreateMeshElem();
        app.spJointSphere[i] = Caustic::CreateSphere(10);
        app.spJointElem[i]->SetMesh(app.spJointSphere[i]);
        app.spJointElem[i]->SetName((std::wstring(L"Joint") + std::wstring(AzureKinect::JointNames[i])).c_str());
        app.spJointElem[i]->SetShader(app.spModelShader);
        app.spJointMaterialElem[i]->AddChild(spJointElem[i]);
        spJointMaterialElem->AddChild(app.spJointMaterialElem[i]);
    }
    app.m_spLightCollectionElem->AddChild(spJointMaterialElem);

    // Add objects to our light collection
    app.m_spLightCollectionElem->AddChild(app.spPlaneMaterialElem);
    app.m_spLightCollectionElem->AddChild(app.spCubeMaterialElem);
    spSceneGraph->AddChild(app.m_spLightCollectionElem);
}

void CApp::SetupAzureCameraParameters(IShader *pShader, uint32_t depthW, uint32_t depthH, uint32_t colorW, uint32_t colorH)
{
    app.spRenderer->Freeze();
    auto intrinsics = app.spCamera->GetAzureColorIntrinsics();
    pShader->SetVSParamFloat(L"codx", intrinsics.codx);
    pShader->SetVSParamFloat(L"cody", intrinsics.cody);
    pShader->SetVSParamFloat(L"cx", intrinsics.cx);
    pShader->SetVSParamFloat(L"cy", intrinsics.cy);
    pShader->SetVSParamFloat(L"fx", intrinsics.fx);
    pShader->SetVSParamFloat(L"fy", intrinsics.fy);
    pShader->SetVSParamFloat(L"k1", intrinsics.k1);
    pShader->SetVSParamFloat(L"k2", intrinsics.k2);
    pShader->SetVSParamFloat(L"k3", intrinsics.k3);
    pShader->SetVSParamFloat(L"k4", intrinsics.k4);
    pShader->SetVSParamFloat(L"k5", intrinsics.k5);
    pShader->SetVSParamFloat(L"k6", intrinsics.k6);
    pShader->SetVSParamFloat(L"metricRadius", intrinsics.metricRadius);
    pShader->SetVSParamFloat(L"p1", intrinsics.p1);
    pShader->SetVSParamFloat(L"p2", intrinsics.p2);
    pShader->SetVSParamInt(L"type", intrinsics.type);
    Matrix m(app.cameraExt);
    std::any mat = std::any(m);
    pShader->SetVSParam(L"colorExt", mat);
    pShader->SetVSParamInt(L"colorWidth", colorW);
    pShader->SetVSParamInt(L"colorHeight", colorH);
    pShader->SetPSParamInt(L"depthWidth", depthW);
    pShader->SetPSParamInt(L"depthHeight", depthH);
    pShader->SetVSParamFloat(L"minDepth", 10.0f);
    pShader->SetPSParamFloat(L"minDepth", 10.0f);
    pShader->SetVSParamFloat(L"maxDepth", 8000.0f);
    pShader->SetPSParamFloat(L"maxDepth", 8000.0f);
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
    app.spLineShader->BeginRender(app.spRenderer, nullptr, lights, nullptr);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    pContext->Draw(2, 0);
    app.spLineShader->EndRender(app.spRenderer);

    spCtx2->EndEvent();
}

struct ImGuiEvent
{
    HWND hWnd;
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
};
bool initializedCS = false;
CRITICAL_SECTION s_cs;
std::vector<ImGuiEvent> events;

void CApp::InitializeCaustic(HWND hwnd)
{
    app.hwnd = hwnd;
    spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    spRenderWindow = CreateRenderWindow(hwnd, viewport, shaderFolder,
        [](IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)
        {
            if (pass != Caustic::c_PassOpaque)
                return;
            app.cameraExt = app.spCamera->ColorExtrinsics();
            app.cameraInt = app.spCamera->ColorIntrinsics();
            uint32_t depthW = app.spCamera->GetDepthWidth();
            uint32_t depthH = app.spCamera->GetDepthHeight();
            uint32_t colorW = app.spCamera->GetColorWidth();
            uint32_t colorH = app.spCamera->GetColorHeight();
            app.SetupAzureCameraParameters(app.spModelShader, depthW, depthH, colorW, colorH);
            app.SetupAzureCameraParameters(app.spDesktopShader, depthW, depthH, colorW, colorH);
            app.SetupAzureCameraParameters(app.spLightShader, depthW, depthH, colorW, colorH);
            app.spDesktopTexture->Update(pRenderer);

            std::any spDesktopParam(app.spDesktopTexture);
            app.spDesktopShader->SetPSParam(L"diffuseTexture", spDesktopParam);

            Matrix m(app.cameraExt);
            std::any mat = std::any(m);
            app.spBokehShader->SetPSParam(L"colorExt", mat);

            std::any lightPos(Float3(app.lightPos.x, app.lightPos.y, app.lightPos.z));
            app.spBokehShader->SetPSParam(L"lightPos", lightPos);
            std::any lightClr(Float3(app.lightClr.x, app.lightClr.y, app.lightClr.z));
            app.spBokehShader->SetPSParam(L"lightClr", lightClr);

            EnterCriticalSection(&s_cs);
            for (size_t i = 0; i < events.size(); i++)
                ImGui_ImplWin32_WndProcHandler(events[i].hWnd, events[i].msg, events[i].wParam, events[i].lParam);
            events.resize(0);
            LeaveCriticalSection(&s_cs);

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Checkbox("SendToCamera", &app.sendToCamera);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Should we send image to camera driver?");
            ImGui::Checkbox("Display Normals", &app.displayNorms);
            ImGui::Checkbox("Display Color Image", &app.displayColorImage);
            ImGui::Checkbox("Display Normalized Depth Image", &app.displayDepthImage);
            ImGui::Checkbox("Display Color2Depth Image", &app.displayColor2DepthImage);
            ImGui::Checkbox("SendOrigToCamera", &app.sendOrigToCamera);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Sends original (unprocessed image) to camera driver");
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
            ImGui::Checkbox("RenderDesktopView", &app.renderDesktopView);
            ImGui::Checkbox("FillHoles", &app.smooth);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Fill holes in depth map");
            ImGui::Checkbox("RenderSkeleton", &app.renderSkeleton);
            ImGui::Checkbox("Detect Hand Raise", &app.detectHandRaised);
            ImGui::Checkbox("UseFocusTracking", &app.focusTracking);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Use head tracking to set focus distance");
            ImGui::SliderFloat("FocusDistance", &app.focusDistance, 0.0f, 1.0f, "%f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderFloat("FocusWidth", &app.focusWidth, 0.0f, 3.0f, "%f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderFloat("FocusMaxWidth", &app.focusMaxWidth, 0.0f, 3.0f, "%f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderFloat("BokehRadius", &app.BokehRadius, 0.0f, 30.0f, "%f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::DragInt("HoleSize", &app.holeSize, 1.0f, 0, 128);
            ImGui::DragFloat("ModelScale", &app.modelScale, 0.1f, 0.5f, 1000.0f);
            ImGui::DragFloat3("RotatePlane", (float*)&app.planeRot.x, 0.1f, -360.0f, 360.0f);
            ImGui::DragFloat3("ScalePlane", (float*)&app.planeScale.x, 0.1f, 0.01f, 3000.0f);
            ImGui::DragFloat3("XlatePlane", (float*)&app.planeXlate.x, 0.1f, -3000.0f, 3000.0f);
            ImGui::DragFloat3("LightPos", (float*)&app.lightPos.x, 0.1f, 0.0f, 3000.0f);
            ImGui::ColorEdit3("LightColor", (float*)&app.lightClr.x);

             // transform plane
             auto rotmat = Matrix4x4::RotationMatrix(Caustic::DegreesToRadians(app.planeRot.x), Caustic::DegreesToRadians(app.planeRot.y), Caustic::DegreesToRadians(app.planeRot.z));
             auto scalemat = Matrix4x4::ScalingMatrix(app.planeScale.x, app.planeScale.y, app.planeScale.z);
             auto transmat = Matrix4x4::TranslationMatrix(app.planeXlate.x, app.planeXlate.y, app.planeXlate.z);
             auto transform = rotmat * scalemat * transmat;
             app.spPlaneMaterialElem->SetTransform(transform);

             CRefObj<IImage> spColorImage;
             CRefObj<IImage> spDepthImage;
             if (app.spCamera)
             {
                 if (app.spCamera->NextVideoFrame(&spColorImage, &spDepthImage, nullptr))
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
#ifdef _DEBUG
                     CComPtr<ID3D11DeviceContext2> spCtx2;
                     CT(app.spRenderer->GetContext()->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
                     spCtx2->BeginEventInt(L"SetSource", 0);
#endif
                     app.spSourceColorNode->SetSource(app.spGPUPipeline, app.spLastColorImage);
                     app.spSourceDepthNode->SetSource(app.spGPUPipeline, app.spLastDepthImage);
#ifdef _DEBUG
                     spCtx2->EndEvent();
#endif

                     // Run the pipeline. Normally we would run spGPUPipeline->Process() however, we don't have
                     // any sink nodes in our graph (GPUPipeline::Process() runs the sink nodes). So, we need
                     // to explicitly run each end point of the graph.
                     app.spGPUPipeline->IncrementCurrentEpoch();
                     app.spColor2Depth->Process(app.spGPUPipeline, pRenderer, pRenderCtx);
                     if (app.smooth)
                         app.spDepthOfFieldFilled->Process(app.spGPUPipeline, pRenderer, pRenderCtx);
                     else
                         app.spDepthOfField->Process(app.spGPUPipeline, pRenderer, pRenderCtx);
                     if (app.displayNorms)
                         app.spNormNode->Process(app.spGPUPipeline, pRenderer, pRenderCtx);

                     //**********************************************************************
                     // First run preprocess step to populate the depth buffer with depth from the camera
                     CComPtr<ID3D11DepthStencilState> spOldDepthStencilState;
                     UINT oldStencilRef;
                     app.spRenderer->GetContext()->OMGetDepthStencilState(&spOldDepthStencilState, &oldStencilRef);

                     CComPtr<ID3D11DepthStencilState> spDepthStencilState;
                     D3D11_DEPTH_STENCIL_DESC depthDesc;
                     depthDesc.DepthEnable = TRUE;
                     depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                     depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
                     depthDesc.StencilEnable = FALSE;
                     depthDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
                     depthDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
                     const D3D11_DEPTH_STENCILOP_DESC defaultStencilOp =
                     { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
                     depthDesc.FrontFace = defaultStencilOp;
                     depthDesc.BackFace = defaultStencilOp;
                     app.spRenderer->GetDevice()->CreateDepthStencilState(&depthDesc, &spDepthStencilState);

                     app.spRenderer->GetContext()->OMSetDepthStencilState(spDepthStencilState, 0);

                     app.spZPrePassShader->SetPSParamInt(L"depthWidth", app.spCamera->GetColorWidth());
                     app.spZPrePassShader->SetPSParamInt(L"depthHeight", app.spCamera->GetColorHeight());
                     auto spTexture = ((app.smooth) ? app.spDepthOfFieldFilled : app.spDepthOfField)->GetOutputTexture(app.spGPUPipeline);
                     std::any depthTex(app.spNormDepth->GetOutputTexture(app.spGPUPipeline));
                     app.spZPrePassShader->SetPSParam(L"depthtex", depthTex);
                     app.spRenderer->DrawScreenQuadWithCustomShader(app.spZPrePassShader, 0.0f, 0.0f, 1.0f, 1, spTexture, nullptr, false);

                     app.spRenderer->GetContext()->OMSetDepthStencilState(spOldDepthStencilState, oldStencilRef);
                     //**********************************************************************
                 }
                 if (app.renderModel)
                     app.spCubeElem->SetFlags(app.spCubeElem->GetFlags() & ~ESceneElemFlags::Hidden);
                 else
                     app.spCubeElem->SetFlags(app.spCubeElem->GetFlags() | ESceneElemFlags::Hidden);
                 if (app.renderDesktopView)
                     app.spPlaneElem->SetFlags(app.spPlaneElem->GetFlags() & ~ESceneElemFlags::Hidden);
                 else
                     app.spPlaneElem->SetFlags(app.spPlaneElem->GetFlags() | ESceneElemFlags::Hidden);
                 if (app.spCamera->BodyTrackingOn() && app.spCamera->NumberBodiesDetected() >= 1)
                 {
                     if (app.focusTracking)
                     {
                         auto mat0 = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::Head);
                         app.focusDistance = mat0.v[3][2] / 8.0f;
                         auto mat1 = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::Neck);
                         app.focusWidth = static_cast<float>(fabs(mat1.v[3][2] - mat0.v[3][2]) / 8.0f);
                         app.focusWidth = 0.03f;
                     }

                     if (app.renderSkeleton)
                     {
                         for (int i = 0; i < (int)Caustic::AzureKinect::Joint::Count; i++)
                         {
                             auto mat1 = app.spCamera->GetJointMatrix(0, (Caustic::AzureKinect::Joint)i);
                             auto scale = Matrix4x4::ScalingMatrix(1.0f / 100.0f, 1.0f / 100.0f, 1.0f / 100.0f) * mat1 * Matrix4x4::ScalingMatrix(1000.0f, 1000.0f, 1000.0f);
                             app.spJointMaterialElem[i]->SetTransform(scale);
                             auto parent = app.spCamera->GetParentJoint((Caustic::AzureKinect::Joint)i);
                             if (parent != i)
                             {
                                 auto mat0 = app.spCamera->GetJointMatrix(0, parent);
                                 Vector3 p1(mat0.v[3][0] * 1000.0f, mat0.v[3][1] * 1000.0f, mat0.v[3][2] * 1000.0f);
                                 Vector3 p2(mat1.v[3][0] * 1000.0f, mat1.v[3][1] * 1000.0f, mat1.v[3][2] * 1000.0f);
                                 Vector4 clr(1.0f, 0.0f, 0.0f, 1.0f);
                                 app.DrawLine(p1, p2);
                             }
                         }
                     }

                     if (app.detectHandRaised)
                     {
                         auto mat1 = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::HandLeft);
                         auto mat2 = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::ElbowLeft);
                         auto mat3 = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::HandTipLeft);
                         Vector3 handPos(mat1[3][0] * 1000.0f, mat1[3][1] * 1000.0f, mat1[3][2] * 1000.0f);
                         Vector3 elbowPos(mat2[3][0] * 1000.0f, mat2[3][1] * 1000.0f, mat2[3][2] * 1000.0f);
                         Vector3 handTipPos(mat3[3][0] * 1000.0f, mat3[3][1] * 1000.0f, mat3[3][2] * 1000.0f);
                         Vector3 dir = (handPos - elbowPos).Normalize();
                         Vector3 dir2 = (handPos - handTipPos).Normalize();
                         if (fabs(dir.y) > 0.8f && fabs(dir2.y) > 0.8f)
                             app.spCubeElem->SetFlags(app.spCubeElem->GetFlags() & ~ESceneElemFlags::Hidden);
                         else
                             app.spCubeElem->SetFlags(app.spCubeElem->GetFlags() | ESceneElemFlags::Hidden);
                     }
                     auto mat = app.spCamera->GetJointMatrix(0, Caustic::AzureKinect::Joint::HandLeft);
                     Vector3 headPos(mat[3][0] * 1000.0f, mat[3][1] * 1000.0f, mat[3][2] * 1000.0f);
                     Vector3 center = (app.modelBbox.maxPt + app.modelBbox.minPt) / 2.0f;
                     Matrix4x4 m(app.modelScale, 0.0f, 0.0f, 0.0f,
                                 0.0f, app.modelScale, 0.0f, 0.0f,
                                 0.0f, 0.0f, app.modelScale, 0.0f,
                                 app.modelScale * -center.x + headPos.x, 
                                 app.modelScale * -center.y + headPos.y, 
                                 app.modelScale * -center.z + headPos.z, 1.0f);
                     app.spCubeMaterialElem->SetTransform(m);

                     auto lightBulbRot = Caustic::Matrix4x4::RotationMatrix(0.0f, 0.0f, Caustic::DegreesToRadians(180.0f));
                     auto lightBulbXlate = Caustic::Matrix4x4::TranslationMatrix(-app.lightPos.x, app.lightPos.y, app.lightPos.z);
                     auto lightBulbMat = lightBulbRot* lightBulbXlate;
                     app.spLightBulbMaterialElem->SetTransform(lightBulbMat);
                 }
             }

         },
         [](IRenderer* pRenderer)
         {
             auto DisplayTexture = [](const char* label, IGPUPipelineNode* spNode, bool& flag, bool flip) {
                 auto spTexture = spNode->GetOutputTexture(app.spGPUPipeline);
                 if (spTexture)
                 {
                     ImGui::Begin(label, &flag);
                     ImVec2 winsize = ImGui::GetContentRegionAvail();
                     if (winsize.y == 0)
                     {
                         winsize = ImVec2(256, 256);
                         ImGui::SetWindowSize(winsize);
                     }
                     ImVec2 uv0(0.0f, (flip) ? 1.0f : 0.0f);
                     ImVec2 uv1(1.0f, (flip) ? 0.0f : 1.0f);
                     ImGui::Image((ImTextureID)spTexture->GetD3DTextureRV(), winsize, uv0, uv1);
                     ImGui::End();
                 }
             };

             if (app.displayNorms)
                 DisplayTexture("Normals", app.spNormNode, app.displayNorms, true);
             if (app.displayColorImage)
                 DisplayTexture("ColorSource", app.spSourceColorNode, app.displayColorImage, false);
             if (app.displayDepthImage)
                 DisplayTexture("Normalized Depth", app.spNormDepth, app.displayDepthImage, true);
             if (app.displayColor2DepthImage)
                 DisplayTexture("Color2Depth", app.spColor2Depth, app.displayColor2DepthImage, true);

             ImGui::Render();
             ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

             if (app.pBackImage == nullptr)
             {
                 uint32_t w = pRenderer->GetBackBufferWidth();
                 uint32_t h = pRenderer->GetBackBufferHeight();
                 app.pBackImage = Caustic::CreateImage(1920, 1080, EImageType::RGBA_32bpp);
             }
             DWORD res = WaitForSingleObject(app.hCanWrite[app.currentFrame], 0);
             if (res == WAIT_OBJECT_0)
             {
                 ResetEvent(app.hCanWrite[app.currentFrame]);
                 pRenderer->CopyFrameBackBuffer(app.pBackImage);
                 uint32_t w = app.spRenderer->GetBackBufferWidth();
                 uint32_t h = app.spRenderer->GetBackBufferHeight();
                 uint32_t mw = min(w, app.pBackImage->GetWidth());
                 uint32_t mh = min(h, app.pBackImage->GetHeight());
                 const int imageSize = 1920 * 1080 * 4;
                 BYTE* pDstRow = (BYTE*)&app.pMemBuf[imageSize * app.currentFrame];
                 BYTE* pSrcRow = app.pBackImage->GetData();
                 ZeroMemory(pDstRow, 1920 * 1080 * 4);
                 for (uint32_t y = 0; y < mh; y++)
                 {
                     CopyMemory((PVOID)pDstRow, pSrcRow, mw * 4);
                     pSrcRow += app.pBackImage->GetStride();
                     pDstRow += 1920 * 4;
                 }
                 SetEvent(app.hCanRead[app.currentFrame]);
                 app.currentFrame = (app.currentFrame == 0) ? 1 : 0;
             }
         },
         true);
    app.spRenderer = app.spRenderWindow->GetRenderer();

    app.spBackgroundTexture = Caustic::LoadVideoTexture(L"g:\\github\\caustic\\background.mp4", app.spRenderer);

    app.hCanRead[0] = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, TRUE, L"Global\\VirtualCameraMutexRead0");
    app.hCanRead[1] = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, TRUE, L"Global\\VirtualCameraMutexRead1");
    app.hCanWrite[0] = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, TRUE, L"Global\\VirtualCameraMutexWrite0");
    app.hCanWrite[1] = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, TRUE, L"Global\\VirtualCameraMutexWrite1");
    SetEvent(app.hCanWrite[0]);

    app.spCamera = Caustic::AzureKinect::CreateAzureKinect(0, AzureKinect::Color1080p, AzureKinect::Depth1024x1024, AzureKinect::FPS30, true);
    app.cameraExt = app.spCamera->ColorExtrinsics();
    app.cameraInt = app.spCamera->ColorIntrinsics();
    uint32_t depthW = app.spCamera->GetDepthWidth();
    uint32_t depthH = app.spCamera->GetDepthHeight();
    uint32_t colorW = app.spCamera->GetColorWidth();
    uint32_t colorH = app.spCamera->GetColorHeight();

    Setup3DScene(app.spRenderWindow);
    SetupAzureCameraParameters(app.spLineShader, depthW, depthH, colorW, colorH);

    // Create shared memory that we will use for sending image to virtual camera driver
    app.hMapFile = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, L"Global\\VirtualCameraImage");
    if (app.hMapFile)
        app.pMemBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, 1920 * 1080 * 4 * 2);

    app.spGPUPipeline = Caustic::CreateGPUPipeline(app.spRenderer);

    //**********************************************************************
    // Overall our pipeline looks like:
    //    +-------------+      +-------------+
    //    | Depth Image |-+--> | Color2Depth |
    //    +-------------+ |    +-------------+
    //                    |
    //                    |    +-------------+
    //                    +--> | DepthResize | --+
    //                         +-------------+   |
    //                    +<---------------------+
    //                    |    +-------------+    +-------------+    +-----------------+        +-------+
    //                    +--> | FillHolesHP |--> | FillHolesVP |--> | NormFilledDepth |------> | Bokeh |
    //                    |    +-------------+    +-------------+    +-----------------+   +--> | (DoF) |
    //                    |                                                                |    +-------+
    //                    |    +-----------+         +-------+                             |
    //                    +--> | NormDepth |-------> | Bokeh |                             |
    //                         +-----------+    +--> | (DoF) |                             |
    //                                          |    +-------+                             |
    //    +-------------+                       |                                          |
    //    | Color Image |-----------------------+------------------------------------------+
    //    +-------------+
    // We have a color+depth image in. We then convert the depth to match
    // the color image dimensions. We then select between a blurred version
    // of the color image and pristine color image based on the depth.
    //**********************************************************************

    app.spRawCopyShader = app.spRenderer->GetShaderMgr()->FindShader(L"RawCopy");

    //**********************************************************************
    // Create source nodes for color/depth images from camera
    //**********************************************************************
    app.spSourceColorNode = app.spGPUPipeline->CreateSourceNode(L"SourceColorImage", nullptr, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spSourceDepthNode = app.spGPUPipeline->CreateSourceNode(L"SourceDepthImage", nullptr, depthW, depthH, DXGI_FORMAT_R16_UINT);

    //**********************************************************************
    // Create texture that maps depth pixels into rays
    //**********************************************************************
    app.spRayMap = app.spCamera->BuildRayMap(depthW, depthH, true);
    app.spRayTex = Caustic::CreateTexture(app.spRenderer, depthW, depthH, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
    app.spRayTex->CopyFromImage(app.spRenderer, app.spRayMap);

    //**********************************************************************
    // Create node that maps color pixels into depth pixels
    //**********************************************************************
    auto extrinsics = app.spCamera->ColorExtrinsics();
    auto intrinsics = app.spCamera->GetAzureColorIntrinsics();
    app.spColor2Depth = app.spGPUPipeline->CreatePredefinedNode(c_CustomNode_Color2Depth, (unsigned int)depthW, (unsigned int)depthH, (unsigned int)colorW, (unsigned int)colorH,
        app.spRayTex, extrinsics, intrinsics, 10.0, 8000.0,
        DXGI_FORMAT_R32G32_FLOAT, L"Color2Depth");
    app.spColor2Depth->SetInput(nullptr, L"depthTex", nullptr, app.spSourceDepthNode);

    //**********************************************************************
    // Create shader to convert depth map into normal map
    //**********************************************************************
    auto spNormTex = Caustic::CreateTexture(app.spRenderer, depthW, depthH, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
    auto spNormShader = app.spRenderer->GetShaderMgr()->FindShader(L"Depth2Norm");
    std::any raymap = std::any(app.spRayTex);
    spNormShader->SetVSParam(L"rayTexture", raymap);
    spNormShader->SetPSParam(L"rayTexture", raymap);
    spNormShader->SetPSParamInt(L"depthWidth", depthW);
    spNormShader->SetPSParamInt(L"depthHeight", depthH);
    app.spNormNode = app.spGPUPipeline->CreateNode(L"Depth2Normals", spNormShader, colorW, colorH, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
    app.spNormNode->SetInput(nullptr, L"depthTex", nullptr, app.spSourceDepthNode);
    app.spNormNode->SetInput(nullptr, L"color2Depth", nullptr, app.spColor2Depth);

    //**********************************************************************
    // Setup Z prepass shader
    //**********************************************************************
    app.spZPrePassShader = app.spRenderer->GetShaderMgr()->FindShader(L"CameraZPrePass");

    //**********************************************************************
    // Create node that generates depth texture matching the color texture's size
    //**********************************************************************
    auto spResizeDepthShader = app.spRenderer->GetShaderMgr()->FindShader(L"ResizeDepth");
    auto spResizeDepthNode = app.spGPUPipeline->CreateNode(L"ResizeDepth", spResizeDepthShader, colorW, colorH, DXGI_FORMAT_R16_UINT);
    spResizeDepthShader->SetPSParamFloat(L"depthW", (float)depthW);
    spResizeDepthShader->SetPSParamFloat(L"depthH", (float)depthH);
    spResizeDepthShader->SetPSParamFloat(L"minDepth", 10.0f);
    spResizeDepthShader->SetPSParamFloat(L"maxDepth", 8000.0f);
    spResizeDepthNode->SetInput(nullptr, L"depthTex", nullptr, app.spSourceDepthNode);
    spResizeDepthNode->SetInput(nullptr, L"color2Depth", nullptr, app.spColor2Depth);

    //**********************************************************************
    // Create downsampled depth using a median filter
    //**********************************************************************
    app.spHoleFillShaderH = app.spRenderer->GetShaderMgr()->FindShader(L"FillHolesHP");
    app.spHoleFillShaderV = app.spRenderer->GetShaderMgr()->FindShader(L"FillHolesVP");
    auto spFillHolesH = app.spGPUPipeline->CreateNode(L"FillHoleHorizontal", app.spHoleFillShaderH, colorW, colorH, DXGI_FORMAT_R16_UINT);
    spFillHolesH->SetInput(nullptr, L"depthTex", nullptr, spResizeDepthNode);
    auto spFillHolesV = app.spGPUPipeline->CreateNode(L"FillHoleVertical", app.spHoleFillShaderV, colorW, colorH, DXGI_FORMAT_R16_UINT);
    spFillHolesV->SetInput(nullptr, L"depthTex", nullptr, spFillHolesH);

    //**********************************************************************
    // Normalize the depth map to be in the range 0..1
    //**********************************************************************
    auto spnd = app.spRenderer->GetShaderMgr()->FindShader(L"NormDepth");
    auto spNormFilledDepth = app.spGPUPipeline->CreateNode(L"NormalizeDepth", spnd, colorW, colorH, DXGI_FORMAT_R32_FLOAT);
    spNormFilledDepth->SetInput(nullptr, L"depthTex", nullptr, spFillHolesV);

    app.spNormDepth = app.spGPUPipeline->CreateNode(L"NormalizeDepth", spnd, colorW, colorH, DXGI_FORMAT_R32_FLOAT);
    app.spNormDepth->SetInput(nullptr, L"depthTex", nullptr, spResizeDepthNode);
    
    //**********************************************************************
    // Create depth of field node
    //**********************************************************************
    app.spBokehShader = app.spRenderer->GetShaderMgr()->FindShader(L"Bokeh");
    app.spDepthOfField = app.spGPUPipeline->CreateNode(L"ComputeDoF", app.spBokehShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spDepthOfField->SetInput(nullptr, L"colorTex", L"colorSampler", app.spSourceColorNode);
    app.spDepthOfField->SetInput(nullptr, L"depthTex", nullptr, app.spNormDepth);
    app.spDepthOfField->SetInput(nullptr, L"meshNorms", nullptr, app.spNormNode);
    app.spDepthOfField->SetInput(nullptr, L"color2Depth", nullptr, app.spColor2Depth);
    app.spBokehShader->SetPSParamFloat(L"depthW", (float)depthW);
    app.spBokehShader->SetPSParamFloat(L"depthH", (float)depthH);
    app.spBokehShader->SetPSParamFloat(L"colorW", (float)colorW);
    app.spBokehShader->SetPSParamFloat(L"colorH", (float)colorH);
    app.spBokehShader->SetPSParam(L"rayTexture", raymap);

    app.spDepthOfFieldFilled = app.spGPUPipeline->CreateNode(L"bokehFilled", app.spBokehShader, colorW, colorH, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spDepthOfFieldFilled->SetInput(nullptr, L"colorTex", L"colorSampler", app.spSourceColorNode);
    app.spDepthOfFieldFilled->SetInput(nullptr, L"depthTex", nullptr, spNormFilledDepth);

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
        CW_USEDEFAULT, 0, 1920, 1080, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    // Setup our renderer
    app.InitializeCaustic(hWnd);
    auto ctx = ImGui::CreateContext();
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(app.spRenderer->GetDevice(), app.spRenderer->GetContext());

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

void Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    if (app.pMemBuf)
        UnmapViewOfFile(app.pMemBuf);
    if (app.hMapFile)
        CloseHandle(app.hMapFile);
    DeleteCriticalSection(&s_cs);
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
    if (!initializedCS)
    {
        initializedCS = true;
        InitializeCriticalSection(&s_cs);
    }
    ImGuiEvent evt;
    evt.hWnd = hWnd;
    evt.lParam = lParam;
    evt.wParam = wParam;
    evt.msg = message;
    EnterCriticalSection(&s_cs);
    events.push_back(evt);
    LeaveCriticalSection(&s_cs);
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
        Shutdown();
        PostQuitMessage(0);
        break;
//    case WM_LBUTTONDOWN:
//            app.spRenderWindow->MouseDown((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32_t)wParam);
//        break;
//    case WM_LBUTTONUP:
//            app.spRenderWindow->MouseUp((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32_t)wParam);
//        break;
//    case WM_MOUSEMOVE:
//            app.spRenderWindow->MouseMove((int)LOWORD(lParam), (int)HIWORD(lParam), (uint32_t)wParam);
//        break;
//    case WM_MOUSEWHEEL:
//            app.spRenderWindow->MouseWheel((int)wParam);
//        break;
    case WM_KEYDOWN:
        //app.spRenderWindow->MapKey((uint32_t)wParam, (uint32_t)lParam);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
