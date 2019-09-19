//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include <d3d11.h>
#include "Geometry\Mesh\Mesh.h"
#include "Rendering\Caustic\IGraphics.h"
#include "Renderer.h"

//**********************************************************************
// File: MeshToVB.cpp
// Creates a vertex buffer from an IMesh object
//**********************************************************************

namespace Caustic
{
    const int c_Vertex_Unreferenced = -1;
    const int c_Vertex_Referenced = -2;

    //**********************************************************************
    // Function: IsDegenerate
    // Returns whether the specified three vertices form a degenerate triangle
    // (surface area == 0)
    //
    // Parameters:
    // v0 - vertex 1
    // v1 - vertex 2
    // v2 - vertex 3
    //
    // Returns:
    // True if triangle is degenerate. False otherwise.
    //**********************************************************************
    static bool IsDegenerate(CGeomVertex *v0, CGeomVertex *v1, CGeomVertex *v2)
    {
        Vector3 u = (v1->pos - v0->pos);
        Vector3 v = (v2->pos - v0->pos);
        return IsZero(u.cross(v).Length());
    }

    static void BuildReferencedVertexList(ISubMesh *pMesh, std::vector<int> &vertexReferenced)
    {
        uint32 numFaces = pMesh->GetNumberFaces();
        for (uint32 i = 0; i < numFaces; i++)
        {
            CFace *pFace = pMesh->GetFace(i);
            uint32 numVerts = pFace->GetNumberVertices();
            _ASSERT(numVerts == 3 || numVerts == 4);
            if (numVerts == 3)
            {
                CGeomVertex *v0 = pFace->GetVertex(0);
                CGeomVertex *v1 = pFace->GetVertex(1);
                CGeomVertex *v2 = pFace->GetVertex(2);
                if (!IsDegenerate(v0, v1, v2))
                {
                    vertexReferenced[pMesh->VertexToIndex(v0)] = c_Vertex_Referenced;
                    vertexReferenced[pMesh->VertexToIndex(v1)] = c_Vertex_Referenced;
                    vertexReferenced[pMesh->VertexToIndex(v2)] = c_Vertex_Referenced;
                }
            }
            else if (numVerts == 4)
            {
                CGeomVertex *v0 = pFace->GetVertex(0);
                CGeomVertex *v1 = pFace->GetVertex(1);
                CGeomVertex *v2 = pFace->GetVertex(2);
                CGeomVertex *v3 = pFace->GetVertex(3);
                if (!IsDegenerate(v0, v1, v2))
                {
                    vertexReferenced[pMesh->VertexToIndex(pFace->GetVertex(0))] = c_Vertex_Referenced;
                    vertexReferenced[pMesh->VertexToIndex(pFace->GetVertex(1))] = c_Vertex_Referenced;
                    vertexReferenced[pMesh->VertexToIndex(pFace->GetVertex(2))] = c_Vertex_Referenced;
                }
                if (!IsDegenerate(v0, v2, v3))
                {
                    vertexReferenced[pMesh->VertexToIndex(pFace->GetVertex(0))] = c_Vertex_Referenced;
                    vertexReferenced[pMesh->VertexToIndex(pFace->GetVertex(2))] = c_Vertex_Referenced;
                    vertexReferenced[pMesh->VertexToIndex(pFace->GetVertex(3))] = c_Vertex_Referenced;
                }
            }
        }
    }

    static void BuildIndexBufferData_V1(ISubMesh *pMesh, uint32 *pNumIndices, uint32 **ppIndexData, std::vector<int> &vertexReferenced)
    {
        uint32 numFaces = pMesh->GetNumberFaces();
        uint32 *pIndexBuffer = new uint32[numFaces * 6 /* 6 is worst case */];
        std::unique_ptr<uint32> spIndexBuffer(pIndexBuffer);
        uint32 outputIndex = 0;
        for (uint32 i = 0; i < numFaces; i++)
        {
            CFace *pFace = pMesh->GetFace(i);
            uint32 numVerts = pFace->GetNumberVertices();
            _ASSERT(numVerts == 3 || numVerts == 4);
            if (numVerts == 3)
            {
                CGeomVertex *v0 = pFace->GetVertex(0);
                CGeomVertex *v1 = pFace->GetVertex(1);
                CGeomVertex *v2 = pFace->GetVertex(2);
                uint32 i0 = pMesh->VertexToIndex(v0);
                uint32 i1 = pMesh->VertexToIndex(v1);
                uint32 i2 = pMesh->VertexToIndex(v2);
                if (!IsDegenerate(v0, v1, v2))
                {
                    pIndexBuffer[outputIndex++] = vertexReferenced[i0];
                    pIndexBuffer[outputIndex++] = vertexReferenced[i1];
                    pIndexBuffer[outputIndex++] = vertexReferenced[i2];
                }
            }
            else if (numVerts == 4)
            {
                CGeomVertex *v0 = pFace->GetVertex(0);
                CGeomVertex *v1 = pFace->GetVertex(1);
                CGeomVertex *v2 = pFace->GetVertex(2);
                CGeomVertex *v3 = pFace->GetVertex(3);
                uint32 i0 = pMesh->VertexToIndex(v0);
                uint32 i1 = pMesh->VertexToIndex(v1);
                uint32 i2 = pMesh->VertexToIndex(v2);
                uint32 i3 = pMesh->VertexToIndex(v3);
                if (!IsDegenerate(v0, v1, v2))
                {
                    pIndexBuffer[outputIndex++] = vertexReferenced[i0];
                    pIndexBuffer[outputIndex++] = vertexReferenced[i1];
                    pIndexBuffer[outputIndex++] = vertexReferenced[i2];
                }
                if (!IsDegenerate(v0, v2, v3))
                {
                    pIndexBuffer[outputIndex++] = vertexReferenced[i0];
                    pIndexBuffer[outputIndex++] = vertexReferenced[i2];
                    pIndexBuffer[outputIndex++] = vertexReferenced[i3];
                }
            }
        }
        *pNumIndices = outputIndex;
        *ppIndexData = spIndexBuffer.release();
    }

    static void BuildIndexBufferGPU(ID3D11Device *pDevice, uint32 *pIndexData, uint32 numIndices, ID3D11Buffer **ppBuffer)
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(uint32) * numIndices;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = sizeof(uint32);
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = pIndexData;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CT(pDevice->CreateBuffer(&desc, &data, ppBuffer));
    }

    //**********************************************************************
    // Function:BuildIndexBuffer_V1
    // This function builds an index buffer using the specified mesh. Each
    // index is a V1 index (trilist where each index is a uint32)
    //
    // Parameters:
    // pDevice - D3D device
    // pMesh - Mesh for which to create index buffer
    // ppBuffer - Returns the created index buffer
    //**********************************************************************
    static void BuildIndexBuffer_V1(ID3D11Device *pDevice, ISubMesh *pMesh, ID3D11Buffer **ppBuffer, uint32 *pNumIndices,
                                    std::vector<int> &vertexReferenced)
    {
        uint32 *pIndexData;
        BuildIndexBufferData_V1(pMesh, pNumIndices, &pIndexData, vertexReferenced);
        std::unique_ptr<uint32[]> spIndexData(pIndexData);
        BuildIndexBufferGPU(pDevice, pIndexData, *pNumIndices, ppBuffer);
    }

#if 0
    CAUSTICAPI void StoreSubMeshRenderableDataToStream(IStream *pStream, ISubMesh *pMesh, int vertexVersion, int indexVersion)
    {
        std::vector<int> vertexReferenced(pMesh->GetNumberVertices(), c_Vertex_Unreferenced);
        BuildReferencedVertexList(pMesh, vertexReferenced);
        ULONG bytesWritten;
        CT(pStream->Write(&vertexVersion, sizeof(vertexVersion), &bytesWritten));
        CT(pStream->Write(&indexVersion, sizeof(indexVersion), &bytesWritten));
        switch (vertexVersion)
        {
        case 1:
            {
                uint32 numVertices;
                CT(pStream->Write(&numVertices, sizeof(numVertices), &bytesWritten));
                for (uint32 i = 0; i < numVertices; i++)
                    CT(pStream->Write(&pVertices[i], sizeof(SVertex_1), &bytesWritten));
            }
            break;
        case 2:
            {
                uint32 numVertices;
                SVertex_2 *pVertices;
                BuildVertexBufferData_V2(pMesh, &numVertices, &pVertices, vertexReferenced);
                std::unique_ptr<SVertex_2> spVertices(pVertices);
                CT(pStream->Write(&numVertices, sizeof(numVertices), &bytesWritten));
                for (uint32 i = 0; i < numVertices; i++)
                    CT(pStream->Write(&pVertices[i], sizeof(SVertex_2), &bytesWritten));
            }
            break;
        }
        switch (indexVersion)
        {
        case 1:
            {
                uint32 numIndices;
                uint32 *pIndexData;
                BuildIndexBufferData_V1(pMesh, &numIndices, &pIndexData, vertexReferenced);
                std::unique_ptr<uint32> spIndexData(pIndexData);
                CT(pStream->Write(&numIndices, sizeof(numIndices), &bytesWritten));
                for (uint32 i = 0; i < numIndices; i++)
                    CT(pStream->Write(&pIndexData[i], sizeof(uint32), &bytesWritten));
            }
            break;
        }
    }

    //**********************************************************************
    // Function: LoadSubMeshRenderableDataFromStream
    // This function is for loading the renderable data that was stored using StoreSubMeshRenderableDataToStream().
    //**********************************************************************
    CAUSTICAPI void LoadSubMeshRenderableDataFromStream(IStream *pStream, ID3D11Device *pDevice, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVertices, int *pVertexVersion, int *pIndexVersion)
    {
        ULONG bytesRead;
        CT(pStream->Read(pVertexVersion, sizeof(int), &bytesRead));
        CT(pStream->Read(pIndexVersion, sizeof(int), &bytesRead));
        switch (*pVertexVersion)
        {
        case 1:
            {
                CT(pStream->Read(pNumVertices, sizeof(uint32), &bytesRead));
                std::unique_ptr<SVertex_1[]> spVertices(new SVertex_1[*pNumVertices]);
                for (uint32 i = 0; i < *pNumVertices; i++)
                    CT(pStream->Read(&spVertices[i], sizeof(SVertex_1), &bytesRead));
                BuildVertexBufferGPU_V1(pDevice, *pNumVertices, spVertices.get(), ppVertexBuffer);
            }
            break;
        case 2:
            {
                CT(pStream->Read(pNumVertices, sizeof(uint32), &bytesRead));
                std::unique_ptr<SVertex_2[]> spVertices(new SVertex_2[*pNumVertices]);
                for (uint32 i = 0; i < *pNumVertices; i++)
                    CT(pStream->Read(&spVertices[i], sizeof(SVertex_2), &bytesRead));
                BuildVertexBufferGPU_V2(pDevice, *pNumVertices, spVertices.get(), ppVertexBuffer);
            }
            break;
        }
        switch (*pIndexVersion)
        {
        case 1:
            {
                CT(pStream->Read(pNumIndices, sizeof(uint32), &bytesRead));
                std::unique_ptr<uint32[]> spIndexData(new uint32[*pNumIndices]);
                for (uint32 i = 0; i < *pNumIndices; i++)
                    CT(pStream->Read(&(spIndexData.get())[i], sizeof(uint32), &bytesRead));
                BuildIndexBufferGPU(pDevice, spIndexData.get(), *pNumIndices, ppIndexBuffer);
            }
            break;
        }
    }
#endif
};
