//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include <d3d11.h>
#include <atlbase.h>
import Base.Core.Core;
import Base.Core.IRefCount;
#include "Base\Math\BBox.h"
#include "IShaderMgr.h"
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
    struct ILight;
    struct IRenderer;

    enum EBufferType
    {
        AppendStructuredBuffer,
        RWStructuredBuffer,
        StructuredBuffer,
        RWByteAddressBuffer,
    };

    //**********************************************************************
    // Interface: IGPUBuffer
    // Defines the public interface to a buffer used by the GPU. When clients
    // need to pass data to a compute shader (i.e. as either a StructuredBuffer,
    // RWStructuredBuffer, AppendStructuredBuffer, or RWByteAddressBuffer) they
    // will pass one of these objects via IShader::SetCSParam().
    //
    // Header:
    // [Link:Rendering/Caustic/IShader.h]
    //**********************************************************************
    struct IGPUBuffer : public IRefCount
    {
        virtual CComPtr<ID3D11Buffer> GetBuffer() = 0;
        virtual CComPtr<ID3D11Buffer> GetStagingBuffer() = 0;
        virtual CComPtr<ID3D11UnorderedAccessView> GetUAView() = 0;
        virtual CComPtr<ID3D11ShaderResourceView> GetSRView() = 0;
        virtual EBufferType GetBufferType() = 0;

        //**********************************************************************
        // Method: CopyFromCPU
        // CopyFromCPU() is used to move data from the CPU (i.e. client data)
        // to the underlying GPU buffer.
        //
        // Parameters:
        // pRenderer - renderer
        // pData - pointer to buffer owned by client that contains the data
        //**********************************************************************
        virtual void CopyFromCPU(IRenderer* pRenderer, uint8* pData) = 0;

        //**********************************************************************
        // Method: CopyToCPU
        // CopyToCPU() is used to move data from the CPU (i.e. client data)
        // to the underlying GPU buffer.
        //
        // Parameters:
        // pRenderer - renderer
        // pData - pointer to buffer owned by client that contains the data
        //**********************************************************************
        virtual void CopyToCPU(IRenderer* pRenderer, uint8* pData) = 0;
    };

    //**********************************************************************
    // Function: CreateGPUBuffer
    // Creates a buffer on the GPU
    //
    // Parameters:
    // pRenderer - renderer
    // type - which type of buffer we are creating (Structured, RWStructured, etc)
    // numElems - number of elements in buffer
    // elemSize - size in bytes of each element (on CPU without unalignment)
    // bindFlags - additional bind flags
    //**********************************************************************
    CRefObj<IGPUBuffer> CreateGPUBuffer(IRenderer* pRenderer, EBufferType type, uint32 numElems, uint32 elemSize, uint32 bindFlags);

        
    //**********************************************************************
    // Interface: IShader
    // Defines the public interface for using <CShader>
    //
    // Header:
    // [Link:Rendering/Caustic/IShader.h]
    //**********************************************************************
    struct IShader : public IRefCount
    {
        //**********************************************************************
        // Method: Clone
        // Clones the shader. This is useful when you want to reuse a shader
        // (such as those returned by IShaderMgr::FindShader) but want the shader
        // to have its own set of parameters.
        //
        // Parameters:
        // pDevice - device
        //
        // Returns:
        // Copy of shader
        //**********************************************************************
        virtual CRefObj<IShader> Clone(ID3D11Device* pDevice) = 0;

        //**********************************************************************
        // Method: Name
        //
        // Returns:
        // Name of shader
        //**********************************************************************
        virtual std::wstring &Name() = 0; // Returns the name of this shader

        //**********************************************************************
        // Method: BeginRender
        // Starts rendering.
        //
        // Parameters:
        // pRenderer - graphics device
        // pFrontMaterial - material to use for front faces
        // pBackMaterial - material to use for back faces
        // lights - list of lights to use
        // pWorld - matrix to apply
        //**********************************************************************
        virtual void BeginRender(IRenderer *pRenderer, IRenderMaterial *pMaterial, std::vector<CRefObj<ILight>> &lights, DirectX::XMMATRIX *pWorld) = 0;

        //**********************************************************************
        // Method: SetPSParam
        // Sets a pixel shader parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetPSParam(const std::wstring& paramName, const std::any& value) = 0;
        virtual void SetPSParam(const wchar_t* paramName, const std::any& value) = 0;

        //**********************************************************************
        // Method: SetPSParamFloat
        // Sets a pixel shader float parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetPSParamFloat(const std::wstring& paramName, float value) = 0;
        virtual void SetPSParamFloat(const wchar_t* paramName, float value) = 0;

        //**********************************************************************
        // Method: SetPSParamInt
        // Sets a pixel shader int parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetPSParamInt(const std::wstring& paramName, int value) = 0;
        virtual void SetPSParamInt(const wchar_t* paramName, int value) = 0;

        //**********************************************************************
        // Method: SetVSParam
        // Sets a vertex shader parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetVSParam(const std::wstring& paramName, const std::any& value) = 0;
        virtual void SetVSParam(const wchar_t* paramName, const std::any& value) = 0;

        //**********************************************************************
        // Method: SetVSParamFloat
        // Sets a vertex shader float parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetVSParamFloat(const std::wstring& paramName, float value) = 0;
        virtual void SetVSParamFloat(const wchar_t* paramName, float value) = 0;

        //**********************************************************************
        // Method: SetVSParamInt
        // Sets a pixel shader int parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetVSParamInt(const std::wstring& paramName, int value) = 0;
        virtual void SetVSParamInt(const wchar_t* paramName, int value) = 0;

        //**********************************************************************
        // Method: SetCSParam
        // Sets a computer shader parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetCSParam(const std::wstring& paramName, const std::any& value) = 0;
        virtual void SetCSParam(const wchar_t* paramName, const std::any& value) = 0;

        //**********************************************************************
        // Method: SetCSParamFloat
        // Sets a vertex shader float parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetCSParamFloat(const std::wstring& paramName, float value) = 0;
        virtual void SetCSParamFloat(const wchar_t* paramName, float value) = 0;

        //**********************************************************************
        // Method: SetCSParamInt
        // Sets a pixel shader int parameter
        //
        // Parameters:
        // paramName - name of parameter
        // value - value to set
        //**********************************************************************
        virtual void SetCSParamInt(const std::wstring& paramName, int value) = 0;
        virtual void SetCSParamInt(const wchar_t* paramName, int value) = 0;

        //**********************************************************************
        // Method: SetPSParam
        // Sets a pixel shader parameter that is part of an array
        //
        // Parameters:
        // paramName - name of parameter
        // index - array index to set
        // value - value to set
        //**********************************************************************
        virtual void SetPSParam(const std::wstring& paramName, int index, const std::any& value) = 0;
        virtual void SetPSParam(const wchar_t* paramName, int index, const std::any& value) = 0;

        //**********************************************************************
        // Method: SetVSParam
        // Sets a vertex shader parameter that is part of an array
        //
        // Parameters:
        // paramName - name of parameter
        // index - array index to set
        // value - value to set
        //**********************************************************************
        virtual void SetVSParam(const std::wstring& paramName, int index, const std::any& value) = 0;
        virtual void SetVSParam(const wchar_t* paramName, int index, const std::any& value) = 0;

        //**********************************************************************
        // Method: SetCSParam
        // Sets a compute shader parameter that is part of an array
        //
        // Parameters:
        // paramName - name of parameter
        // index - array index to set
        // value - value to set
        //**********************************************************************
        virtual void SetCSParam(const std::wstring& paramName, int index, const std::any& value) = 0;
        virtual void SetCSParam(const wchar_t* paramName, int index, const std::any& value) = 0;

        //**********************************************************************
        // Method: EndRender
        // Ends rendering. Called after the rendering using the shader has completed.
        //
        // Parameters:
        // pRenderer - graphics device
        //**********************************************************************
        virtual void EndRender(IRenderer* pRenderer) = 0;

        //**********************************************************************
        // Method: GetShaderInfo
        // Returns:
        // Returns the shader info associated with the shader
        //**********************************************************************
        virtual CRefObj<IShaderInfo> GetShaderInfo() = 0;

        //**********************************************************************
        // Method: Dispatch
        // This runs the compute shader, if any.
        //
        // Parameters:
        // pRenderer - graphics device
        // xThreads - Number of threads in X axis
        // yThreads - Number of threads in Y axis
        // zThreads - Number of threads in Z axis
        //**********************************************************************
        virtual void Dispatch(IRenderer* pRenderer, int xThreads, int yThreads, int zThreads) = 0;
    };
}
