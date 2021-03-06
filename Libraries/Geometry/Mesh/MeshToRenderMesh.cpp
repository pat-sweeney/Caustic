//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Geometry\Mesh\Mesh.h"
#include "Rendering\Caustic\RenderTypes.h"
#include "Rendering\Caustic\IRenderer.h"
#include "Rendering\Caustic\IRenderMesh.h"
#include "Rendering\Caustic\IShaderInfo.h"
#include <d3d11.h>

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
	// Method: BuildVertexBuffer
	// Creates a vertex buffer using the specified submesh.
	//
	// Parameters:
	// pRenderer - Graphics device
	// pShaderInfo - Information about the shader
	// vertexReferenced - List of vertices that are referenced
	// MeshData - place to store created vertex buffer
	//**********************************************************************
	void CSubMesh::BuildVertexBuffer(IRenderer* pRenderer,
		IShaderInfo* pShaderInfo, std::vector<int>& vertexReferenced, MeshData* pMeshData)
	{
		CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
		std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout = pShaderInfo->VertexLayout();
		uint32 numVerts = GetNumberVertices();
		uint32 vertexSize = pShaderInfo->GetVertexSize();

		if (numVerts == 0)
			return;

		BBox3 bbox;

		// Create buffer with our vertex data
		byte* pVertexBuffer = new byte[numVerts * vertexSize];
		std::unique_ptr<byte> spVertBuffer(pVertexBuffer);
		uint32 vout = 0;
		for (uint32 i = 0; i < numVerts; i++)
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
						fp[0] = pVertex->uvs[0].x;
						fp[1] = pVertex->uvs[0].y;
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
		CT(spDevice->CreateBuffer(&desc, &data, &spVB));
		pMeshData->m_numVertices = vout;
		pMeshData->m_vertexSize = vertexSize;
		pMeshData->m_spVB = spVB;
		pMeshData->m_bbox = bbox;
	}

	//**********************************************************************
	// Method: BuildIndexBuffer
    // Creates a index buffer using the specified submesh.
    //
    // Parameters:
    // pRenderer - Graphics device
    // vertexReferenced - List of vertices that are referenced
    // MeshData - place to store created index buffer
    //**********************************************************************
    void CSubMesh::BuildIndexBuffer(IRenderer *pRenderer, std::vector<int> &vertexReferenced, MeshData *pMeshData)
	{
        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        uint32 numFaces = GetNumberFaces();
		uint32 *pIndexBuffer = new uint32[numFaces * 6 /* 6 is worst case */];
		std::unique_ptr<uint32> spIndexBuffer(pIndexBuffer);
		uint32 numIndices = 0;
		for (uint32 i = 0; i < numFaces; i++)
		{
			CFace *pFace = GetFace(i);
			uint32 numVerts = pFace->GetNumberVertices();
			_ASSERT(numVerts == 3 || numVerts == 4);
			if (numVerts == 3)
			{
				CGeomVertex *v0 = pFace->GetVertex(0);
				CGeomVertex *v1 = pFace->GetVertex(1);
				CGeomVertex *v2 = pFace->GetVertex(2);
				uint32 i0 = VertexToIndex(v0);
				uint32 i1 = VertexToIndex(v1);
				uint32 i2 = VertexToIndex(v2);
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
				uint32 i0 = VertexToIndex(v0);
				uint32 i1 = VertexToIndex(v1);
				uint32 i2 = VertexToIndex(v2);
				uint32 i3 = VertexToIndex(v3);
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
		UINT ibSize = (UINT)(numIndices * sizeof(uint32));
		D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = ibSize;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = sizeof(uint32);
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = pIndexBuffer;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CComPtr<ID3D11Buffer> spIB;
        CT(spDevice->CreateBuffer(&desc, &data, &spIB));
        pMeshData->m_numIndices = numIndices;
        pMeshData->m_spIB = spIB;
	}

	void CSubMesh::BuildReferencedVertexList(std::vector<int> &vertexReferenced)
	{
		uint32 numFaces = GetNumberFaces();
		for (uint32 i = 0; i < numFaces; i++)
		{
			CFace *pFace = GetFace(i);
			uint32 numVerts = pFace->GetNumberVertices();
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
    // Method: ToRenderSubMesh
	// Converts a CSubMesh object into a renderable form
    //
    // Parameters:
	// pRenderer - Renderer
	// pShader - shader
	//
	// Returns:
	// Returns the new submesh
	//**********************************************************************
	CRefObj<IRenderSubMesh> CSubMesh::ToRenderSubMesh(IRenderer *pRenderer, IShader *pShader)
	{
        CRefObj<ICausticFactory> spFactory = Caustic::CreateCausticFactory();
		CRefObj<IRenderSubMesh> spRenderSubMesh = spFactory->CreateRenderSubMesh();
		std::vector<int> vertexReferenced(GetNumberVertices(), c_Vertex_Unreferenced);
		BuildReferencedVertexList(vertexReferenced);
		CRefObj<IShaderInfo> spShaderInfo = pShader->GetShaderInfo();
        MeshData md;
		BuildVertexBuffer(pRenderer, spShaderInfo, vertexReferenced, &md);
		BuildIndexBuffer(pRenderer, vertexReferenced, &md);
        spRenderSubMesh->SetMeshData(md);
		spRenderSubMesh->SetName(GetName().c_str());
		return spRenderSubMesh;
	}

	void CMesh::ToRenderMaterials(IRenderer* pRenderer, IShader* pShader, IRenderMesh* pRenderMesh, IMaterialAttrib *pDefaultMaterial)
	{
		CRefObj<ICausticFactory> spFactory = CreateCausticFactory();
		for (int i = 0; i < m_subMeshes.size(); i++)
		{
			ISubMesh* pSubMesh = m_subMeshes[i];
			CRefObj<IRenderSubMesh> spRenderSubMesh = pRenderMesh->GetSubMesh(i);
			
			// Assign appropriate materials
			CRefObj<IMaterialAttrib> spMaterialAttrib = this->GetMaterial(pSubMesh->GetMaterialID());
			CRefObj<IRenderMaterial> spRenderMaterial = spFactory->CreateRenderMaterial(pRenderer, (spMaterialAttrib) ? spMaterialAttrib : pDefaultMaterial, pShader);
			spRenderSubMesh->SetFrontMaterial(spRenderMaterial);

			// For now assume now back material
		}
	}

	//**********************************************************************
    // Method: ToRenderMesh
    // Converts a CMesh object into a renderable form. NOTE: The client will
	// also have to convert the mesh's materials to a RenderMaterial via
	// <ToRenderMaterial>. The reason this is split out as a separate call
	// (versus just being part of this call) is to allow the client to update
	// the materials without having to completely rebuild the mesh.
    //
    // Parameters:
    // pRenderer - Renderer
    // pShader - shader
	//
	// Returns:
    // Returns the new mesh
    //**********************************************************************
    CRefObj<IRenderMesh> CMesh::ToRenderMesh(IRenderer *pRenderer, IShader *pShader)
    {
        CRefObj<ICausticFactory> spFactory = CreateCausticFactory();
        CRefObj<IRenderMesh> spRenderMesh = spFactory->CreateRenderMesh();
        for (auto pSubMesh : m_subMeshes)
        {
            CRefObj<IRenderSubMesh> spRenderSubMesh = pSubMesh->ToRenderSubMesh(pRenderer, pShader);
            spRenderMesh->AddSubMesh(spRenderSubMesh);
        }
        return spRenderMesh;
    }
};
