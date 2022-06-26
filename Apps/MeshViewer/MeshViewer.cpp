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
#include <any>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Geometry.Mesh.IMeshConstructor;
import Geometry.MeshImport;
import Geometry.Mesh.Mesh;
import Imaging.Color;
import Parsers.JSon.JSonParser;
import Rendering.Caustic.ISpotLight;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.IRenderer;
import Rendering.GuiControls.Common;
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
    ImFont* m_pFont;
    CRefObj<IJSonObj> m_spSceneAsJson;

    CApp() : m_ImGuiInitialized(false)
    {
        InitializeCriticalSection(&m_cs);
    }

    void InitializeCaustic(HWND hwnd);
};
CApp app;

static void WalkScene(IJSonObj* pObj)
{
    if (pObj == nullptr)
        return;
    if (pObj->GetType() == CJSonType::Object)
    {
        if (ImGui::TreeNode(pObj->GetName().c_str()))
        {
            std::map<std::string, CRefObj<IJSonObj>>* pMap = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(pObj->GetValue());
            for (auto it : *pMap)
                WalkScene(it.second);
            ImGui::TreePop();
        }
    }
    else if (pObj->GetType() == CJSonType::Array)
    {
        if (ImGui::TreeNode(pObj->GetName().c_str()))
        {
            std::vector<CRefObj<IJSonObj>>* pMap = std::any_cast<std::vector<CRefObj<IJSonObj>>*>(pObj->GetValue());
            for (auto it : *pMap)
                WalkScene(it);
            ImGui::TreePop();
        }
    }
    else if (pObj->GetType() == CJSonType::Bool)
    {
        bool f;
        ImGui::Checkbox(pObj->GetName().c_str(), &f);
    }
    else
        ImGui::Text(pObj->GetName().c_str());
}

void BuildGroupUI(ISceneGroupElem* pGroup);

//**********************************************************************
// Function: BuildLightCollectionUI
// Build the UI for displaying a light collection in the scene graph pane
// 
// Parameters:
// pCollection - list of light elements
//**********************************************************************
void BuildLightCollectionUI(ISceneLightCollectionElem *pCollection)
{
    std::wstring name = pCollection->GetName();
    if (ImGui::TreeNode((name.length() == 0) ? "LightCollection" : Caustic::wstr2str(name).c_str()))
    {
        uint32 numLights = pCollection->NumberLights();
        for (uint32 i = 0; i < numLights; i++)
        {
            std::string lightName = std::string("Light-") + std::to_string(i);
            if (ImGui::TreeNode(lightName.c_str()))
            {
                CRefObj<ILight> spLight = pCollection->GetLight(i);
                bool f = spLight->GetOnOff();
                if (ImGui::Checkbox("Enable", &f))
                {
                    spLight->SetOnOff(f);
                }
                ImGui_Color("Color:", i,
                    [spLight]()->FRGBColor { return spLight.p->GetColor(); },
                    [spLight](FRGBColor v) { spLight.p->SetColor(v); });

                ImGui_Vector("Position:",
                    [spLight]()->Vector3 { return spLight.p->GetPosition(); },
                    [spLight](Vector3 v) { spLight.p->SetPosition(v); });

                if (spLight->GetType() == ELightType::DirectionalLight)
                {
                    ImGui_Vector("Direction:",
                        [spLight]()->Vector3 { return spLight.p->GetDirection(); },
                        [spLight](Vector3 v) { spLight.p->SetDirection(v); });
                }

                ImGui::TreePop();
            }
        }
        BuildGroupUI(static_cast<ISceneGroupElem*>(pCollection));
        ImGui::TreePop();
    }

}
void BuildGroupUI(ISceneGroupElem* pGroup)
{
    uint32 numChildren = pGroup->NumberChildren();
    for (uint32 i = 0; i < numChildren; i++)
    {
        CRefObj<ISceneElem> spChild = pGroup->GetChild(i);
        std::wstring name = spChild->GetName();
        switch (spChild->GetType())
        {
        case ESceneElemType::Unknown:
            ImGui::Text((name.length() == 0) ? "Unknown" : Caustic::wstr2str(name).c_str());
            break;
        case ESceneElemType::SceneGraph:
            if (ImGui::TreeNode((name.length() == 0) ? "SceneGraph" : Caustic::wstr2str(name).c_str()))
            {
                BuildGroupUI(static_cast<ISceneGroupElem*>(spChild.p));
                ImGui::TreePop();
            }
            break;
        case ESceneElemType::Mesh:
            if (ImGui::TreeNode((name.length() == 0) ? "Mesh" : Caustic::wstr2str(name).c_str()))
            {
                uint32 flags = spChild->GetFlags();
                bool isHidden = (flags & ESceneElemFlags::Hidden) ? true : false;
                bool isSelected = (flags & ESceneElemFlags::Selected) ? true : false;
                bool depthTested = (flags & ESceneElemFlags::DepthTested) ? true : false;
                bool bboxDirty = (flags & ESceneElemFlags::BBoxDirty) ? true : false;
                bool renderableDirty = (flags & ESceneElemFlags::RenderableDirty) ? true : false;
                bool materialDirty = (flags & ESceneElemFlags::MaterialDirty) ? true : false;
                uint32 finalFlags = 0;
                ImGui::Checkbox("Hidden", &isHidden);
                finalFlags |= (isHidden) ? ESceneElemFlags::Hidden : 0;
                ImGui::Checkbox("Selected", &isSelected);
                finalFlags |= (isSelected) ? ESceneElemFlags::Selected : 0;
                ImGui::Checkbox("DepthTested", &depthTested);
                finalFlags |= (depthTested) ? ESceneElemFlags::DepthTested : 0;
                ImGui::Checkbox("BBoxDirty", &bboxDirty);
                finalFlags |= (bboxDirty) ? ESceneElemFlags::BBoxDirty : 0;
                ImGui::Checkbox("RenderableDirty", &renderableDirty);
                finalFlags |= (renderableDirty) ? ESceneElemFlags::RenderableDirty : 0;
                ImGui::Checkbox("MaterialDirty", &materialDirty);
                finalFlags |= (materialDirty) ? ESceneElemFlags::MaterialDirty : 0;
                spChild->SetFlags(finalFlags);
                ImGui::TreePop();
            }
            break;
        case ESceneElemType::CustomRenderElem:
            ImGui::Text((name.length() == 0) ? "CustomRender" : Caustic::wstr2str(name).c_str());
            break;
        case ESceneElemType::Group:
            if (ImGui::TreeNode((name.length() == 0) ? "Group" : Caustic::wstr2str(name).c_str()))
            {
                BuildGroupUI(static_cast<ISceneGroupElem*>(spChild.p));
                ImGui::TreePop();
            }
            break;
        case ESceneElemType::Renderable:
            ImGui::Text((name.length() == 0) ? "Renderable" : Caustic::wstr2str(name).c_str());
            break;
        case ESceneElemType::LightCollection:
            BuildLightCollectionUI(static_cast<ISceneLightCollectionElem*>(spChild.p));
            break;
        case ESceneElemType::Material:
        {
            CRefObj<IMaterialAttrib> spMaterial;
            static_cast<ISceneMaterialElem*>(spChild.p)->GetMaterial(&spMaterial);
            std::string matname = spMaterial->GetName();
            if (ImGui::TreeNode((matname.length() == 0) ? "Material" : matname.c_str()))
            {
                //CRefObj<IMaterialAttrib> spMaterial;
                //static_cast<ISceneMaterialElem*>(spChild.p)->GetMaterial(&spMaterial);
                D3D11_CULL_MODE cullMode = spMaterial->GetCullMode();
                const char* cullModes[] = { "Front", "Back", "None" };
                static const char* currentCullItem;
                switch (cullMode)
                {
                case D3D11_CULL_MODE::D3D11_CULL_FRONT: currentCullItem = cullModes[0]; break;
                case D3D11_CULL_MODE::D3D11_CULL_BACK: currentCullItem = cullModes[1]; break;
                case D3D11_CULL_MODE::D3D11_CULL_NONE: currentCullItem = cullModes[2]; break;
                }
                if (ImGui::BeginCombo("CullMode", currentCullItem))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(cullModes); n++)
                    {
                        bool is_selected = (currentCullItem == cullModes[n]);
                        if (ImGui::Selectable(cullModes[n], is_selected))
                            currentCullItem = cullModes[n];
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                D3D11_FILL_MODE fillMode = spMaterial->GetFillMode();
                const char* fillModes[] = { "Wireframe", "Solid" };
                static const char* currentFillItem;
                switch (fillMode)
                {
                case D3D11_FILL_MODE::D3D11_FILL_WIREFRAME: currentFillItem = fillModes[0]; break;
                case D3D11_FILL_MODE::D3D11_FILL_SOLID: currentFillItem = fillModes[1]; break;
                }
                if (ImGui::BeginCombo("FillMode", currentFillItem))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(fillModes); n++)
                    {
                        bool is_selected = (currentFillItem == fillModes[n]);
                        if (ImGui::Selectable(fillModes[n], is_selected))
                        {
                            currentFillItem = fillModes[n];
                            if (n == 0)
                                spMaterial->SetFillMode(D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
                            else
                                spMaterial->SetFillMode(D3D11_FILL_MODE::D3D11_FILL_SOLID);
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                bool isTransparent = spMaterial->GetIsTransparent();
                ImGui::Checkbox("Transparent", &isTransparent);
                bool isShadowReceiver = spMaterial->GetIsShadowReceiver();
                ImGui::Checkbox("ShadowReceiver", &isShadowReceiver);
                uint32 materialID = spMaterial->GetMaterialID();
                ImGui::Text((std::string("MaterialID: ") + std::to_string(materialID)).c_str());
                spMaterial->EnumerateColors([&](const wchar_t* pName, FRGBAColor& v) {
                    std::wstring s(pName);
                    ImGui_Color(Caustic::wstr2str(s).c_str(), spMaterial->GetMaterialID(),
                        [spMaterial, pName]()->FRGBColor { return spMaterial.p->GetColor(pName); },
                        [spMaterial, pName](FRGBColor v) { spMaterial.p->SetColor(pName, v); });
                        });
                spMaterial->EnumerateScalars([](const wchar_t* pName, float v) {
                    std::wstring s(pName);
                    float q;
                    ImGui::SliderFloat(Caustic::wstr2str(s).c_str(), &q, 0.0f, 1.0f);
                    });
                //                float value = spMaterial->GetScalar("");
//                virtual CRefObj<IImage> GetTexture(const wchar_t* pName) override;
//                virtual void (std::function<void(const wchar_t* pName, float s)> func) override;
//                virtual void EnumerateTextures(std::function<void(const wchar_t* pName, IImage* pTexture, EShaderAccess access)> func) override;
//
                BuildGroupUI(static_cast<ISceneGroupElem*>(spChild.p));
                ImGui::TreePop();
            }
        }
            break;
        case ESceneElemType::ComputeShaderElem:
            ImGui::Text((name.length() == 0) ? "ComputeShader" : Caustic::wstr2str(name).c_str());
            break;
        case ESceneElemType::Overlay2D:
            ImGui::Text((name.length() == 0) ? "Overlay2D" : Caustic::wstr2str(name).c_str());
            break;
        case ESceneElemType::LineElem:
            ImGui::Text((name.length() == 0) ? "Line" : Caustic::wstr2str(name).c_str());
            break;
        case ESceneElemType::CubeElem:
            ImGui::Text((name.length() == 0) ? "Cube" : Caustic::wstr2str(name).c_str());
            break;
        case ESceneElemType::CylinderElem:
            ImGui::Text((name.length() == 0) ? "Cylinder" : Caustic::wstr2str(name).c_str());
            break;
        case ESceneElemType::SphereElem:
            ImGui::Text((name.length() == 0) ? "Sphere" : Caustic::wstr2str(name).c_str());
            break;
        }
    }
}

void BuildSceneUI(ISceneGraph *pSceneGraph)
{
    BuildGroupUI(pSceneGraph);
}

void InitializeCaustic(HWND hwnd)
{
    app.m_spSceneFactory = Caustic::CreateSceneFactory();
    app.m_spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    RECT rect2;
    GetClientRect(app.m_hwnd, &rect2);
    viewport.minPt.x = 400.0f / float(rect2.right - rect2.left);
    viewport.minPt.y = 32.0f / float(rect2.bottom - rect2.top);
    app.m_spRenderWindow = CreateRenderWindow(hwnd, viewport, shaderFolder,
        [](IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass) {
            if (app.m_ImGuiInitialized && pass == c_PassOpaque)
            {
                auto io = ImGui::GetIO();
                EnterCriticalSection(&app.m_cs);
                for (size_t i = 0; i < app.m_events.size(); i++)
                {
                    if (io.WantCaptureMouse)
                        ImGui_ImplWin32_WndProcHandler(app.m_events[i].hWnd, app.m_events[i].msg, app.m_events[i].wParam, app.m_events[i].lParam);
                }
                app.m_events.resize(0);
                LeaveCriticalSection(&app.m_cs);

                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();

                ImVec2 menuSize;
                ImGui::PushFont(app.m_pFont);
                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("Load Mesh..."))
                        {
                            wchar_t fn[MAX_PATH + 1] = { 0 };
                            OPENFILENAME ofn;
                            ZeroMemory(&ofn, sizeof(ofn));
                            ofn.lStructSize = sizeof(OPENFILENAME);
                            ofn.hwndOwner = app.m_hwnd;
                            ofn.hInstance = app.m_hInst;
                            ofn.lpstrFilter = L"OBJ Files\0*.obj\0PLY Files\0*.ply\0All Files\0*.*\0\0\0";
                            ofn.lpstrFile = fn;
                            ofn.nMaxFile = MAX_PATH;
                            ofn.lpstrTitle = L"Open Mesh";
                            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
                            ofn.lpstrDefExt = L"obj";
                            if (GetOpenFileName(&ofn))
                            {
                                wchar_t* ext = StrRChrW(fn, nullptr, L'.');
                                CRefObj<ISceneMeshElem> spMeshElem = app.m_spSceneFactory->CreateMeshElem();
                                CRefObj<IMesh> spMesh = nullptr;
                                const wchar_t* pShaderName = L"Textured";
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

                                CRefObj<IJSonParser> spJsonParser = CreateJSonParser();
                                app.m_spSceneAsJson = spSceneGraph->AsJson("SceneGraph", spJsonParser);
                            }
                        }
                        ImGui::Separator();
                        if (ImGui::MenuItem("Exit"))
                            DestroyWindow(app.m_hwnd);
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Create"))
                    {
                        if (ImGui::MenuItem("PDF Sphere"))
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
                        ImGui::EndMenu();
                    }
                    static bool isAboutOpen = false;
                    if (ImGui::BeginMenu("Help"))
                    {
                        if (ImGui::MenuItem("About"))
                        {
                            isAboutOpen = true;
                        }
                        ImGui::EndMenu();
                    }
                    menuSize = ImGui::GetWindowSize();
                    ImGui::EndMainMenuBar();

                    if (isAboutOpen)
                    {
                        ImGui::Begin("About", &isAboutOpen);
                        ImGui::Text("MeshViewer, Version 1.0");
                        ImGui::Text("Copyright (c) 2022");
                        ImGui::End();
                    }
                }
                ImGui::PopFont();

                auto spSceneGraph = app.m_spRenderWindow->GetSceneGraph();
                ImGui::SetNextWindowPos(ImVec2(0, menuSize.y));
                RECT rect;
                GetClientRect(app.m_hwnd, &rect);
                ImGui::SetNextWindowSize(ImVec2(400, float(rect.bottom - rect.top) - menuSize.y));
                ImGui::Begin("Scene");
                BuildSceneUI(spSceneGraph);//                WalkScene(app.m_spSceneAsJson);
                ImGui::End();
                
                ImGui::SetNextWindowPos(ImVec2(menuSize.x - 400, menuSize.y));
                ImGui::GetStyle().WindowRounding = 0.0f;
                bool isOpen = true;
                ImGui::Begin("FrameRate", &isOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                ImGui::End();
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
    wcex.lpszMenuName   = nullptr;
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
           
#pragma warning(push)
#pragma warning(disable: 4996)
           const char *pCausticPixel = std::getenv("CausticRoot");
#pragma warning(pop)
           if (pCausticPixel == nullptr)
               pCausticPixel = "d:\\github\\Caustic";
           std::string fontPath = std::string(pCausticPixel) + "\\" + "External\\imgui\\misc\\fonts\\DroidSans.ttf";
           app.m_pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), 24);
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
        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
            app.m_spRenderWindow->MouseDown((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
        break;
    case WM_LBUTTONUP:
        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
            app.m_spRenderWindow->MouseUp((int)LOWORD(lParam), (int)HIWORD(lParam), c_LeftButton, (uint32)wParam);
        break;
    case WM_MOUSEMOVE:
        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
            app.m_spRenderWindow->MouseMove((int)LOWORD(lParam), (int)HIWORD(lParam), (uint32)wParam);
        break;
    case WM_MOUSEWHEEL:
        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
            app.m_spRenderWindow->MouseWheel((int)wParam);
        break;
    case WM_KEYDOWN:
        app.m_spRenderWindow->MapKey((uint32)wParam, (uint32)lParam);
        break;
    case WM_SIZE:
        {
            RECT rect2;
            GetClientRect(app.m_hwnd, &rect2);
            BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
            viewport.minPt.x = 400.0f / float(rect2.right - rect2.left);
            viewport.minPt.y = 32.0f / float(rect2.bottom - rect2.top);
            int w = LOWORD(lParam);
            int h = HIWORD(lParam);
            app.m_spRenderer->RunOnRenderer([w, h, viewport](IRenderer* pRenderer) {
                pRenderer->DeviceWindowResized((uint32)w, (uint32)h);
                pRenderer->SetViewport(viewport.minPt.x, viewport.minPt.y, viewport.maxPt.x, viewport.maxPt.y);
                });
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
