//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\BBox.h"

namespace Caustic
{
    struct ICamera;

    //**********************************************************************
    // Struct: MeshData
    // Defines class for holding mesh data (i.e. vertices and indices)
    //
    // Members:
    // m_spVB - The vertex buffer
    // m_spIB - The index buffer (maybe null)
    // m_vertexSize - Size in bytes of each vertex
    // m_numVertices - Number of vertices
    // m_numIndices - Number of indices. Maybe 0
    //**********************************************************************
    struct MeshData
    {
        CComPtr<ID3D11Buffer> m_spVB;
        CComPtr<ID3D11Buffer> m_spIB;
        uint32 m_vertexSize;
        uint32 m_numVertices;
        uint32 m_numIndices;
        BBox3 m_bbox;

        MeshData() :
            m_vertexSize(0),
            m_numVertices(0),
            m_numIndices(0)
        {
        }
    };

    //**********************************************************************
    // Interface: IGraphics
	// Defines a simple wrapper around our D3D renderer
    //**********************************************************************
    struct IGraphics : public IRefCount
    {
        virtual CComPtr<ID3D11Device> GetDevice() = 0;
        virtual CComPtr<ID3D11DeviceContext> GetContext() = 0;
        virtual CRefObj<ICamera> GetCamera() = 0;
        virtual void SetCamera(ICamera *pCamera) = 0;
    };
};
