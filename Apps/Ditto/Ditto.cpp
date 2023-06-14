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
import Audio.AudioPlayback.AudioPlayback;
import Audio.AudioPlayback.IAudioPlayback;
import Imaging.Image.IImage;
import Imaging.Image.ImageFilter;
import Imaging.Image.ImageFilter.FaceLandmarks;
import Geometry.Mesh.IMeshConstructor;
import Geometry.MeshImport;
import Geometry.Mesh.MeshFuncs;
import Geometry.Mesh.IMesh;
import Geometry.Mesh.IDelaunay;
import Geometry.Mesh.Delaunay;
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
import Parsers.JSon.IJSonParser;
import Parsers.JSon.JSonParser;
import Parsers.Lex.ILex;

using namespace Caustic;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const int c_GridX = 100;
const int c_GridY = 100;

static bool s_useFixedDeltas = true;
static int c_MaxDeltas = 100;

// For a given phoneme, this structure contains the list
// of landmark deltas that form the phoneme
struct PhonemeLandmarkDelta
{
    DWORD m_startFrame;
    DWORD m_endFrame;
    DWORD m_numDeltaFrames; // Number of frames to animate over
    std::vector<std::vector<Vector2>> m_frameDeltas; // list of deltas for each frame
};

//**********************************************************************
class CWarpNode : public CGPUPipelineNodeBase
{
    CRefObj<IMesh> m_spMesh;
    CRefObj<IRenderMesh> m_spRenderMesh;

    CRefObj<IMesh> CreateSimpleTriangulation(float imageW1, float imageH1, std::vector<Vector2>& landmarks)
    {
        CGeomVertex vertices[100] = {
            {
                .pos = Vector3(0.0f, 0.0f, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 0
            },
            {
                .pos = Vector3(imageW1 / 4, 0.0f, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.25f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 1
            },
            {
                .pos = Vector3((imageW1 * 2) / 4, 0.0f, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.5f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 2
            },
            {
                .pos = Vector3((imageW1 * 3) / 4, 0.0f, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.75f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 3
            },
            {
                .pos = Vector3(imageW1, 0.0f, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(1.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 4
            },
            {
                .pos = Vector3(imageW1, imageH1 / 4.0f, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(1.0f, 0.25f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 5
            },
            {
                .pos = Vector3(imageW1, ((imageH1 * 2) / 4), 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(1.0f, 0.50f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 6
            },
            {
                .pos = Vector3(imageW1, ((imageH1 * 3) / 4), 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(1.0f, 0.75f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 7
            },
            {
                .pos = Vector3(imageW1, imageH1, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 8
            },
            {
                .pos = Vector3((imageW1 * 3) / 4, imageH1, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.75f, 1.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 9
            },
            {
                .pos = Vector3((imageW1 * 2) / 4, imageH1, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.50f, 1.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 10
            },
            {
                .pos = Vector3(imageW1 / 4, imageH1, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.25f, 1.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 11
            },
            {
                .pos = Vector3(0.0f, imageH1, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.0f, 1.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 12
            },
            {
                .pos = Vector3(0.0f, (imageH1 * 3) / 4, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.0f, 0.75f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 13
            },
            {
                .pos = Vector3(0.0f, (imageH1 * 2) / 4, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.0f, 0.50f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 14
            },
            {
                .pos = Vector3(0.0f, imageH1 / 4, 0.0f),
                .norm = Vector3(1.0f, 0.0f, 0.0f),
                .uvs = { Vector2(0.0f, 0.25f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) },
                .index = 15
            },
        };
        int vertIndex = 16;
        for (size_t landmarkIndex = c_FaceLandmark_Mouth_FirstIndex; landmarkIndex <= c_FaceLandmark_Mouth_LastIndex; landmarkIndex++)
        {
            vertices[vertIndex].pos = Vector3(landmarks[landmarkIndex].x, landmarks[landmarkIndex].y, 0.0f);
            vertices[vertIndex].uvs[0] = Vector2(landmarks[landmarkIndex].x / imageW1, landmarks[landmarkIndex].y / imageH1);
            vertices[vertIndex].uvs[1] = Vector2(0.0f, 0.0f);
            vertices[vertIndex].uvs[2] = Vector2(0.0f, 0.0f);
            vertices[vertIndex].uvs[3] = Vector2(0.0f, 0.0f);
            vertices[vertIndex].norm = Vector3(1.0f, 0.0f, 0.0f);
            vertices[vertIndex].index = vertIndex;
            vertIndex++;
        }
        int pIndices[] = {
            0, 49 - 32, 50 - 32,
            0, 50 - 32, 1,
            1, 50 - 32, 2,
            50 - 32, 51 - 32, 2,
            2, 51 - 32, 3,
            51 - 32, 52 - 32, 3,
            3, 52 - 32, 4,
            4, 52 - 32, 53 - 32,
            4, 53 - 32, 5,
            5, 53 - 32, 54 - 32,
            5, 54 - 32, 6,
            6, 54 - 32, 55 - 32,
            6, 55 - 32, 7,
            7, 55 - 32, 8,
            8, 55 - 32, 56 - 32,
            8, 56 - 32, 9,
            9, 56 - 32, 57 - 32,
            9, 57 - 32, 10,
            10, 57 - 32, 58 - 32,
            10, 58 - 32, 11,
            11, 58 - 32, 12,
            12, 58 - 32, 59 - 32,
            12, 59 - 32, 13,
            13, 59 - 32, 14,
            14, 59 - 32, 48 - 32,
            14, 48 - 32, 15,
            15, 48 - 32, 49 - 32,
            15, 49 - 32, 0,
            48 - 32,60 - 32,49 - 32,
            49 - 32,60 - 32,61 - 32,
            49 - 32,61 - 32,50 - 32,
            50 - 32,61 - 32,62 - 32,
            50 - 32,62 - 32,51 - 32,
            51 - 32,62 - 32,63 - 32,
            51 - 32,63 - 32,52 - 32,
            52 - 32,63 - 32,64 - 32,
            52 - 32,64 - 32,53 - 32,
            53 - 32,64 - 32,54 - 32,
            54 - 32,64 - 32,55 - 32,
            55 - 32,64 - 32,56 - 32,
            64 - 32,65 - 32,56 - 32,
            56 - 32,65 - 32,57 - 32,
            65 - 32,66 - 32,57 - 32,
            66 - 32,58 - 32,57 - 32,
            66 - 32,67 - 32,58 - 32,
            67 - 32,59 - 32,58 - 32,
            67 - 32,60 - 32,59 - 32,
            59 - 32,60 - 32,48 - 32
        };
        CRefObj<IMeshConstructor> spMeshConstructor = IMeshConstructor::Create();
        spMeshConstructor->MeshOpen();
        spMeshConstructor->SubMeshOpen();
        int numTris = (int)_countof(pIndices);
        for (int triIndex = 0; triIndex < numTris; triIndex+=3)
        {
            Vector2 v0, v1, v2;
            Vector3 normal(0.0f, 1.0f, 0.0f);
            Vector3 p0(vertices[pIndices[triIndex+0]].pos.x, vertices[pIndices[triIndex+0]].pos.y, 0.0f);
            Vector3 p1(vertices[pIndices[triIndex+1]].pos.x, vertices[pIndices[triIndex+1]].pos.y, 0.0f);
            Vector3 p2(vertices[pIndices[triIndex+2]].pos.x, vertices[pIndices[triIndex+2]].pos.y, 0.0f);
            spMeshConstructor->FaceOpen();
            Vector2 vuv;
            Vector2 uv0[4] = { Vector2(vertices[pIndices[triIndex + 0]].uvs[0].x, vertices[pIndices[triIndex + 0]].uvs[0].y),
                Vector2((float)vertices[pIndices[triIndex + 0]].index / 100.0f, 0.0f),
                Vector2(0.0f, 0.0f),
                Vector2(0.0f, 0.0f) };
            Vector2 uv1[4] = { Vector2(vertices[pIndices[triIndex + 1]].uvs[0].x, vertices[pIndices[triIndex + 1]].uvs[0].y),
                Vector2((float)vertices[pIndices[triIndex + 1]].index / 100.0f, 0.0f),
                Vector2(0.0f, 0.0f),
                Vector2(0.0f, 0.0f) };
            Vector2 uv2[4] = { Vector2(vertices[pIndices[triIndex + 2]].uvs[0].x, vertices[pIndices[triIndex + 2]].uvs[0].y),
                Vector2((float)vertices[pIndices[triIndex + 2]].index / 100.0f, 0.0f),
                Vector2(0.0f, 0.0f),
                Vector2(0.0f, 0.0f) };
            p0.x /= imageW1; p0.x = p0.x * 2.0f - 1.0f;
            p0.y /= imageH1; p0.y = p0.y * 2.0f - 1.0f;
            p1.x /= imageW1; p1.x = p1.x * 2.0f - 1.0f;
            p1.y /= imageH1; p1.y = p1.y * 2.0f - 1.0f;
            p2.x /= imageW1; p2.x = p2.x * 2.0f - 1.0f;
            p2.y /= imageH1; p2.y = p2.y * 2.0f - 1.0f;
            spMeshConstructor->VertexAdd(p0, normal, uv0);
            spMeshConstructor->VertexAdd(p1, normal, uv1);
            spMeshConstructor->VertexAdd(p2, normal, uv2);
            spMeshConstructor->FaceClose();
        }
        CRefObj<ISubMesh> spSubMesh = spMeshConstructor->SubMeshClose();
        spSubMesh->SetMeshFlags(EMeshFlags::TwoSided);
        return spMeshConstructor->MeshClose();
    }

    CRefObj<IDelaunay2> CreateTriangulation(float imageW1, float imageH1, std::vector<Vector2>& landmarks)
    {
        // Define our super triangles to be larger than our image
////        BBox2 sqbb;
////        sqbb.minPt = Vector2(-1000.0f, -1000.0f);
////        sqbb.maxPt = Vector2(imageW1 + 1000.0f, imageH1 + 1000.0f);
////        CRefObj<IDelaunay2> spDelaunay = Caustic::CreateDelaunay2(sqbb);

#pragma region("BuildTriangulation")
////#if 0
////        // Add the corners of the image as our boundaries
////        int numVertices = c_FaceLandmark_Mouth_LastIndex - c_FaceLandmark_Mouth_FirstIndex + 1;
////        Caustic::DelaunayVertex* pVertices = new Caustic::DelaunayVertex[numVertices];
////        for (size_t landmarkIndex = c_FaceLandmark_Mouth_FirstIndex; landmarkIndex <= c_FaceLandmark_Mouth_LastIndex; landmarkIndex++)
////        {
////            pVertices[landmarkIndex - c_FaceLandmark_Mouth_FirstIndex].flags = (landmarkIndex < c_FaceLandmark_InnerMouth_FirstIndex) ? c_InteriorVertex : c_BoundaryVertex;
////            pVertices[landmarkIndex - c_FaceLandmark_Mouth_FirstIndex].pos = landmarks[landmarkIndex];
////            pVertices[landmarkIndex - c_FaceLandmark_Mouth_FirstIndex].uv.x = landmarks[landmarkIndex].x / imageW1;
////            pVertices[landmarkIndex - c_FaceLandmark_Mouth_FirstIndex].uv.y = landmarks[landmarkIndex].y / imageH1;
////        }
////        int pIndices[] = {
////            48 - c_FaceLandmark_Mouth_FirstIndex, 60 - c_FaceLandmark_Mouth_FirstIndex, 49 - c_FaceLandmark_Mouth_FirstIndex,
////            60 - c_FaceLandmark_Mouth_FirstIndex, 61 - c_FaceLandmark_Mouth_FirstIndex, 49 - c_FaceLandmark_Mouth_FirstIndex,
////            49 - c_FaceLandmark_Mouth_FirstIndex, 61 - c_FaceLandmark_Mouth_FirstIndex, 50 - c_FaceLandmark_Mouth_FirstIndex,
////            61 - c_FaceLandmark_Mouth_FirstIndex, 62 - c_FaceLandmark_Mouth_FirstIndex, 50 - c_FaceLandmark_Mouth_FirstIndex,
////            50 - c_FaceLandmark_Mouth_FirstIndex, 62 - c_FaceLandmark_Mouth_FirstIndex, 51 - c_FaceLandmark_Mouth_FirstIndex,
////            51 - c_FaceLandmark_Mouth_FirstIndex, 62 - c_FaceLandmark_Mouth_FirstIndex, 63 - c_FaceLandmark_Mouth_FirstIndex,
////            51 - c_FaceLandmark_Mouth_FirstIndex, 63 - c_FaceLandmark_Mouth_FirstIndex, 52 - c_FaceLandmark_Mouth_FirstIndex,
////            52 - c_FaceLandmark_Mouth_FirstIndex, 63 - c_FaceLandmark_Mouth_FirstIndex, 64 - c_FaceLandmark_Mouth_FirstIndex,
////            52 - c_FaceLandmark_Mouth_FirstIndex, 64 - c_FaceLandmark_Mouth_FirstIndex, 53 - c_FaceLandmark_Mouth_FirstIndex,
////            53 - c_FaceLandmark_Mouth_FirstIndex, 64 - c_FaceLandmark_Mouth_FirstIndex, 54 - c_FaceLandmark_Mouth_FirstIndex,
////            53 - c_FaceLandmark_Mouth_FirstIndex, 64 - c_FaceLandmark_Mouth_FirstIndex, 55 - c_FaceLandmark_Mouth_FirstIndex,
////            55 - c_FaceLandmark_Mouth_FirstIndex, 64 - c_FaceLandmark_Mouth_FirstIndex, 65 - c_FaceLandmark_Mouth_FirstIndex,
////            55 - c_FaceLandmark_Mouth_FirstIndex, 65 - c_FaceLandmark_Mouth_FirstIndex, 56 - c_FaceLandmark_Mouth_FirstIndex,
////            56 - c_FaceLandmark_Mouth_FirstIndex, 65 - c_FaceLandmark_Mouth_FirstIndex, 66 - c_FaceLandmark_Mouth_FirstIndex,
////            56 - c_FaceLandmark_Mouth_FirstIndex, 66 - c_FaceLandmark_Mouth_FirstIndex, 57 - c_FaceLandmark_Mouth_FirstIndex,
////            57 - c_FaceLandmark_Mouth_FirstIndex, 66 - c_FaceLandmark_Mouth_FirstIndex, 67 - c_FaceLandmark_Mouth_FirstIndex,
////            57 - c_FaceLandmark_Mouth_FirstIndex, 67 - c_FaceLandmark_Mouth_FirstIndex, 58 - c_FaceLandmark_Mouth_FirstIndex,
////            58 - c_FaceLandmark_Mouth_FirstIndex, 67 - c_FaceLandmark_Mouth_FirstIndex, 60 - c_FaceLandmark_Mouth_FirstIndex,
////            58 - c_FaceLandmark_Mouth_FirstIndex, 60 - c_FaceLandmark_Mouth_FirstIndex, 59 - c_FaceLandmark_Mouth_FirstIndex,
////            59 - c_FaceLandmark_Mouth_FirstIndex, 60 - c_FaceLandmark_Mouth_FirstIndex, 48 - c_FaceLandmark_Mouth_FirstIndex
////        };
////        spDelaunay->AddFixedMesh(pVertices, numVertices, pIndices, _countof(pIndices));
////
////#endif
        DelaunayVertex* pVertices = new DelaunayVertex[38];
        Vector2 uv;
        Vector2 pos;
        pVertices[0] = DelaunayVertex(pos = Vector2(0.0f, 0.0f), uv = Vector2(0.0f, 0.0f), c_SuperTriangleVertex);
        pVertices[1] = DelaunayVertex(pos = Vector2(imageW1 / 4, 0.0f), uv = Vector2(0.25f, 0.0f), c_SuperTriangleVertex);
        pVertices[2] = DelaunayVertex(pos = Vector2((imageW1 * 2) / 4, 0.0f), uv = Vector2(0.50f, 0.0f), c_SuperTriangleVertex);
        pVertices[3] = DelaunayVertex(pos = Vector2((imageW1 * 3) / 4, 0.0f), uv = Vector2(0.75f, 0.0f), c_SuperTriangleVertex);
        pVertices[4] = DelaunayVertex(pos = Vector2(imageW1, 0.0f), uv = Vector2(1.0f, 0.0f), c_SuperTriangleVertex);
        pVertices[5] = DelaunayVertex(pos = Vector2(imageW1, imageH1 / 4), uv = Vector2(1.0f, 0.25f), c_SuperTriangleVertex);
        pVertices[6] = DelaunayVertex(pos = Vector2(imageW1, (imageH1 * 2) / 4), uv = Vector2(1.0f, 0.50f), c_SuperTriangleVertex);
        pVertices[7] = DelaunayVertex(pos = Vector2(imageW1, (imageH1 * 3) / 4), uv = Vector2(1.0f, 0.75f), c_SuperTriangleVertex);
        pVertices[8] = DelaunayVertex(pos = Vector2(imageW1, imageH1), uv = Vector2(1.0f, 1.0f), c_SuperTriangleVertex);
        pVertices[9] = DelaunayVertex(pos = Vector2((imageW1 * 3) / 4, imageH1), uv = Vector2(0.75f, 1.0f), c_SuperTriangleVertex);
        pVertices[10] = DelaunayVertex(pos = Vector2((imageW1 * 2) / 4, imageH1), uv = Vector2(0.50f, 1.0f), c_SuperTriangleVertex);
        pVertices[11] = DelaunayVertex(pos = Vector2(imageW1 / 4, imageH1), uv = Vector2(0.25f, 1.0f), c_SuperTriangleVertex);
        pVertices[12] = DelaunayVertex(pos = Vector2(0.0f, imageH1), uv = Vector2(0.0f, 1.0f), c_SuperTriangleVertex);
        pVertices[13] = DelaunayVertex(pos = Vector2(0.0f, (imageH1 * 3) / 4), uv = Vector2(0.0f, 0.75f), c_SuperTriangleVertex);
        pVertices[14] = DelaunayVertex(pos = Vector2(0.0f, (imageH1 * 2) / 4), uv = Vector2(0.0f, 0.50f), c_SuperTriangleVertex);
        pVertices[15] = DelaunayVertex(pos = Vector2(0.0f, imageH1 / 4), uv = Vector2(0.0f, 0.25f), c_SuperTriangleVertex);

        int ptIndex = 16;
        for (size_t landmarkIndex = c_FaceLandmark_Mouth_FirstIndex; landmarkIndex <= c_FaceLandmark_Mouth_LastIndex; landmarkIndex++)
        {
            Vector2 point(landmarks[landmarkIndex].x, landmarks[landmarkIndex].y);
            Vector2 uv(landmarks[landmarkIndex].x / imageW1, landmarks[landmarkIndex].y / imageH1);
            pVertices[ptIndex++] = DelaunayVertex(point, uv, c_SuperTriangleVertex);
        }
        for (size_t k = 0; k < ptIndex; k++)
        {
            wchar_t buf[1024];
            swprintf_s(buf, L"Vert: %f,%f\n", pVertices[k].pos.x, pVertices[k].pos.y);
            OutputDebugString(buf);
        }
        int pIndices[] = {
            0, 49 - 32, 50 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            0, 50 - 32, 1, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            1, 50 - 32, 2, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            50 - 32, 51 - 32, 2, (int)c_BoundaryEdge, (int)c_NormalEdge, (int)c_NormalEdge,
            2, 51 - 32, 3, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            51 - 32, 52 - 32, 3, (int)c_BoundaryEdge, (int)c_NormalEdge, (int)c_NormalEdge,
            3, 52 - 32, 4, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            4, 52 - 32, 53 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            4, 53 - 32, 5, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            5, 53 - 32, 54 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            5, 54 - 32, 6, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            6, 54 - 32, 55 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            6, 55 - 32, 7, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            7, 55 - 32, 8, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            8, 55 - 32, 56 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            8, 56 - 32, 9, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            9, 56 - 32, 57 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            9, 57 - 32, 10, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            10, 57 - 32, 58 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            10, 58 - 32, 11, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            11, 58 - 32, 12, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            12, 58 - 32, 59 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            12, 59 - 32, 13, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            13, 59 - 32, 14, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            14, 59 - 32, 48 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            14, 48 - 32, 15, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge,
            15, 48 - 32, 49 - 32, (int)c_NormalEdge, (int)c_BoundaryEdge, (int)c_NormalEdge,
            15, 49 - 32, 0, (int)c_NormalEdge, (int)c_NormalEdge, (int)c_BoundaryEdge
        };
        CRefObj<IDelaunay2> spDelaunay = Caustic::CreateDelaunay2(pVertices, ptIndex, pIndices, _countof(pIndices));
        spDelaunay->Open();
        int gridDeltaX = (int)(imageW1 / c_GridX);
        int gridDeltaY = (int)(imageH1 / c_GridY);
        for (int y = 1; y < c_GridY - 1; y++)
        {
            for (int x = 1; x < c_GridX - 1; x++)
            {
                Vector2 pos(float(x * gridDeltaX), float(y * gridDeltaY));
                Vector2 uv(float(x) / float(c_GridY - 1), float(y) / float(c_GridX - 1));
                spDelaunay->AddPoint(pos, uv, false);
            }
            break;
        }
        spDelaunay->Close(false);
        return spDelaunay;
#pragma endregion
    }

    CRefObj<IMesh> BuildMesh(float imageW1, float imageH1, std::vector<Vector2>& landmarks)
    {
        CRefObj<IDelaunay2> spDelaunay = CreateTriangulation(imageW1, imageH1, landmarks);

        Vector3 normal(0.0f, 1.0f, 0.0f);

        CRefObj<IMeshConstructor> spMeshConstructor = IMeshConstructor::Create();
        spMeshConstructor->MeshOpen();
        spMeshConstructor->SubMeshOpen();
        int numTris = spDelaunay->GetNumberTriangles();
        for (int triIndex = 0; triIndex < numTris; triIndex++)
        {
            bool isBoundary[3];
            Vector2 v0, v1, v2;
            spDelaunay->GetTriangle(triIndex, v0, v1, v2, isBoundary);
            Vector3 p0(v0.x, v0.y, 0.0f);
            Vector3 p1(v1.x, v1.y, 0.0f);
            Vector3 p2(v2.x, v2.y, 0.0f);
            spMeshConstructor->FaceOpen();
            Vector2 vuv;
            const int c_FirstRealVert = 4; // first 4 vertices in trianglation are the Delaunay super triangles
            Vector2 uv0(p0.x / imageW1, p0.y / imageH1);
            Vector2 uv1(p1.x / imageW1, p1.y / imageH1);
            Vector2 uv2(p2.x / imageW1, p2.y / imageH1);
            spMeshConstructor->VertexAdd(p0, normal, uv0);
            spMeshConstructor->VertexAdd(p1, normal, uv1);
            spMeshConstructor->VertexAdd(p2, normal, uv2);
            spMeshConstructor->FaceClose();
        }
        CRefObj<ISubMesh> spSubMesh = spMeshConstructor->SubMeshClose();
        spSubMesh->SetMeshFlags(EMeshFlags::TwoSided);
        return spMeshConstructor->MeshClose();
    }

public:
    CWarpNode(const wchar_t* pName, IRenderer* pRenderer, std::vector<Vector2> &landmarks, IShader* pShader, uint32 inputWidth, uint32 inputHeight, DXGI_FORMAT format) :
        CGPUPipelineNodeBase(inputWidth, inputHeight, format)
    {
        SetName(pName);
        SetShader(pShader);

        m_cpuFlags = (D3D11_CPU_ACCESS_FLAG)0;
        m_bindFlags = (D3D11_BIND_FLAG)(D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        //m_spMesh = Caustic::CreateGrid(c_GridX, c_GridY);
        //m_spMesh = BuildMesh((float)inputWidth - 1.0f, (float)inputHeight - 1.0f, landmarks);
        m_spMesh = CreateSimpleTriangulation((float)inputWidth - 1.0f, (float)inputHeight - 1.0f, landmarks);
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

class CApp
{
public:
    CApp()
    {
        m_curFrameIndex = -1;
        m_phonemeFrameIndex = -1;
        m_showLandmarks = false;
        m_slowPlayback = false;
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
    CRefObj<IAudioPlayback> m_spAudioPlayback;
    std::vector<BBox2> m_faceBbox; // Bounding box of the entire face based on face detection (ignoring landmarks)
    std::vector<BBox2> m_faceLandmarksInfluenceBounds; // Bounding box of face landmarks per video frame adjusted to include influence distance
    std::vector<std::vector<Vector2>> m_faceLandmarks; // List of landmark positions per video frame
    int m_curVideoIndex;
    int m_nextVideoIndex;
    int m_curFrameIndex;
    bool m_texLoaded;
    HWND m_hwnd;
    bool m_showLandmarks;
    bool m_slowPlayback;
    std::chrono::time_point<std::chrono::system_clock> m_prevRenderTime;
    CRefObj<IVirtualCamera> m_spVirtualCam;
    CRefObj<IVideo> m_spVideo;
    CRefObj<INDIStream> m_spNDIStream;
    std::map<std::wstring, PhonemeLandmarkDelta> m_phonemeLandmarkDeltaMap;
    std::map<std::wstring, std::vector<uint8>> m_phonemeAudioMap;
    std::unique_ptr<float2> m_spGridLocations;
    std::unique_ptr<float2> m_spFixedDeltas;
    std::vector<std::string> m_words;           // List of words in current sentence being played
    int m_wordIndex;                            // Current index into m_words
    std::vector<std::wstring> m_phonemesInCurrentWord;        // List of phonemes in the current word for the sentence being played
    int m_phonemeIndex;                         // Index into m_phonemesInCurrentWord
    bool m_playPhonemes;                        // Are we playing back phonemes?
    int m_phonemeFrameIndex;                    // Index into m_curPhonemeLandmarkDeltas.m_frameDeltas
    PhonemeLandmarkDelta m_curPhonemeLandmarkDeltas;   // List of frames with list of landmark deltas for a given phoneme
    CRefObj<IGPUPipelineNode> m_spWarpNode;
    CRefObj<IShader> m_spWarpShader;
    CRefObj<ITexture> m_spGridPosTex;
    CRefObj<ITexture> m_spFixedDeltaTex;
    CRefObj<IGPUPipeline> m_spGPUPipeline;
    CRefObj<IGPUPipelineSourceNode> m_spGPUSource;
    CRefObj<IGPUPipelineSinkNode> m_spGPUSink;

    void InitializeCaustic(HWND hWnd);
    void LiveWebCam();
    void BuildUI(ITexture* pFinalRT, ImFont* pFont);
    void FindLandmarks();
    void LoadPhonemeAudio();
    void LoadDeltas();
    void ComputeGridWarp(IImage* pImageToWarp, int frameIndex, std::wstring& phoneme, int phonemeFrameIndex);
    CRefObj<IImage> WarpImage(IRenderer* pRenderer, IImage* pImageToWarp, int frameIndex, std::wstring &phoneme, int phonemeFrameIndex);
    void PlaySentence(const char* pSentence);
    void AdvanceToNextWord();
    void PlayPhonemeAudio();
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

void CApp::LoadPhonemeAudio()
{
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(L"d:\\data\\Audio_Phoneme_*.bin", &findData);
    if (hFind == INVALID_HANDLE_VALUE)
        return;
    while (true)
    {
        std::wstring fn(std::wstring(L"d:\\data\\") + findData.cFileName);
        HANDLE f = CreateFile(fn.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        if (f != INVALID_HANDLE_VALUE)
        {
            std::wstring phonemeName = std::wstring(findData.cFileName);
            phonemeName = phonemeName.substr(phonemeName.find_last_of('_') + 1);
            phonemeName = phonemeName.substr(0, phonemeName.find_first_of('.'));
            DWORD bytesRead;
            DWORD dwNumBytes;
            ReadFile(f, &dwNumBytes, sizeof(DWORD), &bytesRead, nullptr);
            std::vector<uint8> phonemeAudio;
            phonemeAudio.resize(dwNumBytes);
            ReadFile(f, &phonemeAudio[0], dwNumBytes, &bytesRead, nullptr);
            m_phonemeAudioMap.insert(std::make_pair(phonemeName, phonemeAudio));
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
            float imageW1 = (float)spImage->GetWidth() - 1.0f;
            float imageH1 = (float)spImage->GetHeight() - 1.0f;
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

void CApp::PlayPhonemeAudio()
{
    if (m_playPhonemes)
    {
        std::wstring modPhoneme = m_phonemesInCurrentWord[m_phonemeIndex];
        if (!m_phonemeLandmarkDeltaMap.contains(modPhoneme) && isdigit(modPhoneme[modPhoneme.length() - 1]))
            modPhoneme = modPhoneme.substr(0, modPhoneme.length() - 1);
        m_curPhonemeLandmarkDeltas = m_phonemeLandmarkDeltaMap[modPhoneme];
        if (m_phonemeIndex < m_phonemesInCurrentWord.size())
        {
            std::wstring phoneme = m_phonemesInCurrentWord[m_phonemeIndex];
            uint8* pBuffer;
            uint32 bufferLen;
            if ( ! m_phonemeAudioMap.contains(phoneme) && isdigit(phoneme[phoneme.length() - 1]))
                phoneme = phoneme.substr(0, phoneme.length() - 1);
            assert(m_phonemeAudioMap.contains(phoneme));
            pBuffer = (uint8*)&m_phonemeAudioMap[phoneme][0];
            bufferLen = (uint32)m_phonemeAudioMap[phoneme].size();
            m_spAudioPlayback->Play(pBuffer, bufferLen);
        }
    }
}

void CApp::AdvanceToNextWord()
{
    m_wordIndex++;
    if (m_wordIndex == m_words.size())
    {
        m_playPhonemes = false;
        return;
    }
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
}

void CApp::PlaySentence(const char* pSentence)
{
    m_words = SplitSentence(pSentence);
    m_wordIndex = -1;
    m_playPhonemes = true;
    AdvanceToNextWord();
    PlayPhonemeAudio();
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
        app.PlaySentence("Aardvark");
    }
    if (ImGui::Button((app.m_slowPlayback) ? "Normal playback" : "Slow playback"))
        app.m_slowPlayback = !app.m_slowPlayback;
    if (ImGui::Button((app.m_showLandmarks) ? "Hide Landmarks" : "Show Landmarks"))
        app.m_showLandmarks = !app.m_showLandmarks;
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

    pRenderer->BeginMarker(L"WarpGPU");

    if (s_useFixedDeltas)
    {
        D3D11_MAPPED_SUBRESOURCE ms;
        auto ctx = pRenderer->GetContext();
        CT(ctx->Map(m_spFixedDeltaTex->GetD3DTexture(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        float* pDst = reinterpret_cast<float*>(ms.pData);
        float2 *pDeltas = m_spFixedDeltas.get();
        float imageW = (float)pImageToWarp->GetWidth() - 1.0f;
        float imageH = (float)pImageToWarp->GetHeight() - 1.0f;
        for (int i = 0; i < c_MaxDeltas; i++, pDeltas++)
        {
            int j = i + 32;
            if (j >= 49 && j <= 67)
            {
                *pDst++ = 2.0f * pDeltas->x / imageW;
                *pDst++ = 2.0f * pDeltas->y / imageH;
            }
            else
            {
                *pDst++ = 0.0f;
                *pDst++ = 0.0f;
            }
//            *pDst++ = 0.0f;// pDeltas->y / imageH;
//            if ((j >= 49 && j <= 53) || (j >= 61 && j <= 63))
//                *pDst++ = 100.0f / imageW;
//            else if ((j >= 55 && j <= 59) || (j >= 65 && j <= 67))
//                *pDst++ = -100.0f / imageW;
//            else
//                *pDst++ = 0.0f;
        }
        ctx->Unmap(m_spFixedDeltaTex->GetD3DTexture(), 0);
    }
    else
    {
        D3D11_MAPPED_SUBRESOURCE ms;
        auto ctx = pRenderer->GetContext();
        CT(ctx->Map(m_spGridPosTex->GetD3DTexture(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        BYTE* pDst = reinterpret_cast<BYTE*>(ms.pData);
        BYTE* pSrc = (BYTE*)m_spGridLocations.get();
        for (int y = 0; y < c_GridY; y++)
        {
            memcpy(pDst, pSrc, c_GridX * sizeof(float) * 2);
            pSrc += sizeof(float) * 2 * c_GridX;
            pDst += ms.RowPitch;
        }
        ctx->Unmap(m_spGridPosTex->GetD3DTexture(), 0);
    }
    m_spGPUSource->SetSource(m_spGPUPipeline, pImageToWarp);
    
    m_spGPUPipeline->IncrementCurrentEpoch();
    m_spGPUPipeline->Process(pRenderer, pRenderer->GetRenderCtx());
    pRenderer->EndMarker();
    CRefObj<IImage> spFinalImage = m_spGPUSink->GetOutput(m_spGPUPipeline);


#pragma region("StoreWarpedImage")
    if (m_showLandmarks)
    {
        for (int landmarkIndex = 0; landmarkIndex < (int)m_faceLandmarks[frameIndex].size(); landmarkIndex++)
        {
            Matrix3x2 mat;
            //if (frameIndex ==  phonemeInfo[phonemeIndex].m_startFrame)
            //{
                mat = Matrix3x2(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
            //}
            //else
            //{
            //    mat = Matrix3x2::Align(
            //        faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Bottom],
            //        faceLandmarks[phonemeInfo[phonemeIndex].m_startFrame][c_FaceLandmark_NoseBridge_Top],
            //        faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Bottom],
            //        faceLandmarks[frameIndex][c_FaceLandmark_NoseBridge_Top]);
            //
            //}

            std::map<std::wstring, PhonemeLandmarkDelta> m_phonemeLandmarkDeltaMap;
            Vector2 phonemeLandmark = m_faceLandmarks[m_phonemeLandmarkDeltaMap[m_phonemesInCurrentWord[m_phonemeIndex]].m_startFrame][landmarkIndex];
            Caustic::uint8 color1[4] = { 255, 0, 0, 255 };
            spFinalImage->DrawCircle(phonemeLandmark, 3, color1);
            Vector2 alignedLandmark = m_faceLandmarks[frameIndex][landmarkIndex];
            Caustic::uint8 color2[4] = { 0, 255, 0, 255 };
            spFinalImage->DrawCircle(alignedLandmark, 4, color2);
            //alignedLandmark = alignedLandmark * mat;
            //Caustic::uint8 color3[4] = { 0, 0, 255, 255 };
            //spFinalImage->DrawCircle(alignedLandmark, 5, color3);
        }
    }

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
    IRenderer* pRenderer = m_spRenderWindow->GetRenderer();
    GaussianDistribution distribution(2.0f);
    int imageW = pImageToWarp->GetWidth();
    int imageH = pImageToWarp->GetHeight();
    float gridDeltaX = (float)imageW / (float)c_GridX;
    float gridDeltaY = (float)imageH / (float)c_GridY;

    std::wstring modPhoneme = phoneme;
    if (!m_phonemeLandmarkDeltaMap.contains(modPhoneme) && isdigit(modPhoneme[modPhoneme.length() - 1]))
        modPhoneme = modPhoneme.substr(0, modPhoneme.length() - 1);
    assert(m_phonemeLandmarkDeltaMap.contains(modPhoneme));

    PhonemeLandmarkDelta pdelta = m_phonemeLandmarkDeltaMap[modPhoneme];
    if (s_useFixedDeltas)
    {
        float2* pDeltas = m_spFixedDeltas.get();
        memset(pDeltas, 0, sizeof(float2) * c_MaxDeltas);
        pDeltas += 16; // skip boundary points
        for (int landmarkIndex = c_FaceLandmark_Mouth_FirstIndex; landmarkIndex <= c_FaceLandmark_Mouth_LastIndex; landmarkIndex++)
        {
            pDeltas->x = pdelta.m_frameDeltas[phonemeFrameIndex][landmarkIndex - c_FaceLandmark_Mouth_FirstIndex].x;
            pDeltas->y = pdelta.m_frameDeltas[phonemeFrameIndex][landmarkIndex - c_FaceLandmark_Mouth_FirstIndex].y;
            pDeltas++;
        }
    }
    else
    {
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
                    for (int landmarkIndex = c_FaceLandmark_InnerMouth_FirstIndex; landmarkIndex <= c_FaceLandmark_InnerMouth_LastIndex; landmarkIndex++)
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
                    pGridLocations[index].x = newGridPos.x / ((float)imageW - 1.0f);
                    pGridLocations[index].y = newGridPos.y / ((float)imageH - 1.0f);
                    pGridLocations[index].x = pGridLocations[index].x * 2.0f;
                    pGridLocations[index].y = pGridLocations[index].y * 2.0f;
                }
                else
                {
                    int index = gridY * c_GridX + gridX;
                    pGridLocations[index].x = 0.0f;
                    pGridLocations[index].y = 0.0f;
                }
            }
        }
    }
}

void CApp::LoadVideos(IRenderer* pRenderer, IRenderCtx* pCtx)
{
    CRefObj<IShaderMgr> spShaderMgr = pRenderer->GetShaderMgr();

    m_curVideoIndex = 0;
    m_nextVideoIndex = 0;
    CRefObj<IVideo> p0 = CreateVideo(L"d:\\DittoData\\Listening.mp4");
    m_videos.push_back(p0);
    LoadDeltas();
    LoadPhonemeAudio();
    FindLandmarks();

    CAudioFormat audioFormat;
    CRefObj<IVideo> sourceAudio = CreateVideo(L"d:\\data\\Aardvark.mp4");
    sourceAudio->GetAudioFormat(&audioFormat);
    m_spAudioPlayback = Caustic::CreateAudioPlayback(audioFormat.m_samplesPerSec, audioFormat.m_bitsPerSample, audioFormat.m_numChannels);

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

    CVideoFormat videoFormat;
    p2->GetVideoFormat(&videoFormat);
    m_spNDIStream->Initialize("test", videoFormat.m_width, videoFormat.m_height, 30, audioFormat.m_samplesPerSec, audioFormat.m_bitsPerSample, audioFormat.m_numChannels);
}

void CApp::ProcessNextFrame(IRenderer* pRenderer, IRenderCtx* pCtx)
{
    if (m_slowPlayback)
        Sleep(1000);
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

                if (m_spWarpNode == nullptr)
                {
                    m_spWarpShader = pRenderer->GetShaderMgr()->FindShader(L"Warp");
                    m_spWarpShader->SetVSParam(L"width", std::any(float(c_GridX)));
                    m_spWarpShader->SetVSParam(L"height", std::any(float(c_GridY)));
                    uint32 imageW = m_spLastFrame->GetWidth();
                    uint32 imageH = m_spLastFrame->GetHeight();
                    m_spWarpNode = new CWarpNode(L"Warp", pRenderer, m_faceLandmarks[0], m_spWarpShader, imageW, imageH,
                        DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
                    if (s_useFixedDeltas)
                    {
                        m_spFixedDeltaTex = Caustic::CreateTexture(pRenderer, c_MaxDeltas, 1, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT);
                        m_spWarpShader->SetVSParam(L"posTexture", std::any(m_spFixedDeltaTex));
                        auto spSampler = Caustic::CreateSampler(pRenderer, m_spFixedDeltaTex);
                        spSampler->SetFilter(pRenderer, D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT);
                        m_spWarpShader->SetVSParam(L"posSampler", std::any(spSampler));
                    }
                    else
                    {
                        m_spGridPosTex = Caustic::CreateTexture(pRenderer, c_GridX, c_GridY, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT);
                        m_spWarpShader->SetVSParam(L"posTexture", std::any(m_spGridPosTex));
                        auto spSampler = Caustic::CreateSampler(pRenderer, m_spGridPosTex);
                        m_spWarpShader->SetVSParam(L"posSampler", std::any(spSampler));
                    }

                    // Create our GPU pipeline
                    m_spGPUPipeline = Caustic::CreateGPUPipeline(pRenderer);
                    m_spGPUSource = m_spGPUPipeline->CreateSourceNode(L"Source", nullptr, imageW, imageH,
                        DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
                    m_spWarpNode->SetInput(L"Source", L"sourceTexture1", L"sourceSampler1", m_spGPUSource);
                    m_spGPUPipeline->AddCustomNode(m_spWarpNode);
                    auto spShader2 = pRenderer->GetShaderMgr()->FindShader(L"RawCopy");
                    m_spGPUSink = m_spGPUPipeline->CreateSinkNode(L"Sink", spShader2, imageW, imageH, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
                    m_spGPUSink->SetInput(L"Source", L"sourceTexture1", L"sourceSampler1", m_spWarpNode);
                }

                if (m_playPhonemes)
                {
                    m_spLastFrame = WarpImage(pRenderer, m_spLastFrame, m_curFrameIndex, m_phonemesInCurrentWord[m_phonemeIndex], m_phonemeFrameIndex);
                }
                m_curFrameIndex++;

                // Check if we have reached the last frame of the current phoneme.
                // If so, move to the next phoneme
                if (m_playPhonemes)
                {
                    m_phonemeFrameIndex++;
                    if (m_phonemeFrameIndex == m_curPhonemeLandmarkDeltas.m_frameDeltas.size())
                    {
                        m_phonemeFrameIndex = 0;
                        m_phonemeIndex++;

                        if (m_phonemeIndex == m_phonemesInCurrentWord.size())
                        {
                            // We have reached the last phoneme in the current word. Move to the next word
                            AdvanceToNextWord();
                        }
                        PlayPhonemeAudio();
                    }
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
    m_spFixedDeltas.reset(new float2[100]);
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
