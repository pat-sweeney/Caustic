//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include <string>
#include <any>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

//**********************************************************************
// File: IShader.h
// This file defines the published interface for a shader.
//**********************************************************************

namespace Caustic
{
    struct IShaderInfo;
    struct IRenderMaterial;
    struct IPointLight;

    //**********************************************************************
    // Structure: StructuredBuffer
    // Value to assign to structured buffers (whether they are StructuredBuffer,
    // RWStructuredBuffer, or AppendStructuredBuffer). This structure is what
    // clients will pass to IShader::SetCSParam for any of these shader types.
    //
    // Parameters:
    // m_spData - pointer to the buffer
    // m_dataSize - size of buffer in bytes
    // m_isInputBuffer - indicates whether this is an input or output buffer
    //**********************************************************************
    struct StructuredBuffer
    {
        std::shared_ptr<uint8> m_spData;
        uint32 m_dataSize;
        bool m_isInputBuffer;
    };

    //**********************************************************************
    // Interface: IShader
    // Defines the public interface for using <CShader>
    //**********************************************************************
    struct IShader : public IRefCount
    {
        //**********************************************************************
        // Method: Name
        // Returns:
        // Name of shader
        //**********************************************************************
        virtual std::wstring &Name() = 0; // Returns the name of this shader

        //**********************************************************************
        // Method: BeginRender
        // Starts rendering.
        //
        // Parameters:
        // pGraphics - graphics device
        // pFrontMaterial - material to use for front faces
        // pBackMaterial - material to use for back faces
        // lights - list of lights to use
        // pWorld - matrix to apply
        //**********************************************************************
        virtual void BeginRender(IGraphics *pGraphics, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, std::vector<CRefObj<IPointLight>> &lights, DirectX::XMMATRIX *pWorld) = 0;

        //**********************************************************************
        // Method: SetPSParam
        // Sets a pixel shader parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetPSParam(std::wstring paramName, std::any &value) = 0;

        //**********************************************************************
        // Method: SetVSParam
        // Sets a vertex shader parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetVSParam(std::wstring paramName, std::any &value) = 0;

        //**********************************************************************
        // Method: SetCSParam
        // Sets a computer shader parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetCSParam(std::wstring paramName, std::any& value) = 0;

        //**********************************************************************
        // Method: SetPSParam
        // Sets a pixel shader parameter that is part of an array
        //
        // Parameters:
        // paramName - name of parameter
        // index - array index to set
        // value - value to set
        //**********************************************************************
        virtual void SetPSParam(std::wstring paramName, int index, std::any &value) = 0;

        //**********************************************************************
        // Method: SetVSParam
        // Sets a vertex shader parameter that is part of an array
        //
        // Parameters:
        // paramName - name of parameter
        // index - array index to set
        // value - value to set
        //**********************************************************************
        virtual void SetVSParam(std::wstring paramName, int index, std::any& value) = 0;

        //**********************************************************************
        // Method: SetCSParam
        // Sets a compute shader parameter that is part of an array
        //
        // Parameters:
        // paramName - name of parameter
        // index - array index to set
        // value - value to set
        //**********************************************************************
        virtual void SetCSParam(std::wstring paramName, int index, std::any& value) = 0;

        //**********************************************************************
        // Method: EndRender
        // Ends rendering. Called after the rendering using the shader has completed.
        //
        // Parameters:
        // pGraphics - graphics device
        //**********************************************************************
        virtual void EndRender(IGraphics *pGraphics) = 0;

        //**********************************************************************
        // Method: GetShaderInfo
        // Returns:
        // Returns the shader info associated with the shader
        //**********************************************************************
        virtual CRefObj<IShaderInfo> GetShaderInfo() = 0;

        //**********************************************************************
        // Method: SetThreadCounts
        // This is a bit of a hack. We need some way to specify how many threads
        // to execute for the compute shader. This needs to be rethought (maybe
        // make specific class for shader with underlying compute shader??)
        //
        // Parameters:
        // xThreads - Number of threads in X axis
        // yThreads - Number of threads in Y axis
        // zThreads - Number of threads in Z axis
        //**********************************************************************
        virtual void SetThreadCounts(int xThreads, int yThreads, int zThreads) = 0;
    };
}
