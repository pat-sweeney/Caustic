//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\IGraphics.h"
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
        ShaderType_Float_Array,
        ShaderType_Float2_Array,
        ShaderType_Float3_Array,
        ShaderType_Float4_Array,
        ShaderType_Int_Array,
        ShaderType_Matrix_Array,
    };

    //**********************************************************************
    // Struct: ShaderParamDef
    // ShaderParamDef defines the shader definitions that were parsed from HLSL
    // by ParseShader. These definitions define the variables that each
    // shader defines (i.e. may be set by the client)
    //**********************************************************************
    struct ShaderParamDef
    {
        EShaderParamType m_type; // Defines type of this parameter
        std::wstring m_name;     // Name of shader parameter
        uint32 m_offset;         // register offset
        uint32 m_members;        // Number of elements
    };

    //**********************************************************************
    // Struct: ShaderParamInstance
    // ShaderParamInstance defines each parameter that a shader exposes. These parameters
    // are derived from the ShaderDefs above. This is servers copy of each
    // parameter along with its value and position in the constant buffer.
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
    struct Matrix { float x[16]; Matrix() { ZeroMemory(x, sizeof(x)); } Matrix(float _x[16]) { memcpy(x, _x, sizeof(float) * 16); } };
    
    struct SConstantBuffer
    {
        CComPtr<ID3D11Buffer> m_spBuffer;
        uint32 m_bufferSize;
        uint32 m_heapSize;
    };

    //**********************************************************************
    // Class: CShader
    // Defines a shader used to render materials on an object
    //
    // A shader defines a material on a renderable. It is comprised of a pixel
    // shader, a vertex shader, and constants passed to those shaders.
    //**********************************************************************
    class CShader : public IShader, public CRefCount
    {
        std::wstring m_name;
        std::vector<D3D11_INPUT_ELEMENT_DESC> m_layout;
        CComPtr<ID3D11SamplerState> m_spSamplerState;
        CComPtr<ID3D11InputLayout> m_spLayout;
        CComPtr<ID3D11PixelShader> m_spPixelShader;
        CComPtr<ID3D11VertexShader> m_spVertexShader;
        SConstantBuffer m_vertexConstants;
        SConstantBuffer m_pixelConstants;
        std::vector<ShaderParamInstance> m_psParams;
        std::vector<ShaderParamInstance> m_vsParams;
		CRefObj<IShaderInfo> m_spShaderInfo;
    protected:
        void PushMatrix(const wchar_t *name, std::any mat);
        void PushLights(std::vector<CRefObj<IPointLight>> &lights);
        void PushMatrices(IGraphics *pGraphics, DirectX::XMMATRIX *pWorld);
        uint32 ComputeParamSize(ShaderParamDef *pParams, uint32 numParams, std::vector<ShaderParamInstance> &params);
        void PushConstants(IGraphics *pGraphics, SConstantBuffer *pBuffer, std::vector<ShaderParamInstance> &params);
        void SetParam(std::wstring paramName, std::any &value, std::vector<ShaderParamInstance> &params);
        void SetParam(std::wstring paramName, int index, std::any &value, std::vector<ShaderParamInstance> &params);
    public:
        void Create(IGraphics *pGraphics, const wchar_t *pShaderName, IShaderInfo *pShaderInfo, ID3DBlob *pPSBlob, ID3DBlob *pVSBlob);
        void CreateConstantBuffer(ID3D11Device *pDevice, ShaderParamDef *pDefs, uint32 paramsSize, std::vector<ShaderParamInstance> &params, SConstantBuffer *pConstantBuffer);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IShader
        //**********************************************************************
        virtual std::wstring &Name() override { return m_name; }
        virtual void BeginRender(IGraphics *pGraphics, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, std::vector<CRefObj<IPointLight>> &lights, DirectX::XMMATRIX *pWorld) override;
        virtual void SetPSParam(std::wstring paramName, std::any &value) override;
        virtual void SetPSParam(std::wstring paramName, int index, std::any &value) override;
        virtual void SetVSParam(std::wstring paramName, std::any &value) override;
        virtual void SetVSParam(std::wstring paramName, int index, std::any &value) override;
        virtual void EndRender(IGraphics *pGraphics) override;
        virtual CRefObj<IShaderInfo> GetShaderInfo() override;
    };

    //**********************************************************************
    // Class: CShaderMgr
    // Defines our shader manager. Each shader is registered with this manager.
    //**********************************************************************
    class CShaderMgr : public IShaderMgr, public CRefCount
    {
        std::map<std::wstring, CRefObj<IShader>> m_shaders; // List of registered shaders
        static CShaderMgr s_ShaderMgr; // Our shader manager singleton
    public:
        static IShaderMgr *Instance() { return &s_ShaderMgr; }
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IShaderMgr
        //**********************************************************************
        virtual void FindShader(const wchar_t *pShaderName, IShader **ppShader) override;
        virtual void RegisterShader(const wchar_t *pShaderName, IShader *pShader) override;
    };
}
