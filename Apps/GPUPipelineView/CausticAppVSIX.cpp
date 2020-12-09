// CausticAppVSIX.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "wx\wxprec.h"
#include "wx\sizer.h"
#include "wx\sizer.h"
#include "wx\wx.h"
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

class CApp : public wxApp
{
public:
    CRefObj<ISceneLightCollectionElem> m_spLightCollectionElem;
    CRefObj<IRenderWindow> spRenderWindow;
    CRefObj<Caustic::ICausticFactory> spCausticFactory;
    CRefObj<IRenderer> spRenderer;
    CRefObj<IAzureKinect> spCamera;
    CRefObj<IGPUPipeline> spGPUPipeline;
    CRefObj<IGPUPipelineSourceNode> spSourceColorNode;
    CRefObj<IGPUPipelineSourceNode> spSourceDepthNode;
    CRefObj<IGPUPipelineNode> spDepthMeshNode;
    CRefObj<IGPUPipelineNode> spNode;
    CRefObj<ITexture> spOutputTexture;
    CRefObj<IShader> spShader;
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
    
    virtual bool OnInit();
    void InitializeCaustic2(HWND hwnd);
    void Setup3DScene(IRenderWindow* pRenderWindow);
};
CApp app;

class CD3DPanel : public wxPanel
{
public:
    CD3DPanel() {}
    CD3DPanel(wxWindow* parent, wxWindowID winid, const wxPoint& pt, const wxSize& size) : wxPanel(parent, winid, pt, size) {}
    
    void OnMouseDown(wxMouseEvent& event)
    {
        app.spRenderWindow->MouseDown(event.GetX(), event.GetY(), c_LeftButton, (uint32)event.ControlDown() ? MK_CONTROL : 0);
        event.Skip();
    }

    void OnMouseUp(wxMouseEvent& event)
    {
        app.spRenderWindow->MouseUp(event.GetX(), event.GetY(), c_LeftButton, (uint32)event.ControlDown() ? MK_CONTROL : 0);
        event.Skip();
    }

    void OnMouseMove(wxMouseEvent& event)
    {
            app.spRenderWindow->MouseMove(event.GetX(), event.GetY(), (uint32)event.ControlDown() ? MK_CONTROL : 0);
        event.Skip();
    }

    void OnMouseWheel(wxMouseEvent& event)
    {
        app.spRenderWindow->MouseWheel(event.GetWheelDelta());
        event.Skip();
    }

    void OnKeyDown(wxKeyEvent& event)
    {
        app.spRenderWindow->MapKey((uint32)event.GetKeyCode(), (uint32)0);
    }

private:
    wxDECLARE_EVENT_TABLE();
};

class CAppFrame : public wxFrame
{
    wxPanel *m_wpD3DPanel;
    bool initialized;
public:
    CAppFrame();
    ~CAppFrame();
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnWindowShow(wxShowEvent& event);
    
    void OnWindowCreate(wxWindowCreateEvent& event);
    void OnColorChanged(wxCommandEvent& event);
    void OnSliderChange1(wxCommandEvent& event);
    void OnSliderChange2(wxCommandEvent& event);
    void OnSliderChange3(wxCommandEvent& event);
    void OnSliderChange4(wxCommandEvent& event);
private:
    wxDECLARE_EVENT_TABLE();
};
const int DepthSliderID1 = wxID_HIGHEST + 1;
const int DepthSliderID2 = wxID_HIGHEST + 2;
const int DepthSliderID3 = wxID_HIGHEST + 3;
const int DepthSliderID4 = wxID_HIGHEST + 4;
const int ColorCheckedID = wxID_HIGHEST + 5;
wxBEGIN_EVENT_TABLE(CAppFrame, wxFrame)
    EVT_MENU(wxID_EXIT, CAppFrame::OnExit)
    EVT_MENU(wxID_ABOUT, CAppFrame::OnAbout)
    EVT_CHECKBOX(ColorCheckedID, CAppFrame::OnColorChanged)
    EVT_SLIDER(DepthSliderID1, CAppFrame::OnSliderChange1)
    EVT_SLIDER(DepthSliderID2, CAppFrame::OnSliderChange2)
    EVT_SLIDER(DepthSliderID3, CAppFrame::OnSliderChange3)
    EVT_SLIDER(DepthSliderID4, CAppFrame::OnSliderChange4)
    EVT_SHOW(CAppFrame::OnWindowShow)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(CD3DPanel, wxPanel)
//    EVT_MOUSEWHEEL(CD3DPanel::OnMouseWheel)
//    EVT_LEFT_DOWN(CD3DPanel::OnMouseDown)
//    EVT_LEFT_UP(CD3DPanel::OnMouseUp)
//    EVT_MOTION(CD3DPanel::OnMouseMove)
//    EVT_KEY_DOWN(CD3DPanel::OnKeyDown)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(CApp);

bool CApp::OnInit()
{
    CAppFrame *frame = new CAppFrame();
    frame->Show(true);
    return true;
}

CAppFrame::~CAppFrame()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

CAppFrame::CAppFrame() :
    wxFrame(NULL, wxID_ANY, "GPUPipeline", wxDefaultPosition, wxSize(1920, 1080)),
    initialized(false)
{
    std::unique_ptr<wxPanel> spD3DPanel(new CD3DPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1920, 1080)));
    m_wpD3DPanel = spD3DPanel.get();

    std::unique_ptr<wxBoxSizer> spTopSizer(new wxBoxSizer(wxHORIZONTAL));
    std::unique_ptr<wxBoxSizer> spSliderGroup(new wxBoxSizer(wxVERTICAL));

    std::unique_ptr<wxBoxSizer> spCheckBoxSizer(new wxBoxSizer(wxHORIZONTAL));
    spCheckBoxSizer->Add(new wxCheckBox(this, ColorCheckedID, "Color"));
    spSliderGroup->Add(spCheckBoxSizer.release(), 0, wxALIGN_LEFT);

    std::unique_ptr<wxBoxSizer> spSliderSizer1(new wxBoxSizer(wxHORIZONTAL));
    spSliderSizer1->Add(new wxStaticText(this, wxID_ANY, "Depth 1:", wxDefaultPosition, wxSize(100, 60)));
    spSliderSizer1->Add(new wxSlider(this, DepthSliderID1, 1, 1, 100, wxDefaultPosition, wxSize(200, 20)));
    spSliderGroup->Add(spSliderSizer1.release(), 0, wxALIGN_LEFT);

    std::unique_ptr<wxBoxSizer> spSliderSizer2(new wxBoxSizer(wxHORIZONTAL));
    spSliderSizer2->Add(new wxStaticText(this, wxID_ANY, "Depth 2:", wxDefaultPosition, wxSize(100, 60)));
    spSliderSizer2->Add(new wxSlider(this, DepthSliderID2, 1, 1, 100, wxDefaultPosition, wxSize(200, 20)));
    spSliderGroup->Add(spSliderSizer2.release(), 0, wxALIGN_LEFT);

    std::unique_ptr<wxBoxSizer> spSliderSizer3(new wxBoxSizer(wxHORIZONTAL));
    spSliderSizer3->Add(new wxStaticText(this, wxID_ANY, "Depth 3:", wxDefaultPosition, wxSize(100, 60)));
    spSliderSizer3->Add(new wxSlider(this, DepthSliderID3, 1, 1, 100, wxDefaultPosition, wxSize(200, 20)));
    spSliderGroup->Add(spSliderSizer3.release(), 0, wxALIGN_LEFT);

    std::unique_ptr<wxBoxSizer> spSliderSizer4(new wxBoxSizer(wxHORIZONTAL));
    spSliderSizer4->Add(new wxStaticText(this, wxID_ANY, "Depth 4:", wxDefaultPosition, wxSize(100, 60)));
    spSliderSizer4->Add(new wxSlider(this, DepthSliderID4, 1, 1, 100, wxDefaultPosition, wxSize(200, 20)));
    spSliderGroup->Add(spSliderSizer4.release(), 0, wxALIGN_LEFT);

    spTopSizer->Add(spSliderGroup.release(), 0, wxALIGN_TOP);

    spD3DPanel->SetBackgroundColour(wxColor(0xff77ffff));
    spTopSizer->Add(spD3DPanel.release(), 1, wxSHAPED | wxALL | wxALIGN_CENTER, 2);
    SetSizer(spTopSizer.release());

    wxMenu *menuFile = new wxMenu;
    //menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
    //                 "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}

void CAppFrame::OnColorChanged(wxCommandEvent& event)
{
    app.colored = event.GetInt() ? true : false;
}

void CAppFrame::OnSliderChange1(wxCommandEvent& event)
{
    app.depth[0] = (float)event.GetInt() / 100.0f;
}

void CAppFrame::OnSliderChange2(wxCommandEvent& event)
{
    app.depth[1] = (float)event.GetInt() / 100.0f;
}

void CAppFrame::OnSliderChange3(wxCommandEvent& event)
{
    app.depth[2] = (float)event.GetInt() / 100.0f;
}

void CAppFrame::OnSliderChange4(wxCommandEvent& event)
{
    app.depth[3] = (float)event.GetInt() / 100.0f;
}

static WNDPROC s_wxWndProc;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK ImGuiOverride(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
    return CallWindowProc(s_wxWndProc, hwnd, uMsg, wParam, lParam);
}

void CAppFrame::OnWindowShow(wxShowEvent& event)
{
    if (!initialized)
    {
        s_wxWndProc = (WNDPROC)SetWindowLongPtr(m_wpD3DPanel->GetHWND(), GWLP_WNDPROC, (LONG_PTR)ImGuiOverride);
        app.InitializeCaustic2(m_wpD3DPanel->GetHWND());
        auto ctx = ImGui::CreateContext();
        ImGui_ImplWin32_Init(m_wpD3DPanel->GetHWND());
        ImGui_ImplDX11_Init(app.spRenderer->GetDevice(), app.spRenderer->GetContext());
        initialized = true;
    }
}

void CAppFrame::OnWindowCreate(wxWindowCreateEvent& event)
{
}

void CAppFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void CAppFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

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

void CApp::InitializeCaustic2(HWND hwnd)
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
            static float v = 0.0f;
            static bool open = true;
            ImGui::ShowAboutWindow(&open);
            ImGui::SliderFloat("Depth #1:", &v, 0.0f, 1.0f, "%f", 1.0f);
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
                if (app.spCamera->BodyTrackingOn() && app.spCamera->NumberBodiesDetected() >= 1)
                {
                    auto mat = app.spCamera->GetJoint(0, AzureKinect::Joints::HandLeft);
                    app.m_spLightCollectionElem->SetTransform(mat);
                }
                if (app.spSourceColorNode && app.spLastColorImage && app.spLastDepthImage)
                {
                    std::any d1(app.depth[0]);
                    std::any d2(app.depth[1]);
                    std::any d3(app.depth[2]);
                    std::any d4(app.depth[3]);
                    app.spDepthSelShader->SetPSParam(L"depth1", d1);
                    app.spDepthSelShader->SetPSParam(L"depth2", d2);
                    app.spDepthSelShader->SetPSParam(L"depth3", d3);
                    app.spDepthSelShader->SetPSParam(L"depth4", d4);
                    std::any f((app.colored)?1.0f:0.0f);
                    app.spDepthSelShader->SetPSParam(L"colored", f);
                    app.spSourceColorNode->SetSource(app.spGPUPipeline, app.spLastColorImage);
                    app.spSourceDepthNode->SetSource(app.spGPUPipeline, app.spLastDepthImage);
                    app.spNode->Process(app.spGPUPipeline);
                    auto spTexture = app.spNode->GetOutputTexture(app.spGPUPipeline);
                    if (spTexture)
                        app.spRenderer->DrawScreenQuad(0.0f, 0.0f, 1.0f, 1.0f, spTexture, nullptr);

                    app.spRenderer->ClearDepth();
                }
            }
            ImGui::Text("Testing");
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        });
    app.spRenderer = app.spRenderWindow->GetRenderer();

    app.spRenderer->Freeze();

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
    //    +-------------+      +--------------+   +--> |          |
    //    | Color Image | -+-> | BlurredColor | -----> | DepthSel |
    //    +-------------+  |   +--------------+   +--> |          |
    //                     |                      |    +----------+
    //                     +----------------------+
    // We have a color+depth image in. We then convert the depth to match
    // the color image dimensions. We then select between a blurred version
    // of the color image and pristine color image based on the depth.
    //**********************************************************************

    //**********************************************************************
    // Create source nodes for color/depth images from camera
    //**********************************************************************
    app.spSourceColorNode = app.spGPUPipeline->CreateSourceNode(nullptr, 1920, 1080, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spSourceDepthNode = app.spGPUPipeline->CreateSourceNode(nullptr, 1024, 1024, DXGI_FORMAT_R16_UINT);

    //**********************************************************************
    // Create node for blurred texture
    //**********************************************************************
    app.spShader = app.spRenderer->GetShaderMgr()->FindShader(L"RawCopy");

    auto spDownSample1 = app.spGPUPipeline->CreateNode(app.spShader, 1920 / 2, 1080 / 2, DXGI_FORMAT_B8G8R8A8_UNORM);
    spDownSample1->SetInput(L"sourceTexture1", L"sourceSampler1", app.spSourceColorNode);

    auto spDownSample2 = app.spGPUPipeline->CreateNode(app.spShader, 1920 / 4, 1080 / 4, DXGI_FORMAT_B8G8R8A8_UNORM);
    spDownSample2->SetInput(L"sourceTexture1", L"sourceSampler1", app.spSourceColorNode);

    auto spDownSample3 = app.spGPUPipeline->CreateNode(app.spShader, 1920 / 16, 1080 / 16, DXGI_FORMAT_B8G8R8A8_UNORM);
    spDownSample3->SetInput(L"sourceTexture1", L"sourceSampler1", app.spSourceColorNode);

    auto spDownSample4 = app.spGPUPipeline->CreateNode(app.spShader, 1920 / 32, 1080 / 32, DXGI_FORMAT_B8G8R8A8_UNORM);
    spDownSample4->SetInput(L"sourceTexture1", L"sourceSampler1", app.spSourceColorNode);

    auto spDownSample5 = app.spGPUPipeline->CreateNode(app.spShader, 1920 / 128, 1080 / 128, DXGI_FORMAT_B8G8R8A8_UNORM);
    spDownSample5->SetInput(L"sourceTexture1", L"sourceSampler1", app.spSourceColorNode);

    //**********************************************************************
    // Create texture that maps depth pixels into rays
    //**********************************************************************
    app.spRayMap = app.spCamera->BuildRayMap(1024, 1024);
    app.spRayTex = Caustic::CreateTexture(app.spRenderer, 1024, 1024, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
    app.spRayTex->CopyFromImage(app.spRenderer, app.spRayMap);

    //**********************************************************************
    // Create node that generates depth texture matching color texture
    //**********************************************************************
    auto extrinsics = app.spCamera->ColorExtrinsics();
    auto intrinsics = app.spCamera->GetAzureColorIntrinsics();
    app.spDepthMeshNode = app.spGPUPipeline->CreatePredefinedNode(c_CustomNode_DepthMeshNode, 1024, 1024, 1920, 1080,
        app.spRayTex, extrinsics, intrinsics, 10.0f, 8000.0f,
        DXGI_FORMAT_R32_FLOAT);
    app.spDepthMeshNode->SetInput(L"depthTex", nullptr, app.spSourceDepthNode);

//    app.spBokehNode = app.spGPUPipeline->CreateNode(app.spDepthSelShader, 1920, 1080, DXGI_FORMAT_B8G8R8A8_UNORM);

    //**********************************************************************
    // Create node for selecting between blurred and pristine color image
    //**********************************************************************
    app.spDepthSelShader = app.spRenderer->GetShaderMgr()->FindShader(L"DepthSel2");
    app.spNode = app.spGPUPipeline->CreateNode(app.spDepthSelShader, 1920, 1080, DXGI_FORMAT_B8G8R8A8_UNORM);
    app.spNode->SetInput(L"colorTex", L"colorSampler", app.spSourceColorNode);
    app.spNode->SetInput(L"blurTex1", L"colorSampler", spDownSample1);
    app.spNode->SetInput(L"blurTex2", L"colorSampler", spDownSample2);
    app.spNode->SetInput(L"blurTex3", L"colorSampler", spDownSample3);
    app.spNode->SetInput(L"blurTex4", L"colorSampler", spDownSample4);
    app.spNode->SetInput(L"blurTex5", L"colorSampler", spDownSample5);
    app.spNode->SetInput(L"depthTex", nullptr, app.spDepthMeshNode);
    app.spRenderer->Unfreeze();
}

#if 0
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
#endif
