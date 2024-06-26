//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include <d3d11.h>
#include <memory>
#include <atlbase.h>
#include <Windows.h>
#include <string>
import Base.Core.Core;
import Base.Core.Error;
import Geometry.Mesh.RenderTypes;
import Geometry.Mesh.Mesh;
import Base.Math.Vector;

namespace Caustic
{
    const int c_Vertex_Unreferenced = -1;
    const int c_Vertex_Referenced = -2;

    //**********************************************************************
    // Function: IsDegenerate
    // Determines if the specified triangle is degenerate (i.e. area == 0)
    //
    // Parameters:
    // v0 - Position of vertex 1
    // v1 - Position of vertex 2
    // v2 - Position of vertex 3
    //
    // Returns:
    // True if triangle is degenerate. False otherwise.
    //**********************************************************************
    static bool IsDegenerate(CGeomVertex *v0, CGeomVertex *v1, CGeomVertex *v2)
    {
        Vector3 u = (v1->pos - v0->pos);
        Vector3 v = (v2->pos - v0->pos);
        return IsZero(u.Cross(v).Length());
    }

    //**********************************************************************
    void CSubMesh::BuildVertexBuffer(ID3D11Device* pDevice, std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout, uint32_t vertexSize,
        std::vector<int>& vertexReferenced, MeshData* pMeshData)
    {
        uint32_t numVerts = GetNumberVertices();

        if (numVerts == 0)
            return;

        BBox3 bbox;

        // Create buffer with our vertex data
        byte* pVertexBuffer = new byte[numVerts * vertexSize];
        std::unique_ptr<byte> spVertBuffer(pVertexBuffer);
        uint32_t vout = 0;
        for (uint32_t i = 0; i < numVerts; i++)
        {
            if (vertexReferenced[i] == c_Vertex_Referenced)
            {
                CGeomVertex* pVertex = GetVertex(i);
                byte* pVB = &pVertexBuffer[vout * vertexSize];
                for (size_t j = 0; j < vertexLayout.size(); j++)
                {
                    if (_strnicmp(vertexLayout[j].SemanticName, "POSITION", 8) == 0 ||
                        _strnicmp(vertexLayout[j].SemanticName, "SV_POSITION", 11) == 0)
                    {
                        _ASSERT(vertexLayout[j].Format == DXGI_FORMAT_R32G32B32_FLOAT);
                        float* fp = (float*)pVB;
                        fp[0] = pVertex->pos.x;
                        fp[1] = pVertex->pos.y;
                        fp[2] = pVertex->pos.z;
                        bbox.AddPoint(pVertex->pos);
                        pVB += sizeof(float) * 3;
                    }
                    else if (_strnicmp(vertexLayout[j].SemanticName, "TEXCOORD", 8) == 0)
                    {
                        _ASSERT(vertexLayout[j].Format == DXGI_FORMAT_R32G32_FLOAT);
                        float* fp = (float*)pVB;
                        fp[0] = pVertex->uvs[vertexLayout[j].SemanticIndex].x;
                        fp[1] = pVertex->uvs[vertexLayout[j].SemanticIndex].y;
                        pVB += sizeof(float) * 2;
                    }
                    else if (_strnicmp(vertexLayout[j].SemanticName, "NORMAL", 6) == 0)
                    {
                        _ASSERT(vertexLayout[j].Format == DXGI_FORMAT_R32G32B32_FLOAT);
                        float* fp = (float*)pVB;
                        fp[0] = pVertex->norm.x;
                        fp[1] = pVertex->norm.y;
                        fp[2] = pVertex->norm.z;
                        pVB += sizeof(float) * 3;
                    }
                }
                vertexReferenced[i] = vout++;
            }
        }

        UINT vbSize = (UINT)(numVerts * vertexSize);
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = vbSize;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = vertexSize;
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = pVertexBuffer;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CComPtr<ID3D11Buffer> spVB;
        CT(pDevice->CreateBuffer(&desc, &data, &spVB));
        pMeshData->m_numVertices = vout;
        pMeshData->m_vertexSize = vertexSize;
        pMeshData->m_spVB = spVB;
        pMeshData->m_bbox = bbox;
    }

    //**********************************************************************
    void CSubMesh::BuildIndexBuffer(ID3D11Device* pDevice, std::vector<int> &vertexReferenced, MeshData *pMeshData)
    {
        uint32_t numFaces = GetNumberFaces();
        uint32_t *pIndexBuffer = new uint32_t[numFaces * 6 /* 6 is worst case */];
        std::unique_ptr<uint32_t> spIndexBuffer(pIndexBuffer);
        uint32_t numIndices = 0;
        for (uint32_t i = 0; i < numFaces; i++)
        {
            CFace *pFace = GetFace(i);
            uint32_t numVerts = pFace->GetNumberVertices();
            _ASSERT(numVerts == 3 || numVerts == 4);
            if (numVerts == 3)
            {
                CGeomVertex *v0 = pFace->GetVertex(0);
                CGeomVertex *v1 = pFace->GetVertex(1);
                CGeomVertex *v2 = pFace->GetVertex(2);
                uint32_t i0 = VertexToIndex(v0);
                uint32_t i1 = VertexToIndex(v1);
                uint32_t i2 = VertexToIndex(v2);
                if (!IsDegenerate(v0, v1, v2))
                {
                    pIndexBuffer[numIndices++] = vertexReferenced[i0];
                    pIndexBuffer[numIndices++] = vertexReferenced[i1];
                    pIndexBuffer[numIndices++] = vertexReferenced[i2];
                }
            }
            else if (numVerts == 4)
            {
                CGeomVertex *v0 = pFace->GetVertex(0);
                CGeomVertex *v1 = pFace->GetVertex(1);
                CGeomVertex *v2 = pFace->GetVertex(2);
                CGeomVertex *v3 = pFace->GetVertex(3);
                uint32_t i0 = VertexToIndex(v0);
                uint32_t i1 = VertexToIndex(v1);
                uint32_t i2 = VertexToIndex(v2);
                uint32_t i3 = VertexToIndex(v3);
                if (!IsDegenerate(v0, v1, v2))
                {
                    pIndexBuffer[numIndices++] = vertexReferenced[i0];
                    pIndexBuffer[numIndices++] = vertexReferenced[i1];
                    pIndexBuffer[numIndices++] = vertexReferenced[i2];
                }
                if (!IsDegenerate(v0, v2, v3))
                {
                    pIndexBuffer[numIndices++] = vertexReferenced[i0];
                    pIndexBuffer[numIndices++] = vertexReferenced[i2];
                    pIndexBuffer[numIndices++] = vertexReferenced[i3];
                }
            }
        }
        
        if (numIndices == 0)
            return;
        UINT ibSize = (UINT)(numIndices * sizeof(uint32_t));
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = ibSize;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = sizeof(uint32_t);
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = pIndexBuffer;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CComPtr<ID3D11Buffer> spIB;
        CT(pDevice->CreateBuffer(&desc, &data, &spIB));
        pMeshData->m_numIndices = numIndices;
        pMeshData->m_spIB = spIB;
    }

    void CSubMesh::BuildReferencedVertexList(std::vector<int> &vertexReferenced)
    {
        uint32_t numFaces = GetNumberFaces();
        for (uint32_t i = 0; i < numFaces; i++)
        {
            CFace *pFace = GetFace(i);
            uint32_t numVerts = pFace->GetNumberVertices();
            _ASSERT(numVerts == 3 || numVerts == 4);
            if (numVerts == 3)
            {
                CGeomVertex *v0 = pFace->GetVertex(0);
                CGeomVertex *v1 = pFace->GetVertex(1);
                CGeomVertex *v2 = pFace->GetVertex(2);
                if (!IsDegenerate(v0, v1, v2))
                {
                    vertexReferenced[VertexToIndex(v0)] = c_Vertex_Referenced;
                    vertexReferenced[VertexToIndex(v1)] = c_Vertex_Referenced;
                    vertexReferenced[VertexToIndex(v2)] = c_Vertex_Referenced;
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
                    vertexReferenced[VertexToIndex(pFace->GetVertex(0))] = c_Vertex_Referenced;
                    vertexReferenced[VertexToIndex(pFace->GetVertex(1))] = c_Vertex_Referenced;
                    vertexReferenced[VertexToIndex(pFace->GetVertex(2))] = c_Vertex_Referenced;
                }
                if (!IsDegenerate(v0, v2, v3))
                {
                    vertexReferenced[VertexToIndex(pFace->GetVertex(0))] = c_Vertex_Referenced;
                    vertexReferenced[VertexToIndex(pFace->GetVertex(2))] = c_Vertex_Referenced;
                    vertexReferenced[VertexToIndex(pFace->GetVertex(3))] = c_Vertex_Referenced;
                }
            }
        }
    }

    //**********************************************************************
    MeshData CSubMesh::ToMeshData(ID3D11Device* pDevice, std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout, uint32_t vertexSize)
    {
        std::vector<int> vertexReferenced(GetNumberVertices(), c_Vertex_Unreferenced);
        BuildReferencedVertexList(vertexReferenced);
        MeshData md;
        BuildVertexBuffer(pDevice, vertexLayout, vertexSize, vertexReferenced, &md);
        BuildIndexBuffer(pDevice, vertexReferenced, &md);
        md.m_name = std::string(GetName().c_str());
        return md;
    }

    //**********************************************************************
    // Method: ToMeshData
    // Converts a CMesh object into a renderable form. NOTE: The client will
    // also have to convert the mesh's materials to a RenderMaterial via
    // <ToRenderMaterial>. The reason this is split out as a separate call
    // (versus just being part of this call) is to allow the client to update
    // the materials without having to completely rebuild the mesh.
    //
    // Parameters:
    // pRenderer - Renderer
    // vertexLayout - layout of each vertex
    // vertexSize - size of each vertex in bytes
    //
    // Returns:
    // Returns array of meshData for each submesh
    //**********************************************************************
    std::vector<MeshData> CMesh::ToMeshData(ID3D11Device* pDevice, std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout, uint32_t vertexSize)
    {
        std::vector<MeshData> mdv;
        for (auto pSubMesh : m_subMeshes)
        {
            MeshData md = pSubMesh->ToMeshData(pDevice, vertexLayout, vertexSize);
            mdv.push_back(md);
        }
        return mdv;
    }
};
