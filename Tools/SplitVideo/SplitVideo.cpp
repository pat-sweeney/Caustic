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
#include <d3d11.h>
#include <DXGItype.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <DXProgrammableCapture.h>
#include <shlwapi.h>
#include <vector>

import Caustic.Base;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.Error;
import Base.Core.ConvertStr;
import Base.Math.Matrix;
import Base.Math.Distributions;
import Base.Math.Vector;
import Imaging.Image.ImageFilter;
import Imaging.Image.ImageFilter.FaceLandmarks;
import Geometry.Mesh.IMesh;
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
import Parsers.JSon.IJSonParser;
import Parsers.JSon.JSonParser;

#define MAX_LOADSTRING 100

using namespace Caustic;

struct PhonemeInfo
{
    int m_startFrame;
    int m_endFrame;
};

const int c_GridX = 100;
const int c_GridY = 100;

struct CTimings
{
    std::string m_filename;
    std::string m_word;
    std::vector<int> m_timings;
    std::vector<std::string> m_phonemes;
};

class CApp
{
public:
    CRefObj<IRenderWindow> m_spRenderWindow;
    CRefObj<Caustic::ICausticFactory> m_spCausticFactory;
    HWND m_hwnd;
    void InitializeCaustic(HWND hWnd);
    char VideoPath[1024];
    char VideoFile[1024];
    CRefObj<IPhonemes> m_spPhonemes;
    CRefObj<IImageFilter> m_spLandmarkFilter;
    std::wstring m_videoPath;
    std::wstring m_videoName;
    std::wstring m_videoFullPath;
    CRefObj<IVideo> m_spVideo;
    std::vector<std::string> m_phonemes;
    std::unique_ptr<float2> m_spGridLocations;
    bool m_doConvert;
    std::map<std::string, CTimings*> m_timings;

    void Convert(Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx);
    void FindLandmarks(std::vector<BBox2>& faceBbox, std::vector<std::vector<Vector2>>& faceLandmarks);
    void WriteLandmarks(BBox2& bb, std::vector<Vector2>& landmarks, int frameIndex);
    void ComputeWarps(Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx, std::vector<BBox2>& faceBbox, std::vector<std::vector<Vector2>>& faceLandmarks,
        std::vector<PhonemeInfo>& phonemeFrames);
    void DeterminePhonemeInfo(std::vector<int> timings, std::vector<std::vector<Vector2>>& faceLandmarks, int numPhonemes, std::vector<PhonemeInfo>& phonemeInfo);
    void ComputeGridWarp(IRenderer* pRenderer, int frameIndex, int phonemeIndex, PhonemeInfo& phonemeInfo, std::vector<std::vector<Vector2>>& faceLandmarks,
        std::vector<Vector2>& landmarkDeltas, int roiX, int roiY, int roiWidth, int roiHeight, int imageWidth, int imageHeight);
    void WritePhonemeDeltas(int phonemeIndex, std::vector<PhonemeInfo>& phonemeInfo, std::vector<std::vector<Vector2>>& phonemeLandmarkDeltas);
    void WritePhonemeAudio(int phonemeIndex, std::vector<PhonemeInfo>& phonemeInfo, std::vector<uint8>& phonemeAudio);
    void LoadPhonemeTimings(std::wstring& fn);
};
CApp app;

void CApp::ComputeGridWarp(IRenderer *pRenderer, int frameIndex, int phonemeIndex, PhonemeInfo& phonemeInfo,
    std::vector<std::vector<Vector2>>& faceLandmarks, std::vector<Vector2>& landmarkDeltas,
    int roiX, int roiY, int roiWidth, int roiHeight, int imageWidth, int imageHeight)
{
    GaussianDistribution distribution(2.0f);
    float gridDeltaX = (float)imageWidth / (float)c_GridX;
    float gridDeltaY = (float)imageHeight / (float)c_GridY;

    float2 *pGridLocations = new float2[c_GridX * c_GridY];
    m_spGridLocations.reset(pGridLocations);
    for (int gridY = 0; gridY < c_GridY; gridY++)
    {
        for (int gridX = 0; gridX < c_GridX; gridX++)
        {
            Vector2 gridPixel((float)gridX * gridDeltaX, (float)gridY * gridDeltaY);

            // Walk each of our face landmarks and see if it effects the current grid vertex. We will
            // use a gaussian placed on the landmark to determine influence.
            Vector2 newGridPos(0.0f, 0.0f);
            for (int landmarkIndex = c_FaceLandmark_Mouth_FirstIndex; landmarkIndex <= c_FaceLandmark_Mouth_LastIndex; landmarkIndex++)
            {
                // Determine distance from the grid location to the landmark
                float dx = gridPixel.x - faceLandmarks[phonemeInfo.m_startFrame][landmarkIndex].x;
                float dy = gridPixel.y - faceLandmarks[phonemeInfo.m_startFrame][landmarkIndex].y;
                float dist = sqrtf(dx * dx + dy * dy);
                const float c_LandmarkInfluence = 1.5f * imageWidth / c_GridX; // Number of pixels away influenced by a landmark
                dist = std::min<float>(dist, c_LandmarkInfluence) / c_LandmarkInfluence;
                float weight = distribution.Sample(dist);
                newGridPos += landmarkDeltas[landmarkIndex - c_FaceLandmark_Mouth_FirstIndex] * weight;
            }
            // Compute where the pixel moves to
            int index = gridY * c_GridX + gridX;
            pGridLocations[index].x =
                std::min<float>((float)imageWidth - 1.0f,
                    std::max<float>(0.0f,
                        gridPixel.x + newGridPos.x)) / ((float)imageWidth - 1.0f);
            pGridLocations[index].y = 
                std::min<float>((float)imageHeight - 1.0f, 
                    std::max<float>(0.0f,
                        gridPixel.y + newGridPos.y)) / ((float)imageHeight - 1.0f);
            pGridLocations[index].x = pGridLocations[index].x * 2.0f - 1.0f;
            pGridLocations[index].y = (1.0f - pGridLocations[index].y) * 2.0f - 1.0f;
        }
    }
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

//**********************************************************************
class CWarpNode : public CGPUPipelineNodeBase
{
    CRefObj<IMesh> m_spMesh;
    CRefObj<IRenderMesh> m_spRenderMesh;
public:
    CWarpNode(const wchar_t* pName, IRenderer* pRenderer, IShader* pShader, uint32 inputWidth, uint32 inputHeight, DXGI_FORMAT format, float2* pGridLocations) :
        CGPUPipelineNodeBase(inputWidth, inputHeight, format)
    {
        SetName(pName);
        SetShader(pShader);

        m_cpuFlags = (D3D11_CPU_ACCESS_FLAG)0;
        m_bindFlags = (D3D11_BIND_FLAG)(D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        m_spMesh = Caustic::CreateWarpedGrid(c_GridX, c_GridY, pGridLocations);
        m_spRenderMesh = pRenderer->ToRenderMesh(m_spMesh, pShader);
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

bool FileExists(const wchar_t* path)
{
    DWORD dwAttrib = GetFileAttributes(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CApp::LoadPhonemeTimings(std::wstring& fn)
{
    if (!FileExists(fn.c_str()))
        return;
    CRefObj<IJSonParser> spParser = Caustic::CreateJSonParser();
    CRefObj<IJSonObj> spDOM = spParser->LoadDOM(fn);
    Caustic::JSonEnumerator iter(spDOM);
    CRefObj<IJSonObj> spCurObj = iter.CurrentObj();
    while (spCurObj != nullptr)
    {
        CTimings* pTiming = new CTimings();
        pTiming->m_word = spCurObj->GetName();
        pTiming->m_filename = spCurObj->FindValue_String("filename");
        pTiming->m_timings = spCurObj->FindValue_IntArray("timings");
        pTiming->m_phonemes = spCurObj->FindValue_StringArray("phonemes");
        m_timings.insert(std::make_pair(pTiming->m_word, pTiming));
        spCurObj = iter.NextObj();
    }
    return;
}

void CApp::WritePhonemeDeltas(int phonemeIndex, std::vector<PhonemeInfo>& phonemeInfo, std::vector<std::vector<Vector2>>& phonemeLandmarkDeltas)
{
    // Write the landmark deltas to the database (which for now is just a file)
    wchar_t buf[1024];
    std::wstring phonemeName = Caustic::str2wstr(m_phonemes[phonemeIndex]);
    swprintf_s(buf, L"%s\\Phoneme_%Ls_Deltas.bin", m_videoPath.c_str(), phonemeName.c_str());
    if (!FileExists(buf))
    {
        HANDLE hLandmarkFile = CreateFile(buf, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, 0, nullptr);
        if (hLandmarkFile != INVALID_HANDLE_VALUE)
        {
            DWORD dwBytesWritten;
            WriteFile(hLandmarkFile, &phonemeInfo[phonemeIndex].m_startFrame, sizeof(DWORD), &dwBytesWritten, nullptr);
            WriteFile(hLandmarkFile, &phonemeInfo[phonemeIndex].m_endFrame, sizeof(DWORD), &dwBytesWritten, nullptr);
            DWORD numDeltaFrames = (DWORD)phonemeLandmarkDeltas.size();
            assert(numDeltaFrames == phonemeInfo[phonemeIndex].m_endFrame - phonemeInfo[phonemeIndex].m_startFrame);
            WriteFile(hLandmarkFile, &numDeltaFrames, sizeof(DWORD), &dwBytesWritten, nullptr);
            for (size_t i = 0; i < (size_t)numDeltaFrames; i++)
            {
                DWORD numDeltas = (DWORD)phonemeLandmarkDeltas[i].size();
                WriteFile(hLandmarkFile, &numDeltas, sizeof(DWORD), &dwBytesWritten, nullptr);
                WriteFile(hLandmarkFile, &phonemeLandmarkDeltas[i][0], (DWORD)sizeof(Vector2) * numDeltas, &dwBytesWritten, nullptr);
            }
            CloseHandle(hLandmarkFile);
        }
    }
    phonemeLandmarkDeltas.clear();
}

void CApp::WritePhonemeAudio(int phonemeIndex, std::vector<PhonemeInfo>& phonemeInfo, std::vector<uint8>& phonemeAudio)
{
    // Write the landmark deltas to the database (which for now is just a file)
    wchar_t buf[1024];
    std::wstring phonemeName = Caustic::str2wstr(m_phonemes[phonemeIndex]);
    swprintf_s(buf, L"%s\\Audio_Phoneme_%Ls.bin", m_videoPath.c_str(), phonemeName.c_str());
    if (!FileExists(buf))
    {
        HANDLE hAudioFile = CreateFile(buf, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, 0, nullptr);
        if (hAudioFile != INVALID_HANDLE_VALUE)
        {
            DWORD dwBytesWritten;
            DWORD numAudioBytes = (DWORD)phonemeAudio.size();
            WriteFile(hAudioFile, &numAudioBytes, sizeof(DWORD), &dwBytesWritten, nullptr);
            WriteFile(hAudioFile, &phonemeAudio[0], numAudioBytes, &dwBytesWritten, nullptr);
            CloseHandle(hAudioFile);
        }
    }
}

void CApp::ComputeWarps(Caustic::IRenderer* pRenderer, Caustic::IRenderCtx* pCtx,
    std::vector<BBox2>& faceBbox, std::vector<std::vector<Vector2>>& faceLandmarks,
    std::vector<PhonemeInfo>& phonemeInfo)
{
    int numFrames = (int)faceLandmarks.size();
    int phonemeIndex = 0;
    m_spVideo->Restart();
    std::vector<std::vector<Vector2>> phonemeLandmarkDeltas;
    std::vector<uint8> curPhonemeAudio;
    std::vector<std::vector<uint8>> phonemeSounds;
    CRefObj<IImage> spImageToWarp;
    uint32 curtime = 0;
    int frameIndex = 0;
    CAudioFormat audioFormat;
    m_spVideo->GetAudioFormat(&audioFormat);
    bool nextVideoFrame = false;
    while (frameIndex < numFrames)
    //for (int frameIndex = 0; frameIndex < numFrames; frameIndex++)
    {
        // Read next block of audio.
        auto spAudioSample = m_spVideo->NextAudioSample();
        if (spAudioSample != nullptr)
        {
            uint8* pAudioData = spAudioSample->GetData();
            uint32 audioLen = spAudioSample->GetDataSize();
            uint32 startIndex = (uint32)curPhonemeAudio.size();
            curPhonemeAudio.resize(startIndex + audioLen);
            memcpy(&curPhonemeAudio[startIndex], pAudioData, audioLen);

            // Update time based on audio samples read
            int numSamples = audioLen / (audioFormat.m_numChannels * audioFormat.m_bitsPerSample / 8);
            curtime += (1000 * numSamples) / audioFormat.m_samplesPerSec;
            if (curtime >= 33)
            {
                // Advance to next video frame
                nextVideoFrame = true;
                curtime -= 33;
            }
        }
        if (!nextVideoFrame)
            continue;

        assert(!m_spVideo->EndOfStream());
        auto spVideoSample = m_spVideo->NextVideoSample();
        CRefObj<IImage> spImage;
        if (spVideoSample != nullptr)
        {
            spImage = spVideoSample->GetImage();
            if (frameIndex == phonemeInfo[phonemeIndex].m_endFrame)
            {
                WritePhonemeDeltas(phonemeIndex, phonemeInfo, phonemeLandmarkDeltas);
                WritePhonemeAudio(phonemeIndex, phonemeInfo, curPhonemeAudio);
                phonemeSounds.push_back(curPhonemeAudio);
                curPhonemeAudio.clear();
                phonemeIndex++; // Move to next phoneme
            }

            if (phonemeInfo[phonemeIndex].m_startFrame == frameIndex)
                spImageToWarp = spImage;
        }

        std::vector<Vector2> landmarkDeltas;
        landmarkDeltas.resize(c_FaceLandmark_Mouth_LastIndex - c_FaceLandmark_Mouth_FirstIndex + 1);
        if (frameIndex == phonemeInfo[phonemeIndex].m_startFrame)
        {
            for (int landmarkIndex = c_FaceLandmark_Mouth_FirstIndex; landmarkIndex <= c_FaceLandmark_Mouth_LastIndex; landmarkIndex++)
                landmarkDeltas[landmarkIndex - c_FaceLandmark_Mouth_FirstIndex] = Vector2(0.0f, 0.0f);
        }
        else
        {
            // Compute a matrix to transform the current set of landmarks to
            // such that the bridge of the nose is aligned with the first
            // frame of this phonemene's landmarks.
            Matrix3x2 mat;
            static bool alignLandmarks = false;
            if (alignLandmarks)
                mat = Matrix3x2::Align(
                    faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Bottom],
                    faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Top],
                    faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Bottom],
                    faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Top]);
            else
                mat = Matrix3x2(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

            // Compute the deltas of the current frame's landmarks
            // as compared to the first frame in the current phoneme's landmarks
            for (int landmarkIndex = c_FaceLandmark_Mouth_FirstIndex; landmarkIndex < c_FaceLandmark_Mouth_LastIndex; landmarkIndex++)
            {
                Vector2 alignedLandmark = faceLandmarks[frameIndex][landmarkIndex];
                alignedLandmark = alignedLandmark * mat;
                landmarkDeltas[landmarkIndex - c_FaceLandmark_Mouth_FirstIndex] = alignedLandmark - faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][landmarkIndex];
            }
        }

        phonemeLandmarkDeltas.push_back(landmarkDeltas);

        int roiX = (int)faceBbox[frameIndex].minPt.x;
        int roiY = (int)faceBbox[frameIndex].minPt.y;
        int roiWidth = (int)faceBbox[frameIndex].maxPt.x - (int)faceBbox[frameIndex].minPt.x + 1;
        int roiHeight = (int)faceBbox[frameIndex].maxPt.y - (int)faceBbox[frameIndex].minPt.y + 1;
        ComputeGridWarp(pRenderer, frameIndex, phonemeIndex, phonemeInfo[phonemeIndex], faceLandmarks, landmarkDeltas,
            roiX, roiY, roiWidth, roiHeight, spImageToWarp->GetWidth(), spImageToWarp->GetHeight());

#pragma region("WarpImage")

        CRefObj<IGPUPipeline> spGPUPipeline = Caustic::CreateGPUPipeline(pRenderer);

        CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();

        auto spSource = spGPUPipeline->CreateSourceNode(L"Source", spImageToWarp, spImageToWarp->GetWidth(), spImageToWarp->GetHeight(),
            DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);

        auto spShader = spRenderer->GetShaderMgr()->FindShader(L"Warp");
        std::unique_ptr<CWarpNode> spWarpNode(
            new CWarpNode(L"Warp", pRenderer, spShader, spImageToWarp->GetWidth(), spImageToWarp->GetHeight(), DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
                m_spGridLocations.get()));
        spWarpNode->SetInput(L"Source", L"sourceTexture1", L"sourceSampler1", spSource);
        spGPUPipeline->AddCustomNode(spWarpNode.get());

        auto spShader2 = spRenderer->GetShaderMgr()->FindShader(L"RawCopy");
        CRefObj<IGPUPipelineSinkNode> spSink = spGPUPipeline->CreateSinkNode(L"Sink", spShader2, spImageToWarp->GetWidth(), spImageToWarp->GetHeight(), DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
        spSink->SetInput(L"Source", L"sourceTexture1", L"sourceSampler1", spWarpNode.release());

        spGPUPipeline->IncrementCurrentEpoch();
        spGPUPipeline->Process(pRenderer, pRenderer->GetRenderCtx());
        CRefObj<IImage> spFinalImage = spSink->GetOutput(spGPUPipeline);

#pragma region("StoreWarpedImage")
        static bool drawLandmarks = true;
        if (drawLandmarks)
        {
            for (int landmarkIndex = 0; landmarkIndex < (int)faceLandmarks[frameIndex].size(); landmarkIndex++)
            {
                Matrix3x2 mat;
                if (frameIndex == phonemeInfo[phonemeIndex].m_startFrame)
                {
                    mat = Matrix3x2(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
                }
                else
                {
                    mat = Matrix3x2::Align(
                        faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Bottom],
                        faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Top],
                        faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Bottom],
                        faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Top]);

                }

                Vector2 phonemeLandmark = faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][landmarkIndex];
                Caustic::uint8 color1[4] = { 255, 0, 0, 255 };
                phonemeLandmark.y = 1080 - phonemeLandmark.y;
                spFinalImage->DrawCircle(phonemeLandmark, 3, color1);
                Vector2 alignedLandmark = faceLandmarks[frameIndex][landmarkIndex];
                Caustic::uint8 color2[4] = { 0, 255, 0, 255 };
                alignedLandmark.y = 1080 - alignedLandmark.y;
                spFinalImage->DrawCircle(alignedLandmark, 4, color2);
                alignedLandmark = alignedLandmark * mat;
                Caustic::uint8 color3[4] = { 0, 0, 255, 255 };
                spFinalImage->DrawCircle(alignedLandmark, 5, color3);
            }
        }

        static bool drawGrid = false;
        if (drawGrid)
        {
            for (int i = 0; i < c_GridY - 1; i++)
            {
                for (int j = 0; j < c_GridX - 1; j++)
                {
                    uint8 color[4] = { 255, 255, 255, 255 };
                    int index00 = i * c_GridX + j;
                    int index10 = index00 + 1;
                    int index01 = (i + 1) * c_GridX + j;
                    int index11 = index01 + 1;
                    Vector2 v0, v1;
                    v0 = Vector2(m_spGridLocations.get()[index00].x, m_spGridLocations.get()[index00].y);
                    v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
                    v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
                    v1 = Vector2(m_spGridLocations.get()[index10].x, m_spGridLocations.get()[index10].y);
                    v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
                    v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
                    spFinalImage->DrawLine(v0, v1, color);

                    v0 = Vector2(m_spGridLocations.get()[index10].x, m_spGridLocations.get()[index10].y);
                    v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
                    v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
                    v1 = Vector2(m_spGridLocations.get()[index11].x, m_spGridLocations.get()[index11].y);
                    v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
                    v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
                    spFinalImage->DrawLine(v0, v1, color);

                    v0 = Vector2(m_spGridLocations.get()[index00].x, m_spGridLocations.get()[index00].y);
                    v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
                    v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
                    v1 = Vector2(m_spGridLocations.get()[index11].x, m_spGridLocations.get()[index11].y);
                    v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
                    v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
                    spFinalImage->DrawLine(v0, v1, color);

                    v0 = Vector2(m_spGridLocations.get()[index00].x, m_spGridLocations.get()[index00].y);
                    v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
                    v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
                    v1 = Vector2(m_spGridLocations.get()[index01].x, m_spGridLocations.get()[index01].y);
                    v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
                    v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
                    spFinalImage->DrawLine(v0, v1, color);

                    v0 = Vector2(m_spGridLocations.get()[index01].x, m_spGridLocations.get()[index01].y);
                    v0.x = (spFinalImage->GetWidth() - 1) * ((v0.x + 1.0f) / 2.0f);
                    v0.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v0.y + 1.0f) / 2.0f));
                    v1 = Vector2(m_spGridLocations.get()[index11].x, m_spGridLocations.get()[index11].y);
                    v1.x = (spFinalImage->GetWidth() - 1) * ((v1.x + 1.0f) / 2.0f);
                    v1.y = (spFinalImage->GetHeight() - 1) * (1.0f - ((v1.y + 1.0f) / 2.0f));
                    spFinalImage->DrawLine(v0, v1, color);
                }
            }
        }

        static bool storeWarpedResults = true;
        if (storeWarpedResults)
        {
            wchar_t buf[1024];
            swprintf_s(buf, L"%s\\warped\\%s_warped_%d.png", m_videoPath.c_str(), m_videoName.c_str(), frameIndex);
            Caustic::StoreImage(buf, spFinalImage);
        }
#pragma endregion
#pragma endregion
        frameIndex++;
    }
    WritePhonemeDeltas(phonemeIndex, phonemeInfo, phonemeLandmarkDeltas);
    WritePhonemeAudio(phonemeIndex, phonemeInfo, curPhonemeAudio);
    phonemeSounds.push_back(curPhonemeAudio);
    curPhonemeAudio.clear();
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
#pragma region("WriteMarkImages")
            static bool writeLandmarkImages = true;
            if (writeLandmarkImages)
            {
                wchar_t buf[1024];
                swprintf_s(buf, L"%s\\landmarks\\%s_marked_%d.png", m_videoPath.c_str(), m_videoName.c_str(), frameIndex);
                Caustic::StoreImage(buf, marked);
            }
#pragma endregion
            frameIndex++;
        }
    }

}

void CApp::DeterminePhonemeInfo(std::vector<int> timings, std::vector<std::vector<Vector2>>& faceLandmarks, int numPhonemes, std::vector<PhonemeInfo> & phonemeInfo)
{
    int curPhoneme = 0;
    int numFramesInVideo = (int)faceLandmarks.size();
    int framesPerPhoneme = numFramesInVideo / numPhonemes;
    int extraFrames = numFramesInVideo - framesPerPhoneme * numPhonemes;
    int lastFrame = 0;
    int timeIndex = 0;
    for (int i = 0; i < numPhonemes; i++)
    {
        PhonemeInfo info;
        if (timings[timeIndex] != -1)
        {
            info.m_startFrame = timings[timeIndex];
            info.m_endFrame = timings[timeIndex + 1];
            timeIndex += 2;
        }
        else
        {
            info.m_startFrame = lastFrame;
            info.m_endFrame = lastFrame + framesPerPhoneme;
            if (extraFrames > 0)
            {
                info.m_endFrame++;
                extraFrames--;
            }
        }
        lastFrame = info.m_endFrame;
        phonemeInfo.push_back(info);
    }
}

void CApp::Convert(Caustic::IRenderer *pRenderer, Caustic::IRenderCtx* pCtx)
{
    m_videoPath = Caustic::str2wstr(VideoPath);
    std::wstring wstrFn = m_videoPath + L"\\words.json";
    LoadPhonemeTimings(wstrFn);

    m_spLandmarkFilter = CreateFaceLandmarksFilter();
    std::string vfile(VideoFile);
    std::transform(vfile.begin(), vfile.end(), vfile.begin(),
        [](unsigned char c) { return std::toupper(c); });
    std::map<std::string, CTimings*>::iterator it = m_timings.begin();
    while (it != m_timings.end())
    {
        if (vfile.length() != 0 && it->second->m_word != vfile)
        {
            it++;
            continue;
        }
        m_videoName = Caustic::str2wstr(it->second->m_word);
        if (it->second->m_filename.empty())
            m_videoFullPath = m_videoPath + L"\\" + m_videoName + L".mp4";
        else
            m_videoFullPath = m_videoPath + L"\\" + Caustic::str2wstr(it->second->m_filename);

        m_spVideo = CreateVideo(m_videoFullPath.c_str());

        // Get list of phonemes for the current word
        std::string word(it->second->m_word);
        std::transform(word.begin(), word.end(), word.begin(),
            [](unsigned char c) { return std::toupper(c); });
        if (word == "EXCLAMATIONPOINT")
            word = "!EXCLAMATION-POINT";
        m_spPhonemes->GetPhonemes(word.c_str(), m_phonemes);

        //    std::wstring wstrFn = m_videoPath + L"\\" + m_videoName + L".json";
        //    LoadPhonemeTimings(wstrFn);

            // Make the first pass across the stream to find all the face
            // landmarks.
        std::vector<BBox2> faceBbox;
        std::vector<std::vector<Vector2>> faceLandmarks;
        FindLandmarks(faceBbox, faceLandmarks);

        std::vector<PhonemeInfo> phonemeInfo;
        
        DeterminePhonemeInfo(m_timings[it->second->m_word]->m_timings, faceLandmarks, (int)m_phonemes.size(), phonemeInfo);

        ComputeWarps(pRenderer, pCtx, faceBbox, faceLandmarks, phonemeInfo);
        it++;
    }
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
            ImGui::Text("VideoFile:");
            ImGui::SameLine();
            ImGui::InputText("##VideoFile", app.VideoFile, sizeof(app.VideoFile));
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
