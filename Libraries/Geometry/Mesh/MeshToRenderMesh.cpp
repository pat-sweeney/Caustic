//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Geometry\Mesh\Mesh.h"
#include "Rendering/Caustic/IRenderer.h"
#include "Rendering/Caustic/IRenderMesh.h"
#include <d3d12.h>
#include <d3dx12.h>

namespace Caustic
{
	const int c_Vertex_Unreferenced = -1;
	const int c_Vertex_Referenced = -2;

	static bool IsDegenerate(CGeomVertex *v0, CGeomVertex *v1, CGeomVertex *v2)
	{
		Vector3 u = (v1->pos - v0->pos);
		Vector3 v = (v2->pos - v0->pos);
		return IsZero(u.cross(v).Length());
	}

	//**********************************************************************
	// BuildVertexBuffer() creates a vertex buffer using the specified submesh.
	// pMesh - Submesh to use for vertex data
	// pNumVerts - Returns the number of vertices in the output vertex buffer
	// ppVertexBuffer - Returns pointer to array of vertices
	// Returns the index of each referenced vertex
	//**********************************************************************
	void CSubMesh::BuildVertexBuffer(IRenderSubMesh *pRenderSubMesh, ID3D12Device *pDevice, ID3D12GraphicsCommandList *pCommandList,
		IShaderInfo *pShaderInfo, std::vector<int> &vertexReferenced)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> vertexLayout = pShaderInfo->VertexLayout();
		uint32 numVerts = GetNumberVertices();
		uint32 vertexSize = pShaderInfo->GetVertexSize();

		// Create buffer with our vertex data
		byte *pVertexBuffer = new byte[numVerts * vertexSize];
		std::unique_ptr<byte> spVertBuffer(pVertexBuffer);
		uint32 vout = 0;
		for (uint32 i = 0; i < numVerts; i++)
		{
			if (vertexReferenced[i] == c_Vertex_Referenced)
			{
				CGeomVertex *pVertex = GetVertex(i);
				byte *pVB = &pVertexBuffer[vout * vertexSize];
				for (size_t j = 0; j < vertexLayout.size(); j++)
				{
					if (_strnicmp(vertexLayout[j].SemanticName, "POSITION", 8) == 0 ||
						_strnicmp(vertexLayout[j].SemanticName, "SV_POSITION", 11) == 0)
					{
						_ASSERT(vertexLayout[j].Format == DXGI_FORMAT_R32G32B32_FLOAT);
						float *fp = (float*)pVB;
						fp[0] = pVertex->pos.x;
						fp[1] = pVertex->pos.y;
						fp[2] = pVertex->pos.z;
						m_bbox.AddPoint(pVertex->pos);
						pVB += sizeof(float) * 3;
					}
					else if (_strnicmp(vertexLayout[j].SemanticName, "TEXCOORD", 8) == 0)
					{
						_ASSERT(vertexLayout[j].Format == DXGI_FORMAT_R32G32_FLOAT);
						float *fp = (float*)pVB;
						fp[0] = pVertex->uvs[0].x;
						fp[1] = pVertex->uvs[0].y;
						pVB += sizeof(float) * 2;
					}
					else if (_strnicmp(vertexLayout[j].SemanticName, "NORMAL", 6) == 0)
					{
						_ASSERT(vertexLayout[j].Format == DXGI_FORMAT_R32G32B32_FLOAT);
						float *fp = (float*)pVB;
						fp[0] = pVertex->norm.x;
						fp[1] = pVertex->norm.y;
						fp[2] = pVertex->norm.z;
						pVB += sizeof(float) * 3;
					}
				}
				vertexReferenced[i] = vout++;
			}
		}

		// Create our upload buffer
		UINT vbSize = (UINT)(numVerts * vertexSize);
		CComPtr<ID3D12Resource> spVBUpload;
		CD3DX12_RESOURCE_DESC descUpload = CD3DX12_RESOURCE_DESC::Buffer(vbSize);
		CD3DX12_HEAP_PROPERTIES heapdescUpload(D3D12_HEAP_TYPE_UPLOAD);
		CT(pDevice->CreateCommittedResource(&heapdescUpload, D3D12_HEAP_FLAG_NONE,
			&descUpload, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource), (void**)&spVBUpload));
		CT(spVBUpload->SetName(L"spVBUpload"));

		// Create final buffer
		CComPtr<ID3D12Resource> spVB;
		CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(vbSize);
		CD3DX12_HEAP_PROPERTIES heapdesc(D3D12_HEAP_TYPE_DEFAULT);
		CT(pDevice->CreateCommittedResource(&heapdesc, D3D12_HEAP_FLAG_NONE,
			&desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, __uuidof(ID3D12Resource), (void**)&spVB));
		CT(spVB->SetName(L"spVB"));

		// store vertex buffer in upload heap
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = reinterpret_cast<BYTE*>(pVertexBuffer);
		vertexData.RowPitch = vbSize;
		vertexData.SlicePitch = vbSize;

		// we are now creating a command with the command list to copy the data from
		// the upload heap to the default heap
		UpdateSubresources(pCommandList, spVB, spVBUpload, 0, 0, 1, &vertexData);

		// transition the vertex buffer data from copy destination state to vertex buffer state
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(spVB, D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		pCommandList->ResourceBarrier(1, &barrier);
		
		pRenderSubMesh->SetNumberVertices(vout);
		pRenderSubMesh->SetVertexBuffer(spVB);
	}

	void CSubMesh::BuildIndexBuffer(IRenderSubMesh *pRenderSubMesh, ID3D12Device *pDevice, ID3D12GraphicsCommandList *pCommandList,
		std::vector<int> &vertexReferenced)
	{
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
		
		// Create our upload buffer
		UINT ibSize = (UINT)(numIndices * sizeof(uint32));
		CComPtr<ID3D12Resource> spIBUpload;
		CD3DX12_HEAP_PROPERTIES heapUpload(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC descUpload = CD3DX12_RESOURCE_DESC::Buffer(ibSize);
		CT(pDevice->CreateCommittedResource(&heapUpload, D3D12_HEAP_FLAG_NONE,
			&descUpload, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource), (void**)&spIBUpload));
		CT(spIBUpload->SetName(L"spIBUpload"));

		// Create final buffer
		CComPtr<ID3D12Resource> spIB;
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC heapDesc = CD3DX12_RESOURCE_DESC::Buffer(ibSize);
		CT(pDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
			&heapDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, __uuidof(ID3D12Resource), (void**)&spIB));
		CT(spIB->SetName(L"spIB"));

		// store vertex buffer in upload heap
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = reinterpret_cast<BYTE*>(pIndexBuffer);
		vertexData.RowPitch = ibSize;
		vertexData.SlicePitch = ibSize;

		// we are now creating a command with the command list to copy the data from
		// the upload heap to the default heap
		UpdateSubresources(pCommandList, spIB, spIBUpload, 0, 0, 1, &vertexData);

		// transition the vertex buffer data from copy destination state to vertex buffer state
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(spIB, D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_INDEX_BUFFER);
		pCommandList->ResourceBarrier(1, &barrier);

		pRenderSubMesh->SetNumberIndices(numIndices);
		pRenderSubMesh->SetVertexBuffer(spIB);
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
	// ToRenderSubMesh converts a CSubMesh object into a renderable form
	// pRenderer - Renderer
	// pShaderInfo - shader info (used to access vertex layout)
	// ppRenderSubMesh - returns the new submesh
	//**********************************************************************
	void CSubMesh::ToRenderSubMesh(Caustic::IRenderer *pRenderer, IShaderInfo *pShaderInfo, IRenderSubMesh **ppRenderSubMesh)
	{
		CRefObj<ICausticFactory> spFactory;
		Caustic::CreateCausticFactory(&spFactory);
		CRefObj<IRenderSubMesh> spRenderSubMesh;
		spFactory->CreateRenderSubMesh(&spRenderSubMesh);
		std::vector<int> vertexReferenced(GetNumberVertices(), c_Vertex_Unreferenced);
		BuildReferencedVertexList(vertexReferenced);
		BuildVertexBuffer(spRenderSubMesh, pRenderer->GetDevice(), pRenderer->GetCommandList(), pShaderInfo, vertexReferenced);
		BuildIndexBuffer(spRenderSubMesh, pRenderer->GetDevice(), pRenderer->GetCommandList(), vertexReferenced);
		*ppRenderSubMesh = spRenderSubMesh;
		(*ppRenderSubMesh)->AddRef();
	}
};