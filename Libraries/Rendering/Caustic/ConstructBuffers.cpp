//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Base\Math\BBox.h"
#include "Rendering\Caustic\IRenderer.h"
#include "Rendering\Caustic\IRenderMesh.h"
#include "Rendering\Caustic\IShaderInfo.h"
#include "Shader.h"
#include <d3d11.h>

namespace Caustic
{
	// TODO: This sort of sucks. This functionality is currently duplicated in the mesh library. Need to go back and clean this all up.
	void BuildVertexBuffer(IRenderer *pRenderer, IShaderInfo *pShaderInfo, std::vector<CGeomVertex> &pts, MeshData *pMeshData)
	{
		CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
		std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout = pShaderInfo->VertexLayout();
		uint32 numVerts = (uint32)pts.size();
		uint32 vertexSize = pShaderInfo->GetVertexSize();
		BBox3 bbox;

		// Create buffer with our vertex data
		byte* pVertexBuffer = new byte[numVerts * vertexSize];
		std::unique_ptr<byte> spVertBuffer(pVertexBuffer);
		for (uint32 i = 0; i < numVerts; i++)
		{
			CGeomVertex* pVertex = &pts[i];
			byte* pVB = &pVertexBuffer[i * vertexSize];
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
		pMeshData->m_numVertices = numVerts;
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
	// faces - List of vertices that are referenced
	// MeshData - place to store created index buffer
	//**********************************************************************
	void BuildIndexBuffer(IRenderer* pRenderer, std::vector<uint32>& faces, MeshData* pMeshData)
	{
		CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
		UINT ibSize = (UINT)(faces.size() * sizeof(uint32));
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = ibSize;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(uint32);
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &faces[0];
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;
		CComPtr<ID3D11Buffer> spIB;
		CT(spDevice->CreateBuffer(&desc, &data, &spIB));
		pMeshData->m_numIndices = (uint32)faces.size();
		pMeshData->m_spIB = spIB;
	}
}
