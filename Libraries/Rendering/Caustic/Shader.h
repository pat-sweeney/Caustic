//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\BBox.h"
#include "Rendering\Caustic\Caustic.h"
#include <string>
#include <atlbase.h>
#include <d3d11.h>
#include <vector>
#include <any>
#include <map>

const int c_MaxFrames = 2; // Maximum number of frames being buffered

// A shader is a object that manages the vertex and pixel shader
//
namespace Caustic
{
    //**********************************************************************
    // EShaderParamType defines the various types of parameters that can
    // be passed to a CShader
    //
    // Header:
    // [Link:Rendering/Caustic/Shader.h]
    //**********************************************************************
    enum EShaderParamType
    {
        ShaderType_Undefined,
        ShaderType_Texture,
        ShaderType_Sampler,
        ShaderType_Float,
        ShaderType_Float2,
        ShaderType_Float3,
        ShaderType_Float4,
        ShaderType_Int,
        ShaderType_Matrix,
        ShaderType_Matrix3x3,
        ShaderType_Float_Array,
        ShaderType_Float2_Array,
        ShaderType_Float3_Array,
        ShaderType_Float4_Array,
        ShaderType_Int_Array,
        ShaderType_Matrix_Array,
        ShaderType_Matrix3x3_Array,
        ShaderType_StructuredBuffer,
        ShaderType_RWStructuredBuffer,
        ShaderType_AppendStructuredBuffer,
        ShaderType_RWByteAddressBuffer
    };

    //**********************************************************************
    // Struct: ShaderParamDef
    // ShaderParamDef defines the shader definitions that were parsed from HLSL
    // by ParseShader. These definitions define the variables that each
    // shader defines (i.e. may be set by the client)
    //
    // Parameters:
    // m_type - Defines type of this parameter 
    // m_name - Name of shader parameter
    // m_offset - Register offset
    // m_members - Number of elements (i.e. some parameters can be arrays) 
    // m_elemSize - sizeof a single element in bytes (used by buffers)
    //
    // Header:
    // [Link:Rendering/Caustic/Shader.h]
    //**********************************************************************
    struct ShaderParamDef
    {
        EShaderParamType m_type; // Defines type of this parameter
        std::wstring m_name;     // Name of shader parameter
        uint32 m_offset;         // register offset
        uint32 m_members;        // Number of elements (i.e. some parameters can be arrays)
        uint32 m_elemSize;       // size of a single element (used by buffers)
    };

    //**********************************************************************
    // Struct: ShaderParamInstance
    // ShaderParamInstance defines each parameter that a shader exposes. These parameters
    // are derived from the ShaderParamDefs above. This is servers copy of each
    // parameter along with its value and position in the constant buffer.
    //
    // Parameters:
    // m_value - Value assigned to this parameter
    // m_values - Array of values assigned to this parameter
    // m_dirty - Is parameter dirty and needs to be pushed to constant buffer
    //
    // Header:
    // [Link:Rendering/Caustic/Shader.h]
    //**********************************************************************
    struct ShaderParamInstance : public ShaderParamDef
    {
        std::any m_value;      // Value assigned to this parameter
        std::vector<std::any> m_values;
        bool m_dirty;            // Is parameter dirty and needs to be pushed to constant buffer
    };

    struct Float { float x; Float(float _x) { x = _x; } };
    struct Int { int x; Int(int _x) { x = _x; } };
    struct Float2 { float x; float y; Float2(float _x, float _y) { x = _x; y = _y; } };
    struct Float3 { float x; float y; float z; Float3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; } };
    struct Float4 { float x; float y; float z; float w; Float4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; } };
    struct Matrix
    { 
        float x[16]; 
        Matrix() 
        { 
            ZeroMemory(x, sizeof(x));
        }
    
        Matrix(Matrix4x4& m)
        {
            int index = 0;
            for (int col = 0; col < 4; col++)
                for (int row = 0; row < 4; row++)
                    x[index++] = m[row][col];
        }

        Matrix(Matrix3x3& m)
        {
            int index = 0;
            x[index++] = m[0][0];
            x[index++] = m[1][0];
            x[index++] = 0.0f;
            x[index++] = m[2][0];
            x[index++] = m[0][1];
            x[index++] = m[1][1];
            x[index++] = 0.0f;
            x[index++] = m[2][1];
            x[index++] = m[0][2];
            x[index++] = m[1][2];
            x[index++] = 0.0f;
            x[index++] = m[2][2];
            x[index++] = 0.0f;
            x[index++] = 0.0f;
            x[index++] = 0.0f;
            x[index++] = 1.0f;
        }

        Matrix(float _x[16])
        {
            memcpy(x, _x, sizeof(float) * 16);
        }
    };
    
    struct Matrix_3x3
    {
        float x[16];

        Matrix_3x3()
        {
            ZeroMemory(x, sizeof(x));
        }
        
        Matrix_3x3(Matrix3x3& m)
        {
            int index = 0;
            for (int col = 0; col < 4; col++)
                for (int row = 0; row < 4; row++)
                    if (row == 3 || col == 3)
                        x[index++] = 0.0f;
                    else
                        x[index++] = m[row][col];
        }

        Matrix_3x3(float _x[16])
        {
            memcpy(x, _x, sizeof(float) * 16);
        }
    };

    //**********************************************************************
    // Class: SBuffer
    // Defines a buffer (which may be either a constant buffer or
    // and unordered access buffer used by a compute shader).
    //
    // Members:
    // m_spBuffer - the D3D buffer (that will be passed to the compute shader)
    // m_spStagingBuffer - staging buffer for going between CPU and GPU
    // m_spView - A view onto the buffer m_spBuffer
    // m_bufferSize - size of buffer in bytes
    // m_heapSize - size of the total heap the buffer is contained in
    //
    // Header:
    // [Link:Rendering/Caustic/Shader.h]
    //**********************************************************************
    struct SBuffer
    {
        CComPtr<ID3D11Buffer> m_spBuffer;
        CComPtr<ID3D11Buffer> m_spStagingBuffer;
        CComPtr<ID3D11UnorderedAccessView> m_spUAView;
        CComPtr<ID3D11ShaderResourceView> m_spSRView;
        uint32 m_bufferSize;
        uint32 m_heapSize;
        bool m_isInput;
        int m_bufferSlot;
        std::wstring m_name;

        uint8* m_wpOutputBuffer; // Weak reference to the output buffer

        SBuffer() :
            m_wpOutputBuffer(nullptr),
            m_bufferSize(0),
            m_heapSize(0),
            m_isInput(true),
            m_bufferSlot(0)
        {
        }
    };

    //**********************************************************************
    // Class: CShader
    // Defines a shader used to render materials on an object
    //
    // A shader defines a material on a renderable. It is comprised of a pixel
    // shader, a vertex shader, and constants passed to those shaders.
    //
    // Header:
    // [Link:Rendering/Caustic/Shader.h]
    //**********************************************************************
    class CShader : public IShader, public CRefCount
    {
        std::wstring m_name;
        std::vector<D3D11_INPUT_ELEMENT_DESC> m_layout;
        CComPtr<ID3D11SamplerState> m_spSamplerState;
        CComPtr<ID3D11InputLayout> m_spLayout;
        CComPtr<ID3D11PixelShader> m_spPixelShader;
        CComPtr<ID3D11VertexShader> m_spVertexShader;
        CComPtr<ID3D11ComputeShader> m_spComputeShader;
        SBuffer m_vertexConstants;
        SBuffer m_pixelConstants;
        SBuffer m_computeConstants;
        std::vector<ShaderParamInstance> m_psParams;
        std::vector<ShaderParamInstance> m_vsParams;
        std::vector<ShaderParamInstance> m_csParams;
        std::vector<SBuffer> m_csBuffers;
        CRefObj<IShaderInfo> m_spShaderInfo;
        int m_xThreads;
        int m_yThreads;
        int m_zThreads;
        int m_maxTextureSlot;
    protected:
        void PushMatrix(const wchar_t *name, std::any mat);
        void PushLights(std::vector<CRefObj<ILight>>& lights);
        void PushMatrices(IRenderer* pRenderer, DirectX::XMMATRIX *pWorld);
        uint32 ComputeParamSize(ShaderParamDef *pParams, uint32 numParams, std::vector<ShaderParamInstance> &params);
        uint32 ShaderTypeSize(ShaderParamDef& paramDef);
        void PushConstants(IRenderer* pRenderer, SBuffer *pBuffer, std::vector<ShaderParamInstance> &params);
        void ClearSamplers(IRenderer* pRenderer);
        void PushSamplers(IRenderer* pRenderer, std::vector<ShaderParamInstance>& params, bool isPixelShader);
        void PushBuffers(IRenderer* pRenderer, std::vector<ShaderParamInstance>& params);
        void PopBuffers(IRenderer* pRenderer);
        void SetParam(std::wstring paramName, std::any &value, std::vector<ShaderParamInstance> &params);
        void SetParam(std::wstring paramName, int index, std::any &value, std::vector<ShaderParamInstance> &params);
    public:
        void Create(IRenderer *pRenderer, const wchar_t *pShaderName, IShaderInfo *pShaderInfo, ID3DBlob *pPSBlob, ID3DBlob* pVSBlob, ID3DBlob* pCSBlob);
        void CreateBuffer(ID3D11Device* pDevice, uint32 bufSize, uint32 bindFlags, uint32 cpuAccessFlags, D3D11_USAGE usage, uint32 miscFlags, uint32 stride, uint32 alignment, SBuffer* pBuffer, ID3D11Buffer **ppBuffer);
        void CreateBuffers(ID3D11Device* pDevice, ShaderParamDef* pDefs, uint32 paramsSize, std::vector<ShaderParamInstance>& params);
        void CreateConstantBuffer(ID3D11Device *pDevice, ShaderParamDef *pDefs, uint32 paramsSize, std::vector<ShaderParamInstance> &params, SBuffer *pConstantBuffer);

        CShader() :
            m_xThreads(32),
            m_yThreads(32),
            m_zThreads(1),
            m_maxTextureSlot(0)
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IShader
        //**********************************************************************
        virtual CRefObj<IShader> Clone(ID3D11Device *pDevice) override;
        virtual std::wstring &Name() override { return m_name; }
        virtual void BeginRender(IRenderer* pRenderer, IRenderMaterial* pFrontMaterial, IRenderMaterial* pBackMaterial, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld) override;
        virtual void SetPSParam(std::wstring paramName, std::any &value) override;
        virtual void SetPSParam(std::wstring paramName, int index, std::any &value) override;
        virtual void SetVSParam(std::wstring paramName, std::any &value) override;
        virtual void SetVSParam(std::wstring paramName, int index, std::any& value) override;
        virtual void SetCSParam(std::wstring paramName, std::any& value) override;
        virtual void SetCSParam(std::wstring paramName, int index, std::any& value) override;
        virtual void EndRender(IRenderer* pRenderer) override;
        virtual CRefObj<IShaderInfo> GetShaderInfo() override;
        virtual void SetThreadCounts(int xThreads, int yThreads, int zThreads) override;
    };

    //**********************************************************************
    // Class: CShaderMgr
    // Defines our shader manager. Each shader is registered with this manager.
    //
    // Header:
    // [Link:Rendering/Caustic/Shader.h]
    //**********************************************************************
    class CShaderMgr : public IShaderMgr, public CRefCount
    {
        std::map<std::wstring, CRefObj<IShader>> m_shaders; // List of registered shaders
    public:
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IShaderMgr
        //**********************************************************************
        virtual CRefObj<IShader> FindShader(const wchar_t *pShaderName) override;
        virtual void RegisterShader(const wchar_t *pShaderName, IShader *pShader) override;
    };
}
