#if 0
//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include <d3d12.h>
#include "Geometry\Mesh\Mesh.h"
#include "Renderer.h"

namespace Caustic
{
    const int c_Vertex_Unreferenced = -1;
    const int c_Vertex_Referenced = -2;

    //**********************************************************************
    // BuildVertexBufferData_V1() creates a vertex buffer for
    //! vertex type V1 using the specified submesh.
    // pMesh Submesh to use for vertex data
    //! \param[out] pNumVerts Returns the number of vertices in the output vertex buffer
    //! \param[out] ppVertexBuffer Returns pointer to array of vertices
    //! \param[out] Returns the index of each referenced vertex
    //**********************************************************************
    static void BuildVertexBufferData_V1(ISubMesh *pMesh, uint32 *pNumVerts, SVertex_1 **ppVertexBuffer, std::vector<int> &vertexReferenced)
    {
        uint32 numVerts = pMesh->GetNumberVertices();
        SVertex_1 *pVertexBuffer = new SVertex_1[numVerts];
        std::unique_ptr<SVertex_1[]> spVertBuffer(pVertexBuffer);
        uint32 vout = 0;
        for (uint32 i = 0; i < numVerts; i++)
        {
            if (vertexReferenced[i] == c_Vertex_Referenced)
            {
                CGeomVertex *pVertex = pMesh->GetVertex(i);
                pVertexBuffer[vout].m_pos[0] = pVertex->pos.x;
                pVertexBuffer[vout].m_pos[1] = pVertex->pos.y;
                pVertexBuffer[vout].m_pos[2] = pVertex->pos.z;
                pVertexBuffer[vout].m_uvs[0] = pVertex->uvs[0].x;
                pVertexBuffer[vout].m_uvs[1] = pVertex->uvs[0].y;
                vertexReferenced[i] = vout++;
            }
        }
        *pNumVerts = vout;
        *ppVertexBuffer = spVertBuffer.release();
    }
    
    static void BuildVertexBufferGPU_V1(ID3D12Device *pDevice, uint32 numVerts, SVertex_1 *pVertexBuffer, ID3D12Resource **ppBuffer)
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(SVertex_1) * numVerts;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = sizeof(SVertex_1);

        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = pVertexBuffer;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CT(pDevice->CreateBuffer(&desc, &data, ppBuffer));
    }
    
    //**********************************************************************
    // BuildVertexBuffer_V1 builds a vertex buffer using the specified mesh
    // pDevice Our D3D device
    // pMesh Mesh for which to build vertex buffer
    //! \param[out] ppBuffer Returns the created vertex buffer
    //!
    //! This function builds a vertex buffer using the specified mesh. Each
    //! vertex is a V1 vertex (position+UV)
    //**********************************************************************
    static void BuildVertexBuffer_V1(ID3D12Device *pDevice, ISubMesh *pMesh, ID3D11Buffer **ppBuffer, uint32 *pNumVerts,
        std::vector<int> &vertexReferenced)
    {
        SVertex_1 *pVertexBuffer;
        BuildVertexBufferData_V1(pMesh, pNumVerts, &pVertexBuffer, vertexReferenced);
        std::unique_ptr<SVertex_1[]> spVertBuffer(pVertexBuffer);
        BuildVertexBufferGPU_V1(pDevice, *pNumVerts, pVertexBuffer, ppBuffer);
    }

    static void BuildVertexBufferData_V2(ISubMesh *pMesh, uint32 *pNumVerts, SVertex_2 **ppVertexBuffer, std::vector<int> &vertexReferenced)
    {
        uint32 numVerts = pMesh->GetNumberVertices();
        SVertex_2 *pVertexBuffer = new SVertex_2[numVerts];
        std::unique_ptr<SVertex_2[]> spVertBuffer(pVertexBuffer);
        uint32 vout = 0;
        for (uint32 i = 0; i < numVerts; i++)
        {
            if (vertexReferenced[i] == c_Vertex_Referenced)
            {
                CGeomVertex *pVertex = pMesh->GetVertex(i);
                pVertexBuffer[vout].m_pos[0] = pVertex->pos.x;
                pVertexBuffer[vout].m_pos[1] = pVertex->pos.y;
                pVertexBuffer[vout].m_pos[2] = pVertex->pos.z;
                pVertexBuffer[vout].m_norm[0] = pVertex->norm.x;
                pVertexBuffer[vout].m_norm[1] = pVertex->norm.y;
                pVertexBuffer[vout].m_norm[2] = pVertex->norm.z;
                pVertexBuffer[vout].m_uvs[0] = pVertex->uvs[0].x;
                pVertexBuffer[vout].m_uvs[1] = pVertex->uvs[0].y;
                vertexReferenced[i] = vout++;
            }
        }
        *pNumVerts = vout;
        *ppVertexBuffer = spVertBuffer.release();
    }

    static void BuildVertexBufferGPU_V2(ID3D11Device *pDevice, uint32 numVerts, SVertex_2 *pVertexBuffer, ID3D11Buffer **ppBuffer)
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(SVertex_2) * numVerts;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = sizeof(SVertex_2);

        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = pVertexBuffer;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CT(pDevice->CreateBuffer(&desc, &data, ppBuffer));
    }

    //**********************************************************************
    // BuildVertexBuffer_V2 builds a vertex buffer using the specified mesh
    // pDevice Our D3D device
    // pMesh Mesh for which to build vertex buffer
    //! \param[out] ppBuffer Returns the created vertex buffer
    //!
    //! This function builds a vertex buffer using the specified mesh. Each
    //! vertex is a V2 vertex (position+normal+UV)
    //**********************************************************************
    static void BuildVertexBuffer_V2(ID3D11Device *pDevice, ISubMesh *pMesh, ID3D11Buffer **ppBuffer, uint32 *pNumVerts,
        std::vector<int> &vertexReferenced)
    {
        SVertex_2 *pVertexBuffer;
        BuildVertexBufferData_V2(pMesh, pNumVerts, &pVertexBuffer, vertexReferenced);
        std::unique_ptr<SVertex_2> spVertexBuffer(pVertexBuffer);
        BuildVertexBufferGPU_V2(pDevice, *pNumVerts, pVertexBuffer, ppBuffer);
    }

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
    // BuildIndexBuffer_V1 builds an index buffer using the specified mesh
    // pDevice D3D device
    // pMesh Mesh for which to create index buffer
    //! \param[out] ppBuffer Returns the created index buffer
    //!
    //! This function builds an index buffer using the specified mesh. Each
    //! index is a V1 index (trilist where each index is a uint32)
    //**********************************************************************
    static void BuildIndexBuffer_V1(ID3D11Device *pDevice, ISubMesh *pMesh, ID3D11Buffer **ppBuffer, uint32 *pNumIndices,
                                    std::vector<int> &vertexReferenced)
    {
        uint32 *pIndexData;
        BuildIndexBufferData_V1(pMesh, pNumIndices, &pIndexData, vertexReferenced);
        std::unique_ptr<uint32[]> spIndexData(pIndexData);
        BuildIndexBufferGPU(pDevice, pIndexData, *pNumIndices, ppBuffer);
    }

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
                SVertex_1 *pVertices;
                BuildVertexBufferData_V1(pMesh, &numVertices, &pVertices, vertexReferenced);
                std::unique_ptr<SVertex_1> spVertices(pVertices);
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
    // Loads the renderable data from a stream
    //!
    //! This function is for loading the renderable data that was stored using StoreSubMeshRenderableDataToStream().
    //!
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

    //**********************************************************************
    /// MeshToD3D converts a CMesh object into a renderable form
    /// pGraphics Graphics device to use
    /// pMesh Mesh to convert
    /// vertexVersion Version of vertex to use
    ///! \param[out] ppVertexBuffer Returns the created vertex buffer
    /// indexVersion Version of index buffer to use
    ///! \param[out] ppIndexBuffer Returns the created index buffer
    ///! \param[out] pBbox Returns the bounding box of the mesh
    ///! \param[out] pVertexSize Returns the size of each vertex in bytes
    //**********************************************************************
    CAUSTICAPI void MeshToD3D(IGraphics *pGraphics, ISubMesh *pMesh,
        int vertexVersion, ID3D11Buffer **ppVertexBuffer, uint32 *pNumVerts,
        int indexVersion, ID3D11Buffer **ppIndexBuffer, uint32 *pNumIndices,
        BBox3 * /*pBbox*/, uint32 *pVertexSize)
    {
        std::vector<int> vertexReferenced(pMesh->GetNumberVertices(), c_Vertex_Unreferenced);
        BuildReferencedVertexList(pMesh, vertexReferenced);
        switch (vertexVersion)
        {
        case 1:
            if (pVertexSize)
                *pVertexSize = sizeof(SVertex_1);
            BuildVertexBuffer_V1(pGraphics->GetDevice(), pMesh, ppVertexBuffer, pNumVerts, vertexReferenced);
            break;
        case 2:
            if (pVertexSize)
                *pVertexSize = sizeof(SVertex_2);
            BuildVertexBuffer_V2(pGraphics->GetDevice(), pMesh, ppVertexBuffer, pNumVerts, vertexReferenced);
            break;
        }
        switch (indexVersion)
        {
        case 1:
            BuildIndexBuffer_V1(pGraphics->GetDevice(), pMesh, ppIndexBuffer, pNumIndices, vertexReferenced);
            break;
        }
    }

    CAUSTICAPI void MeshToNormals(IGraphics *pGraphics, ISubMesh *pSubMesh, ID3D11Buffer **ppVB, uint32 *pNumVerts)
    {
        D3D11_BUFFER_DESC desc = { 0 };
        uint32 numVerts = pSubMesh->GetNumberVertices();
        uint32 numFaces = pSubMesh->GetNumberFaces();
        uint32 numPoints = 2 * numVerts + 2 * numFaces;
        desc.ByteWidth = sizeof(SVertex_4) * numPoints;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = sizeof(SVertex_4);

        SVertex_4 *pVertexBuffer = new SVertex_4[numPoints];
        std::unique_ptr<SVertex_4[]> spVertBuffer(pVertexBuffer);
        uint32 outVIndex = 0;
        for (uint32 i = 0; i < numVerts; i++)
        {
            CGeomVertex *pVertex = pSubMesh->GetVertex(i);
            pVertexBuffer[outVIndex].m_pos[0] = pVertex->pos.x;
            pVertexBuffer[outVIndex].m_pos[1] = pVertex->pos.y;
            pVertexBuffer[outVIndex].m_pos[2] = pVertex->pos.z;
            pVertexBuffer[outVIndex].m_dir[0] = 0.0f;
            pVertexBuffer[outVIndex].m_dir[1] = 0.0f;
            pVertexBuffer[outVIndex].m_dir[2] = 0.0f;
            pVertexBuffer[outVIndex].m_dir[3] = 1.0f;
            outVIndex++;
            pVertexBuffer[outVIndex].m_pos[0] = pVertex->pos.x;
            pVertexBuffer[outVIndex].m_pos[1] = pVertex->pos.y;
            pVertexBuffer[outVIndex].m_pos[2] = pVertex->pos.z;
            pVertexBuffer[outVIndex].m_dir[0] = pVertex->norm.x;
            pVertexBuffer[outVIndex].m_dir[1] = pVertex->norm.y;
            pVertexBuffer[outVIndex].m_dir[2] = pVertex->norm.z;
            pVertexBuffer[outVIndex].m_dir[3] = 1.0f;
            outVIndex++;
        }
        for (uint32 i = 0; i < numFaces; i++)
        {
            CFace *pFace = pSubMesh->GetFace(i);
            pVertexBuffer[outVIndex].m_pos[0] = pFace->GetCenter().x;
            pVertexBuffer[outVIndex].m_pos[1] = pFace->GetCenter().y;
            pVertexBuffer[outVIndex].m_pos[2] = pFace->GetCenter().z;
            pVertexBuffer[outVIndex].m_dir[0] = 0.0f;
            pVertexBuffer[outVIndex].m_dir[1] = 0.0f;
            pVertexBuffer[outVIndex].m_dir[2] = 0.0f;
            pVertexBuffer[outVIndex].m_dir[3] = 0.0f;
            outVIndex++;
            pVertexBuffer[outVIndex].m_pos[0] = pFace->GetCenter().x;
            pVertexBuffer[outVIndex].m_pos[1] = pFace->GetCenter().y;
            pVertexBuffer[outVIndex].m_pos[2] = pFace->GetCenter().z;
            pVertexBuffer[outVIndex].m_dir[0] = pFace->GetNormal().x/10.0f;
            pVertexBuffer[outVIndex].m_dir[1] = pFace->GetNormal().y/10.0f;
            pVertexBuffer[outVIndex].m_dir[2] = pFace->GetNormal().z/10.0f;
            pVertexBuffer[outVIndex].m_dir[3] = 0.0f;
            outVIndex++;
        }
        *pNumVerts = numPoints;
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = pVertexBuffer;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CT(pGraphics->GetDevice()->CreateBuffer(&desc, &data, ppVB));
    }
};
#endif