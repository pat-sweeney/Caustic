//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Graphics.h"
#include "Rendering\Caustic\Caustic.h"
#include <string>
#include <atlbase.h>
#include <d3d11.h>
#include <vector>
#include <any>
#include <map>

namespace Caustic
{
    // TODO: Need to determine size of Arrays. For now assume arrays are always 4 deep
    const int c_ArraySize = 4;

    //**********************************************************************
    //! EShaderParamType defines the various types of parameters that can
    //! be passed to a CShader
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
    //! ShaderDefs defines the shader definitions that were parsed from HLSL
    //! by ParseShader. These definitions define the variables that each
    //! shader defines (i.e. may be set by the client)
    //**********************************************************************
    struct ShaderDefs
    {
        EShaderParamType m_type; ///< Defines type of this parameter
        const wchar_t *m_name;   ///< Name of shader parameter
        uint32 m_offset;         ///< register offset
    };

    //**********************************************************************
    //! ShaderParam defines each parameter that a shader exposes. These parameters
    //! are derived from the ShaderDefs above. This is servers copy of each
    //! parameter along with its value and position in the constant buffer.
    //**********************************************************************
    struct ShaderParam
    {
        EShaderParamType m_type; //!< Defines type of this parameter
        uint32 m_offset;         //!< Offset into our constant buffer where this parameter resides
        std::wstring m_name;     //!< name of this parameter
        std::any m_value;      //!< Value assigned to this parameter
        std::vector<std::any> m_values;
        bool m_dirty;            //!< Is parameter dirty and needs to be pushed to constant buffer
    };

    struct Float { float x; Float(float _x) { x = _x; } };
    struct Int { int x; Int(int _x) { x = _x; } };
    struct Float2 { float x; float y; Float2(float _x, float _y) { x = _x; y = _y; } };
    struct Float3 { float x; float y; float z; Float3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; } };
    struct Float4 { float x; float y; float z; float w; Float4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; } };
    struct Matrix { float x[16]; Matrix() { ZeroMemory(x, sizeof(x)); } Matrix(float _x[16]) { memcpy(x, _x, sizeof(float) * 16); } };
    
    //**********************************************************************
    //! \brief CShader defines a shader used to render materials on an object
    //!
    //! A shader defines a material on a renderable. It is comprised of a pixel
    //! shader, a vertex shader, and constants passed to those shaders.
    //**********************************************************************
    class CShader : public IShader, public CRefCount
    {
        std::wstring m_name;
        CComPtr<ID3D11SamplerState> m_spSamplerState;
        CComPtr<ID3D11InputLayout> m_spLayout;
        CComPtr<ID3D11PixelShader> m_spPixelShader;
        CComPtr<ID3D11VertexShader> m_spVertexShader;
        CComPtr<ID3D11Buffer> m_spVertexConstants;
        CComPtr<ID3D11Buffer> m_spPixelConstants;
        std::vector<ShaderParam> m_psParams;
        std::vector<ShaderParam> m_vsParams;
    protected:
        void PushMatrix(const wchar_t *name, std::any mat);
        void PushMatrices(IGraphics *pGraphics, DirectX::XMMATRIX *pWorld);
        uint32 ComputeParamSize(ShaderDefs *pParams, uint32 numParams, std::vector<ShaderParam> &params);
        void PushConstants(IGraphics *pGraphics, ID3D11Buffer *pBuffer, std::vector<ShaderParam> &params);
        void SetParam(std::wstring paramName, std::any &value, std::vector<ShaderParam> &params);
        void SetParam(std::wstring paramName, int index, std::any &value, std::vector<ShaderParam> &params);
    public:
        friend CAUSTICAPI void CreateShader(IGraphics *pGraphics, 
                                            const wchar_t *pShaderName,
                                            ShaderDefs *pPSParams, uint32 psParamsSize,
                                            ShaderDefs *pVSParams, uint32 vsParamsSize,
                                            const byte *pPSByteCodes, uint32 psBufferLen, 
                                            const byte *pVSByteCodes, uint32 vsBufferLen,
                                            D3D11_INPUT_ELEMENT_DESC *pLayout, uint32 numLayoutElems,
                                            IShader **ppShader);
        
        void Create(IGraphics *pGraphics, 
                    const wchar_t *pShaderName,
                    ShaderDefs *pPSParams, uint32 psParamsSize,
                    ShaderDefs *pVSParams, uint32 vsParamsSize,
                    const byte *pVSByteCodes, uint32 vsBufferLen,
                    const byte *pPSByteCodes, uint32 psBufferLen, 
                    D3D11_INPUT_ELEMENT_DESC *pLayout, uint32 numLayoutElems);
        void CreateConstantBuffer(ID3D11Device *pDevice, ShaderDefs *pParams, uint32 paramsSize, std::vector<ShaderParam> &params, ID3D11Buffer **ppBuffer);
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IShader
        //**********************************************************************
        virtual std::wstring &Name() override { return m_name; }
        virtual void BeginRender(IGraphics *pGraphics, DirectX::XMMATRIX *pWorld = nullptr) override;
        virtual void SetPSParam(std::wstring paramName, std::any &value) override;
        virtual void SetPSParam(std::wstring paramName, int index, std::any &value) override;
        virtual void SetVSParam(std::wstring paramName, std::any &value) override;
        virtual void SetVSParam(std::wstring paramName, int index, std::any &value) override;
        virtual void EndRender(IGraphics *pGraphics) override;
    };

    CAUSTICAPI void CreateShader(IGraphics *pGraphics, 
                                 const wchar_t *pShaderName,
                                 ShaderParam *pPSParams, uint32 psParamsSize,
                                 ShaderParam *pVSParams, uint32 vsParamsSize,
                                 const byte *pPSByteCodes, uint32 psBufferLen, 
                                 const byte *pVSByteCodes, uint32 vsBufferLen,
                                 D3D11_INPUT_ELEMENT_DESC *pLayout, uint32 numLayoutElems,
                                 IShader **ppShader);

    //**********************************************************************
    //! \brief CShaderMgr defines our shader manager. Each shader is registered
    //! with this manager.
    //**********************************************************************
    class CShaderMgr : public IShaderMgr, public CRefCount
    {
        std::map<std::wstring, CRefObj<IShader>> m_shaders; ///< List of registered shaders
        static CShaderMgr s_ShaderMgr; ///< Our shader manager singleton
    public:
        static IShaderMgr *GetInstance() { return &s_ShaderMgr; }
        
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
