//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
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
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
import Caustic.Base;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Geometry.Mesh.IMeshConstructor;
import Geometry.MeshImport;
import Geometry.Mesh.Mesh;
import Imaging.Color;
import Parsers.JSon.JSonParser;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.ISpotLight;
import Rendering.Caustic.ILight;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.IRenderer;
import Rendering.GuiControls.Common;
import Rendering.RenderWindow.IRenderWindow;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.SceneGraph.ISceneCubeElem;
import Rendering.SceneGraph.ISceneCameraCollectionElem;
import Rendering.SceneImport.Collada;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneComputeShaderElem;
import Rendering.SceneGraph.ISceneOverlay2DElem;
import Rendering.SceneGraph.ISceneLineElem;
import Rendering.SceneGraph.ISceneCustomRenderElem;
import Rendering.SceneGraph.ISceneCylinderElem;
import Rendering.SceneGraph.ISceneSphereElem;
import Rendering.SceneGraph.ISceneMeshElem;
import Rendering.SceneGraph.ISceneGroupElem;
import Rendering.SceneGraph.ISceneLightCollectionElem;
import Rendering.SceneGraph.ISceneMaterialElem;

#define MAX_LOADSTRING 100

using namespace Caustic;



class CApp
{
public:
    HWND m_hwnd;                                          // HWND bound to renderer
    HINSTANCE m_hInst;
    CRefObj<IRenderer> m_spRenderer;                      // Instance of our renderer
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<ICausticFactory> m_spCausticFactory;
    CRefObj<ISceneFactory> m_spSceneFactory;
    CRefObj<ICamera> m_spDefaultCamera;
    int nodeCounter;
    int selectedNode;
    CRefObj<ISceneElem> m_spSelectedNode;
    std::function<void()> fillInspectorFunc;
    std::string objName;
    std::string objType;

    CApp()
    {
    }

    void InitializeCaustic(HWND hwnd);
};
CApp app;

//**********************************************************************
// Function: FillInspector_Elem
// Populates the inspector window with the base attributes for an ISceneElem
// 
// Parameters:
// pElem - element to use to populate inspector
//**********************************************************************
void FillInspector_Elem(ISceneElem* pElem)
{
    if (ImGui::CollapsingHeader("Base (SceneElem)", ImGuiTreeNodeFlags_None))
    {
        std::wstring wname = pElem->GetName();
        std::string name = Caustic::wstr2str(wname);
        ImGui::Text("Name: "); ImGui::SameLine();
        char buf[1024];
        strcpy_s(buf, name.c_str());
        if (ImGui::InputText("##Name", buf, sizeof(buf)))
        {
            std::string n(buf);
            std::wstring wn = Caustic::str2wstr(n);
            pElem->SetName(wn.c_str());
        }

        uint32_t flags = pElem->GetFlags();
        bool isHidden = (flags & ESceneElemFlags::Hidden) ? true : false;
        bool isSelected = (flags & ESceneElemFlags::Selected) ? true : false;
        bool depthTested = (flags & ESceneElemFlags::DepthTested) ? true : false;
        bool bboxDirty = (flags & ESceneElemFlags::BBoxDirty) ? true : false;
        bool renderableDirty = (flags & ESceneElemFlags::RenderableDirty) ? true : false;
        bool materialDirty = (flags & ESceneElemFlags::MaterialDirty) ? true : false;
        uint32_t finalFlags = 0;
        ImGui::Checkbox("Hidden", &isHidden);
        finalFlags |= (isHidden) ? ESceneElemFlags::Hidden : 0;
        ImGui::Checkbox("Selected", &isSelected);
        finalFlags |= (isSelected) ? ESceneElemFlags::Selected : 0;
        ImGui::Checkbox("DepthTested", &depthTested);
        finalFlags |= (depthTested) ? ESceneElemFlags::DepthTested : 0;
        ImGui::BeginDisabled();
        ImGui::Checkbox("BBoxDirty", &bboxDirty);
        ImGui::Checkbox("RenderableDirty", &renderableDirty);
        ImGui::Checkbox("MaterialDirty", &materialDirty);
        ImGui::EndDisabled();
        pElem->SetFlags(finalFlags);
        BBox3 bbox;
        pElem->GetBBox(&bbox);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::BeginDisabled();
        ImGui_BBox3("BBox:", bbox,
            std::min<float>(bbox.minPt.x, std::min<float>(bbox.minPt.y, bbox.minPt.z)),
            std::max<float>(bbox.maxPt.x, std::max<float>(bbox.maxPt.y, bbox.maxPt.z)));
        ImGui::EndDisabled();
        ImGui::PopStyleColor();
    }
}

void FillInspector_CameraCollection(ISceneCameraCollectionElem* pCameraCollection)
{
    FillInspector_Elem(pCameraCollection);
}

void FillInspector_Camera(ICamera* pCamera, int lightIndex)
{
    static bool f = false;
    if (ImGui::Checkbox("Active", &f))
    {
        app.m_spRenderWindow->SetCamera((f) ? pCamera : app.m_spDefaultCamera.p);
    }

    float fov, aspectRatio, nearZ, farZ;
    pCamera->GetParams(&fov, &aspectRatio, &nearZ, &farZ);

    ImGui::Text("FOV:");
    ImGui::SameLine();
    if (ImGui::SliderFloat("##FOV", &fov, 0.01f, 90.0f))
        pCamera->SetParams(fov, aspectRatio, nearZ, farZ);

    ImGui::Text("Aspect Ratio:");
    ImGui::SameLine();
    if (ImGui::SliderFloat("##AspectRatio", &aspectRatio, 0.01f, 3.0f))
        pCamera->SetParams(fov, aspectRatio, nearZ, farZ);

    ImGui::Text("NearZ:");
    ImGui::SameLine();
    if (ImGui::SliderFloat("##NearZ", &nearZ, 0.01f, 3.0f))
        pCamera->SetParams(fov, aspectRatio, nearZ, farZ);

    ImGui::Text("FarZ:");
    ImGui::SameLine();
    if (ImGui::SliderFloat("##FarZ", &farZ, 0.01f, 3.0f))
        pCamera->SetParams(fov, aspectRatio, nearZ, farZ);

    ImGui_Vector("Position:",
        [pCamera]()->Vector3 {
            Vector3 eye;
            pCamera->GetPosition(&eye, nullptr, nullptr, nullptr, nullptr, nullptr);
            return eye;
        },
        [pCamera](Vector3 v) {
            Vector3 eye, look, up;
            pCamera->GetPosition(&eye, &look, &up, nullptr, nullptr, nullptr);
            pCamera->SetPosition(v, look, up);
        }, 0.0f, 10000.0f);

    ImGui_Vector("Look:",
        [pCamera]()->Vector3 {
            Vector3 look;
            pCamera->GetPosition(nullptr, &look, nullptr, nullptr, nullptr, nullptr);
            return look;
        },
        [pCamera](Vector3 v) {
            Vector3 eye, look, up;
            pCamera->GetPosition(&eye, &look, &up, nullptr, nullptr, nullptr);
            pCamera->SetPosition(eye, v, up);
        }, 0.0f, 10000.0f);

    ImGui_Vector("Up:",
        [pCamera]()->Vector3 {
            Vector3 up;
            pCamera->GetPosition(nullptr, nullptr, &up, nullptr, nullptr, nullptr);
            return up;
        },
        [pCamera](Vector3 v) {
            Vector3 eye, look, up;
            pCamera->GetPosition(&eye, &look, &up, nullptr, nullptr, nullptr);
            pCamera->SetPosition(eye, look, v);
        }, 0.0f, 10000.0f);

    ImGui_Vector("Offset:",
        [pCamera]()->Vector3 {
            Vector3 offset;
            pCamera->GetOffset(offset);
            return offset;
        },
        [pCamera](Vector3 v) {
            pCamera->SetOffset(v);
        }, 0.0f, 10000.0f);
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
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    Vector3 center;
    float width, height, depth;
    pCube->GetPosition(&center, &width, &height, &depth);
    static float maxDim = 1.0f;
    if (width > maxDim * 0.95f)
        maxDim *= 1.02f;
    if (width < maxDim * 0.05f)
        maxDim *= 0.95f;
    if (height > maxDim * 0.95f)
        maxDim *= 1.02f;
    if (height < maxDim * 0.05f)
        maxDim *= 0.95f;
    if (depth > maxDim * 0.95f)
        maxDim *= 1.02f;
    if (depth < maxDim * 0.05f)
        maxDim *= 0.95f;
    bool changed = false;
    ImGui::Text("Width:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##Width").c_str(), &width, 0.01f, maxDim))
        changed = true;
    ImGui::Text("Height:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##Height").c_str(), &height, 0.01f, maxDim))
        changed = true;
    ImGui::Text("Depth:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##Depth").c_str(), &depth, 0.01f, maxDim))
        changed = true;
    ImGui::Text("Center:");
    ImGui::Text("      X:"); ImGui::SameLine();
    static float maxPos = 1.0f;
    if (fabs(center.x) > maxPos * 0.95f) maxPos *= 1.02f;
    if (fabs(center.y) > maxPos * 0.95f) maxPos *= 1.02f;
    if (fabs(center.z) > maxPos * 0.95f) maxPos *= 1.02f;
    if (ImGui::SliderFloat(std::string("##CenterX").c_str(), &center.x, -maxPos, maxPos))
        changed = true;
    ImGui::Text("      Y:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##CenterY").c_str(), &center.y, -maxPos, maxPos))
        changed = true;
    ImGui::Text("      Z:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##CenterZ").c_str(), &center.z, -maxPos, maxPos))
        changed = true;
    if (changed)
    {
        pCube->SetPosition(center, width, height, depth);
    }
    ImGui::PopStyleColor();
}

void FillInspector_Cylinder(ISceneCylinderElem* pCylinder)
{
    FillInspector_Elem(pCylinder);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    Vector3 center;
    float height, topRadius, bottomRadius;
    pCylinder->GetPosition(&center, &height, &topRadius, &bottomRadius);
    static float maxRadius = 1.0f;
    if (topRadius > maxRadius * 0.95f)
        maxRadius *= 1.02f;
    if (topRadius < maxRadius * 0.05f)
        maxRadius *= 0.95f;
    if (bottomRadius > maxRadius * 0.95f)
        maxRadius *= 1.02f;
    if (bottomRadius < maxRadius * 0.05f)
        maxRadius *= 0.95f;
    bool changed = false;
    ImGui::Text("Top Radius:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##TopRadius").c_str(), &topRadius, 0.01f, maxRadius))
        changed = true;
    ImGui::Text("Bottom Radius:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##BottomRadius").c_str(), &bottomRadius, 0.01f, maxRadius))
        changed = true;
    static float maxHeight = 1.0f;
    if (height > maxHeight * 0.95f)
        maxHeight *= 1.02f;
    if (height < maxHeight * 0.05f)
        maxHeight *= 0.95f;
    ImGui::Text("Height:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##Height").c_str(), &height, 0.01f, maxHeight))
        changed = true;
    ImGui::Text("Center:");
    ImGui::Text("      X:"); ImGui::SameLine();
    static float maxPos = 1.0f;
    if (fabs(center.x) > maxPos * 0.95f) maxPos *= 1.02f;
    if (fabs(center.y) > maxPos * 0.95f) maxPos *= 1.02f;
    if (fabs(center.z) > maxPos * 0.95f) maxPos *= 1.02f;
    if (ImGui::SliderFloat(std::string("##CenterX").c_str(), &center.x, -maxPos, maxPos))
        changed = true;
    ImGui::Text("      Y:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##CenterY").c_str(), &center.y, -maxPos, maxPos))
        changed = true;
    ImGui::Text("      Z:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##CenterZ").c_str(), &center.z, -maxPos, maxPos))
        changed = true;
    if (changed)
    {
        pCylinder->SetPosition(center, height, topRadius, bottomRadius);
    }
    ImGui::PopStyleColor();
}

void FillInspector_Sphere(ISceneSphereElem* pSphere)
{
    FillInspector_Elem(pSphere);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImVec4(1.0f,0.0f,0.0f,1.0f));
    ImGui::Text("Radius:"); ImGui::SameLine();
    Vector3 center;
    float radius;
    pSphere->GetPosition(&center, &radius);
    static float maxRadius = 1.0f;
    if (radius > maxRadius * 0.95f)
        maxRadius *= 1.02f;
    if (radius < maxRadius * 0.05f)
        maxRadius *= 0.95f;
    bool changed = false;
    if (ImGui::SliderFloat(std::string("##Radius").c_str(), &radius, 0.01f, maxRadius))
        changed = true;
    ImGui::Text("Center:");
    ImGui::Text("      X:"); ImGui::SameLine();
    static float maxPos = 1.0f;
    if (fabs(center.x) > maxPos * 0.95f) maxPos *= 1.02f;
    if (fabs(center.y) > maxPos * 0.95f) maxPos *= 1.02f;
    if (fabs(center.z) > maxPos * 0.95f) maxPos *= 1.02f;
    if (ImGui::SliderFloat(std::string("##CenterX").c_str(), &center.x, -maxPos, maxPos))
        changed = true;
    ImGui::Text("      Y:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##CenterY").c_str(), &center.y, -maxPos, maxPos))
        changed = true;
    ImGui::Text("      Z:"); ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##CenterZ").c_str(), &center.z, -maxPos, maxPos))
        changed = true;
    if (changed)
    {
        pSphere->SetPosition(center, radius);
    }
    ImGui::PopStyleColor();
}

void FillInspector_Mesh(ISceneMeshElem *pMeshElem)
{
    FillInspector_Elem(pMeshElem);
    auto spMesh = pMeshElem->GetMesh();
    uint32_t numSubmeshes = spMesh->NumberSubMeshes();
    if (ImGui::CollapsingHeader("SubMeshes:", ImGuiTreeNodeFlags_None))
    {
        for (uint32_t i = 0; i < numSubmeshes; i++)
        {
            auto spSubMesh = spMesh->GetSubMesh(i);
            std::string name = spSubMesh->GetName();
            uint32_t numFaces = spSubMesh->GetNumberFaces();
            uint32_t numVerts = spSubMesh->GetNumberVertices();
            uint32_t numEdges = spSubMesh->GetNumberEdges();
            EVertexFlags vertexFlags = spSubMesh->GetVertexFlags();
            EMeshFlags meshFlags = spSubMesh->GetMeshFlags();
            uint32_t materialID = spSubMesh->GetMaterialID();
            BBox3 bbox = spSubMesh->GetBBox();
            bool hasPosition = (vertexFlags & EVertexFlags::HasPosition) ? true : false;
            bool hasNormal = (vertexFlags & EVertexFlags::HasNormal) ? true : false;
            bool hasUV0 = (vertexFlags & EVertexFlags::HasUV0) ? true : false;
            bool hasUV1 = (vertexFlags & EVertexFlags::HasUV1) ? true : false;
            bool hasUV2 = (vertexFlags & EVertexFlags::HasUV2) ? true : false;
            bool hasUV3 = (vertexFlags & EVertexFlags::HasUV3) ? true : false;

            if (ImGui::CollapsingHeader((std::string("SubMesh ") + std::to_string(i) + std::string(": ") + name).c_str(), ImGuiTreeNodeFlags_None))
            {
                ImGui::Text((std::string("  Faces: ") + std::to_string(numFaces)).c_str());
                ImGui::Text((std::string("  Vertices: ") + std::to_string(numVerts)).c_str());
                ImGui::Text((std::string("  Edges: ") + std::to_string(numEdges)).c_str());
                ImGui::BeginDisabled();
                ImGui::Checkbox("Has Position", &hasPosition);
                ImGui::Checkbox("Has Normal", &hasNormal);
                ImGui::Checkbox("Has UV0", &hasUV0);
                ImGui::Checkbox("Has UV1", &hasUV1);
                ImGui::Checkbox("Has UV2", &hasUV2);
                ImGui::Checkbox("Has UV3", &hasUV3);
                ImGui_BBox3("BBox:", bbox,
                    std::min<float>(bbox.minPt.x, std::min<float>(bbox.minPt.y, bbox.minPt.z)),
                    std::max<float>(bbox.maxPt.x, std::max<float>(bbox.maxPt.y, bbox.maxPt.z)));
                ImGui::EndDisabled();
            }
        }
        if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_None))
        {
            uint32_t numMaterials = spMesh->GetNumberMaterials();
            int index = 0;
            for (uint32_t j = 0; j < numMaterials; j++)
            {
                CRefObj<IMaterialAttrib> spMaterial = spMesh->GetMaterial(j);
                if (ImGui::CollapsingHeader((std::string("Material ")+std::to_string(j)).c_str(), ImGuiTreeNodeFlags_None))
                {
                    CRefObj<IMaterialAttrib> spMaterial = spMesh->GetMaterial(j);
                    std::string name = spMaterial->GetName();
                    ImGui::Text((std::string("MaterialID: ") + std::to_string(spMaterial->GetMaterialID())).c_str());
                    ImGui::Text((std::string("Name:") + name).c_str());
                    D3D11_CULL_MODE cullMode = spMaterial->GetCullMode();
                    if (ImGui_CullMode(cullMode))
                        spMaterial->SetCullMode(cullMode);

                    D3D11_FILL_MODE fillMode = spMaterial->GetFillMode();
                    if (ImGui_FillMode(fillMode))
                        spMaterial->SetFillMode(fillMode);
                    bool isTransparent = spMaterial->GetIsTransparent();
                    if (ImGui::Checkbox("Transparent", &isTransparent))
                        spMaterial->SetIsTransparent(isTransparent);
                    bool isShadowReceiver = spMaterial->GetIsShadowReceiver();
                    if (ImGui::Checkbox("Shadow Receiver", &isShadowReceiver))
                        spMaterial->SetIsShadowReceiver(isShadowReceiver);

                    spMaterial->EnumerateColors([&index](const wchar_t* pName, FRGBAColor& v) {
                        std::string name = Caustic::wstr2str(std::wstring(pName));
                        ImGui_Color(name.c_str(), index++, [&v]()->FRGBColor {return FRGBColor(v.r, v.g, v.b); }, [&v](FRGBColor nv) {v = FRGBAColor(nv.r, nv.g, nv.b, 1.0f); });
                        });
                    //    virtual void EnumerateScalars(std::function<void(const wchar_t* pName, float s)> func) = 0;
                    //    virtual void EnumerateTextures(std::function<void(const wchar_t* pName, IImage* pTexture, EShaderAccess access)> func) = 0;
                }
            }
        }
    }
}

void FillInspector_Group(ISceneGroupElem *pGroup)
{
    FillInspector_Elem(pGroup);
    Vector3 scale;
    Vector3 shear;
    Vector3 rotation;
    Vector3 translation;
    Matrix4x4 transform = pGroup->GetTransform();
    transform.Decompose(&scale, &shear, &rotation, &translation);
    ImGui::Text("Transform:");
    BBox3 bb;
    pGroup->GetBBox(&bb);
    float maxv = 1.01f * std::max<float>(std::max<float>(bb.maxPt.x, bb.maxPt.y), bb.maxPt.z);
    float minv = 1.01f * std::max<float>(std::max<float>(bb.minPt.x, bb.minPt.y), bb.minPt.z);
    float maxscale = 1.01f * std::max<float>(std::max<float>(scale.x, scale.y), scale.z);
    bool changed = false;
    if (ImGui_Vector("    Position:",
        [&translation]()->Vector3 { return translation; },
        [&translation](Vector3 v) { translation = v; },
        -maxv, maxv))
        changed = true;
    if (ImGui_Vector("    Rotation:",
        [&rotation]()->Vector3 { return rotation; },
        [&rotation](Vector3 v) { rotation = v; },
        -180.0f, 360.0f))
        changed = true;
    if (ImGui_Vector("    Scale:",
        [&scale]()->Vector3 { return scale; },
        [&scale](Vector3 v) { scale = v; },
        -maxscale, maxscale))
        changed = true;
    if (changed)
    {
        transform = Matrix4x4::ScalingMatrix(scale.x, scale.y, scale.z) *
            Matrix4x4::RotationMatrix(Caustic::DegreesToRadians(rotation.x), Caustic::DegreesToRadians(rotation.y), Caustic::DegreesToRadians(rotation.z)) *
            Matrix4x4::TranslationMatrix(translation.x, translation.y, translation.z);
        pGroup->SetTransform(transform);
    }
}

void FillInspector_LightCollection(ISceneLightCollectionElem* pLightCollection)
{
    FillInspector_Group(static_cast<ISceneGroupElem*>(pLightCollection));
    ImGui::Text((std::string("Number of Lights: ") + std::to_string(pLightCollection->NumberLights())).c_str());
}

void FillInspector_Light(ILight* pLight, int lightIndex)
{
    bool f = pLight->GetOnOff();
    if (ImGui::Checkbox("Enable", &f))
    {
        pLight->SetOnOff(f);
    }
    f = pLight->GetCastsLight();
    if (ImGui::Checkbox("Casts Light", &f))
    {
        pLight->SetCastsLight(f);
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

    BBox3 bbox;
    app.m_spRenderWindow->GetSceneGraph()->GetBBox(&bbox);
    float maxV = std::max<float>(
        std::max<float>(
            std::max<float>(fabs(bbox.maxPt.x), std::max<float>(fabs(bbox.maxPt.y), fabs(bbox.maxPt.z))),
            std::max<float>(fabs(bbox.minPt.x), std::max<float>(fabs(bbox.minPt.y), fabs(bbox.minPt.z)))
            ), 1.0f);

    ImGui_Vector("Position:",
        [pLight]()->Vector3 { return pLight->GetPosition(); },
        [pLight](Vector3 v) { pLight->SetPosition(v); }, 0.0f, (maxV == FLT_MAX) ? 10000.0f : 1.0f);

    if (pLight->GetType() == ELightType::DirectionalLight)
    {
        ImGui_Vector("Direction:",
            [pLight]()->Vector3 { return pLight->GetDirection(); },
            [pLight](Vector3 v) { pLight->SetDirection(v); }, -1.0f, +1.0f);
    }
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
    {
        if (ImGui::CollapsingHeader("Group Info (GroupElem)", ImGuiTreeNodeFlags_None))
            FillInspector_Group(pMaterialElem);
    }

    D3D11_CULL_MODE cullMode = pMaterial->GetCullMode();
    if (ImGui_CullMode(cullMode))
    {
        pMaterial->SetCullMode(cullMode);
    }

    D3D11_FILL_MODE fillMode = pMaterial->GetFillMode();
    if (ImGui_FillMode(fillMode))
    {
        pMaterial->SetFillMode(fillMode);
    }

    bool isTransparent = pMaterial->GetIsTransparent();
    if (ImGui::Checkbox("Transparent", &isTransparent))
        pMaterial->SetIsTransparent(isTransparent);
    bool isShadowReceiver = pMaterial->GetIsShadowReceiver();
    if (ImGui::Checkbox("ShadowReceiver", &isShadowReceiver))
        pMaterial->SetIsShadowReceiver(isShadowReceiver);
    uint32_t materialID = pMaterial->GetMaterialID();
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
    FillInspector_Elem(pSceneGraph);
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
        app.objType = std::string(pDefaultName);
        if (pElem)
            app.objName = Caustic::wstr2str(pElem->GetName());
        app.selectedNode = nodeCounter;
        app.m_spSelectedNode = pElem;
    }
}

void BuildGroupUI(ISceneGraph* pSceneGraph, ISceneGroupElem* pGroup, const char* pDefaultName,
    std::function<void()> onSelect);

void AddGroupNodesCallback(ISceneGraph* pSceneGraph, ISceneGroupElem* pGroup)
{
    uint32_t numChildren = pGroup->NumberChildren();
    for (uint32_t i = 0; i < numChildren; i++)
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
                uint32_t numMats = spMesh->GetNumberMaterials();
                for (uint32_t i = 0; i < numMats; i++)
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
            BuildCollapsableNode(pSceneGraph, spChild, false, "LightCollection",
                [pSceneGraph, spChild]()
                {
                    ISceneLightCollectionElem* pCollection = (ISceneLightCollectionElem*)spChild.p;
                    uint32_t numLights = pCollection->NumberLights();
                    for (uint32_t i = 0; i < numLights; i++)
                    {
                        CRefObj<ILight> spLight = pCollection->GetLight(i);
                        std::string lightName = std::string("Light-") + std::to_string(i);
                        BuildCollapsableNode(pSceneGraph, nullptr, true, lightName.c_str(),
                            nullptr, [spLight, i]() { FillInspector_Light(spLight.p, i); });
                    }
                    AddGroupNodesCallback(pSceneGraph, static_cast<ISceneGroupElem*>(pCollection));
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
        [pSceneGraph, pGroup]()
        {
            AddGroupNodesCallback(pSceneGraph, pGroup);
        }, onSelect);
}

void AddNewElement(CRefObj<ISceneElem> pElem)
{
    bool added = false;
    if (app.m_spSelectedNode != nullptr)
    {
        auto t = app.m_spSelectedNode->GetType();
        if (t == ESceneElemType::Material || t == ESceneElemType::Group || t == ESceneElemType::LightCollection)
        {
            ((ISceneGroupElem*)app.m_spSelectedNode.p)->AddChild(pElem);
            added = true;
        }
    }
    if (!added)
    {
        CRefObj<ISceneGraph> spSceneGraph = app.m_spRenderWindow->GetSceneGraph();
        spSceneGraph->AddChild(pElem);
    }
}

void AddNewLight(CRefObj<ILight> pLight)
{
    bool added = false;
    if (app.m_spSelectedNode != nullptr)
    {
        auto t = app.m_spSelectedNode->GetType();
        if (t == ESceneElemType::LightCollection)
        {
            ((ISceneLightCollectionElem*)app.m_spSelectedNode.p)->AddLight(pLight);
            added = true;
        }
    }
}

ImVec2 BuildMenuBar(ImFont *pFont)
{
    ImVec2 menuSize;
    ImGui::PushFont(pFont);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load Scene..."))
            {
                wchar_t fn[1024] = { 0 };
                OPENFILENAME ofn;
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = app.m_hwnd;
                ofn.hInstance = app.m_hInst;
                ofn.lpstrFilter = L"Collada Files\0*.dae\0All Files\0*.*\0\0\0";
                ofn.lpstrFile = fn;
                ofn.nMaxFile = MAX_PATH;
                ofn.lpstrTitle = L"Open Scene";
                ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
                ofn.lpstrDefExt = L"dae";
                if (GetOpenFileName(&ofn))
                {
                    wchar_t* ext = StrRChrW(fn, nullptr, L'.');
                    if (StrCmpW(ext, L".dae") == 0)
                    {
                        CColladaImporter *importer = new CColladaImporter();
                        CRefObj<ISceneGraph> spSceneGraph;
                        std::string sfn = Caustic::wstr2str(std::wstring(fn));
                        importer->Import(sfn.c_str(), &spSceneGraph);
                        app.m_spRenderWindow->SetSceneGraph(spSceneGraph);
                    }
                }
            }
            else if (ImGui::MenuItem("Load Mesh..."))
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
                    CRefObj<ILight> spLight(app.m_spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f, true));
                    spLightCollectionElem->AddLight(spLight);

                    Vector3 lightDir(-1.0f, -1.0f, -1.0f);
                    spLight = app.m_spCausticFactory->CreateDirectionalLight(lightPos, lightDir, lightColor, 1.0f, true);
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
            if (ImGui::BeginMenu("Primitives"))
            {
                if (ImGui::MenuItem("Sphere"))
                {
                    Vector3 center(0.0f, 0.0f, 0.0f);
                    auto spSphere = app.m_spSceneFactory->CreateSphereElem(center, 1.0f);
                    AddNewElement(spSphere.p);
                }
                if (ImGui::MenuItem("Cylinder"))
                {
                    Vector3 center(0.0f, 0.0f, 0.0f);
                    auto spCylinder = app.m_spSceneFactory->CreateCylinderElem(center, 2.0f, 0.5f, 1.0f);
                    AddNewElement(spCylinder.p);
                }
                if (ImGui::MenuItem("Cube"))
                {
                    Vector3 center(0.0f, 0.0f, 0.0f);
                    auto spCube = app.m_spSceneFactory->CreateCubeElem(center, 1.0f, 1.0f, 1.0f);
                    AddNewElement(spCube.p);
                }
                if (ImGui::MenuItem("Line"))
                {
                    Vector3 p0(0.0f, 0.0f, 0.0f);
                    Vector3 p1(1.0f, 1.0f, 1.0f);
                    auto spLine = app.m_spSceneFactory->CreateLineElem(p0, p1);
                    AddNewElement(spLine.p);
                }
                if (ImGui::MenuItem("Tetrahedron"))
                {
                    auto spMesh = Caustic::CreateTetrahedron();
                    auto spMeshElem = app.m_spSceneFactory->CreateMeshElem();
                    spMeshElem->SetMesh(spMesh);
                    AddNewElement(spMeshElem.p);
                }
                if (ImGui::MenuItem("Grid"))
                {
                    auto spMesh = Caustic::CreateGrid(10, 10);
                    auto spMeshElem = app.m_spSceneFactory->CreateMeshElem();
                    spMeshElem->SetMesh(spMesh);
                    AddNewElement(spMeshElem.p);
                }
                if (ImGui::MenuItem("Plane"))
                {
                    auto spMesh = Caustic::CreateGrid(1, 1);
                    auto spMeshElem = app.m_spSceneFactory->CreateMeshElem();
                    spMeshElem->SetMesh(spMesh);
                    AddNewElement(spMeshElem.p);
                }
                /*
                if (ImGui::MenuItem("ComputeShader"))
                {
                }
                if (ImGui::MenuItem("Overlay2D"))
                {
                }
                if (ImGui::MenuItem("Mesh"))
                {
                }
                if (ImGui::MenuItem("Marching Cubes"))
                {
                }
                if (ImGui::MenuItem("CreateSurfaceRevolution"))
                {
                }
                */
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Grouping"))
            {
                if (ImGui::MenuItem("Material"))
                {
                    AddNewElement(app.m_spSceneFactory->CreateMaterialElem().p);
                }
                if (ImGui::MenuItem("LightCollection"))
                {
                    AddNewElement(app.m_spSceneFactory->CreateLightCollectionElem().p);
                }
                if (ImGui::MenuItem("Group"))
                {
                    AddNewElement(app.m_spSceneFactory->CreateGroupElem().p);
                }
                if (ImGui::MenuItem("LevelOfDetail"))
                {
                    auto spLOD = app.m_spSceneFactory->CreateLevelOfDetailElem();
                    AddNewElement(spLOD.p);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Lighting"))
            {
                bool disabled = true;
                auto t = (app.m_spSelectedNode == nullptr) ? ESceneElemType::Unknown : app.m_spSelectedNode->GetType();
                if (t == ESceneElemType::LightCollection)
                    disabled = false;

                if (disabled)
                {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }
                if (ImGui::MenuItem("Point Light"))
                {
                    Vector3 pos(0.0f, 0.0f, 0.0f);
                    FRGBColor color(1.0f, 1.0f, 1.0f);
                    AddNewLight(app.m_spCausticFactory->CreatePointLight(pos, color, 1.0f, true).p);
                }
                if (ImGui::MenuItem("Spot Light"))
                {
                    Vector3 pos(10.0f, 10.0f, 10.0f);
                    Vector3 dir(0.0f, 0.0f, -1.0f);
                    FRGBColor color(1.0f, 1.0f, 1.0f);
                    AddNewLight(app.m_spCausticFactory->CreateSpotLight(pos, dir, color, 1.0f, 35.0f, 45.0f, true).p);
                }
                if (ImGui::MenuItem("Directional Light"))
                {
                    Vector3 pos(10.0f, 10.0f, 10.0f);
                    Vector3 dir(0.0f, 0.0f, -1.0f);
                    FRGBColor color(1.0f, 1.0f, 1.0f);
                    AddNewLight(app.m_spCausticFactory->CreateDirectionalLight(pos, dir, color, 1.0f, true).p);
                }
                if (ImGui::MenuItem("Area Light"))
                {
                }
                if (disabled)
                {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Camera"))
            {
                CRefObj<ICamera> spCamera = app.m_spCausticFactory->CreateCamera(true);
                app.m_spRenderWindow->GetSceneGraph()->GetCameras()->AddCamera(spCamera);
            }
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
                CRefObj<ILight> spLight(app.m_spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f, true));
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

    ImGui::ShowDemoWindow();

    ImVec2 menuSize = BuildMenuBar(pFont);
    
    RECT rect;
    GetClientRect(app.m_hwnd, &rect);

    auto spSceneGraph = app.m_spRenderWindow->GetSceneGraph();
    ImGui::Begin("Scene");
    app.nodeCounter = 0;
    
    CRefObj<ISceneCameraCollectionElem> spCameraCollection = spSceneGraph->GetCameras();
    ISceneCameraCollectionElem* pCameraCollection = spCameraCollection.p;
    ISceneGraph* pSceneGraph = spSceneGraph.p;
    BuildCollapsableNode(spSceneGraph, spCameraCollection, true, "CameraCollection",
        [pSceneGraph, pCameraCollection]()
        {
            uint32_t numCameras = pCameraCollection->NumberCameras();
            for (uint32_t i = 0; i < numCameras; i++)
            {
                CRefObj<ICamera> spCamera = pCameraCollection->GetCamera(i);
                std::string cameraName = std::string("Camera-") + std::to_string(i);
                BuildCollapsableNode(pSceneGraph, nullptr, true, cameraName.c_str(),
                    nullptr, [spCamera, i]() { FillInspector_Camera(spCamera.p, i); });
            }
        },
        [spCameraCollection]() { FillInspector_CameraCollection((ISceneCameraCollectionElem*)spCameraCollection.p); });
    
    BuildGroupUI(spSceneGraph, spSceneGraph, "SceneGraphRoot", [spSceneGraph]() { FillInspector_SceneGraph(spSceneGraph.p); });
    ImGui::End();

    std::string inspectorTitle = "Inspector";
    if (app.objName.length() > 0)
        inspectorTitle += std::string(" - \"") + app.objName + std::string("\"");
    if (app.objType.length() > 0)
        inspectorTitle += std::string(" (") + app.objType + std::string(")");
    inspectorTitle += "###Inspector";
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
                app.m_spRenderWindow->MouseDown((int)mousePositionRelative.x, (int)mousePositionRelative.y, c_LeftButton, (uint32_t)0);
                wasDown = true;
            }
            else if (wasDown && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                app.m_spRenderWindow->MouseUp((int)mousePositionRelative.x, (int)mousePositionRelative.y, c_LeftButton, (uint32_t)0);
                wasDown = false;
            }
            float dx = lastRel.x - mousePositionRelative.x;
            float dy = lastRel.y - mousePositionRelative.y;
            static ImVec2 lastRel = mousePositionRelative;
            if (wasDown && (!IsZero(dx) || !IsZero(dy)))
            {
                app.m_spRenderWindow->MouseMove((int)mousePositionRelative.x, (int)mousePositionRelative.y, (uint32_t)0);
            }
            if ((int)ImGui::GetIO().MouseWheel != 0)
                app.m_spRenderWindow->MouseWheel((int)ImGui::GetIO().MouseWheel);

            // TODO: Still need to support KeyDown along with modifier keys on mouse events
            //    case WM_KEYDOWN:
            //        app.m_spRenderWindow->MapKey((uint32_t)wParam, (uint32_t)lParam);
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
            [](Caustic::IRenderer*, Caustic::IRenderCtx*) {},
            [](Caustic::IRenderer* pRenderer, ITexture *pFinalRT, ImFont *pFont)
            {
                BuildPanels(pFinalRT, pFont);
            });
    app.m_spRenderer = app.m_spRenderWindow->GetRenderer();
    app.m_spDefaultCamera = app.m_spRenderWindow->GetCamera();
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
   Caustic::SystemStartup();
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
    if (app.m_spRenderWindow != nullptr && message != WM_NCDESTROY)
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
    case WM_QUIT:
        Caustic::SystemShutdown();
        break;
    case WM_SIZE:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
