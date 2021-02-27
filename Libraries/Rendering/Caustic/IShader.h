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

    //**********************************************************************
    // Structure: ClientBuffer
    // Data from/to the client that is mapped to/from a shader's StructuredBuffer,
    // RWStructuredBuffer, or AppendStructuredBuffer). This structure is what
    // clients will pass to IShader::SetCSParam for any of these shader types.
    //
    // Parameters:
    // m_pData - pointer to the buffer. If output buffer then client owns this. Otherwise do (i.e. we free it).
    // m_dataSize - size of buffer in bytes
    // m_stride - size of each buffer element in bytes
    //
    // Header:
    // [Link:Rendering/Caustic/IShader.h]
    //**********************************************************************
    struct ClientBuffer
    {
        std::shared_ptr<uint8> m_spInputData; // Copy of client data. Owned by this object.
        uint8* m_wpOutputData; // Weak reference to client output buffer. Owned by client.
        uint32 m_dataSize;
        uint32 m_stride; // Size of each element

        ClientBuffer() :
            m_dataSize(0),
            m_stride(0),
            m_wpOutputData(nullptr)
        {
        }
        ClientBuffer(const ClientBuffer& s)
        {
            *this = s;
        }

        ~ClientBuffer()
        {
            m_dataSize = 0;
        }
    };
    
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
        virtual void SetPSParam(const std::wstring& paramName, std::any& value) = 0;
        virtual void SetPSParam(const wchar_t* paramName, std::any& value) = 0;

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
        virtual void SetVSParam(const std::wstring& paramName, std::any& value) = 0;
        virtual void SetVSParam(const wchar_t* paramName, std::any& value) = 0;

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
        virtual void SetCSParam(const std::wstring& paramName, std::any& value) = 0;
        virtual void SetCSParam(const wchar_t* paramName, std::any& value) = 0;

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
        virtual void SetPSParam(const std::wstring& paramName, int index, std::any& value) = 0;
        virtual void SetPSParam(const wchar_t* paramName, int index, std::any& value) = 0;

        //**********************************************************************
        // Method: SetVSParam
        // Sets a vertex shader parameter that is part of an array
        //
        // Parameters:
        // paramName - name of parameter
        // index - array index to set
        // value - value to set
        //**********************************************************************
        virtual void SetVSParam(const std::wstring& paramName, int index, std::any& value) = 0;
        virtual void SetVSParam(const wchar_t* paramName, int index, std::any& value) = 0;

        //**********************************************************************
        // Method: SetCSParam
        // Sets a compute shader parameter that is part of an array
        //
        // Parameters:
        // paramName - name of parameter
        // index - array index to set
        // value - value to set
        //**********************************************************************
        virtual void SetCSParam(const std::wstring& paramName, int index, std::any& value) = 0;
        virtual void SetCSParam(const wchar_t* paramName, int index, std::any& value) = 0;

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
