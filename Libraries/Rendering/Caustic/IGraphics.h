//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include <d3d11.h>
#include <atlbase.h>
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\BBox.h"

//**********************************************************************
// File: IGraphics.h
// Primary interface for talking to the graphics device
//**********************************************************************

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
    // Defines our basic rendering device.
    //
    // <IGraphics> differs from <IRenderer>
    // in that it is essentially just a wrapper around our underlying graphics
    // device. It does not support things such as a scene graph, complex lighting
    // (list of lights), or HMD support. Also, all rendering occurs on whatever
    // thread this object is created on (versus marshalling the rendering over
    // to a render thread). For those features, use <IRenderer>.
    //**********************************************************************
    struct IGraphics : public IRefCount
    {
        //**********************************************************************
        // Method: GetDevice
        // Returns:
        // The underlying D3D11 device
        //**********************************************************************
        virtual CComPtr<ID3D11Device> GetDevice() = 0;

        //**********************************************************************
        // Method: GetContext
        // Returns:
        // The underlying D3D11 device context
        //**********************************************************************
        virtual CComPtr<ID3D11DeviceContext> GetContext() = 0;

        //**********************************************************************
        // Method: GetCamera
        // Returns:
        // The camera associated with this device
        //**********************************************************************
        virtual CRefObj<ICamera> GetCamera() = 0;

        //**********************************************************************
        // Method: SetCamera
        // Sets the camera associated with this device
        //
        // Parameters:
        // pCamera - camera
        //**********************************************************************
        virtual void SetCamera(ICamera *pCamera) = 0;
    };
};
