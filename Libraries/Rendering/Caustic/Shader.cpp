//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Shader.h"
#include "Sampler.h"
#include <DirectXMath.h>
#include <memory>

namespace Caustic
{
    //**********************************************************************
    // \brief CShader::ComputeParamSize parses the definitions of each shader
    // parameter that was read from the HLSL (using ParseShader.exe). For each
    // definition we will compute the offset of the parameter within the constant
    // buffer. This is used later when updating the constant buffer.
    // \param[in] pDefs List of shader definitions defining each parameter.
    // \param[in] numParams Number of parameters in params
    // \param[out] params Parameter list we will copy definitions into
    //**********************************************************************
    uint32 CShader::ComputeParamSize(ShaderDefs *pDefs, uint32 numParams, std::vector<ShaderParam> &params)
    {
        uint32 s = 0;
        params.resize(numParams - 1); // -1 since we won't add the last (undefined) entry
        for (size_t i = 0; i < numParams; i++)
        {
            if (wcslen(pDefs[i].m_name) == 0)
                continue;
            params[i].m_dirty = true;
            params[i].m_name = pDefs[i].m_name;
            params[i].m_type = pDefs[i].m_type;
            params[i].m_offset = s;
            if (pDefs[i].m_type == EShaderParamType::ShaderType_Float)
            {
                params[i].m_value = std::any(0.0f);
                s += sizeof(float);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float_Array)
            {
                for (size_t j = 0; j < c_ArraySize; j++)
                    params[i].m_values.push_back(Float(0.0f));
                s += c_ArraySize * sizeof(float);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float2)
            {
                params[i].m_value = std::any(Float2(0.0f, 0.0f));
                s += sizeof(float) * 2;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float2_Array)
            {
                for (size_t j = 0; j < c_ArraySize; j++)
                    params[i].m_values.push_back(Float2(0.0f, 0.0f));
                s += c_ArraySize * sizeof(float) * 2;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float3)
            {
                params[i].m_value = std::any(Float3(0.0f, 0.0f, 0.0f));
                s += sizeof(float) * 3;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float3_Array)
            {
                for (size_t j = 0; j < c_ArraySize; j++)
                    params[i].m_values.push_back(Float3(0.0f, 0.0f, 0.0f));
                s += c_ArraySize * sizeof(float) * 3;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float4)
            {
                params[i].m_value = std::any(Float4(0.0f, 0.0f, 0.0f, 0.0f));
                s += sizeof(float) * 4;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float4_Array)
            {
                for (size_t j = 0; j < c_ArraySize; j++)
                    params[i].m_values.push_back(Float4(0.0f, 0.0f, 0.0f, 0.0f));
                s += c_ArraySize * sizeof(float) * 4;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Int)
            {
                params[i].m_value = std::any(Int(0));
                s += sizeof(int);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Int_Array)
            {
                for (size_t j = 0; j < c_ArraySize; j++)
                    params[i].m_values.push_back(Int(0));
                s += c_ArraySize * sizeof(int);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Matrix)
            {
                float v[16] = {
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                };
                Matrix m(v);
                params[i].m_value = std::any(m);
                s += 16 * sizeof(float);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Matrix_Array)
            {
                float v[16] = {
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                };
                Matrix m(v);
                for (size_t j = 0; j < c_ArraySize; j++)
                    params[i].m_values.push_back(m);
                s += c_ArraySize * 16 * sizeof(float);
            }
        }
        return s;
    }

    //**********************************************************************
    // \brief CShader::PushConstants pushes each shader parameter into the D3D11 constant buffer.
    // \param pGraphics D3D11 device/context to use
    // \param pBuffer Constant buffer to push values into
    // \param params List of parameters to push
    //**********************************************************************
    void CShader::PushConstants(IGraphics *pGraphics, ID3D11Buffer *pBuffer, std::vector<ShaderParam> &params)
    {
        // First push samplers
        for (size_t i = 0; i < params.size(); i++)
        {
            if ( ! params[i].m_dirty)
                continue;
            switch(params[i].m_type)
            {
            case EShaderParamType::ShaderType_Sampler:
                {
                    Caustic::CSamplerRef v = std::any_cast<CSamplerRef>(params[i].m_value);
                    v.m_spSampler->Render(pGraphics, params[i].m_offset);
                }
                break;
            }
        }

        // Next push constants
        if (pBuffer == nullptr)
            return;
        D3D11_MAPPED_SUBRESOURCE ms;
        CT(pGraphics->GetContext()->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        BYTE *pb = reinterpret_cast<BYTE*>(ms.pData);
        for (size_t i = 0; i < params.size(); i++)
        {
            if ( ! params[i].m_dirty)
                continue;
            switch(params[i].m_type)
            {
            case EShaderParamType::ShaderType_Sampler:
                break;
            case EShaderParamType::ShaderType_Texture:
                break;
            case EShaderParamType::ShaderType_Float:
                {
                    float f = std::any_cast<float>(params[i].m_value);
                    memcpy(pb, &f, sizeof(float));
                    pb += sizeof(float);
                }
                break;
            case EShaderParamType::ShaderType_Float_Array:
                {
                    for (size_t j = 0; j < c_ArraySize; j++)
                    {
                        Float v = std::any_cast<Float>(params[i].m_values[j]);
                        memcpy(pb, &v, sizeof(Float));
                        pb += sizeof(Float);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Float2:
                {
                    Float2 f = std::any_cast<Float2>(params[i].m_value);
                    memcpy(pb, &f, sizeof(Float2));
                    pb += sizeof(Float2);
                }
                break;
            case EShaderParamType::ShaderType_Float2_Array:
                {
                    for (size_t j = 0; j < c_ArraySize; j++)
                    {
                        Float2 v = std::any_cast<Float2>(params[i].m_values[j]);
                        memcpy(pb, &v, sizeof(Float2));
                        pb += sizeof(Float2);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Float3:
                {
                    Float3 f = std::any_cast<Float3>(params[i].m_value);
                    memcpy(pb, &f, sizeof(Float3));
                    pb += sizeof(Float3);
                }
                break;
            case EShaderParamType::ShaderType_Float3_Array:
                {
                    for (size_t j = 0; j < c_ArraySize; j++)
                    {
                        Float3 v = std::any_cast<Float3>(params[i].m_values[j]);
                        memcpy(pb, &v, sizeof(Float3));
                        pb += sizeof(Float3);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Float4:
                {
                    Float4 f = std::any_cast<Float4>(params[i].m_value);
                    memcpy(pb, &f, sizeof(Float4));
                    pb += sizeof(Float4);
                }
                break;
            case EShaderParamType::ShaderType_Float4_Array:
                {
                    for (size_t j = 0; j < c_ArraySize; j++)
                    {
                        Float4 v = std::any_cast<Float4>(params[i].m_values[j]);
                        memcpy(pb, &v, sizeof(Float4));
                        pb += sizeof(Float4);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Int:
                {
                    Int v = std::any_cast<Int>(params[i].m_value);
                    memcpy(pb, &v, sizeof(int));
                    pb += sizeof(int);
                }
                break;
            case EShaderParamType::ShaderType_Int_Array:
                {
                    for (size_t j = 0; j < c_ArraySize; j++)
                    {
                        Int v = std::any_cast<Int>(params[i].m_values[j]);
                        memcpy(pb, &v.x, sizeof(int));
                        pb += sizeof(int);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Matrix:
                {
                    Matrix m = std::any_cast<Matrix>(params[i].m_value);
                    memcpy(pb, m.x, 16 * sizeof(float));
                    pb += 16 * sizeof(float);
                }
                break;
            case EShaderParamType::ShaderType_Matrix_Array:
                {
                    for (size_t j = 0; j < c_ArraySize; j++)
                    {
                        Matrix m = std::any_cast<Matrix>(params[i].m_values[j]);
                        memcpy(pb, m.x, 16 * sizeof(float));
                        pb += 16 * sizeof(float);
                    }
                }
                break;
            }
        }
        pGraphics->GetContext()->Unmap(pBuffer, 0);
    }

    //**********************************************************************
    // \brief SetParam sets shader constant, texture or sampler
    // \param[in] paramName Name of the parameter
    // \param[in] value Value of parameter
    // \param[in] params List of parameters to update
    //**********************************************************************
    void CShader::SetParam(std::wstring paramName, std::any &value, std::vector<ShaderParam> &params)
    {
        for (size_t i = 0; i < params.size(); i++)
        {
            if (params[i].m_name == paramName)
            {
                params[i].m_value = value;
                params[i].m_dirty = true;
                break;
            }
        }
    }

    //**********************************************************************
    // \brief SetParam sets shader constant, texture or sampler
    // \param[in] paramName Name of the parameter
    // \param[in] value Value of parameter
    // \param[in] params List of parameters to update
    //**********************************************************************
    void CShader::SetParam(std::wstring paramName, int index, std::any &value, std::vector<ShaderParam> &params)
    {
        for (size_t i = 0; i < params.size(); i++)
        {
            if (params[i].m_name == paramName)
            {
                params[i].m_values[index] = value;
                params[i].m_dirty = true;
                break;
            }
        }
    }

    //**********************************************************************
    // \brief SetPSParam sets a pixel shader parameter
    // \param[in] paramName Name of parameter
    // \param[in] value Value of parameter
    //**********************************************************************
    void CShader::SetPSParam(std::wstring paramName, std::any &value)
    {
        SetParam(paramName, value, m_psParams);
    }

    //**********************************************************************
    // \brief SetPSParam sets a pixel shader array element parameter
    // \param[in] paramName Name of parameter
    // \param[in] index Index into array
    // \param[in] value Value of parameter
    //**********************************************************************
    void CShader::SetPSParam(std::wstring paramName, int index, std::any &value)
    {
        SetParam(paramName, index, value, m_psParams);
    }

    //**********************************************************************
    // \brief SetVSParam sets a vertex shader parameter
    // \param[in] paramName Name of parameter
    // \param[in] value Value of parameter
    //**********************************************************************
    void CShader::SetVSParam(std::wstring paramName, std::any &value)
    {
        SetParam(paramName, value, m_vsParams);
    }

    //**********************************************************************
    // \brief SetPSParam sets a vertex shader array element parameter
    // \param[in] paramName Name of parameter
    // \param[in] index Index into array
    // \param[in] value Value of parameter
    //**********************************************************************
    void CShader::SetVSParam(std::wstring paramName, int index, std::any &value)
    {
        SetParam(paramName, index, value, m_psParams);
    }

    static Matrix D3DMatrixToMatrix(DirectX::XMMATRIX &mat)
    {
        Matrix m;
        m.x[0] = DirectX::XMVectorGetX(mat.r[0]);
        m.x[1] = DirectX::XMVectorGetX(mat.r[1]);
        m.x[2] = DirectX::XMVectorGetX(mat.r[2]);
        m.x[3] = DirectX::XMVectorGetX(mat.r[3]);
        m.x[4] = DirectX::XMVectorGetY(mat.r[0]);
        m.x[5] = DirectX::XMVectorGetY(mat.r[1]);
        m.x[6] = DirectX::XMVectorGetY(mat.r[2]);
        m.x[7] = DirectX::XMVectorGetY(mat.r[3]);
        m.x[8] = DirectX::XMVectorGetZ(mat.r[0]);
        m.x[9] = DirectX::XMVectorGetZ(mat.r[1]);
        m.x[10] = DirectX::XMVectorGetZ(mat.r[2]);
        m.x[11] = DirectX::XMVectorGetZ(mat.r[3]);
        m.x[12] = DirectX::XMVectorGetW(mat.r[0]);
        m.x[13] = DirectX::XMVectorGetW(mat.r[1]);
        m.x[14] = DirectX::XMVectorGetW(mat.r[2]);
        m.x[15] = DirectX::XMVectorGetW(mat.r[3]);
        return m;
    }

    void CShader::PushMatrix(const wchar_t *pParamName, std::any mat)
    {
        SetVSParam(pParamName, mat);
        SetPSParam(pParamName, mat);
    }

    void CShader::PushMatrices(IGraphics *pGraphics, DirectX::XMMATRIX *pWorld)
    {
        DirectX::XMMATRIX view = pGraphics->GetCamera()->GetView();
        DirectX::XMMATRIX proj = pGraphics->GetCamera()->GetProjection();

        DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
        if (pWorld == nullptr)
            pWorld = &identity;
        PushMatrix(L"world", std::any(D3DMatrixToMatrix(*pWorld)));
        PushMatrix(L"worldInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, *pWorld))));
        PushMatrix(L"worldInvTranspose", std::any(D3DMatrixToMatrix(DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, *pWorld)))));
        PushMatrix(L"view", std::any(D3DMatrixToMatrix(view)));
        PushMatrix(L"viewInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, view))));
        PushMatrix(L"proj", std::any(D3DMatrixToMatrix(proj)));
        PushMatrix(L"projInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, proj))));
        DirectX::XMMATRIX wv = DirectX::XMMatrixMultiply(*pWorld, view);
        PushMatrix(L"worldView", std::any(D3DMatrixToMatrix(wv)));
        PushMatrix(L"worldViewInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, wv))));
        DirectX::XMMATRIX wvp = DirectX::XMMatrixMultiply(wv, proj);
        PushMatrix(L"worldViewProj", std::any(D3DMatrixToMatrix(wvp)));
        PushMatrix(L"worldViewProjInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, wvp))));
    }

    //**********************************************************************
    // \brief BeginRender is called before rendering using this shader occurs.
    // This call is responsible for setting up the pGraphics device to use the shader.
    // \param[in] pGraphics D3D11 device/context to use
    //**********************************************************************
    void CShader::BeginRender(IGraphics *pGraphics, DirectX::XMMATRIX *pWorld)
    {
        pGraphics->GetContext()->IASetInputLayout(m_spLayout);
        pGraphics->GetContext()->PSSetShader(m_spPixelShader, nullptr, 0);
        pGraphics->GetContext()->VSSetShader(m_spVertexShader, nullptr, 0);
        
        PushMatrices(pGraphics, pWorld);
        PushConstants(pGraphics, m_spVertexConstants.p, m_vsParams);
        PushConstants(pGraphics, m_spPixelConstants.p, m_psParams);
        
        pGraphics->GetContext()->VSSetConstantBuffers(0, 1, &m_spVertexConstants.p);
        pGraphics->GetContext()->PSSetConstantBuffers(0, 1, &m_spPixelConstants.p);
    }

    //**********************************************************************
    // EndRender is called after rendering using this shader occurs.
    // During this call the shader may clean up any state/memory it needed
    // \param[in] pGraphics D3D11 device/context to use
    //**********************************************************************
    void CShader::EndRender(IGraphics * /*pGraphics*/)
    {
    }

    //**********************************************************************
    // \brief CreateConstantBuffer creates the constant buffer (pixel or vertex shader)
    // \param[in] pDevice D3D device
    // \param[in] pDefs List of parameter definitions (parsed from HLSL)
    // \param[in] paramsSize Length of pDefs
    // \param[out] params Generated parameter list
    // \param[out] ppBuffer Returns the created constant buffer
    //**********************************************************************
    void CShader::CreateConstantBuffer(ID3D11Device *pDevice, ShaderDefs *pDefs, uint32 paramsSize, std::vector<ShaderParam> &params, ID3D11Buffer **ppBuffer)
    {
        uint32 s = ComputeParamSize(pDefs, paramsSize, params);
        if (s > 0)
        {
            D3D11_BUFFER_DESC buffDesc;
            buffDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
            buffDesc.ByteWidth = ((s+15)/16)*16;
            buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
            buffDesc.MiscFlags = 0;
            buffDesc.StructureByteStride = 0;
            buffDesc.Usage = D3D11_USAGE_DYNAMIC;
            CT(pDevice->CreateBuffer(&buffDesc, nullptr, ppBuffer));
        }
        else
            *ppBuffer = nullptr;
    }

    //**********************************************************************
    // \brief Create creates a shader
    // \param[in] pGraphics Graphics device to use
    // \param[in] pShaderName Name of shader
    // \param[in] pPSParams List of vertex shader definitions (from ParseShader.exe)
    // \param[in] psParamsSize Length of pPSParams
    // \param[in] pVSParams List of pixel shader definitions (from ParseShader.exe)
    // \param[in] vsParamsSize Length of pVSParams
    // \param[in] pPSByteCodes Byte code for pixel shader
    // \param[in] psBufferLen Length of pPSByteCodes in bytes
    // \param[in] pVSByteCodes Byte code for vertex shader
    // \param[in] vsBufferLen Length of pVSByteCodes in bytes
    // \param[in] pLayout Vertex layout
    // \param[in] numLayoutElems Length of pLayout (in elements)
    //**********************************************************************
    void CShader::Create(
        IGraphics *pGraphics,
        const wchar_t *pShaderName,
        ShaderDefs *pPSParams, uint32 psParamsSize,
        ShaderDefs *pVSParams, uint32 vsParamsSize,
        const byte *pPSByteCodes, uint32 psBufferLen, 
        const byte *pVSByteCodes, uint32 vsBufferLen,
        D3D11_INPUT_ELEMENT_DESC *pLayout, uint32 numLayoutElems)
    {
        if (pShaderName)
            m_name = std::wstring(pShaderName);
        ID3D11Device *pDevice = pGraphics->GetDevice();
        CT(pDevice->CreateVertexShader(pVSByteCodes, vsBufferLen, nullptr, &m_spVertexShader));
        CT(pDevice->CreatePixelShader(pPSByteCodes, psBufferLen, nullptr, &m_spPixelShader));
        CD3D11_SAMPLER_DESC sdesc(D3D11_DEFAULT);
        CT(pDevice->CreateSamplerState(&sdesc, &m_spSamplerState));
        CT(pDevice->CreateInputLayout(pLayout, numLayoutElems, pVSByteCodes, vsBufferLen, &m_spLayout));
        CreateConstantBuffer(pDevice, pVSParams, vsParamsSize, m_vsParams, &m_spVertexConstants);
        CreateConstantBuffer(pDevice, pPSParams, psParamsSize, m_psParams, &m_spPixelConstants);
    }
    
    //**********************************************************************
    // \brief CreateShader is a helper function to create a shader
    // \param[in] pGraphics Graphics device to use
    // \param[in] pShaderName Name of shader
    // \param[in] pPSParams List of vertex shader definitions (from ParseShader.exe)
    // \param[in] psParamsSize Length of pPSParams
    // \param[in] pVSParams List of pixel shader definitions (from ParseShader.exe)
    // \param[in] vsParamsSize Length of pVSParams
    // \param[in] pPSByteCodes Byte code for pixel shader
    // \param[in] psBufferLen Length of pPSByteCodes in bytes
    // \param[in] pVSByteCodes Byte code for vertex shader
    // \param[in] vsBufferLen Length of pVSByteCodes in bytes
    // \param[in] pLayout Vertex layout
    // \param[in] numLayoutElems Length of pLayout (in elements)
    // \param[out] ppShader Returns the created shader
    //**********************************************************************
    CAUSTICAPI void CreateShader(IGraphics *pGraphics, 
                                 const wchar_t *pShaderName,
                                 ShaderDefs *pPSParams, uint32 psParamsSize,
                                 ShaderDefs *pVSParams, uint32 vsParamsSize,
                                 const byte *pPSByteCodes, uint32 psBufferLen, 
                                 const byte *pVSByteCodes, uint32 vsBufferLen,
                                 D3D11_INPUT_ELEMENT_DESC *pLayout, uint32 numLayoutElems,
                                 IShader **ppShader)
    {
        std::unique_ptr<CShader> spShader(new CShader());
        spShader->Create(pGraphics, pShaderName,
                         pPSParams, psParamsSize, 
                         pVSParams, vsParamsSize,
                         pPSByteCodes, psBufferLen,
                         pVSByteCodes, vsBufferLen, 
                         pLayout, numLayoutElems);
        *ppShader = spShader.release();
        (*ppShader)->AddRef();
    }
    
    CShaderMgr CShaderMgr::s_ShaderMgr;

    //**********************************************************************
    // \brief FindShader returns the requested shader
    // \param[in] pShaderName Name of shader to locate
    // \param[in] ppShader Returns the requested shader
    //**********************************************************************
    void CShaderMgr::FindShader(const wchar_t *pShaderName, IShader **ppShader)
    {
        std::map<std::wstring,CRefObj<IShader>>::iterator it = m_shaders.find(pShaderName);
        if (it == m_shaders.end())
        {
            *ppShader = nullptr;
        }
        else
        {
            *ppShader = it->second.p;
            (*ppShader)->AddRef();
        }
    }

    //**********************************************************************
    // \brief RegisterShader registers the specified shader with the shader manager
    // \param[in] pShaderName Name of shader
    // \param[in] pShader Shader to register
    //**********************************************************************
    void CShaderMgr::RegisterShader(const wchar_t *pShaderName, IShader *pShader)
    {
        std::map<std::wstring,CRefObj<IShader>>::iterator it = m_shaders.find(pShaderName);
        if (it == m_shaders.end())
        {
            m_shaders[std::wstring(pShaderName)] = CRefObj<IShader>(pShader);
        }
        else
        {
            CT(E_FAIL);
        }
    }
};
