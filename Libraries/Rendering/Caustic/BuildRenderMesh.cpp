//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Rendering\Caustic\CausticFactory.h"
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
	static bool IsDegenerate(CGeomVertex* v0, CGeomVertex* v1, CGeomVertex* v2)
	{
		Vector3 u = (v1->pos - v0->pos);
		Vector3 v = (v2->pos - v0->pos);
		return IsZero(u.Cross(v).Length());
	}

	//**********************************************************************
	// Method: BuildVertexBuffer
	// Creates a vertex buffer using the specified vertex list.
	//
	// Parameters:
	// pRenderer - Graphics device
	// verts - list of vertices
	// pShaderInfo - Information about the shader
	// vertexReferenced - List indicating which vertices were referenced by the face list
	// MeshData - place to store created vertex buffer
	//**********************************************************************
	static void BuildVertexBuffer(IRenderer* pRenderer,
		std::vector<CGeomVertex> &verts, IShaderInfo* pShaderInfo,
		std::vector<int>& vertexReferenced, MeshData* pMeshData)
	{
		CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
		std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout = pShaderInfo->VertexLayout();
		uint32 numVerts = uint32(verts.size());
		uint32 vertexSize = pShaderInfo->GetVertexSize();

		BBox3 bbox;

		// Create buffer with our vertex data
		byte* pVertexBuffer = new byte[numVerts * vertexSize];
		std::unique_ptr<byte> spVertBuffer(pVertexBuffer);
		uint32 vout = 0;
		for (uint32 i = 0; i < numVerts; i++)
		{
			if (vertexReferenced[i] == c_Vertex_Referenced)
			{
				CGeomVertex* pVertex = &verts[i];
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
		CT(spDevice->CreateBuffer(&desc, &data, &spVB));
		pMeshData->m_numVertices = vout;
		pMeshData->m_vertexSize = vertexSize;
		pMeshData->m_spVB = spVB;
		pMeshData->m_bbox = bbox;
	}

	//**********************************************************************
	// Method: BuildIndexBuffer
	// Creates a index buffer using the specified vertex and face lists.
	//
	// Parameters:
	// pRenderer - Graphics device
	// faces - list of faces
	// verts - list of vertices
	// vertexReferenced - List indicating which vertices were referenced by the face list
	// MeshData - place to store created index buffer
	//**********************************************************************
	void BuildIndexBuffer(IRenderer* pRenderer, std::vector<CGeomFace>& faces,
		std::vector<CGeomVertex>& verts,
		std::vector<int>& vertexReferenced, MeshData* pMeshData)
	{
		CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
		uint32 numFaces = (uint32)faces.size();
		uint32* pIndexBuffer = new uint32[numFaces * 3];
		std::unique_ptr<uint32> spIndexBuffer(pIndexBuffer);
		uint32 numIndices = 0;
		for (uint32 i = 0; i < numFaces; i++)
		{
			CGeomFace* pFace = &faces[i];
			uint32 i0 = pFace->indices[0];
			uint32 i1 = pFace->indices[1];
			uint32 i2 = pFace->indices[2];
			CGeomVertex* v0 = &verts[i0];
			CGeomVertex* v1 = &verts[i1];
			CGeomVertex* v2 = &verts[i2];
			if (!IsDegenerate(v0, v1, v2))
			{
				pIndexBuffer[numIndices++] = vertexReferenced[i0];
				pIndexBuffer[numIndices++] = vertexReferenced[i1];
				pIndexBuffer[numIndices++] = vertexReferenced[i2];
			}
		}

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

	//**********************************************************************
	// Method: BuildReferencedVertexList
	// Determines which vertices are part of non-degenerate faces. This function
	// is used to prune out degenerate faces when building the vertex/index buffers.
	//
	// Parameters:
	// faces - list of faces
	// verts - list of vertices
	// vertexReferenced - List indicating which vertices were referenced by the face list
	//**********************************************************************
	static void BuildReferencedVertexList(std::vector<CGeomFace> &faces,
		std::vector<CGeomVertex> &verts,
		std::vector<int>& vertexReferenced)
	{
		uint32 numFaces = (uint32)faces.size();
		for (uint32 i = 0; i < numFaces; i++)
		{
			CGeomFace* pFace = &faces[i];
			CGeomVertex* v0 = &verts[pFace->indices[0]];
			CGeomVertex* v1 = &verts[pFace->indices[1]];
			CGeomVertex* v2 = &verts[pFace->indices[2]];
			if (!IsDegenerate(v0, v1, v2))
			{
				vertexReferenced[pFace->indices[0]] = c_Vertex_Referenced;
				vertexReferenced[pFace->indices[1]] = c_Vertex_Referenced;
				vertexReferenced[pFace->indices[2]] = c_Vertex_Referenced;
			}
		}
	}

	//**********************************************************************
	// Method: BuildRenderSubMesh
	// Converts a list of faces and vertices into a IRenderSubMesh.
	//
	// Parameters:
	// pRenderer - Renderer
	// faces - list of faces
	// verts - list of vertices
	// pShader - shader
	//
	// Returns:
	// Returns the new render submesh
	//**********************************************************************
	CAUSTICAPI CRefObj<IRenderSubMesh> BuildRenderSubMesh(IRenderer* pRenderer, 
		std::vector<CGeomFace> &faces, std::vector<CGeomVertex> &verts,
		IShader* pShader)
	{
		CRefObj<ICausticFactory> spFactory = Caustic::CreateCausticFactory();
		CRefObj<IRenderSubMesh> spRenderSubMesh = spFactory->CreateRenderSubMesh();
		std::vector<int> vertexReferenced(verts.size(), c_Vertex_Unreferenced);
		BuildReferencedVertexList(faces, verts, vertexReferenced);
		CRefObj<IShaderInfo> spShaderInfo = pShader->GetShaderInfo();
		MeshData md;
		BuildVertexBuffer(pRenderer, verts, spShaderInfo, vertexReferenced, &md);
		BuildIndexBuffer(pRenderer, faces, verts, vertexReferenced, &md);
		spRenderSubMesh->SetMeshData(md);
		//// spRenderSubMesh->SetShader(pShader);
		return spRenderSubMesh;
	}

	//**********************************************************************
	// Method: CreateDepthGridMesh
	// Creates a render submesh that is a grid. This is used for rendering
	// a depth map as a mesh.
	//
	// Parameters:
	// pRenderer - Renderer
	// width - width of depth map
	// height - height of depth map
	// pShader - shader
	//
	// Returns:
	// Returns the new render submesh
	//**********************************************************************
	CAUSTICAPI CRefObj<IRenderSubMesh> CreateDepthGridMesh(IRenderer* pRenderer, uint32 width, uint32 height, IShader* pShader)
	{
		std::vector<CGeomVertex> verts;
		std::vector<CGeomFace> faces;
		float dx = 2.0f / float(width);
		float dy = 2.0f / float(height);
		float cy = -1.0f;
		faces.resize(2 * (width - 1) * (height - 1));
		verts.resize(width * height);
		Vector3 normal(0.0f, 1.0f, 0.0f);
		int vertIndex = 0;
		int faceIndex = 0;
		for (uint32 y = 0; y < height; y++)
		{
			float cx = -1.0f;
			for (uint32 x = 0; x < width; x++)
			{
				verts[vertIndex].pos = Vector3(cx, cy, 0.0f);
				verts[vertIndex].norm = normal;
				verts[vertIndex].uvs[0] = Vector2(float(x) / float(width - 1), float(y) / float(height - 1));
				verts[vertIndex].uvs[1] = Vector2(float(x), float(y));
				cx += dx;
				if (x > 0 && y > 0)
				{
					faces[faceIndex].indices[0] = vertIndex - width - 1;
					faces[faceIndex].indices[1] = vertIndex - 1;
					faces[faceIndex].indices[2] = vertIndex - width;
					faceIndex++;
					faces[faceIndex].indices[0] = vertIndex;
					faces[faceIndex].indices[1] = vertIndex - width;
					faces[faceIndex].indices[2] = vertIndex - 1;
					faceIndex++;
				}
				vertIndex++;
			}
			cy += dy;
		}
		return Caustic::BuildRenderSubMesh(pRenderer, faces, verts, pShader);
	}
};
