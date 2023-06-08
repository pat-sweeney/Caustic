//**********************************************
#include "framework.h"
#include "Ditto.h"
#include <string>
#include <chrono>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <map>
#include <any>
#include <iostream>
#include <sstream>
#include <d3d11.h>

import Caustic.Base;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.IRefCount;
import Base.Core.ConvertStr;
import Base.Math.Vector;
import Base.Math.BBox;
import Base.Math.Matrix;
import Base.Math.Distributions;
import Imaging.Image.IImage;
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
import Cameras.WebCamera.WebCamera;
import Cameras.WebCamera.IWebCamera;
import Cameras.VirtualCamera.IVirtualCamera;
import Cameras.VirtualCamera.VirtualCamera;
import Cameras.NDIStream.INDIStream;
import Imaging.Video.IVideo;
import Parsers.Phonemes.IPhonemes;
import Imaging.Image.GPUPipeline;
import Imaging.Image.IGPUPipeline;

using namespace Caustic;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const int c_GridX = 100;
const int c_GridY = 100;

// For a given phoneme, this structure contains the list
// of landmark deltas that form the phoneme
struct PhonemeLandmarkDelta
{
    DWORD m_startFrame;
    DWORD m_endFrame;
    DWORD m_numDeltaFrames; // Number of frames to animate over
    std::vector<std::vector<Vector2>> m_frameDeltas; // list of deltas for each frame
};

class CApp
{
public:
    CApp()
    {
        m_curFrameIndex = -1;
        m_phonemeFrameIndex = -1;
    }
    ~CApp() {}
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<Caustic::ICausticFactory> m_spCausticFactory;
    CRefObj<ITexture> defaultTex;
    CRefObj<ISampler> m_spSampler;
    CRefObj<IImage> m_spLastFrame;
    CRefObj<ITexture> m_spLastTex;
    CRefObj<IPhonemes> m_spPhonemes;
    CRefObj<IImageFilter> m_spLandmarkFilter;
    std::vector<CRefObj<IVideo>> m_videos;
    std::vector<BBox2> m_faceBbox; // Bounding box of the entire face based on face detection (ignoring landmarks)
    std::vector<BBox2> m_faceLandmarksInfluenceBounds; // Bounding box of face landmarks per video frame adjusted to include influence distance
    std::vector<std::vector<Vector2>> m_faceLandmarks; // List of landmark positions per video frame
    int m_curVideoIndex;
    int m_nextVideoIndex;
    int m_curFrameIndex;
    bool m_texLoaded;
    HWND m_hwnd;
    std::chrono::time_point<std::chrono::system_clock> m_prevRenderTime;
    CRefObj<IVirtualCamera> m_spVirtualCam;
    CRefObj<IVideo> m_spVideo;
    CRefObj<INDIStream> m_spNDIStream;
    std::map<std::wstring, PhonemeLandmarkDelta> m_phonemeLandmarkDeltaMap;
    std::unique_ptr<float2> m_spGridLocations;
    std::vector<std::string> m_words;           // List of words in current sentence being played
    int m_wordIndex;                            // Current index into m_words
    std::vector<std::wstring> m_phonemesInCurrentWord;        // List of phonemes in the current word for the sentence being played
    int m_phonemeIndex;                         // Index into m_phonemesInCurrentWord
    bool m_playPhonemes;                        // Are we playing back phonemes?
    int m_phonemeFrameIndex;                    // Index into m_curPhonemeLandmarkDeltas
    PhonemeLandmarkDelta m_curPhonemeLandmarkDeltas;   // List of frames with list of landmark deltas for a given phoneme
    std::unique_ptr<CWarpNode> m_spWarpNode;

    void InitializeCaustic(HWND hWnd);
    void LiveWebCam();
    void BuildUI(ITexture* pFinalRT, ImFont* pFont);
    void FindLandmarks();
    void LoadDeltas();
    void ComputeGridWarp(IImage* pImageToWarp, int frameIndex, std::wstring& phoneme, int phonemeFrameIndex);
    CRefObj<IImage> WarpImage(IRenderer* pRenderer, IImage* pImageToWarp, int frameIndex, std::wstring &phoneme, int phonemeFrameIndex);
    void PlaySentence(const char* pSentence);
    void ProcessNextFrame(IRenderer* pRenderer, IRenderCtx* pCtx);
    void LoadVideos(IRenderer* pRenderer, IRenderCtx* pCtx);
};
CApp app;

struct Vertex
{
    float pos[3];
    float norm[3];
};

bool FileExists(const wchar_t* path)
{
    DWORD dwAttrib = GetFileAttributes(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CApp::FindLandmarks()
{
    if (FileExists(L"d:\\data\\idle_landmarks.bin"))
    {
        HANDLE f = CreateFile(L"d:\\data\\idle_landmarks.bin", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        DWORD dw;
        DWORD bytesRead;
        ReadFile(f, &dw, sizeof(DWORD), &bytesRead, nullptr);
        m_faceLandmarks.resize(dw);
        m_faceLandmarksInfluenceBounds.resize(dw);
        for (int i = 0; i < (int)dw; i++)
        {
            DWORD dw2;
            ReadFile(f, &dw2, sizeof(DWORD), &bytesRead, nullptr);
            m_faceLandmarks[i].resize(dw2);
            ReadFile(f, &m_faceLandmarks[i][0], sizeof(Vector2) * dw2, &bytesRead, nullptr);
            ReadFile(f, &m_faceLandmarksInfluenceBounds[i], sizeof(BBox2), &bytesRead, nullptr);
        }
        CloseHandle(f);
        return;
    }

    // Make the first pass across the stream to find all the face landmarks.
    int frameIndex = 0;
    while (!m_videos[0]->EndOfStream())
    {
        auto spVideoSample = m_videos[0]->NextVideoSample();
        if (spVideoSample != nullptr)
        {
            auto spImage = spVideoSample->GetImage();
            Caustic::ImageFilterParams params;
            params.params.insert(std::make_pair("outputImage", std::any(true)));

            auto marked = m_spLandmarkFilter->Apply(spImage, &params);

            auto bb = std::any_cast<BBox2>(params.params["Face0"]);
            m_faceBbox.push_back(bb);

            int numLandmarks = (int)std::any_cast<size_t>(params.params["Face0_NumLandmarks"]);
            std::vector<Vector2> landmarks;
            BBox2 landmarksBBoxInfluenceAdjusted;
            for (int j = 0; j < numLandmarks; j++)
            {
                char buf[1024];
                sprintf_s(buf, "Face%d_Point%d", 0, j);
                Vector2 pt = std::any_cast<Vector2>(params.params[buf]);
                landmarks.push_back(pt);

                // Adjust pt by maximum influence distance.
                // The resulting bbox is the area which mouth landmarks can effect the warping.
                const float c_LandmarkInfluence = 1.5f * spImage->GetWidth() / c_GridX;
                if (j >= c_FaceLandmark_Mouth_FirstIndex && j <= c_FaceLandmark_Mouth_LastIndex)
                {
                    pt.x += c_LandmarkInfluence / 2.0f;
                    landmarksBBoxInfluenceAdjusted.AddPoint(pt);
                    pt.x -= c_LandmarkInfluence;
                    landmarksBBoxInfluenceAdjusted.AddPoint(pt);
                    pt.x += c_LandmarkInfluence / 2.0f;
                    pt.y += c_LandmarkInfluence / 2.0f;
                    landmarksBBoxInfluenceAdjusted.AddPoint(pt);
                    pt.y -= c_LandmarkInfluence;
                    landmarksBBoxInfluenceAdjusted.AddPoint(pt);
                }
            }
            m_faceLandmarksInfluenceBounds.push_back(landmarksBBoxInfluenceAdjusted);
            m_faceLandmarks.push_back(landmarks);
            frameIndex++;
        }
    }
    HANDLE f = CreateFile(L"d:\\data\\idle_landmarks.bin", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, 0, nullptr);
    DWORD dw = (DWORD)m_faceLandmarks.size();
    DWORD bytesWritten;
    WriteFile(f, &dw, sizeof(DWORD), &bytesWritten, nullptr);
    for (int i = 0; i < (int)dw; i++)
    {
        DWORD dw2 = (DWORD)m_faceLandmarks[i].size();
        WriteFile(f, &dw2, sizeof(DWORD), &bytesWritten, nullptr);
        WriteFile(f, &m_faceLandmarks[i][0], sizeof(Vector2) * dw2, &bytesWritten, nullptr);
        WriteFile(f, &m_faceLandmarksInfluenceBounds[i], sizeof(BBox2), &bytesWritten, nullptr);
    }
    CloseHandle(f);
}

void CApp::LoadDeltas()
{
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(L"d:\\data\\Phoneme_*.bin", &findData);
    if (hFind == INVALID_HANDLE_VALUE)
        return;
    while (true)
    {
        std::wstring fn(std::wstring(L"d:\\data\\") + findData.cFileName);
        HANDLE f = CreateFile(fn.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        if (f != INVALID_HANDLE_VALUE)
        {
            std::wstring phonemeName = std::wstring(findData.cFileName);
            phonemeName = phonemeName.substr(8);
            phonemeName = phonemeName.substr(0, phonemeName.find_first_of('_'));
            DWORD bytesRead;
            PhonemeLandmarkDelta phonemeLandmarkDeltas;
            ReadFile(f, &phonemeLandmarkDeltas.m_startFrame, sizeof(DWORD), &bytesRead, nullptr);
            ReadFile(f, &phonemeLandmarkDeltas.m_endFrame, sizeof(DWORD), &bytesRead, nullptr);
            ReadFile(f, &phonemeLandmarkDeltas.m_numDeltaFrames, sizeof(DWORD), &bytesRead, nullptr);
            phonemeLandmarkDeltas.m_frameDeltas.resize(phonemeLandmarkDeltas.m_numDeltaFrames);
            for (size_t i = 0; i < (size_t)phonemeLandmarkDeltas.m_numDeltaFrames; i++)
            {
                DWORD numDeltas;
                ReadFile(f, &numDeltas, sizeof(DWORD), &bytesRead, nullptr);
                phonemeLandmarkDeltas.m_frameDeltas[i].resize(numDeltas);
                ReadFile(f, &phonemeLandmarkDeltas.m_frameDeltas[i][0], sizeof(Vector2) * numDeltas, &bytesRead, nullptr);
                
            }
            m_phonemeLandmarkDeltaMap.insert(std::make_pair(phonemeName, phonemeLandmarkDeltas));
            CloseHandle(f);
        }
        else
            assert(false); // failed to open file

        if (!FindNextFile(hFind, &findData))
            break;
        DWORD dwError = GetLastError();
        if (dwError == ERROR_NO_MORE_FILES)
            break;
    }
    FindClose(hFind);
}

void CApp::LiveWebCam()
{
    auto videoDevices = IWebCamera::GetAvailableVideoDevices();
    int i = 0;
    for (; i < (int)videoDevices.size(); i++)
        if (videoDevices[i].name.contains(L"LifeCam"))
            break;
    auto audioDevices = IWebCamera::GetAvailableAudioDevices();

    std::wstring audioDeviceName;
    int samplingRate = 0;
    int bitsPerSample = 0;
    int numChannels = 0;

    if (audioDevices.size() > 0)
    {
        int j = 0;
        for (; j < (int)audioDevices.size(); j++)
            if (audioDevices[j].name.contains(L"LifeCam"))
                break;
        audioDeviceName = audioDevices[j].endpointID;
        samplingRate = audioDevices[j].samplingRates[0];
        bitsPerSample = audioDevices[j].bitsPerSample[0];
        numChannels = audioDevices[j].channels[0];
    }
    auto spWebCamera = CreateWebCamera(
        videoDevices[i].symlink.c_str(), -1, -1, 30, audioDeviceName, samplingRate, bitsPerSample, numChannels);
}

ImVec2 BuildMenuBar(ImFont* pFont)
{
    ImVec2 menuSize;
    ImGui::PushFont(pFont);
    if (ImGui::BeginMenuBar())
    {
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

std::vector<std::string> SplitSentence(std::string sentence)
{
    std::vector<std::string> words;
    std::stringstream ss(sentence);
    std::string word;
    while (ss >> word)
        words.push_back(word);
    return words;
}

void CApp::PlaySentence(const char *pSentence)
{
    m_words = SplitSentence(pSentence);
    m_wordIndex = 0;
    std::vector<std::string> wordPhonemes;
    auto word = m_words[m_wordIndex];
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::toupper(c); });
    m_spPhonemes->GetPhonemes(word, wordPhonemes);
    for (auto w : wordPhonemes)
    {
        std::wstring wstr = Caustic::str2wstr(w);
        m_phonemesInCurrentWord.push_back(wstr);
    }
    m_phonemeIndex = 0;
    m_phonemeFrameIndex = 0;
    m_curPhonemeLandmarkDeltas = m_phonemeLandmarkDeltaMap[m_phonemesInCurrentWord[m_phonemeIndex]];
    m_playPhonemes = true;
}

void CApp::BuildUI(ITexture* pFinalRT, ImFont* pFont)
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

    //ImGui::ShowDemoWindow();

    ImGui::Begin("Actions", nullptr);
    if (ImGui::Button("Aardvark"))
    {
        app.PlaySentence("Now is the time for all good men to come to the aid of their party");
    }
    if (ImGui::Button("Listening"))
        app.m_nextVideoIndex = 0;
    if (ImGui::Button("FollowUp"))
        app.m_nextVideoIndex = 1;
    if (ImGui::Button("FollowUpLater"))
        app.m_nextVideoIndex = 2;
    if (ImGui::Button("FollowUpEmail"))
        app.m_nextVideoIndex = 3;
    if (ImGui::Button("ScratchHead"))
        app.m_nextVideoIndex = 4;
    if (ImGui::Button("ScratchFace"))
        app.m_nextVideoIndex = 5;
    ImGui::End();

    ImVec2 menuSize = BuildMenuBar(pFont);

    RECT rect;
    GetClientRect(app.m_hwnd, &rect);

    if (pFinalRT != nullptr)
    {
        ImGui::Begin("FinalRender", nullptr);
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        ImVec2 vSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
        auto x = ImGui::GetCursorPos();
        ImGui::Image((void*)pFinalRT->GetD3DTextureRV(), vSize);
        ImGui::End();
    }
    ImGui::End();
}

//**********************************************************************
class CWarpNode : public CGPUPipelineNodeBase
{
    CRefObj<IMesh> m_spMesh;
    CRefObj<IRenderMesh> m_spRenderMesh;
public:
    CWarpNode(const wchar_t* pName, IRenderer* pRenderer, IShader* pShader, uint32 inputWidth, uint32 inputHeight, DXGI_FORMAT format) :
        CGPUPipelineNodeBase(inputWidth, inputHeight, format)
    {
        SetName(pName);
        SetShader(pShader);

        m_cpuFlags = (D3D11_CPU_ACCESS_FLAG)0;
        m_bindFlags = (D3D11_BIND_FLAG)(D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);

        auto startTime = std::chrono::system_clock::now();

        m_spMesh = Caustic::CreateGrid(c_GridX, c_GridY);
        m_spRenderMesh = pRenderer->ToRenderMesh(m_spMesh, pShader);

        auto endTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = endTime - startTime;
        wchar_t buf[1024];
        swprintf_s(buf, L"CWarpNode time:%lf\n", elapsed_seconds.count() * 1000.0);
        OutputDebugString(buf);

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
    virtual void SetInput(const wchar_t* pName, const wchar_t* pTextureName, const wchar_t* pSamplerName, IGPUPipelineNode* pNode) override { CGPUPipelineNodeBase::SetInput(pName, pTextureName, pSamplerName, pNode); }
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

//**********************************************************************
// Method: WarpImage
// For a given input image (from our idle loop) we will warp that frame
// to match the Nth frame of some phoneme
//
// Parameters:
// pImageToWarp - image (from our idle loop) to warp
// frameIndex - index of this image in our entire video (used to look up
//           the precomputed landmarks
// phoneme - which phoneme are we warping into
// phonemeFrameIndex - index in the list of deltas for the phoneme. i.e. a phoneme
//           is comprised of N frames of video. This is the index into the phonemes
//           list of deltas (read from disk Phoneme_<phoneme>_Deltas.bin)
//**********************************************************************
CRefObj<IImage> CApp::WarpImage(IRenderer* pRenderer, IImage *pImageToWarp, int frameIndex, std::wstring& phoneme, int phonemeFrameIndex)
{
    // Compute the grid with which to warp the image
    ComputeGridWarp(pImageToWarp, m_curFrameIndex, phoneme, phonemeFrameIndex);

    // Next setup our GPU pipeline to warp the image
    CRefObj<IGPUPipeline> spGPUPipeline = Caustic::CreateGPUPipeline(pRenderer);

    auto spSource = spGPUPipeline->CreateSourceNode(L"Source", pImageToWarp, pImageToWarp->GetWidth(), pImageToWarp->GetHeight(),
        DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);

    auto startTime = std::chrono::system_clock::now();

    auto spShader = pRenderer->GetShaderMgr()->FindShader(L"Warp");
    spShader->SetVSParam(L"width", std::any(float(c_GridX)));
    spShader->SetVSParam(L"height", std::any(float(c_GridY)));

    auto spGridPosTex = Caustic::CreateTexture(pRenderer, c_GridX, c_GridY, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT);
    D3D11_MAPPED_SUBRESOURCE ms;
    auto ctx = pRenderer->GetContext();
    CT(ctx->Map(spGridPosTex->GetD3DTexture(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
    BYTE* pDst = reinterpret_cast<BYTE*>(ms.pData);
    BYTE* pSrc = pImageToWarp->GetData();
    for (int y = 0; y < c_GridY; y++)
    {
        memcpy(pDst, pSrc, c_GridX * sizeof(float) * 2);
        pSrc += sizeof(float) * 2 * c_GridX;
        pDst += ms.RowPitch;
    }
    ctx->Unmap(spGridPosTex->GetD3DTexture(), 0);
    
    spShader->SetVSParam(L"posTexture", std::any(spGridPosTex));
    auto spDepthSampler = Caustic::CreateSampler(pRenderer, spGridPosTex);
    spShader->SetVSParam(L"posSampler", std::any(spDepthSampler));
    spWarpNode->SetInput(L"Source", L"sourceTexture1", L"sourceSampler1", spSource);
    spGPUPipeline->AddCustomNode(spWarpNode.get());

    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    wchar_t buf[1024];
    swprintf_s(buf, L"GPUPipeline time:%lf\n", elapsed_seconds.count() * 1000.0);
    OutputDebugString(buf);

    auto spShader2 = pRenderer->GetShaderMgr()->FindShader(L"RawCopy");
    CRefObj<IGPUPipelineSinkNode> spSink = spGPUPipeline->CreateSinkNode(L"Sink", spShader2, pImageToWarp->GetWidth(), pImageToWarp->GetHeight(), DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
    spSink->SetInput(L"Source", L"sourceTexture1", L"sourceSampler1", spWarpNode.release());

    spGPUPipeline->IncrementCurrentEpoch();
    spGPUPipeline->Process(pRenderer, pRenderer->GetRenderCtx());
    CRefObj<IImage> spFinalImage = spSink->GetOutput(spGPUPipeline);


#pragma region("StoreWarpedImage")
////    static bool drawLandmarks = false;
////    if (drawLandmarks)
////    {
////        for (int landmarkIndex = 0; landmarkIndex < (int)m_faceLandmarks[frameIndex].size(); landmarkIndex++)
////        {
////            Matrix3x2 mat;
////            if (frameIndex == phonemeInfo[phonemeIndex].m_startFrame)
////            {
////                mat = Matrix3x2(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
////            }
////            else
////            {
////                mat = Matrix3x2::Align(
////                    faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Bottom],
////                    faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Top],
////                    faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Bottom],
////                    faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Top]);
////
////            }
////
////            Vector2 phonemeLandmark = faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][landmarkIndex];
////            Caustic::uint8 color1[4] = { 255, 0, 0, 255 };
////            spFinalImage->DrawCircle(phonemeLandmark, 3, color1);
////            Vector2 alignedLandmark = faceLandmarks[frameIndex][landmarkIndex];
////            Caustic::uint8 color2[4] = { 0, 255, 0, 255 };
////            spFinalImage->DrawCircle(alignedLandmark, 4, color2);
////            alignedLandmark = alignedLandmark * mat;
////            Caustic::uint8 color3[4] = { 0, 0, 255, 255 };
////            spFinalImage->DrawCircle(alignedLandmark, 5, color3);
////        }
////    }
////
////    static bool drawGrid = false;
////    if (drawGrid)
////    {
////        for (int i = 0; i < c_GridY - 1; i++)
////        {
////            for (int j = 0; j < c_GridX - 1; j++)
////            {
////                uint8 color[4] = { 255, 255, 255, 255 };
////                int index00 = i * c_GridX + j;
////                int index10 = index00 + 1;
////                int index01 = (i + 1) * c_GridX + j;
////                int index11 = index01 + 1;
////                Vector2 v0, v1;
////                v0 = Vector2(m_spGridLocations.get()[index00].x, m_spGridLocations.get()[index00].y);
////                v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
////                v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
////                v1 = Vector2(m_spGridLocations.get()[index10].x, m_spGridLocations.get()[index10].y);
////                v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
////                v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
////                spFinalImage->DrawLine(v0, v1, color);
////
////                v0 = Vector2(m_spGridLocations.get()[index10].x, m_spGridLocations.get()[index10].y);
////                v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
////                v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
////                v1 = Vector2(m_spGridLocations.get()[index11].x, m_spGridLocations.get()[index11].y);
////                v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
////                v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
////                spFinalImage->DrawLine(v0, v1, color);
////
////                v0 = Vector2(m_spGridLocations.get()[index00].x, m_spGridLocations.get()[index00].y);
////                v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
////                v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
////                v1 = Vector2(m_spGridLocations.get()[index11].x, m_spGridLocations.get()[index11].y);
////                v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
////                v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
////                spFinalImage->DrawLine(v0, v1, color);
////
////                v0 = Vector2(m_spGridLocations.get()[index00].x, m_spGridLocations.get()[index00].y);
////                v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
////                v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
////                v1 = Vector2(m_spGridLocations.get()[index01].x, m_spGridLocations.get()[index01].y);
////                v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
////                v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
////                spFinalImage->DrawLine(v0, v1, color);
////
////                v0 = Vector2(m_spGridLocations.get()[index01].x, m_spGridLocations.get()[index01].y);
////                v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
////                v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
////                v1 = Vector2(m_spGridLocations.get()[index11].x, m_spGridLocations.get()[index11].y);
////                v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
////                v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
////                spFinalImage->DrawLine(v0, v1, color);
////            }
////        }
////    }
////
////    static bool storeWarpedResults = false;
////    if (storeWarpedResults)
////    {
////        wchar_t buf[1024];
////        swprintf_s(buf, L"%s\\warped\\%s_warped_%d.png", m_videoPath.c_str(), m_videoName.c_str(), frameIndex);
////        Caustic::StoreImage(buf, spFinalImage);
////    }
////
    return spFinalImage;
}

//**********************************************************************
// Method: ComputeGridWarp
// This method builds the location of the grid locations to form the warped
// image to match the specified phoneme
//
// Parameters:
// pImageToWarp - source frame (from idle loop) to warp
// frameIndex - index of 'pImageToWarp' in the entire video
// phoneme - which phoneme are we trying to reproduce
// phonemeFrameIndex - which frame in the phonemes list of frame deltas are we on
//**********************************************************************
void CApp::ComputeGridWarp(IImage *pImageToWarp, int frameIndex, std::wstring& phoneme, int phonemeFrameIndex)
{
    auto startTime = std::chrono::system_clock::now();

    IRenderer* pRenderer = m_spRenderWindow->GetRenderer();
    GaussianDistribution distribution(2.0f);
    int imageW = pImageToWarp->GetWidth();
    int imageH = pImageToWarp->GetHeight();
    float gridDeltaX = (float)imageW / (float)c_GridX;
    float gridDeltaY = (float)imageH / (float)c_GridY;

    PhonemeLandmarkDelta pdelta = m_phonemeLandmarkDeltaMap[phoneme];
    float2* pGridLocations = m_spGridLocations.get();
    const float c_LandmarkInfluence = 1.5f * pImageToWarp->GetWidth() / c_GridX;
    for (int gridY = 0; gridY < c_GridY; gridY++)
    {
        for (int gridX = 0; gridX < c_GridX; gridX++)
        {
            Vector2 gridPixel((float)gridX * gridDeltaX, (float)gridY * gridDeltaY);

            if (m_faceLandmarksInfluenceBounds[m_curFrameIndex].PointInside(gridPixel))
            {
                // Walk each of our face landmarks and see if it effects the current grid vertex. We will
                // use a gaussian placed on the landmark to determine influence.
                Vector2 newGridPos(0.0f, 0.0f);
                Vector2* pFaceLandmark = &m_faceLandmarks[m_curFrameIndex][c_FaceLandmark_Mouth_FirstIndex];
                for (int landmarkIndex = c_FaceLandmark_Mouth_FirstIndex; landmarkIndex <= c_FaceLandmark_Mouth_LastIndex; landmarkIndex++)
                {
                    // Determine distance from the grid location to the landmark
                    float dx = gridPixel.x - pFaceLandmark->x;
                    float dy = gridPixel.y - pFaceLandmark->y;
                    float dist = sqrtf(dx * dx + dy * dy);
                    dist = std::min<float>(dist, c_LandmarkInfluence) / c_LandmarkInfluence;
                    float weight = distribution.Sample(dist);
                    newGridPos += pdelta.m_frameDeltas[phonemeFrameIndex][landmarkIndex - c_FaceLandmark_Mouth_FirstIndex] * weight;
                    pFaceLandmark++;
                }
                // Compute where the pixel moves to
                int index = gridY * c_GridX + gridX;
                pGridLocations[index].x = std::min<float>((float)imageW - 1.0f, std::max<float>(0.0f, gridPixel.x + newGridPos.x)) / ((float)imageW - 1.0f);
                pGridLocations[index].y = std::min<float>((float)imageH - 1.0f, std::max<float>(0.0f, gridPixel.y + newGridPos.y)) / ((float)imageH - 1.0f);
                pGridLocations[index].x = pGridLocations[index].x * 2.0f - 1.0f;
                pGridLocations[index].y = (1.0f - pGridLocations[index].y) * 2.0f - 1.0f;
            }
            else
            {
                int index = gridY * c_GridX + gridX;
                pGridLocations[index].x = std::min<float>((float)imageW - 1.0f, std::max<float>(0.0f, gridPixel.x)) / ((float)imageW - 1.0f);
                pGridLocations[index].y = std::min<float>((float)imageH - 1.0f, std::max<float>(0.0f, gridPixel.y)) / ((float)imageH - 1.0f);
                pGridLocations[index].x = pGridLocations[index].x * 2.0f - 1.0f;
                pGridLocations[index].y = (1.0f - pGridLocations[index].y) * 2.0f - 1.0f;
            }
        }
    }
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    wchar_t buf[1024];
    swprintf_s(buf, L"ComputeGridWarp time:%lf\n", elapsed_seconds.count() * 1000.0);
    OutputDebugString(buf);
}

void CApp::LoadVideos(IRenderer* pRenderer, IRenderCtx* pCtx)
{
    CRefObj<IShaderMgr> spShaderMgr = pRenderer->GetShaderMgr();

    m_curVideoIndex = 0;
    m_nextVideoIndex = 0;
    CRefObj<IVideo> p0 = CreateVideo(L"d:\\DittoData\\Listening.mp4");
    m_videos.push_back(p0);
    LoadDeltas();
    FindLandmarks();

    CRefObj<IVideo> p1 = CreateVideo(L"d:\\DittoData\\FollowUp.mp4");
    m_videos.push_back(p1);

    CRefObj<IVideo> p2 = CreateVideo(L"d:\\DittoData\\FollowUpLater.mp4");
    m_videos.push_back(p2);

    CRefObj<IVideo> p3 = CreateVideo(L"d:\\DittoData\\FollowUpEmail.mp4");
    m_videos.push_back(p3);

    CRefObj<IVideo> p4 = CreateVideo(L"d:\\DittoData\\ScratchFace.mp4");
    m_videos.push_back(p4);

    CRefObj<IVideo> p5 = CreateVideo(L"d:\\DittoData\\ScratchFace.mp4");
    m_videos.push_back(p5);

    CAudioFormat audioFormat;
    p2->GetAudioFormat(&audioFormat);
    CVideoFormat videoFormat;
    p2->GetVideoFormat(&videoFormat);
    m_spNDIStream->Initialize("test", videoFormat.m_width, videoFormat.m_height, 30, audioFormat.m_samplesPerSec, audioFormat.m_bitsPerSample, audioFormat.m_numChannels);
}

void CApp::ProcessNextFrame(IRenderer* pRenderer, IRenderCtx* pCtx)
{
    if (!m_texLoaded)
    {
        m_texLoaded = true;
        LoadVideos(pRenderer, pCtx);
    }
    if (m_curVideoIndex != m_nextVideoIndex)
    {
        m_videos[m_curVideoIndex]->Restart();
        m_curFrameIndex = -1;
        m_phonemeFrameIndex = -1;
        m_playPhonemes = false;
        m_curVideoIndex = m_nextVideoIndex;
    }
    else if (m_videos[m_curVideoIndex]->EndOfStream())
    {
        m_videos[m_curVideoIndex]->Restart();
        m_curVideoIndex = m_nextVideoIndex = 0;
        m_curFrameIndex = -1;
        m_phonemeFrameIndex = -1;
        m_playPhonemes = false;
    }
    else
    {
        auto curRenderTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = curRenderTime - m_prevRenderTime;
        if (elapsed_seconds.count() * 1000.0f > 20)
        {
            m_prevRenderTime = std::chrono::system_clock::now();
            auto spVideoSample = m_videos[m_curVideoIndex]->NextVideoSample();
            if (spVideoSample != nullptr)
            {
                m_spLastFrame = spVideoSample->GetImage();

                if (m_spWarpNode.get() == nullptr)
                {
                    auto spShader = pRenderer->GetShaderMgr()->FindShader(L"Warp");
                    m_spWarpNode.reset(new CWarpNode(L"Warp", pRenderer, spShader, m_spLastFrame->GetWidth(), m_spLastFrame->GetHeight(),
                            DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM));
                }

                if (m_playPhonemes)
                {
                    m_spLastFrame = WarpImage(pRenderer, m_spLastFrame, m_curFrameIndex, m_phonemesInCurrentWord[m_phonemeIndex], m_phonemeFrameIndex);
                }
                m_curFrameIndex++;

                // Check if we have reached the last frame of the current phoneme.
                // If so, move to the next phoneme
                m_phonemeFrameIndex++;
                if (m_phonemeFrameIndex == m_curPhonemeLandmarkDeltas.m_frameDeltas.size())
                {
                    m_phonemeFrameIndex = 0;
                    m_phonemeIndex++;
                    if (m_phonemeIndex == m_phonemesInCurrentWord.size())
                    {
                        m_phonemeIndex = 0;
                        // We have reached the last phoneme in the current word. Move to the next word
                        m_wordIndex++;
                        if (m_wordIndex == m_words.size())
                        {
                            // If we have reached the end of the sentence turn off phoneme playback
                            m_playPhonemes = false;
                        }
                    }
                    if (m_playPhonemes)
                        m_curPhonemeLandmarkDeltas = m_phonemeLandmarkDeltaMap[m_phonemesInCurrentWord[m_phonemeIndex]];
                }
                //m_spVirtualCam->SendVideoFrame(m_spLastFrame);
                m_spLastTex = m_spCausticFactory->CreateTexture(pRenderer, m_spLastFrame, D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
                m_spSampler = m_spCausticFactory->CreateSampler(pRenderer, m_spLastTex);
                m_spNDIStream->SendImage(m_spLastFrame);
            }
        }
        auto spAudioSample = m_videos[m_curVideoIndex]->NextAudioSample();
        if (spAudioSample != nullptr)
        {
            //m_spVirtualCam->SendAudioFrame(spAudioSample->GetData(), spAudioSample->GetDataSize());
            m_spNDIStream->SendAudioFrame(spAudioSample->GetData(), spAudioSample->GetDataSize());
        }
    }

    if (m_spLastTex != nullptr)
        pRenderer->DrawScreenQuad(0.0f, 0.0f, 1.0f, 1.0f, m_spLastTex, m_spSampler);
}

void CApp::InitializeCaustic(HWND hwnd)
{
    m_spPhonemes = Caustic::CreatePhonemes();
    m_spPhonemes->LoadDatabase();
    Caustic::SystemStartup();
    m_spLandmarkFilter = CreateFaceLandmarksFilter();
    m_spGridLocations.reset(new float2[c_GridX * c_GridY]);
    m_texLoaded = false;
    m_spCausticFactory = Caustic::CreateCausticFactory();

    // Next create our output window
    m_spNDIStream = CreateNDIStream();
    std::wstring shaderFolder(SHADERPATH);
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    m_spVirtualCam = Caustic::CreateVirtualCamera();
    m_spRenderWindow = CreateImguiRenderWindow(hwnd, viewport, shaderFolder,
        [](Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx)
        {
            app.ProcessNextFrame(pRenderer, pCtx);
        },
        [](Caustic::IRenderer* pRenderer, ITexture* pFinalRT, ImFont* pFont)
        {
            app.BuildUI(pFinalRT, pFont);
        });

    // Load our shaders
    CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();
    spRenderer->Unfreeze();
}

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HBITMAP imgbitmap = nullptr;
int imgwidth, imgheight;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


using namespace Caustic;
CRefObj<IWebCamera> spWebCamera;
CRefObj<IImage> spSourceImage;
CRefObj<IImage> spFilteredSourceImage;
CRefObj<IImageFilter> spFilter;

void DrawImage(HDC hdc, HBITMAP img, int w, int h)
{
    if (img != nullptr)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HGDIOBJ oldBitmap = SelectObject(hdcMem, img);
        RECT rect;
        GetClientRect(app.m_hwnd, &rect);
        SetStretchBltMode(hdc, COLORONCOLOR);
        StretchBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, w, h, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteObject(hdcMem);
    }
}

void SetDisplayImage(IImage* pImage, ImageFilterParams* pParams = nullptr)
{
    IImage* pDisplayImage = pImage;
    if (spFilter)
    {
        spFilteredSourceImage = spFilter->Apply(pImage, pParams);
        pDisplayImage = spFilteredSourceImage;
    }
    spSourceImage = pImage;
    imgwidth = pDisplayImage->GetWidth();
    imgheight = pDisplayImage->GetHeight();
    if (imgbitmap != nullptr)
        DeleteObject(imgbitmap);
    imgbitmap = CreateBitmap(imgwidth, imgheight, 1, 32, pDisplayImage->GetData());
    InvalidateRect(app.m_hwnd, nullptr, false);
}

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
    LoadStringW(hInstance, IDC_DITTO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DITTO));

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
        if (spWebCamera != nullptr)
        {
            CRefObj<IImage> spColorImage;
            if (spWebCamera->NextVideoFrame(&spColorImage.p) && spColorImage != nullptr)
            {
                //SetDisplayImage(spColorImage);
                //DrawImage(GetDC(app.m_hwnd), imgbitmap, imgwidth, imgheight);
                //m_spVirtualCam->SendFrame(spColorImage);
            }
            CRefObj<IAudioFrame> spAudioFrame;
            if (spWebCamera->NextAudioFrame(&spAudioFrame))
            {
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DITTO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DITTO);
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

   // Setup our renderer
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
        app.m_spNDIStream->Shutdown();
        Caustic::SystemShutdown();
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
