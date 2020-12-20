//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Shader.h"
#include "Sampler.h"
#include "IShaderInfo.h"
#include "IRenderMaterial.h"
#include "IPointLight.h"
#include <DirectXMath.h>
#include <memory>
#include <d3d11_4.h>

namespace Caustic
{
    uint32 CShader::ShaderTypeSize(ShaderParamDef& paramDef)
    {
        if (paramDef.m_type == EShaderParamType::ShaderType_Float)
            return sizeof(float);
        else if (paramDef.m_type == EShaderParamType::ShaderType_Float_Array)
            return paramDef.m_members * sizeof(float);
        else if (paramDef.m_type == EShaderParamType::ShaderType_Float2)
            return sizeof(float) * 2;
        else if (paramDef.m_type == EShaderParamType::ShaderType_Float2_Array)
            return paramDef.m_members * sizeof(float) * 2;
        else if (paramDef.m_type == EShaderParamType::ShaderType_Float3)
            return sizeof(float) * 3;
        else if (paramDef.m_type == EShaderParamType::ShaderType_Float3_Array)
            return paramDef.m_members * sizeof(float) * 3;
        else if (paramDef.m_type == EShaderParamType::ShaderType_Float4)
            return sizeof(float) * 4;
        else if (paramDef.m_type == EShaderParamType::ShaderType_Float4_Array)
            return paramDef.m_members * sizeof(float) * 4;
        else if (paramDef.m_type == EShaderParamType::ShaderType_Int)
            return sizeof(int);
        else if (paramDef.m_type == EShaderParamType::ShaderType_Int_Array)
            return paramDef.m_members * sizeof(int);
        else if (paramDef.m_type == EShaderParamType::ShaderType_Matrix)
            return 16 * sizeof(float);
        else if (paramDef.m_type == EShaderParamType::ShaderType_Matrix_Array)
            return paramDef.m_members * 16 * sizeof(float);
        else if (paramDef.m_type == EShaderParamType::ShaderType_Matrix3x3)
            return 12 * sizeof(float); // Each row is float4 in HLSL, hence 12 instead of 9
        else if (paramDef.m_type == EShaderParamType::ShaderType_Matrix3x3_Array)
            return paramDef.m_members * 12 * sizeof(float); // Each row is float4 in HLSL, hence 12 instead of 9
        else if (paramDef.m_type == EShaderParamType::ShaderType_StructuredBuffer ||
            paramDef.m_type == EShaderParamType::ShaderType_AppendStructuredBuffer ||
            paramDef.m_type == EShaderParamType::ShaderType_RWByteAddressBuffer ||
            paramDef.m_type == EShaderParamType::ShaderType_RWStructuredBuffer ||
            paramDef.m_type == EShaderParamType::ShaderType_Texture ||
            paramDef.m_type == EShaderParamType::ShaderType_Sampler)
        {
            // Structured buffers (used by compute shaders) and textures/samplers are not
            // included in the final buffer count since those buffers are not part of
            // the constant buffer. So, do nothing here.
        }
        else
            CT(E_UNEXPECTED);
        return 0;
    }

    //**********************************************************************
    // Method: Clone
    // See <IShader::Clone>
    //**********************************************************************
    CRefObj<IShader> CShader::Clone(ID3D11Device *pDevice)
    {
        std::unique_ptr<CShader> spShader(new CShader());
        spShader->m_name = this->m_name;
        spShader->m_layout = this->m_layout;
        spShader->m_spSamplerState = this->m_spSamplerState;
        spShader->m_spLayout = this->m_spLayout;
        spShader->m_spPixelShader = this->m_spPixelShader;
        spShader->m_spVertexShader = this->m_spVertexShader;
        spShader->m_spComputeShader = this->m_spComputeShader;
        spShader->m_spShaderInfo = this->m_spShaderInfo;
        spShader->m_xThreads = this->m_xThreads;
        spShader->m_yThreads = this->m_yThreads;
        spShader->m_zThreads = this->m_zThreads;
        CreateConstantBuffer(pDevice, m_spShaderInfo->VertexShaderParameterDefs().data(),
            (uint32)m_spShaderInfo->VertexShaderParameterDefs().size(), m_vsParams, &m_vertexConstants);
        CreateConstantBuffer(pDevice, m_spShaderInfo->PixelShaderParameterDefs().data(),
            (uint32)m_spShaderInfo->PixelShaderParameterDefs().size(), m_psParams, &m_pixelConstants);
        CreateConstantBuffer(pDevice, m_spShaderInfo->ComputeShaderParameterDefs().data(),
            (uint32)m_spShaderInfo->ComputeShaderParameterDefs().size(), m_csParams, &m_computeConstants);
        return CRefObj<IShader>(spShader.release());
    }

    //**********************************************************************
    // Method: ComputeParamSize
    // Parses the definitions of each shader parameter that was read from
    // the HLSL (using ParseShader.exe). For each definition we will compute
    // the offset of the parameter within the constant buffer. This is used
    // later when updating the constant buffer.
    //
    // Parameters:
    // pDefs - List of shader definitions defining each parameter.
    // numParams - Number of parameters in params
    // params - Parameter list we will copy definitions into
    //**********************************************************************
    uint32 CShader::ComputeParamSize(ShaderParamDef* pDefs, uint32 numParams, std::vector<ShaderParamInstance>& params)
    {
        if (pDefs == nullptr || numParams == 0)
            return 0;
        uint32 s = 0;
        params.resize(numParams);
        for (size_t i = 0; i < numParams; i++)
        {
            if (pDefs[i].m_name.length() == 0)
                continue;
            ShaderParamDef& d = params[i];
            d = pDefs[i];
            s += ShaderTypeSize(d);
            params[i].m_dirty = true;
            params[i].m_offset = (pDefs[i].m_type == EShaderParamType::ShaderType_Texture ||
                pDefs[i].m_type == EShaderParamType::ShaderType_AppendStructuredBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_RWStructuredBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_RWByteAddressBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_StructuredBuffer) ? pDefs[i].m_offset : s;
            if (pDefs[i].m_type == EShaderParamType::ShaderType_Float)
                params[i].m_value = std::any(0.0f);
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Float(0.0f));
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float2)
                params[i].m_value = std::any(Float2(0.0f, 0.0f));
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float2_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Float2(0.0f, 0.0f));
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float3)
                params[i].m_value = std::any(Float3(0.0f, 0.0f, 0.0f));
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float3_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Float3(0.0f, 0.0f, 0.0f));
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float4)
                params[i].m_value = std::any(Float4(0.0f, 0.0f, 0.0f, 0.0f));
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float4_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Float4(0.0f, 0.0f, 0.0f, 0.0f));
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Int)
                params[i].m_value = std::any(Int(0));
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Int_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Int(0));
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
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(m);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Matrix3x3)
            {
                float v[9] = {
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f
                };
                Matrix_3x3 m(v);
                params[i].m_value = std::any(m);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Matrix3x3_Array)
            {
                float v[9] = {
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f
                };
                Matrix_3x3 m(v);
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(m);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_StructuredBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_AppendStructuredBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_RWByteAddressBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_RWStructuredBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_Texture ||
                pDefs[i].m_type == EShaderParamType::ShaderType_Sampler)
            {
                // Structured buffers (used by compute shaders) and textures/samplers are not
                // included in the final buffer count since those buffers are not part of
                // the constant buffer. So, do nothing here.
            }
            else
                CT(E_UNEXPECTED);
        }
        return s;
    }

    void CShader::ClearSamplers(IRenderer* pRenderer)
    {
        // Set deafult sampler
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
        spCtx->PSSetSamplers(0, 1, &m_spSamplerState.p);
        spCtx->VSSetSamplers(0, 1, &m_spSamplerState.p);
        
        // Clear the old textures
        ID3D11ShaderResourceView* const nullref[1] = { NULL };
        for (int i = 0; i < m_maxTextureSlot; i++)
            spCtx->PSSetShaderResources(i, 1, nullref);
        m_maxTextureSlot = 0;
    }

    void CShader::PushSamplers(IRenderer* pRenderer, std::vector<ShaderParamInstance>& params, bool isPixelShader)
    {
        // First push samplers
        for (size_t i = 0; i < params.size(); i++)
        {
            if (!params[i].m_dirty)
                continue;
            switch (params[i].m_type)
            {
            case EShaderParamType::ShaderType_Texture:
                {
                    if (params[i].m_value.has_value())
                    {
                        CRefObj<ITexture> v = std::any_cast<CRefObj<ITexture>>(params[i].m_value);
                        v->Render(pRenderer, params[i].m_offset, isPixelShader);
                        if (params[i].m_offset > (uint32)m_maxTextureSlot)
                            m_maxTextureSlot = params[i].m_offset;
                    }
                }
                break;
            case EShaderParamType::ShaderType_Sampler:
                {
                    if (params[i].m_value.has_value())
                    {
                        Caustic::CSamplerRef v = std::any_cast<CSamplerRef>(params[i].m_value);
                        v.m_spSampler->Render(pRenderer, params[i].m_offset, isPixelShader);
                    }
                }
                break;
            }
        }
    }

    //**********************************************************************
    // Method: PushConstants
    // Pushes each shader parameter into the D3D11 constant buffer.
    //
    // Parameters:
    // pRenderer - D3D11 device/context to use
    // pBuffer - Constant buffer to push values into
    // params - List of parameters to push
    //**********************************************************************
    void CShader::PushConstants(IRenderer* pRenderer, SBuffer* pBuffer, std::vector<ShaderParamInstance>& params)
    {
        if (pBuffer == nullptr || pBuffer->m_bufferSize == 0)
            return;

        D3D11_MAPPED_SUBRESOURCE ms;
        CT(pRenderer->GetContext()->Map(pBuffer->m_spBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        BYTE* pb = reinterpret_cast<BYTE*>(ms.pData);
        for (size_t i = 0; i < params.size(); i++)
        {
            switch (params[i].m_type)
            {
            case EShaderParamType::ShaderType_Sampler:
                break;
            case EShaderParamType::ShaderType_Texture:
                break;
            case EShaderParamType::ShaderType_Float:
            {
                float f = (params[i].m_value.has_value()) ? std::any_cast<float>(params[i].m_value) : 0.0f;
                memcpy(pb, &f, sizeof(float));
                pb += sizeof(float);
            }
            break;
            case EShaderParamType::ShaderType_Float_Array:
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                {
                    Float v = (params[i].m_values[j].has_value()) ? std::any_cast<Float>(params[i].m_values[j]) : 0.0f;
                    memcpy(pb, &v, sizeof(Float));
                    pb += sizeof(Float);
                }
            }
            break;
            case EShaderParamType::ShaderType_Float2:
            {
                Float2 f = (params[i].m_value.has_value()) ? std::any_cast<Float2>(params[i].m_value) : Float2(0.0f, 0.0f);
                memcpy(pb, &f, sizeof(Float2));
                pb += sizeof(Float2);
            }
            break;
            case EShaderParamType::ShaderType_Float2_Array:
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                {
                    Float2 v = (params[i].m_values[j].has_value()) ? std::any_cast<Float2>(params[i].m_values[j]) : Float2(0.0f, 0.0f);
                    memcpy(pb, &v, sizeof(Float2));
                    pb += sizeof(Float2);
                }
            }
            break;
            case EShaderParamType::ShaderType_Float3:
            {
                Float3 f = (params[i].m_value.has_value()) ? std::any_cast<Float3>(params[i].m_value) : Float3(0.0f, 0.0f, 0.0f);
                memcpy(pb, &f, sizeof(Float3));
                pb += sizeof(Float3);
            }
            break;
            case EShaderParamType::ShaderType_Float3_Array:
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                {
                    Float3 v = (params[i].m_values[j].has_value()) ? std::any_cast<Float3>(params[i].m_values[j]) : Float3(0.0f, 0.0f, 0.0f);
                    memcpy(pb, &v, sizeof(Float3));
                    pb += sizeof(Float3);
                }
            }
            break;
            case EShaderParamType::ShaderType_Float4:
            {
                Float4 f = (params[i].m_value.has_value()) ? std::any_cast<Float4>(params[i].m_value) : Float4(0.0f, 0.0f, 0.0f, 0.0f);
                memcpy(pb, &f, sizeof(Float4));
                pb += sizeof(Float4);
            }
            break;
            case EShaderParamType::ShaderType_Float4_Array:
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                {
                    Float4 v = (params[i].m_values[j].has_value()) ? std::any_cast<Float4>(params[i].m_values[j]) : Float4(0.0f, 0.0f, 0.0f, 0.0f);
                    memcpy(pb, &v, sizeof(Float4));
                    pb += sizeof(Float4);
                }
            }
            break;
            case EShaderParamType::ShaderType_Int:
            {
                Int v = (params[i].m_value.has_value()) ? std::any_cast<Int>(params[i].m_value) : 0;
                memcpy(pb, &v, sizeof(int));
                pb += sizeof(int);
            }
            break;
            case EShaderParamType::ShaderType_Int_Array:
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                {
                    Int v = (params[i].m_values[j].has_value()) ? std::any_cast<Int>(params[i].m_values[j]) : 0;
                    memcpy(pb, &v.x, sizeof(int));
                    pb += sizeof(int);
                }
            }
            break;
            case EShaderParamType::ShaderType_Matrix:
            {
                Matrix m = (params[i].m_value.has_value()) ? std::any_cast<Matrix>(params[i].m_value) : Matrix();
                memcpy(pb, m.x, 16 * sizeof(float));
                pb += 16 * sizeof(float);
            }
            break;
            case EShaderParamType::ShaderType_Matrix_Array:
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                {
                    Matrix m = (params[i].m_values[j].has_value()) ? std::any_cast<Matrix>(params[i].m_values[j]) : Matrix();
                    memcpy(pb, m.x, 16 * sizeof(float));
                    pb += 16 * sizeof(float);
                }
            }
            break;
            case EShaderParamType::ShaderType_Matrix3x3:
            {
                // D3D is so dumb. Apparently it will pack a float3x3 as if
                // it is 3 float4s where the W component is a packing value.
                // So stupid.
                // So here I need to pack the values appropriately.
                Matrix_3x3 m = (params[i].m_value.has_value()) ? std::any_cast<Matrix_3x3>(params[i].m_value) : Matrix_3x3();
                for (int i = 0; i < 3; i++)
                {
                    memcpy(pb, &m.x[i * 3], 3 * sizeof(float));
                    pb += 3 * sizeof(float);
                    *(float*)pb = 0.0f;
                    pb += sizeof(float);
                }
            }
            break;
            case EShaderParamType::ShaderType_Matrix3x3_Array:
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                {
                    Matrix_3x3 m = (params[i].m_values[j].has_value()) ? std::any_cast<Matrix_3x3>(params[i].m_values[j]) : Matrix_3x3();
                    // D3D is so dumb. Apparently it will pack a float3x3 as if
                    // it is 3 float4s where the W component is a packing value.
                    // So stupid.
                    // So here I need to pack the values appropriately.
                    for (int i = 0; i < 3; i++)
                    {
                        memcpy(pb, &m.x[i * 3], 3 * sizeof(float));
                        pb += 3 * sizeof(float);
                        *(float*)pb = 0.0f;
                        pb += sizeof(float);
                    }
                }
            }
            break;
            }
        }
        pRenderer->GetContext()->Unmap(pBuffer->m_spBuffer, 0);
    }

    //**********************************************************************
    // Method: PopBuffers
    // PopBuffers copies GPU buffers back to the CPU (after a compute shader
    // has written to them).
    //
    // Parameters:
    // pRenderer - D3D11 device/context to use
    //**********************************************************************
    void CShader::PopBuffers(IRenderer* pRenderer)
    {
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
        for (int bufferIndex = 0; bufferIndex < (int)m_csBuffers.size(); bufferIndex++)
        {
            SBuffer& s = m_csBuffers[bufferIndex];
            if (!s.m_isInput)
            {
                D3D11_MAPPED_SUBRESOURCE ms;
                spCtx->CopyResource(s.m_spStagingBuffer, s.m_spBuffer);
                CT(spCtx->Map(s.m_spStagingBuffer, 0, D3D11_MAP_READ, 0, &ms));
                BYTE* pb = reinterpret_cast<BYTE*>(ms.pData);
                memcpy(s.m_wpOutputBuffer, pb, s.m_bufferSize);
                spCtx->Unmap(s.m_spStagingBuffer, 0);
            }
        }
        //m_csBuffers.clear();
    }

    //**********************************************************************
    // Method: PushBuffers
    // Pushes buffers from the CPU to the GPU (for instance,
    // from a CPU buffer to a GPU structured buffer).
    //
    // Parameters:
    // pRenderer - D3D11 device/context to use
    // pBuffer - Constant buffer to push values into
    // params - List of parameters to push
    //**********************************************************************
    void CShader::PushBuffers(IRenderer* pRenderer,
        std::vector<ShaderParamInstance>& params)
    {
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
        for (size_t i = 0; i < params.size(); i++)
        {
            if (!params[i].m_value.has_value())
                continue;
            if (params[i].m_type != EShaderParamType::ShaderType_AppendStructuredBuffer &&
                params[i].m_type != EShaderParamType::ShaderType_RWStructuredBuffer &&
                params[i].m_type != EShaderParamType::ShaderType_StructuredBuffer &&
                params[i].m_type != EShaderParamType::ShaderType_RWByteAddressBuffer)
                continue;

            uint32 miscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            uint32 bind = 0;
            switch (params[i].m_type)
            {
            case EShaderParamType::ShaderType_AppendStructuredBuffer:
                CT(E_NOTIMPL);
                break;
            case EShaderParamType::ShaderType_StructuredBuffer:
            case EShaderParamType::ShaderType_RWStructuredBuffer:
                miscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
                bind = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
                break;
            case EShaderParamType::ShaderType_RWByteAddressBuffer:
                miscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
                bind = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
                break;
            }
            
            // Check if our underlying buffer has been created
            int bufferIndex = 0;
            for (; bufferIndex < (int)m_csBuffers.size(); bufferIndex++)
            {
                if (m_csBuffers[bufferIndex].m_name == params[i].m_name)
                    break;
            }
            if (bufferIndex == (int)m_csBuffers.size())
            {
                SBuffer buffer;
                buffer.m_bufferSlot = params[i].m_offset;
                buffer.m_name = params[i].m_name;
                m_csBuffers.push_back(buffer);
                bufferIndex = (int)m_csBuffers.size() - 1;
            }

            ClientBuffer& srcVal = std::any_cast<ClientBuffer>(params[i].m_value);
            if (params[i].m_dirty || bufferIndex == -1)
            {
                params[i].m_dirty = false;

                // Recreate the underlying buffer
                uint32 access = 0;
                D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
                uint32 stride, alignment;
                if (params[i].m_type == EShaderParamType::ShaderType_StructuredBuffer ||
                    params[i].m_type == EShaderParamType::ShaderType_RWStructuredBuffer)
                {
                    // This is lame. Structured buffers must have each element be a multiple of 4
                    stride = ((params[i].m_elemSize + 3) / 4) * 4;
                    alignment = stride;
                }
                else
                {
                    stride = params[i].m_elemSize;
                    alignment = 16;
                }
                CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
                CreateBuffer(spDevice, srcVal.m_dataSize, bind | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, access, usage, miscFlags, stride, alignment, &m_csBuffers[bufferIndex], &m_csBuffers[bufferIndex].m_spBuffer.p);
                usage = D3D11_USAGE::D3D11_USAGE_STAGING;
                access = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
                bind = 0;
                miscFlags = 0;
                CreateBuffer(spDevice, srcVal.m_dataSize, bind, access, usage, miscFlags, stride, alignment, &m_csBuffers[bufferIndex], &m_csBuffers[bufferIndex].m_spStagingBuffer.p);

                if (params[i].m_type == EShaderParamType::ShaderType_StructuredBuffer)
                {
                    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
                    ZeroMemory(&srvDesc, sizeof(srvDesc));
                    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
                    srvDesc.Buffer.ElementWidth = srcVal.m_dataSize / stride;
                    srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
                    CT(spDevice->CreateShaderResourceView(m_csBuffers[bufferIndex].m_spBuffer, &srvDesc, &m_csBuffers[bufferIndex].m_spSRView.p));
                }
                else
                {
                    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
                    ZeroMemory(&uavDesc, sizeof(uavDesc));
                    uavDesc.Buffer.FirstElement = 0;
                    uavDesc.Buffer.NumElements = srcVal.m_dataSize / stride;
                    if (params[i].m_type == EShaderParamType::ShaderType_RWStructuredBuffer)
                    {
                        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
                        uavDesc.Buffer.Flags = 0;
                    }
                    else
                    {
                        uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
                        uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
                    }
                    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
                    CT(spDevice->CreateUnorderedAccessView(m_csBuffers[bufferIndex].m_spBuffer, &uavDesc, &m_csBuffers[bufferIndex].m_spUAView.p));
                }

                m_csBuffers[bufferIndex].m_isInput = (srcVal.m_spInputData != nullptr);
                
                if (srcVal.m_spInputData)
                {
                    // Copy the data from the CPU memory to the buffer
                    D3D11_MAPPED_SUBRESOURCE ms;
                    CT(spCtx->Map(m_csBuffers[bufferIndex].m_spStagingBuffer, 0, D3D11_MAP_WRITE, 0, &ms));
                    BYTE* pb = reinterpret_cast<BYTE*>(ms.pData);
                    memcpy(pb, srcVal.m_spInputData.get(), srcVal.m_dataSize);
                    spCtx->Unmap(m_csBuffers[bufferIndex].m_spStagingBuffer, 0);
                    spCtx->CopyResource(m_csBuffers[bufferIndex].m_spBuffer, m_csBuffers[bufferIndex].m_spStagingBuffer);
                }
                if (srcVal.m_wpOutputData)
                    m_csBuffers[bufferIndex].m_wpOutputBuffer = srcVal.m_wpOutputData;
            }

            // Assign the buffer
            if (params[i].m_type == EShaderParamType::ShaderType_StructuredBuffer)
                spCtx->CSSetShaderResources(m_csBuffers[bufferIndex].m_bufferSlot, 1, &m_csBuffers[bufferIndex].m_spSRView.p);
            else
                spCtx->CSSetUnorderedAccessViews(m_csBuffers[bufferIndex].m_bufferSlot, 1, &m_csBuffers[bufferIndex].m_spUAView.p, nullptr);
        }
    }

    //**********************************************************************
    // Method: SetParam
    // Sets shader constant, texture or sampler
    //
    // Parameters:
    // paramName - Name of the parameter
    // value - Value of parameter
    // params - List of parameters to update
    //**********************************************************************
    void CShader::SetParam(std::wstring paramName, std::any &value, std::vector<ShaderParamInstance> &params)
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
    // Method: SetParam
    // Sets shader constant, texture or sampler
    //
    // Parameters:
    // paramName - Name of the parameter
    // value - Value of parameter
    // params - List of parameters to update
    //**********************************************************************
    void CShader::SetParam(std::wstring paramName, int index, std::any &value, std::vector<ShaderParamInstance> &params)
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
    // Method: SetPSParam
    // Sets a pixel shader parameter
    //
    // Parameters:
    // paramName - Name of the parameter
    // value - Value of parameter
    //**********************************************************************
    void CShader::SetPSParam(std::wstring paramName, std::any &value)
    {
        SetParam(paramName, value, m_psParams);
    }

    //**********************************************************************
    // Method: SetPSParam
    // Sets a pixel shader array element parameter
    //
    // Parameters:
    // paramName - Name of parameter
    // index - Index into array
    // value - Value of parameter
    //**********************************************************************
    void CShader::SetPSParam(std::wstring paramName, int index, std::any &value)
    {
        SetParam(paramName, index, value, m_psParams);
    }

    //**********************************************************************
    // Method: SetVSParam
    // Sets a vertex shader parameter
    //
    // Parameters:
    // paramName - Name of parameter
    // value - Value of parameter
    //**********************************************************************
    void CShader::SetVSParam(std::wstring paramName, std::any& value)
    {
        SetParam(paramName, value, m_vsParams);
    }
    
    //**********************************************************************
    // Method: SetVSParam
    // Sets a vertex shader array element parameter
    //
    // Parameters:
    // paramName - Name of parameter
    // index - Index into array
    // value - Value of parameter
    //**********************************************************************
    void CShader::SetVSParam(std::wstring paramName, int index, std::any &value)
    {
        SetParam(paramName, index, value, m_psParams);
    }

    //**********************************************************************
    // Method: SetCSParam
    // Sets a compute shader parameter
    //
    // Parameters:
    // paramName - Name of parameter
    // value - Value of parameter
    //**********************************************************************
    void CShader::SetCSParam(std::wstring paramName, std::any& value)
    {
        SetParam(paramName, value, m_csParams);
    }

    //**********************************************************************
    // Method: SetCSParam
    // Sets a compute shader array element parameter
    //
    // Parameters:
    // paramName - Name of parameter
    // index - Index into array
    // value - Value of parameter
    //**********************************************************************
    void CShader::SetCSParam(std::wstring paramName, int index, std::any& value)
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

    void CShader::PushLights(std::vector<CRefObj<ILight>> &lights)
    {
        uint32 numLights = (uint32)lights.size();
        for (uint32 i = 0; i < numLights; i++)
        {
            Caustic::FRGBColor color = lights[i]->GetColor();
            Float4 lightColor(color.r, color.g, color.b, 1.0f);
            SetParam(L"lightColor", i, std::any(lightColor), m_psParams);
            Caustic::Vector3 pos = lights[i]->GetPosition();
            Float4 lightPos(pos.x, pos.y, pos.z, 1.0f);
            SetParam(L"lightPosWS", i, std::any(lightPos), m_psParams);
        }
        SetPSParam(L"numLights", std::any((Int)numLights));
    }

    void CShader::PushMatrices(IRenderer *pRenderer, DirectX::XMMATRIX *pWorld)
    {
        DirectX::XMMATRIX view = pRenderer->GetCamera()->GetView();
        DirectX::XMMATRIX proj = pRenderer->GetCamera()->GetProjection();

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
    // Method: SetThreadCounts
    // Specifies the number of thread groups to run when dispatching the shader's
    // compute shader.
    //
    // Parameters:
    // xThreads - number of threads in X
    // yThreads - number of threads in Y
    // zThreads - number of threads in Z
    //**********************************************************************
    void CShader::SetThreadCounts(int xThreads, int yThreads, int zThreads)
    {
        m_xThreads = xThreads;
        m_yThreads = yThreads;
        m_zThreads = zThreads;
    }
    
    //**********************************************************************
    // Method: BeginRender
    // BeginRender is called before rendering using this shader occurs.
    // This call is responsible for setting up the pRenderer device to use the shader.
    //
    // Parameters:
    // pRenderer - D3D11 device/context to use
    //**********************************************************************
    void CShader::BeginRender(IRenderer* pRenderer, IRenderMaterial* pFrontMaterial, IRenderMaterial* pBackMaterial, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld)
    {
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
#ifdef _DEBUG
        CComPtr<ID3DUserDefinedAnnotation> spAnnotations;
        CT(spCtx->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void**)&spAnnotations));
        spAnnotations->BeginEvent(L"BeginRender");
#endif
        
        bool hasVS = m_spShaderInfo->HasShader(EShaderType::TypeVertexShader);
        bool hasPS = m_spShaderInfo->HasShader(EShaderType::TypePixelShader);
        bool hasCS = m_spShaderInfo->HasShader(EShaderType::TypeComputeShader);
        if (hasVS)
        {
            spCtx->IASetInputLayout(m_spLayout);
            spCtx->VSSetShader(m_spVertexShader, nullptr, 0);
        }
        if (hasPS)
            spCtx->PSSetShader(m_spPixelShader, nullptr, 0);
        if (hasCS)
            spCtx->CSSetShader(m_spComputeShader, nullptr, 0);
        
        if (hasVS || hasPS)
        {
            if (pFrontMaterial)
                pFrontMaterial->Render(pRenderer, lights, nullptr, this);
            PushLights(lights);
        }

        PushMatrices(pRenderer, pWorld);
        ClearSamplers(pRenderer);
        if (hasVS)
        {
            PushSamplers(pRenderer, m_vsParams, false);
            PushConstants(pRenderer, &m_vertexConstants, m_vsParams);
            spCtx->VSSetConstantBuffers(0, 1, &m_vertexConstants.m_spBuffer.p);
        }
         if (hasPS)
        {
            PushSamplers(pRenderer, m_psParams, true);
            PushConstants(pRenderer, &m_pixelConstants, m_psParams);
            spCtx->PSSetConstantBuffers(0, 1, &m_pixelConstants.m_spBuffer.p);
        }
        if (hasCS)
        {
            PushSamplers(pRenderer, m_csParams, false);
            PushConstants(pRenderer, &m_computeConstants, m_csParams);
            spCtx->CSSetConstantBuffers(0, 1, &m_computeConstants.m_spBuffer.p);
            PushBuffers(pRenderer, m_csParams);
            spCtx->Dispatch(m_xThreads, m_yThreads, m_zThreads);
            PopBuffers(pRenderer);
            spCtx->CSSetShader(nullptr, nullptr, 0);
            ID3D11UnorderedAccessView* uavNull[1] = { nullptr };
            ID3D11ShaderResourceView* srvNull[1] = { nullptr };
            spCtx->CSSetUnorderedAccessViews(0, 1, uavNull, NULL);
            spCtx->CSSetShaderResources(0, 1, srvNull);
        }
#ifdef _DEBUG
        spAnnotations->EndEvent();
#endif
    }

    //**********************************************************************
    // Method: EndRender
    // EndRender is called after rendering using this shader occurs.
    // During this call the shader may clean up any state/memory it needed
    //
    // Parameters:
    // pRenderer - D3D11 device/context to use
    //**********************************************************************
    void CShader::EndRender(IRenderer * /*pRenderer*/)
    {
    }

    //**********************************************************************
    // Method: GetShaderInfo
    // Returns:
    // Returns information about the shader
    //**********************************************************************
    CRefObj<IShaderInfo> CShader::GetShaderInfo()
	{
		return m_spShaderInfo;
	}
	
    //**********************************************************************
    // Method: CreateBuffer
    // Helper function that creates a buffer for the shader (Constant buffer
    // or StructuredBuffer)
    //
    // Parameters:
    // pDevice - rendering device
    // bufSize - size of buffer in bytes
    // bindFlags - bind flags
    // cpuAccessFlags - access flags
    // usage - Usage model for buffer
    // pBuffer - returns the created buffer
    //**********************************************************************
    void CShader::CreateBuffer(ID3D11Device* pDevice, uint32 bufSize,
        uint32 bindFlags, uint32 cpuAccessFlags, D3D11_USAGE usage,
        uint32 miscFlags, uint32 stride, uint32 alignment, SBuffer* pBuffer, ID3D11Buffer **ppBuffer)
    {
        pBuffer->m_heapSize = 0;
        pBuffer->m_bufferSize = 0;
        if (bufSize > 0)
        {
            D3D11_BUFFER_DESC buffDesc;
            buffDesc.BindFlags = bindFlags;
            buffDesc.ByteWidth = ((bufSize + alignment - 1) / alignment) * alignment;
            buffDesc.CPUAccessFlags = cpuAccessFlags;
            buffDesc.MiscFlags = miscFlags;
            buffDesc.StructureByteStride = stride;
            buffDesc.Usage = usage;
            CT(pDevice->CreateBuffer(&buffDesc, nullptr, ppBuffer));
            pBuffer->m_heapSize = (uint32)buffDesc.ByteWidth;
            pBuffer->m_bufferSize = (uint32)bufSize;
        }
    }

	//**********************************************************************
    // Method: CreateConstantBuffer
    // CreateConstantBuffer creates the constant buffer (pixel or vertex shader)
    //
    // Parameters:
    // pDevice - D3D device
    // pDefs - List of parameter definitions (parsed from HLSL)
    // paramsSize - Length of pDefs
    // params - Generated parameter list
    // ppBuffer - Returns the created constant buffer
    //**********************************************************************
    void CShader::CreateConstantBuffer(ID3D11Device *pDevice, ShaderParamDef *pDefs, uint32 paramsSize, std::vector<ShaderParamInstance> &params, SBuffer *pConstantBuffer)
    {
        uint32 s = ComputeParamSize(pDefs, paramsSize, params);
        CreateBuffer(pDevice, s, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC, 0, 0, 16, pConstantBuffer, &pConstantBuffer->m_spBuffer);
    }

    //**********************************************************************
    // Method: CreateConstantBuffer
    // CreateConstantBuffer creates the constant buffer (pixel or vertex shader)
    //
    // Parameters:
    // pDevice - D3D device
    // pDefs - List of parameter definitions (parsed from HLSL)
    // paramsSize - Length of pDefs
    // params - Generated parameter list
    // ppBuffer - Returns the created constant buffer
    //**********************************************************************
    void CShader::CreateBuffers(ID3D11Device* pDevice, ShaderParamDef* pDefs, uint32 numParams, std::vector<ShaderParamInstance>& params)
    {
        if (pDefs == nullptr || numParams == 0)
            return;
        uint32 s = 0;
        params.resize(numParams);
        for (size_t i = 0; i < numParams; i++)
        {
            if (pDefs[i].m_name.length() == 0)
                continue;
            ShaderParamDef& d = params[i];
            d = pDefs[i];
            params[i].m_dirty = true;
            params[i].m_offset = (pDefs[i].m_type == EShaderParamType::ShaderType_Texture) ? pDefs[i].m_offset : s;
            if (pDefs[i].m_type == EShaderParamType::ShaderType_StructuredBuffer)
            {
                ClientBuffer& buffer = std::any_cast<ClientBuffer>(params[i].m_value);
                s += buffer.m_dataSize;
            }
        }
    }

    //**********************************************************************
    // Method: Create
    // Creates a shader
    //
    // Parameters:
    // pRenderer - Graphics device to use
    // pShaderName - Name of shader
    // pShaderInfo - Shader description
    // pPSBlob - Compiled binary for the pixel shader
    // pVSBlob - Compiled binary for the vertex shader
    // pCSBlob - Compiled binary for the compute shader
    //**********************************************************************
	void CShader::Create(IRenderer *pRenderer, const wchar_t *pShaderName, IShaderInfo *pShaderInfo, ID3DBlob *pPSBlob, ID3DBlob* pVSBlob, ID3DBlob* pCSBlob)
    {
        if (pShaderName)
            m_name = std::wstring(pShaderName);

        ID3D11Device* pDevice = pRenderer->GetDevice();
        m_spShaderInfo = pShaderInfo;
        if (pShaderInfo->HasShader(EShaderType::TypeVertexShader))
        {
            const byte* pVSByteCodes = (const byte*)pVSBlob->GetBufferPointer();
            uint32 vsBufferLen = (uint32)pVSBlob->GetBufferSize();
            std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout = pShaderInfo->VertexLayout();
            D3D11_INPUT_ELEMENT_DESC* pVertexLayout = vertexLayout.data();
            CT(pDevice->CreateInputLayout(pShaderInfo->VertexLayout().data(), (UINT)pShaderInfo->VertexLayout().size(),
                pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_spLayout));
            CreateConstantBuffer(pDevice, pShaderInfo->VertexShaderParameterDefs().data(),
                (uint32)pShaderInfo->VertexShaderParameterDefs().size(), m_vsParams, &m_vertexConstants);
            CT(pDevice->CreateVertexShader(pVSByteCodes, vsBufferLen, nullptr, &m_spVertexShader));
        }
        if (pShaderInfo->HasShader(EShaderType::TypePixelShader))
        {
            const byte* pPSByteCodes = (const byte*)pPSBlob->GetBufferPointer();
            uint32 psBufferLen = (uint32)pPSBlob->GetBufferSize();
            CreateConstantBuffer(pDevice, pShaderInfo->PixelShaderParameterDefs().data(),
                (uint32)pShaderInfo->PixelShaderParameterDefs().size(), m_psParams, &m_pixelConstants);
            CT(pDevice->CreatePixelShader(pPSByteCodes, psBufferLen, nullptr, &m_spPixelShader));
        }
        if (pShaderInfo->HasShader(EShaderType::TypeComputeShader))
        {
            const byte* pPSByteCodes = (const byte*)pCSBlob->GetBufferPointer();
            uint32 psBufferLen = (uint32)pCSBlob->GetBufferSize();
            CT(pDevice->CreateComputeShader(pPSByteCodes, psBufferLen, nullptr, &m_spComputeShader));

            CreateConstantBuffer(pDevice, pShaderInfo->ComputeShaderParameterDefs().data(),
                (uint32)pShaderInfo->ComputeShaderParameterDefs().size(), m_csParams, &m_computeConstants);
        }

        //**********************************************************************
        CD3D11_SAMPLER_DESC sdesc(D3D11_DEFAULT);
        CT(pDevice->CreateSamplerState(&sdesc, &m_spSamplerState));
    }
    
    //**********************************************************************
    // Method: CreateShader
    // A helper function to create a shader
    //
    // Parameters:
    // pRenderer - Graphics device to use
    // pShaderName - Name of shader
    // pShaderInfo - Shader description
    // pPSBlob - Compiled binary for the pixel shader
    // pVSBlob - Compiled binary for the vertexshader
    //
    // Returns:
    // Returns the created shader
    //**********************************************************************
    CAUSTICAPI CRefObj<IShader> CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName, IShaderInfo *pShaderInfo,
        ID3DBlob *pPSBlob, ID3DBlob *pVSBlob, ID3DBlob *pCSBlob)
    {
        std::unique_ptr<CShader> spShader(new CShader());
        spShader->Create(pRenderer, pShaderName, pShaderInfo, pPSBlob, pVSBlob, pCSBlob);
        return CRefObj<IShader>(spShader.release());
    }
    
    //**********************************************************************
    // Method: FindShader
    // Returns the requested shader
    //
    // Parameters:
    // pShaderName - Name of shader to locate
    //
    // Returns:
    // Returns the requested shader
    //**********************************************************************
    CRefObj<IShader> CShaderMgr::FindShader(const wchar_t *pShaderName)
    {
        std::map<std::wstring,CRefObj<IShader>>::iterator it = m_shaders.find(pShaderName);
        if (it == m_shaders.end())
            return CRefObj<IShader>(nullptr);
        return it->second;
    }

    //**********************************************************************
    // Method: RegisterShader
    // Registers the specified shader with the shader manager
    //
    // Parameters:
    // pShaderName - Name of shader
    // pShader - Shader to register
    //**********************************************************************
    void CShaderMgr::RegisterShader(const wchar_t *pShaderName, IShader *pShader)
    {
        std::map<std::wstring,CRefObj<IShader>>::iterator it = m_shaders.find(pShaderName);
        if (it == m_shaders.end())
        {
            m_shaders[std::wstring(pShaderName)] = CRefObj<IShader>(pShader);
        }
    }
};
