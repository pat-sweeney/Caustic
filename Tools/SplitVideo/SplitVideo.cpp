// SplitVideo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SplitVideo.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include <string>
#include <any>
#include <algorithm>
#include <memory>

import Caustic.Base;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Base.Math.Matrix;
import Base.Math.Distributions;
import Base.Math.Vector;
import Imaging.Image.ImageFilter;
import Imaging.Image.ImageFilter.FaceLandmarks;
import Geometry.Mesh.IMeshConstructor;
import Geometry.MeshImport;
import Geometry.Mesh.MeshFuncs;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IVideoTexture;
import Rendering.Caustic.ICausticFactory;
import Rendering.RenderWindow.IRenderWindow;
import Imaging.Video.IVideo;
import Parsers.Phonemes.IPhonemes;
import Imaging.Image.GPUPipeline;
import Imaging.Image.IGPUPipeline;

#define MAX_LOADSTRING 100

using namespace Caustic;

struct PhonemeInfo
{
    int m_startFrame;
    int m_endFrame;
};

const int c_GridX = 40;
const int c_GridY = 40;

class CApp
{
public:
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<Caustic::ICausticFactory> m_spCausticFactory;
    HWND m_hwnd;
    void InitializeCaustic(HWND hWnd);
    char VideoPath[1024];
    char VideoName[1024];
    CRefObj<IPhonemes> m_spPhonemes;
    CRefObj<IImageFilter> m_spLandmarkFilter;
    std::wstring m_videoPath;
    std::wstring m_videoName;
    std::wstring m_videoFullPath;
    CRefObj<IVideo> m_spVideo;
    std::vector<std::string> m_phonemes;
    std::vector<std::vector<std::vector<Vector2>>> m_gridUVs;
    bool m_doConvert;

    void Convert(Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx);
    void FindLandmarks(std::vector<BBox2>& faceBbox, std::vector<std::vector<Vector2>>& faceLandmarks);
    void WriteLandmarks(BBox2& bb, std::vector<Vector2>& landmarks, int frameIndex);
    void ComputeWarps(Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx, std::vector<BBox2>& faceBbox, std::vector<std::vector<Vector2>>& faceLandmarks,
        std::vector<PhonemeInfo>& phonemeFrames);
    void DeterminePhonemeInfo(std::vector<std::vector<Vector2>>& faceLandmarks, int numPhonemes, std::vector<PhonemeInfo>& phonemeInfo);
    void ComputeGridWarp(PhonemeInfo& phonemeInfo, std::vector<std::vector<Vector2>>& faceLandmarks, std::vector<Vector2>& landmarkDeltas, int roiX, int roiY, int roiWidth, int roiHeight);
    void WarpAndDumpImage(IRenderer* pRenderer, IRenderCtx* pRenderCtx, std::vector<PhonemeInfo>& phonemeInfo);
};
CApp app;

void CApp::ComputeGridWarp(PhonemeInfo& phonemeInfo, std::vector<std::vector<Vector2>>& faceLandmarks, std::vector<Vector2>& landmarkDeltas, int roiX, int roiY, int roiWidth, int roiHeight)
{
    GaussianDistribution distribution(2.0f);
    float gridDeltaX = (float)roiWidth / (float)c_GridX;
    float gridDeltaY = (float)roiHeight / (float)c_GridY;
    std::vector<std::vector<Vector2>> gridLocations;
    gridLocations.resize(c_GridX);
    for (int i = 0; i < c_GridY; i++)
        gridLocations[i].resize(c_GridY);
    float normScale = std::max<float>((float)roiWidth, (float)roiHeight);
    normScale = sqrtf(2.0f * normScale * normScale);
    for (int gridY = 0; gridY < c_GridY; gridY++)
    {
        for (int gridX = 0; gridX < c_GridX; gridX++)
        {
            Vector2 gridPixel(roiX + (float)gridX * gridDeltaX, roiY + (float)gridY * gridDeltaY);

            // Walk each of our face landmarks and see if it effects the current grid vertex. We will
            // use a gaussian placed on the landmark to determine influence.
            Vector2 newGridPos(gridPixel.x, gridPixel.y);
            for (int landmarkIndex = 0; landmarkIndex < (int)faceLandmarks[phonemeInfo.m_startFrame].size(); landmarkIndex++)
            {
                // Determine distance from the grid location to the landmark
                float dx = gridPixel.x - faceLandmarks[phonemeInfo.m_startFrame][landmarkIndex].x;
                float dy = gridPixel.y - faceLandmarks[phonemeInfo.m_startFrame][landmarkIndex].y;
                float dist = sqrtf(dx * dx + dy * dy) / normScale;
                float weight = distribution.Sample(dist);
                newGridPos += landmarkDeltas[landmarkIndex] * weight;
            }
            gridLocations[gridX][gridY] = newGridPos;
        }
    }
    m_gridUVs.push_back(gridLocations);
}

// Next I need to build an array per phoneme that indicates how the grid
// deforms based on the landmark motion. 
// 
// Here I assume that the phonemes that make up each word are evenly
// spaced across the sound duration. This is clearly incorrect, but
// I don't have another solution.
//
// I also assume that all the landmarks will be centered around
// points 28 and 31 (the bridge of the nose). This is used to normalize
// across the frames.
// I then lay a grid mesh across the image and place a gaussian distribution
// at each landmark to determine what portions of the image should move
// based on the delta of the landmarks across the phoneme.

void CApp::ComputeWarps(Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx,
    std::vector<BBox2>& faceBbox, std::vector<std::vector<Vector2>>& faceLandmarks,
    std::vector<PhonemeInfo>& phonemeInfo)
{
    int numFrames = (int)faceLandmarks.size();
    int phonemeIndex = 0;

    for (int frameIndex = 0; frameIndex < numFrames; frameIndex++)
    {
        if (frameIndex >= phonemeInfo[phonemeIndex].m_endFrame)
        {
            // Move to next phoneme
            phonemeIndex++;
        }

        std::vector<Vector2> landmarkDeltas;
        if (frameIndex == phonemeInfo[phonemeIndex].m_startFrame)
        {
            for (int landmarkIndex = 0; landmarkIndex < (int)faceLandmarks[frameIndex].size(); landmarkIndex++)
            {
                Vector2 landmarkDelta(0.0f, 0.0f);
                landmarkDeltas.push_back(landmarkDelta);
            }
        }
        else
        {
            // Compute a matrix to transform the current set of landmarks to
            // such that the bridge of the nose is aligned with the first
            // frame of this phonemene's landmarks.
            Matrix3x2 mat = Matrix3x2::Align(
                faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Bottom],
                faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Top],
                faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Bottom],
                faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Top]);

            // Compute the deltas of the current frame's landmarks
            // as compared to the first frame in the current phoneme's landmarks
            for (int landmarkIndex = 0; landmarkIndex < (int)faceLandmarks[frameIndex].size(); landmarkIndex++)
            {
                Vector2 alignedLandmark = faceLandmarks[frameIndex][landmarkIndex];
                alignedLandmark = alignedLandmark * mat;
                Vector2 landmarkDelta = alignedLandmark - faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][landmarkIndex];
                landmarkDeltas.push_back(landmarkDelta);
            }
        }
        int roiX = (int)faceBbox[frameIndex].minPt.x;
        int roiY = (int)faceBbox[frameIndex].minPt.y;
        int roiWidth = (int)faceBbox[frameIndex].maxPt.x - (int)faceBbox[frameIndex].minPt.x + 1;
        int roiHeight = (int)faceBbox[frameIndex].maxPt.y - (int)faceBbox[frameIndex].minPt.y + 1;
        ComputeGridWarp(phonemeInfo[phonemeIndex], faceLandmarks, landmarkDeltas, roiX, roiY, roiWidth, roiHeight);

        WarpAndDumpImage(pRenderer, pCtx, phonemeInfo);
    }
}

//**********************************************************************
class CWarpNode : public CGPUPipelineNodeBase
{
    CRefObj<IMesh> m_spMesh;
    CRefObj<IRenderMesh> m_spRenderMesh;
public:
    CWarpNode(const wchar_t* pName, IRenderer* pRenderer, IShader* pShader, uint32 inputWidth, uint32 inputHeight, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM) :
        CGPUPipelineNodeBase(inputWidth, inputHeight, format)
    {
        SetName(pName);
        SetShader(pShader);

        m_cpuFlags = (D3D11_CPU_ACCESS_FLAG)0;
        m_bindFlags = (D3D11_BIND_FLAG)(D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        m_spMesh = Caustic::CreateGrid(c_GridX, c_GridY);
        m_spRenderMesh = pRenderer->ToRenderMesh(m_spMesh, pShader);
        pRenderer->Freeze();
        //pShader->SetVSParam(L"rayTexture", raymap);
        //pShader->SetPSParam(L"rayTexture", raymap);
        pRenderer->Unfreeze();
    }

    //**********************************************************************
    // IRefCount
    //**********************************************************************
    virtual uint32 AddRef() override { return CRefCount::AddRef(); }
    virtual uint32 Release() override { return CRefCount::Release(); }

    //**********************************************************************
    // IGPUPipelineNode
    //**********************************************************************
    virtual void SetName(const wchar_t* pName) override { CGPUPipelineNodeBase::SetName(pName); }
    virtual bool IsEnabled() override { return CGPUPipelineNodeBase::IsEnabled(); }
    virtual void Enable() override { CGPUPipelineNodeBase::Enable(); }
    virtual void Disable() override { CGPUPipelineNodeBase::Disable(); }
    virtual void SetShader(IShader* pShader) override { CGPUPipelineNodeBase::SetShader(pShader); }
    virtual CRefObj<IShader> GetShader() override { return CGPUPipelineNodeBase::GetShader(); }
    virtual CRefObj<IGPUPipelineNode> GetInput(const wchar_t* pName) override { return CGPUPipelineNodeBase::GetInput(pName); }
    virtual void SetInput(const wchar_t* pName, const wchar_t* pSamplerName, IGPUPipelineNode* pNode) override { CGPUPipelineNodeBase::SetInput(pName, pSamplerName, pNode); }
    virtual void SetOutputSize(uint32 width, uint32 height) override { CGPUPipelineNodeBase::SetOutputSize(width, height); }
    virtual uint32 GetOutputWidth() override { return CGPUPipelineNodeBase::GetOutputWidth(); }
    virtual uint32 GetOutputHeight() override { return CGPUPipelineNodeBase::GetOutputHeight(); }
    virtual CRefObj<ITexture> GetOutputTexture(IGPUPipeline* pPipeline) override { return CGPUPipelineNodeBase::GetOutputTexture(pPipeline); }
    virtual void Process(IGPUPipeline* pPipeline, IRenderer* pRenderer, IRenderCtx* pRenderCtx) override
    {
        ProcessInternal(pPipeline, pRenderer, pRenderCtx,
            [&]() {
                std::vector<CRefObj<ILight>> lights;
                m_spRenderMesh->Render(pRenderer, pRenderCtx, m_spShader, nullptr, lights, nullptr);
            });
    }
};

void CApp::WarpAndDumpImage(IRenderer* pRenderer, IRenderCtx* pRenderCtx, std::vector<PhonemeInfo>& phonemeInfo)
{
    CRefObj<IGPUPipeline> spGPUPipeline = Caustic::CreateGPUPipeline(pRenderer);
    m_spVideo->Restart();
    int frameIndex = 0;
    int phonemeIndex = 0;
    while (!m_spVideo->EndOfStream())
    {
        auto spVideoSample = m_spVideo->NextVideoSample();
        if (spVideoSample != nullptr)
        {
            if (phonemeInfo[phonemeIndex].m_startFrame == frameIndex)
            {
                auto spImage = spVideoSample->GetImage();
                CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();

                auto spSource = spGPUPipeline->CreateSourceNode(L"Source", spImage, spImage->GetWidth(), spImage->GetHeight(), DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
                
                auto spShader = spRenderer->GetShaderMgr()->FindShader(L"ScreenQuad");
                std::unique_ptr<CWarpNode> spWarpNode(
                    new CWarpNode(L"Warp", pRenderer, spShader, spImage->GetWidth(), spImage->GetHeight(), DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM));
                spWarpNode->SetInput(L"Source", nullptr, spSource);
                spGPUPipeline->AddCustomNode(spWarpNode.get());

                CRefObj<IGPUPipelineSinkNode> spSink = spGPUPipeline->CreateSinkNode(L"Sink", spShader, spImage->GetWidth(), spImage->GetHeight(), DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
                spSink->SetInput(L"Source", nullptr, spSource);

                spGPUPipeline->IncrementCurrentEpoch();
                spGPUPipeline->Process(pRenderer, pRenderCtx);
                CRefObj<IImage> spFinalImage = spSink->GetOutput(spGPUPipeline);

                wchar_t buf[1024];
                swprintf_s(buf, L"%s\\%s_warped_%d.png", m_videoPath.c_str(), m_videoName.c_str(), frameIndex);
                Caustic::StoreImage(buf, spFinalImage);

                phonemeIndex++;
            }
            frameIndex++;
        }
    }
}

void CApp::WriteLandmarks(BBox2 &bb, std::vector<Vector2> &landmarks, int frameIndex)
{
    wchar_t rectFN[1024];
    swprintf_s(rectFN, L"%s\\%s_%d.txt", m_videoPath.c_str(), m_videoName.c_str(), frameIndex);
    HANDLE f = CreateFile(rectFN, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
    if (f == INVALID_HANDLE_VALUE)
        return;
    char buf[1024];
    sprintf_s(buf, "FaceBBox: %f,%f..%f,%f\n", bb.minPt.x, bb.minPt.y, bb.maxPt.x, bb.maxPt.y);
    DWORD dw;
    WriteFile(f, buf, (DWORD)strlen(buf), &dw, nullptr);
    for (int j = 0; j < (int)landmarks.size(); j++)
    {
        sprintf_s(buf, "Landmark%d: %f,%f\n", j, landmarks[j].x, landmarks[j].y);
        WriteFile(f, buf, (DWORD)strlen(buf), &dw, nullptr);
    }
    CloseHandle(f);
}

void CApp::FindLandmarks(std::vector<BBox2> &faceBbox, std::vector<std::vector<Vector2>> &faceLandmarks)
{
    // Make the first pass across the stream to find all the face landmarks.
    int frameIndex = 0;
    while (!m_spVideo->EndOfStream())
    {
        auto spVideoSample = m_spVideo->NextVideoSample();
        if (spVideoSample != nullptr)
        {
            auto spImage = spVideoSample->GetImage();
            Caustic::ImageFilterParams params;
            params.params.insert(std::make_pair("outputImage", std::any(true)));

            auto marked = m_spLandmarkFilter->Apply(spImage, &params);

            auto bb = std::any_cast<BBox2>(params.params["Face0"]);
            faceBbox.push_back(bb);

            int numLandmarks = (int)std::any_cast<size_t>(params.params["Face0_NumLandmarks"]);
            std::vector<Vector2> landmarks;
            for (int j = 0; j < numLandmarks; j++)
            {
                char buf[1024];
                sprintf_s(buf, "Face%d_Point%d", 0, j);
                Vector2 pt = std::any_cast<Vector2>(params.params[buf]);
                landmarks.push_back(pt);
            }
            faceLandmarks.push_back(landmarks);

          //  WriteLandmarks(faceBbox[0], landmarks, 0);

          //  wchar_t buf[1024];
          //  swprintf_s(buf, L"%s\\%s_marked_%d.png", m_videoPath.c_str(), m_videoName.c_str(), frameIndex);
          //  Caustic::StoreImage(buf, marked);
          //  swprintf_s(buf, L"%s\\%s_orig_%d.png", m_videoPath.c_str(), m_videoName.c_str(), frameIndex);
          //  Caustic::StoreImage(buf, spImage);
            frameIndex++;
        }
    }

}

void CApp::DeterminePhonemeInfo(std::vector<std::vector<Vector2>>& faceLandmarks, int numPhonemes, std::vector<PhonemeInfo> & phonemeInfo)
{
    int curPhoneme = 0;
    int numFramesInVideo = (int)faceLandmarks.size();
    int framesPerPhoneme = numFramesInVideo / numPhonemes;
    int extraFrames = numFramesInVideo - framesPerPhoneme * numPhonemes;
    int lastFrame = 0;
    for (int i = 0; i < numPhonemes; i++)
    {
        PhonemeInfo info;
        info.m_startFrame = lastFrame;
        info.m_endFrame = lastFrame + framesPerPhoneme;
        if (extraFrames > 0)
        {
            info.m_endFrame++;
            lastFrame = info.m_endFrame;
            extraFrames--;
        }
        phonemeInfo.push_back(info);
    }
}

void CApp::Convert(Caustic::IRenderer *pRenderer, Caustic::IRenderCtx* pCtx)
{
    m_spLandmarkFilter = CreateFaceLandmarksFilter();
    m_videoPath = Caustic::str2wstr(VideoPath);
    m_videoName = Caustic::str2wstr(VideoName);
    m_videoFullPath = m_videoPath + L"\\" + m_videoName + L".mp4";
    m_spVideo = CreateVideo(m_videoFullPath.c_str());

    // Get list of phonemes for the current word
    std::string word(VideoName);
    std::transform(word.begin(), word.end(), word.begin(),
        [](unsigned char c) { return std::toupper(c); });
    m_spPhonemes->GetPhonemes(word.c_str(), m_phonemes);

    // Make the first pass across the stream to find all the face
    // landmarks.
    std::vector<BBox2> faceBbox;
    std::vector<std::vector<Vector2>> faceLandmarks;
    FindLandmarks(faceBbox, faceLandmarks);

    std::vector<PhonemeInfo> phonemeInfo;
    DeterminePhonemeInfo(faceLandmarks, (int)m_phonemes.size(), phonemeInfo);

    ComputeWarps(pRenderer, pCtx, faceBbox, faceLandmarks, phonemeInfo);

    // Generate warped images
    //m_spVideo->Restart();
    //while (!m_spVideo->EndOfStream())
    //{
    //    auto spVideoSample = m_spVideo->NextVideoSample();
    //    if (spVideoSample != nullptr)
    //    {
    //        ComputeWarps(spVideoSample->GetImage());
    //    }
    //}
}

void CApp::InitializeCaustic(HWND hwnd)
{
    Caustic::SystemStartup();
    m_spPhonemes = Caustic::CreatePhonemes();
    m_spPhonemes->LoadDatabase();
    m_spCausticFactory = Caustic::CreateCausticFactory();
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    app.m_doConvert = false;
    m_spRenderWindow = CreateImguiRenderWindow(hwnd, viewport, shaderFolder,
        [](Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx)
        {
            if (app.m_doConvert)
            {
                app.Convert(pRenderer, pCtx);
                app.m_doConvert = false;
            }
        },
        [](Caustic::IRenderer* pRenderer, ITexture* pFinalRT, ImFont* pFont)
        {
            ImGui::Begin("ProcessFile");
            ImGui::Text("VideoPath:");
            ImGui::SameLine();
            ImGui::InputText("##VideoPath", app.VideoPath, sizeof(app.VideoPath));
            ImGui::Text("VideoName:");
            ImGui::SameLine();
            ImGui::InputText("##VideoName", app.VideoName, sizeof(app.VideoName));
            if (ImGui::Button("Convert"))
                app.m_doConvert = true;
            ImGui::End();
        });

    // Load our shaders
    CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();
    spRenderer->Unfreeze();
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
    LoadStringW(hInstance, IDC_SPLITVIDEO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLITVIDEO));

    MSG msg;

    // Main message loop:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLITVIDEO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLITVIDEO);
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

   app.m_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!app.m_hwnd)
   {
      return FALSE;
   }

   app.InitializeCaustic(app.m_hwnd);

   ShowWindow(app.m_hwnd, nCmdShow);
   UpdateWindow(app.m_hwnd);

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
    if (app.m_spRenderWindow != nullptr && message != WM_NCDESTROY)
        app.m_spRenderWindow->RecordEvent(hWnd, message, wParam, lParam);
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
    case WM_QUIT:
        Caustic::SystemShutdown();
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
