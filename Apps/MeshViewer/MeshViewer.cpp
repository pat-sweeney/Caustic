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
#include <functional>
#include <format>
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

class CApp
{
public:
    HWND m_hwnd;                                          // HWND bound to renderer
    HINSTANCE m_hInst;
    CRefObj<IRenderer> m_spRenderer;                      // Instance of our renderer
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<ICausticFactory> m_spCausticFactory;
    CRefObj<ISceneFactory> m_spSceneFactory;
    int nodeCounter;
    int selectedNode;
    std::function<void()> fillInspectorFunc;
    std::string fillInspectorTitle;

    CApp()
    {
    }

    void InitializeCaustic(HWND hwnd);
};
CApp app;

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    ImGui::SameLine();
}

//**********************************************************************
// Function: FillInspector_Elem
// Populates the inspector window with the base attributes for an ISceneElem
// 
// Parameters:
// pElem - element to use to populate inspector
//**********************************************************************
void FillInspector_Elem(ISceneElem* pElem)
{
    uint32 flags = pElem->GetFlags();
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
    pElem->SetFlags(finalFlags);
}

void FillInspector_LightCollection(ISceneLightCollectionElem *pLightCollection)
{
    FillInspector_Elem(pLightCollection);
}

void FillInspector_Light(ILight* pLight, int lightIndex)
{
    bool f = pLight->GetOnOff();
    if (ImGui::Checkbox("Enable", &f))
    {
        pLight->SetOnOff(f);
    }
    f = pLight->GetCastsShadows();
    if (ImGui::Checkbox("Casts Shadows", &f))
    {
        pLight->SetCastsShadows(f);
    }
    ImGui_Color("Color:", lightIndex,
        [pLight]()->FRGBColor { return pLight->GetColor(); },
        [pLight](FRGBColor v) { pLight->SetColor(v); });

    ImGui::Text("Intensity:");
    ImGui::SameLine();
    float intensity = pLight->GetIntensity();
    if (ImGui::SliderFloat(std::string("##").c_str(), &intensity, 0.0f, 1.0f))
        pLight->SetIntensity(intensity);

    ImGui_Vector("Position:",
        [pLight]()->Vector3 { return pLight->GetPosition(); },
        [pLight](Vector3 v) { pLight->SetPosition(v); }, 0.0f, 10000.0f);

    if (pLight->GetType() == ELightType::DirectionalLight)
    {
        ImGui_Vector("Direction:",
            [pLight]()->Vector3 { return pLight->GetDirection(); },
            [pLight](Vector3 v) { pLight->SetDirection(v); }, -1.0f, +1.0f);
    }
}

void FillInspector_ComputeShader(ISceneComputeShaderElem* pComputeShader)
{
    FillInspector_Elem(pComputeShader);
}

void FillInspector_Overlay2D(ISceneOverlay2DElem* pOverlay)
{
    FillInspector_Elem(pOverlay);
}

void FillInspector_Line(ISceneLineElem* pLine)
{
    FillInspector_Elem(pLine);
}

void FillInspector_CustomRender(ISceneCustomRenderElem* pCustomRender)
{
    FillInspector_Elem(pCustomRender);
}

void FillInspector_Cube(ISceneCubeElem* pCube)
{
    FillInspector_Elem(pCube);
}

void FillInspector_Cylinder(ISceneCylinderElem* pCylinder)
{
    FillInspector_Elem(pCylinder);
}

void FillInspector_Sphere(ISceneSphereElem* pSphere)
{
    FillInspector_Elem(pSphere);
}

void FillInspector_Mesh(ISceneMeshElem *pMesh)
{
    FillInspector_Elem(pMesh);
}

void FillInspector_Group(ISceneGroupElem *pGroup)
{
    FillInspector_Elem(pGroup);
}

//**********************************************************************
// Function: FillInspector_Material
// Populates the inspector window with the attributes from a material
// 
// Parameters:
// pMaterialElem - Material element that holds pMaterial. Maybe nullptr.
// pMaterail - the material attributes
//**********************************************************************
void FillInspector_Material(ISceneMaterialElem *pMaterialElem, IMaterialAttrib* pMaterial)
{
    if (pMaterialElem != nullptr)
        FillInspector_Elem(pMaterialElem);

    D3D11_CULL_MODE cullMode = pMaterial->GetCullMode();
    const char* cullModes[] = { "Front", "Back", "None" };
    static const char* currentCullItem;
    switch (cullMode)
    {
    case D3D11_CULL_MODE::D3D11_CULL_FRONT: currentCullItem = cullModes[0]; break;
    case D3D11_CULL_MODE::D3D11_CULL_BACK: currentCullItem = cullModes[1]; break;
    case D3D11_CULL_MODE::D3D11_CULL_NONE: currentCullItem = cullModes[2]; break;
    }
    ImGui::Text("CullMode:");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##CullMode", currentCullItem))
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

    D3D11_FILL_MODE fillMode = pMaterial->GetFillMode();
    const char* fillModes[] = { "Wireframe", "Solid" };
    static const char* currentFillItem;
    switch (fillMode)
    {
    case D3D11_FILL_MODE::D3D11_FILL_WIREFRAME: currentFillItem = fillModes[0]; break;
    case D3D11_FILL_MODE::D3D11_FILL_SOLID: currentFillItem = fillModes[1]; break;
    }
    ImGui::Text("FillMode:");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##FillMode", currentFillItem))
    {
        for (int n = 0; n < IM_ARRAYSIZE(fillModes); n++)
        {
            bool is_selected = (currentFillItem == fillModes[n]);
            if (ImGui::Selectable(fillModes[n], is_selected))
            {
                currentFillItem = fillModes[n];
                if (n == 0)
                    pMaterial->SetFillMode(D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
                else
                    pMaterial->SetFillMode(D3D11_FILL_MODE::D3D11_FILL_SOLID);
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    bool isTransparent = pMaterial->GetIsTransparent();
    if (ImGui::Checkbox("Transparent", &isTransparent))
        pMaterial->SetIsTransparent(isTransparent);
    bool isShadowReceiver = pMaterial->GetIsShadowReceiver();
    if (ImGui::Checkbox("ShadowReceiver", &isShadowReceiver))
        pMaterial->SetIsShadowReceiver(isShadowReceiver);
    uint32 materialID = pMaterial->GetMaterialID();
    ImGui::Text((std::string("MaterialID: ") + std::to_string(materialID)).c_str());
    pMaterial->EnumerateColors([&](const wchar_t* pName, FRGBAColor& v) {
        std::wstring s(pName);
        ImGui_Color(Caustic::wstr2str(s).c_str(), pMaterial->GetMaterialID(),
            [pMaterial, pName]()->FRGBColor { return pMaterial->GetColor(pName); },
            [pMaterial, pName](FRGBColor v) { pMaterial->SetColor(pName, v); });
        });
    pMaterial->EnumerateScalars([pMaterial](const wchar_t* pName, float v) {
        std::wstring s(pName);
        float q;
        if (ImGui::SliderFloat(Caustic::wstr2str(s).c_str(), &q, 0.0f, 1.0f))
            pMaterial->SetScalar(pName, q);
        });
    //        pMaterial->EnumerateTextures([](const wchar_t* pName, IImage* pTexture, EShaderAccess access) {
    //            CRefObj<IRenderer> spRenderer = app.m_spRenderWindow->GetRenderer();
    //            CRefObj<ITexture> spTexture = app.m_spCausticFactory->CreateTexture(spRenderer, pTexture, D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE,
    //                D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
    //            ImGui::Image((void*)spTexture->GetD3DTextureRV(), ImVec2(128, 128));
    //            });
}

void FillInspector_SceneGraph(ISceneGraph* pSceneGraph)
{
    ImGui::Text("Show Proxies:");
    ImGui::SameLine();
    bool f = pSceneGraph->GetShowProxyObjects();
    if (ImGui::Checkbox("##ShowProxies", &f))
        pSceneGraph->SetShowProxyObjects(f);
}

//**********************************************************************
// Function: BuildCollapsableNode
// Builds a node in our scene graph window.
// 
// Parameters:
// pSceneGraph - our parent scene graph
// pElem - element we are adding to the tree
// isLeaf - is this element a leaf node?
// pDefaultName - default name to use if pElem->GetName() is empty
// buildChildren - lambda used to enumerate and build child elements
// onSelect - lambda called when node is selected. This callback is used
// to populate the inspector window with the attributes of the selected node.
//**********************************************************************
void BuildCollapsableNode(ISceneGraph* pSceneGraph, ISceneElem *pElem, bool isLeaf, 
    const char *pDefaultName, std::function<void()> buildChildren, std::function<void()> onSelect)
{
    auto nodeCounter = app.nodeCounter++;
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_SpanAvailWidth;
    if (isLeaf)
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (nodeCounter == app.selectedNode)
        flags |= ImGuiTreeNodeFlags_Selected;
    std::wstring elemName;
    if (pElem != nullptr)
        elemName = pElem->GetName();
    bool nodeOpen = ImGui::TreeNodeEx(pElem, flags, (elemName.length() == 0) ? pDefaultName : Caustic::wstr2str(elemName).c_str());
    bool nodeClicked = ImGui::IsItemClicked();
    if (nodeOpen)
    {
        if (buildChildren != nullptr)
            buildChildren();
        ImGui::TreePop();
    }
    if (nodeClicked)
    {
        if (pElem != nullptr)
        {
            pSceneGraph->ClearSelected();
            pSceneGraph->SelectObject(pElem);
        }
        app.fillInspectorFunc = onSelect;
        app.fillInspectorTitle = std::string(pDefaultName);
        app.selectedNode = nodeCounter;
    }
}

//**********************************************************************
// Function: BuildGroupUI
// Populates our scene graph window with a node for a group along with
// all the children of that group.
// 
// Parameters:
// pSceneGraph - our parent scene graph
// pGroup - group element to add
// pDefaultName - default name
// onSelect - lambda called when node is selected. This callback is used
// to populate the inspector window with the attributes of the selected node.
//**********************************************************************
void BuildGroupUI(ISceneGraph* pSceneGraph, ISceneGroupElem* pGroup, const char * pDefaultName,
    std::function<void()> onSelect)
{
    BuildCollapsableNode(pSceneGraph, pGroup, false, pDefaultName,
        [pSceneGraph, pGroup]() {
            uint32 numChildren = pGroup->NumberChildren();
            for (uint32 i = 0; i < numChildren; i++)
            {
                CRefObj<ISceneElem> spChild = pGroup->GetChild(i);
                std::wstring name = spChild->GetName();
                switch (spChild->GetType())
                {
                case ESceneElemType::Unknown:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "Unknown", nullptr, [spChild]() { FillInspector_Elem(static_cast<ISceneElem*>(spChild.p)); });
                    break;
                case ESceneElemType::SceneGraph:
                    BuildGroupUI(pSceneGraph, (ISceneGroupElem*)spChild.p, "SceneGraph", [spChild]() { FillInspector_SceneGraph(static_cast<ISceneGraph*>(spChild.p)); });
                    break;
                case ESceneElemType::Mesh:
                    BuildCollapsableNode(pSceneGraph, spChild, false, "Mesh", [pSceneGraph, spChild]() {
                        auto spMesh = static_cast<ISceneMeshElem*>(spChild.p)->GetMesh();
                        uint32 numMats = spMesh->GetNumberMaterials();
                        for (uint32 i = 0; i < numMats; i++)
                        {
                            CRefObj<IMaterialAttrib> spMat = spMesh->GetMaterial(i);
                            std::string matName = spMat->GetName();
                            BuildCollapsableNode(pSceneGraph, nullptr, true,
                                (matName.length() == 0) ? "Material" : matName.c_str(), nullptr,
                                [spMat]() { FillInspector_Material(nullptr, spMat.p); });
                        }
                    }, [spChild]() { FillInspector_Mesh((ISceneMeshElem*)spChild.p); });
                    break;
                case ESceneElemType::CustomRenderElem:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "CustomRender", nullptr, [spChild]() { FillInspector_CustomRender((ISceneCustomRenderElem*)spChild.p); });
                    break;
                case ESceneElemType::Group:
                    BuildGroupUI(pSceneGraph, (ISceneGroupElem*)spChild.p, "Group", [spChild]() {FillInspector_Group((ISceneGroupElem*)spChild.p); });
                    break;
                case ESceneElemType::Renderable: // Type not implemented!
                    break;
                case ESceneElemType::LightCollection:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "LightCollection",
                        [pSceneGraph, spChild]()
                        {
                            ISceneLightCollectionElem* pCollection = (ISceneLightCollectionElem*)spChild.p;
                            uint32 numLights = pCollection->NumberLights();
                            for (uint32 i = 0; i < numLights; i++)
                            {
                                CRefObj<ILight> spLight = pCollection->GetLight(i);
                                std::string lightName = std::string("Light-") + std::to_string(i);
                                BuildCollapsableNode(pSceneGraph, nullptr, true, lightName.c_str(),
                                    nullptr, [spLight, i]() { FillInspector_Light(spLight.p, i); });
                            }
                            BuildGroupUI(pSceneGraph, static_cast<ISceneGroupElem*>(pCollection), "Children", nullptr);
                        },
                        [spChild]() { FillInspector_LightCollection((ISceneLightCollectionElem*)spChild.p); });
                    break;
                case ESceneElemType::Material:
                    {
                        ISceneMaterialElem* pMaterialElem = static_cast<ISceneMaterialElem*>(spChild.p);
                        auto spMaterialAttrib = pMaterialElem->GetMaterial();
                        BuildGroupUI(pSceneGraph, (ISceneGroupElem*)pMaterialElem, "Material", [pMaterialElem, spMaterialAttrib]() { FillInspector_Material(pMaterialElem, spMaterialAttrib.p); });
                    }
                    break;
                case ESceneElemType::ComputeShaderElem:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "ComputeShader", nullptr, [spChild]() { FillInspector_ComputeShader((ISceneComputeShaderElem*)spChild.p); });
                    break;
                case ESceneElemType::Overlay2D:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "Overlay2D", nullptr, [spChild]() { FillInspector_Overlay2D((ISceneOverlay2DElem*)spChild.p); });
                    break;
                case ESceneElemType::LineElem:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "Line", nullptr, [spChild]() { FillInspector_Line((ISceneLineElem*)spChild.p); });
                    break;
                case ESceneElemType::CubeElem:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "Cube", nullptr, [spChild]() { FillInspector_Cube((ISceneCubeElem*)spChild.p); });
                    break;
                case ESceneElemType::CylinderElem:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "Cylinder", nullptr, [spChild]() { FillInspector_Cylinder((ISceneCylinderElem*)spChild.p); });
                    break;
                case ESceneElemType::SphereElem:
                    BuildCollapsableNode(pSceneGraph, spChild, true, "Sphere", nullptr, [spChild]() { FillInspector_Sphere((ISceneSphereElem*)spChild.p); });
                    break;
                }
            }
        }, onSelect);
}

ImVec2 BuildMenuBar(ImFont *pFont)
{
    ImVec2 menuSize;
    ImGui::PushFont(pFont);
    if (ImGui::BeginMenuBar())
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
        ImGui::EndMenuBar();

        if (isAboutOpen)
        {
            ImGui::Begin("About", &isAboutOpen);
            ImGui::Text("MeshViewer, Version 1.0");
            ImGui::Text("Copyright (c) 2022");
            ImGui::End();
        }
    }
    ImGui::PopFont();
    return menuSize;
}

void BuildPanels(ITexture *pFinalRT, ImFont *pFont)
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImVec2 menuSize = BuildMenuBar(pFont);
    
    RECT rect;
    GetClientRect(app.m_hwnd, &rect);

    auto spSceneGraph = app.m_spRenderWindow->GetSceneGraph();
    ImGui::Begin("Scene");
    app.nodeCounter = 0;
    BuildGroupUI(spSceneGraph, spSceneGraph, "SceneGraphRoot", [spSceneGraph]() { FillInspector_SceneGraph(spSceneGraph.p); });
    ImGui::End();

    std::string inspectorTitle = "Inspector###Inspector";
    if (app.fillInspectorTitle.length() > 0)
        inspectorTitle = std::string("Inspector (") + app.fillInspectorTitle + std::string(")###Inspector");
    ImGui::Begin(inspectorTitle.c_str());
    if (app.fillInspectorFunc)
        app.fillInspectorFunc();
    ImGui::End();

    if (pFinalRT != nullptr)
    {
        ImGui::Begin("FinalRender", nullptr);
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        ImVec2 vSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
        auto x = ImGui::GetCursorPos();
        ImGui::Image((void*)pFinalRT->GetD3DTextureRV(), vSize);
        ImGui::SetCursorPos(x);
        ImGui::InvisibleButton("##HiddenEventHandler", vSize);
        ImVec2 mousePositionRelative;
        static ImVec2 lastRel;
        if (ImGui::IsItemHovered())
        {
            ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
            ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
            mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
        }
        static bool wasDown = false;
        ImGui::SetItemUsingMouseWheel();
        if (ImGui::IsItemFocused())
        {
            if (!wasDown && ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                app.m_spRenderWindow->MouseDown((int)mousePositionRelative.x, (int)mousePositionRelative.y, c_LeftButton, (uint32)0);
                wasDown = true;
            }
            else if (wasDown && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                app.m_spRenderWindow->MouseUp((int)mousePositionRelative.x, (int)mousePositionRelative.y, c_LeftButton, (uint32)0);
                wasDown = false;
            }
            float dx = lastRel.x - mousePositionRelative.x;
            float dy = lastRel.y - mousePositionRelative.y;
            static ImVec2 lastRel = mousePositionRelative;
            if (wasDown && (!IsZero(dx) || !IsZero(dy)))
            {
                app.m_spRenderWindow->MouseMove((int)mousePositionRelative.x, (int)mousePositionRelative.y, (uint32)0);
            }
            if ((int)ImGui::GetIO().MouseWheel != 0)
                app.m_spRenderWindow->MouseWheel((int)ImGui::GetIO().MouseWheel);

            // TODO: Still need to support KeyDown along with modifier keys on mouse events
            //    case WM_KEYDOWN:
            //        app.m_spRenderWindow->MapKey((uint32)wParam, (uint32)lParam);
            //        break;
            //for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
            //{
            //    SetEmulatorKeyState(i, io.KeysDown[i]);
            //}
        }
        ImGui::End();
    }
    ImGui::End();
}

void InitializeCaustic(HWND hwnd)
{
    app.m_spSceneFactory = Caustic::CreateSceneFactory();
    app.m_spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    app.m_spRenderWindow = CreateImguiRenderWindow(hwnd, viewport, shaderFolder,
            [](Caustic::IRenderer* pRenderer, ITexture *pFinalRT, ImFont *pFont)
            {
                BuildPanels(pFinalRT, pFont);
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
    if (app.m_spRenderWindow != nullptr)
        app.m_spRenderWindow->RecordEvent(hWnd, message, wParam, lParam);
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
    case WM_SIZE:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
