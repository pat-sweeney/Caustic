//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <DirectXMath.h>
#include <memory>
#include <d3d11_4.h>
#include <atlbase.h>
#include <any>
module Rendering.Caustic.Shader;
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.Sampler;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.ILight;
import Rendering.Caustic.IPointLight;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.IShaderInfo;
import Rendering.Caustic.IShader;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IRenderer;

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
    CRefObj<IShader> CShader::Clone(ID3D11Device* pDevice)
    {
        std::unique_ptr<CShader> spShader(new CShader());
        spShader->m_csParams = m_csParams;
        spShader->m_psParams = m_psParams;
        spShader->m_vsParams = m_vsParams;
        spShader->m_layout = m_layout;
        spShader->m_matricesAvail = m_matricesAvail;
        spShader->m_maxTextureSlot = m_maxTextureSlot;
        spShader->m_name = m_name;
        spShader->m_spLayout = m_spLayout;
        spShader->m_spPixelShader = m_spPixelShader;
        spShader->m_spVertexShader = m_spVertexShader;
        spShader->m_spComputeShader = m_spComputeShader;
        spShader->m_spSamplerState = m_spSamplerState;
        spShader->m_spShaderInfo = m_spShaderInfo;
        spShader->m_xThreads = m_xThreads;
        spShader->m_yThreads = m_yThreads;
        spShader->m_zThreads = m_zThreads;
        if (m_spShaderInfo->VertexShaderParameterDefs().size() > 0)
            CreateConstantBuffer(pDevice, m_spShaderInfo->VertexShaderParameterDefs().data(),
                (uint32)m_spShaderInfo->VertexShaderParameterDefs().size(), spShader->m_vsParams, &spShader->m_vertexConstants);
        if (m_spShaderInfo->PixelShaderParameterDefs().size() > 0)
            CreateConstantBuffer(pDevice, m_spShaderInfo->PixelShaderParameterDefs().data(),
                (uint32)m_spShaderInfo->PixelShaderParameterDefs().size(), spShader->m_psParams, &spShader->m_pixelConstants);
        if (m_spShaderInfo->ComputeShaderParameterDefs().size() > 0)
            CreateConstantBuffer(pDevice, m_spShaderInfo->ComputeShaderParameterDefs().data(),
                (uint32)m_spShaderInfo->ComputeShaderParameterDefs().size(), spShader->m_csParams, &spShader->m_computeConstants);
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
    uint32 CShader::ComputeParamSize(ShaderParamDef* pDefs, uint32 numParams, std::map<std::wstring, ShaderParamInstance>& params)
    {
        if (pDefs == nullptr || numParams == 0)
            return 0;
        uint32 s = 0;
        for (size_t i = 0; i < numParams; i++)
        {
            if (pDefs[i].m_name.length() == 0)
                continue;
            std::pair<std::map<std::wstring, ShaderParamInstance>::iterator, bool> result = params.insert(std::make_pair(pDefs[i].m_name, ShaderParamInstance()));
            std::map<std::wstring, ShaderParamInstance>::iterator it = result.first;
            bool success = result.second;
            ShaderParamDef& d = it->second;
            d = pDefs[i];
            it->second.m_cbOffset = s;
            s += ShaderTypeSize(d);
            it->second.m_dirty = true;
            it->second.m_offset = (pDefs[i].m_type == EShaderParamType::ShaderType_Texture ||
                pDefs[i].m_type == EShaderParamType::ShaderType_AppendStructuredBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_RWStructuredBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_RWByteAddressBuffer ||
                pDefs[i].m_type == EShaderParamType::ShaderType_StructuredBuffer) ? pDefs[i].m_offset : 0;
            if (pDefs[i].m_type == EShaderParamType::ShaderType_Float)
                it->second.m_value = std::any(0.0f);
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float_Array)
            {
                for (size_t j = 0; j < it->second.m_members; j++)
                    it->second.m_values.push_back(Float(0.0f));
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float2)
                it->second.m_value = std::any(Float2(0.0f, 0.0f));
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float2_Array)
            {
                for (size_t j = 0; j < it->second.m_members; j++)
                    it->second.m_values.push_back(Float2(0.0f, 0.0f));
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float3)
                it->second.m_value = std::any(Float3(0.0f, 0.0f, 0.0f));
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float3_Array)
            {
                for (size_t j = 0; j < it->second.m_members; j++)
                    it->second.m_values.push_back(Float3(0.0f, 0.0f, 0.0f));
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float4)
                it->second.m_value = std::any(Float4(0.0f, 0.0f, 0.0f, 0.0f));
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float4_Array)
            {
                for (size_t j = 0; j < it->second.m_members; j++)
                    it->second.m_values.push_back(Float4(0.0f, 0.0f, 0.0f, 0.0f));
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Int)
                it->second.m_value = std::any(Int(0));
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Int_Array)
            {
                for (size_t j = 0; j < it->second.m_members; j++)
                    it->second.m_values.push_back(Int(0));
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
                it->second.m_value = std::any(m);
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
                for (size_t j = 0; j < it->second.m_members; j++)
                    it->second.m_values.push_back(m);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Matrix3x3)
            {
                float v[9] = {
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f
                };
                Matrix_3x3 m(v);
                it->second.m_value = std::any(m);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Matrix3x3_Array)
            {
                float v[9] = {
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f
                };
                Matrix_3x3 m(v);
                for (size_t j = 0; j < it->second.m_members; j++)
                    it->second.m_values.push_back(m);
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

    void CShader::PushSamplers(IRenderer* pRenderer, std::map<std::wstring, ShaderParamInstance>& params, bool isPixelShader)
    {
        // First push samplers
        for (auto &it : params)
        {
            switch (it.second.m_type)
            {
            case EShaderParamType::ShaderType_Texture:
            {
                if (it.second.m_value.has_value())
                {
                    CRefObj<ITexture> v = std::any_cast<CRefObj<ITexture>>(it.second.m_value);
                    v->Render(pRenderer, it.second.m_offset, isPixelShader);
                    if (it.second.m_offset > (uint32)m_maxTextureSlot)
                        m_maxTextureSlot = it.second.m_offset;
                }
            }
            break;
            case EShaderParamType::ShaderType_Sampler:
            {
                if (it.second.m_value.has_value())
                {
                    Caustic::CSamplerRef v = std::any_cast<CSamplerRef>(it.second.m_value);
                    v.m_spSampler->Render(pRenderer, it.second.m_offset, isPixelShader);
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
    void CShader::PushConstants(IRenderer* pRenderer, SBuffer* pBuffer, std::map<std::wstring, ShaderParamInstance>& params)
    {
        if (pBuffer == nullptr || pBuffer->m_bufferSize == 0)
            return;

        D3D11_MAPPED_SUBRESOURCE ms;
        CT(pRenderer->GetContext()->Map(pBuffer->m_spBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        // Since we can't update only part of the constant buffer (since our mapping prevents that)
        // we will update all the properties regardless of the dirty state.
        for (auto &it : params)
        {
            BYTE* pb = reinterpret_cast<BYTE*>(ms.pData) + it.second.m_cbOffset;
            switch (it.second.m_type)
            {
            case EShaderParamType::ShaderType_Sampler:
                break;
            case EShaderParamType::ShaderType_Texture:
                break;
            case EShaderParamType::ShaderType_Float:
                {
                    float f = (it.second.m_value.has_value()) ? std::any_cast<float>(it.second.m_value) : 0.0f;
                    memcpy(pb, &f, sizeof(float));
                }
                break;
            case EShaderParamType::ShaderType_Float_Array:
            {
                for (size_t j = 0; j < it.second.m_members; j++)
                {
                    Float v = (it.second.m_values[j].has_value()) ? std::any_cast<Float>(it.second.m_values[j]) : 0.0f;
                    memcpy(pb, &v, sizeof(Float));
                    pb += sizeof(Float);
                }
            }
            break;
            case EShaderParamType::ShaderType_Float2:
                {
                    Float2 f = (it.second.m_value.has_value()) ? std::any_cast<Float2>(it.second.m_value) : Float2(0.0f, 0.0f);
                    memcpy(pb, &f, sizeof(Float2));
                }
                break;
            case EShaderParamType::ShaderType_Float2_Array:
                {
                    for (size_t j = 0; j < it.second.m_members; j++)
                    {
                        Float2 v = (it.second.m_values[j].has_value()) ? std::any_cast<Float2>(it.second.m_values[j]) : Float2(0.0f, 0.0f);
                        memcpy(pb, &v, sizeof(Float2));
                        pb += sizeof(Float2);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Float3:
                {
                    Float3 f = (it.second.m_value.has_value()) ? std::any_cast<Float3>(it.second.m_value) : Float3(0.0f, 0.0f, 0.0f);
                    memcpy(pb, &f, sizeof(Float3));
                }
                break;
            case EShaderParamType::ShaderType_Float3_Array:
                {
                    for (size_t j = 0; j < it.second.m_members; j++)
                    {
                        Float3 v = (it.second.m_values[j].has_value()) ? std::any_cast<Float3>(it.second.m_values[j]) : Float3(0.0f, 0.0f, 0.0f);
                        memcpy(pb, &v, sizeof(Float3));
                        pb += sizeof(Float3);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Float4:
                {
                    Float4 f = (it.second.m_value.has_value()) ? std::any_cast<Float4>(it.second.m_value) : Float4(0.0f, 0.0f, 0.0f, 0.0f);
                    memcpy(pb, &f, sizeof(Float4));
                }
                break;
            case EShaderParamType::ShaderType_Float4_Array:
                {
                    for (size_t j = 0; j < it.second.m_members; j++)
                    {
                        Float4 v = (it.second.m_values[j].has_value()) ? std::any_cast<Float4>(it.second.m_values[j]) : Float4(0.0f, 0.0f, 0.0f, 0.0f);
                        memcpy(pb, &v, sizeof(Float4));
                        pb += sizeof(Float4);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Int:
                {
                    Int v = (it.second.m_value.has_value()) ? std::any_cast<Int>(it.second.m_value) : 0;
                    memcpy(pb, &v, sizeof(int));
                }
                break;
            case EShaderParamType::ShaderType_Int_Array:
                {
                    for (size_t j = 0; j < it.second.m_members; j++)
                    {
                        Int v = (it.second.m_values[j].has_value()) ? std::any_cast<Int>(it.second.m_values[j]) : 0;
                        memcpy(pb, &v.x, sizeof(int));
                        pb += sizeof(int);
                    }
                }
                break;
            case EShaderParamType::ShaderType_Matrix:
                {
                    Matrix m = (it.second.m_value.has_value()) ? std::any_cast<Matrix>(it.second.m_value) : Matrix();
                    memcpy(pb, m.x, 16 * sizeof(float));
                }
                break;
            case EShaderParamType::ShaderType_Matrix_Array:
                {
                    for (size_t j = 0; j < it.second.m_members; j++)
                    {
                        Matrix m = (it.second.m_values[j].has_value()) ? std::any_cast<Matrix>(it.second.m_values[j]) : Matrix();
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
                    Matrix_3x3 m = (it.second.m_value.has_value()) ? std::any_cast<Matrix_3x3>(it.second.m_value) : Matrix_3x3();
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
                    for (size_t j = 0; j < it.second.m_members; j++)
                    {
                        Matrix_3x3 m = (it.second.m_values[j].has_value()) ? std::any_cast<Matrix_3x3>(it.second.m_values[j]) : Matrix_3x3();
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
    // params - list of shader parameters
    //**********************************************************************
    void CShader::PopBuffers(IRenderer* pRenderer, std::map<std::wstring, ShaderParamInstance>& params)
    {
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
        for (auto& it : params)
        {
            if (!it.second.m_value.has_value())
                continue;
            if (it.second.m_type != EShaderParamType::ShaderType_AppendStructuredBuffer &&
                it.second.m_type != EShaderParamType::ShaderType_RWStructuredBuffer &&
                it.second.m_type != EShaderParamType::ShaderType_StructuredBuffer &&
                it.second.m_type != EShaderParamType::ShaderType_RWByteAddressBuffer)
                continue;

            CRefObj<IGPUBuffer> spBuffer = std::any_cast<CRefObj<IGPUBuffer>>(it.second.m_value);
            if (spBuffer->GetBufferType() == EBufferType::AppendStructuredBuffer ||
                spBuffer->GetBufferType() == EBufferType::RWStructuredBuffer ||
                spBuffer->GetBufferType() == EBufferType::RWByteAddressBuffer)
            {
                spCtx->CopyResource(spBuffer->GetStagingBuffer(), spBuffer->GetBuffer());
            }

            if (it.second.m_type == EShaderParamType::ShaderType_StructuredBuffer)
            {
                ID3D11ShaderResourceView* nullar[1] = { nullptr };
                spCtx->CSSetShaderResources(it.second.m_offset, 1, &nullar[0]);
            }
            else
            {
                ID3D11UnorderedAccessView* nullar[1] = { nullptr };
                spCtx->CSSetUnorderedAccessViews(it.second.m_offset, 1, &nullar[0], nullptr);
            }
        }
    }

    //**********************************************************************
    // Method: PushBuffers
    // Pushes buffers from the CPU to the GPU (for instance,
    // from a CPU buffer to a GPU structured buffer).
    //
    // Parameters:
    // pRenderer - D3D11 device/context to use
    // params - List of parameters to push
    //**********************************************************************
    void CShader::PushBuffers(IRenderer* pRenderer,
        std::map<std::wstring, ShaderParamInstance>& params)
    {
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
        for (auto &it : params)
        {
            if (!it.second.m_value.has_value())
                continue;
            if (it.second.m_type != EShaderParamType::ShaderType_AppendStructuredBuffer &&
                it.second.m_type != EShaderParamType::ShaderType_RWStructuredBuffer &&
                it.second.m_type != EShaderParamType::ShaderType_StructuredBuffer &&
                it.second.m_type != EShaderParamType::ShaderType_RWByteAddressBuffer)
                continue;

            CRefObj<IGPUBuffer> spBuffer = std::any_cast<CRefObj<IGPUBuffer>>(it.second.m_value);
            if (it.second.m_type == EShaderParamType::ShaderType_StructuredBuffer)
            {
                auto spSRView = spBuffer->GetSRView();
                spCtx->CSSetShaderResources(it.second.m_offset, 1, &spSRView.p);
            }
            else
            {
                auto spUAView = spBuffer->GetUAView();
                spCtx->CSSetUnorderedAccessViews(it.second.m_offset, 1, &spUAView.p, nullptr);
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
    void CShader::SetParam(const std::wstring& paramName, const std::any& value, std::map<std::wstring, ShaderParamInstance>& params)
    {
        auto it = params.find(paramName);
        if (it != params.end())
        {
            it->second.m_value = value;
            it->second.m_dirty = true;
        }
    }

    //**********************************************************************
    void CShader::SetParam(const wchar_t* paramName, const std::any& value, std::map<std::wstring, ShaderParamInstance>& params)
    {
        std::wstring wstr(paramName);
        SetParam(wstr, value, params);
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
    void CShader::SetParam(const std::wstring& paramName, int index, const std::any& value, std::map<std::wstring, ShaderParamInstance>& params)
    {
        auto it = params.find(paramName);
        if (it != params.end())
        {
            it->second.m_values[index] = value;
            it->second.m_dirty = true;
        }
    }

    //**********************************************************************
    void CShader::SetParam(const wchar_t* paramName, int index, const std::any& value, std::map<std::wstring, ShaderParamInstance>& params)
    {
        std::wstring wstr(paramName);
        SetParam(wstr, index, value, params);
    }

    //**********************************************************************
    // Method: SetPSParam
    // Sets a pixel shader parameter
    //
    // Parameters:
    // paramName - Name of the parameter
    // value - Value of parameter
    //**********************************************************************
    void CShader::SetPSParam(const std::wstring& paramName, const std::any& value)
    {
        SetParam(paramName, value, m_psParams);
    }

    //**********************************************************************
    void CShader::SetPSParam(const wchar_t* paramName, const std::any& value)
    {
        SetParam(paramName, value, m_psParams);
    }

    //**********************************************************************
    // Method: SetPSParamFloat
    // See <IShader::SetPSParamFloat>
    //**********************************************************************
    void CShader::SetPSParamFloat(const std::wstring& paramName, float value)
    {
        std::any v(value);
        SetPSParam(paramName, v);
    }

    //**********************************************************************
    void CShader::SetPSParamFloat(const wchar_t* paramName, float value)
    {
        std::any v(value);
        SetPSParam(paramName, v);
    }

    //**********************************************************************
    // Method: SetPSParamInt
    // See <IShader::SetPSParamInt>
    //**********************************************************************
    void CShader::SetPSParamInt(const std::wstring& paramName, int value)
    {
        std::any v(Int((int)value));
        SetPSParam(paramName, v);
    }

    //**********************************************************************
    void CShader::SetPSParamInt(const wchar_t* paramName, int value)
    {
        std::any v(Int((int)value));
        SetPSParam(paramName, v);
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
    void CShader::SetPSParam(const std::wstring& paramName, int index, const std::any& value)
    {
        SetParam(paramName, index, value, m_psParams);
    }

    //**********************************************************************
    void CShader::SetPSParam(const wchar_t* paramName, int index, const std::any& value)
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
    void CShader::SetVSParam(const std::wstring& paramName, const std::any& value)
    {
        SetParam(paramName, value, m_vsParams);
    }

    //**********************************************************************
    void CShader::SetVSParam(const wchar_t* paramName, const std::any& value)
    {
        SetParam(paramName, value, m_vsParams);
    }

    //**********************************************************************
    // Method: SetVSParamFloat
    // See <IShader::SetVSParamFloat>
    //**********************************************************************
    void CShader::SetVSParamFloat(const std::wstring& paramName, float value)
    {
        std::any v(value);
        SetVSParam(paramName, v);
    }

    //**********************************************************************
    void CShader::SetVSParamFloat(const wchar_t* paramName, float value)
    {
        std::any v(value);
        SetVSParam(paramName, v);
    }

    //**********************************************************************
    // Method: SetVSParamInt
    // See <IShader::SetVSParamInt>
    //**********************************************************************
    void CShader::SetVSParamInt(const std::wstring& paramName, int value)
    {
        std::any v(Int((int)value));
        SetVSParam(paramName, v);
    }

    //**********************************************************************
    void CShader::SetVSParamInt(const wchar_t* paramName, int value)
    {
        std::any v(Int((int)value));
        SetVSParam(paramName, v);
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
    void CShader::SetVSParam(const std::wstring& paramName, int index, const std::any& value)
    {
        SetParam(paramName, index, value, m_vsParams);
    }

    //**********************************************************************
    void CShader::SetVSParam(const wchar_t* paramName, int index, const std::any& value)
    {
        SetParam(paramName, index, value, m_vsParams);
    }

    //**********************************************************************
    // Method: SetCSParam
    // Sets a compute shader parameter
    //
    // Parameters:
    // paramName - Name of parameter
    // value - Value of parameter
    //**********************************************************************
    void CShader::SetCSParam(const std::wstring& paramName, const std::any& value)
    {
        SetParam(paramName, value, m_csParams);
    }

    //**********************************************************************
    void CShader::SetCSParam(const wchar_t* paramName, const std::any& value)
    {
        SetParam(paramName, value, m_csParams);
    }

    //**********************************************************************
    // Method: SetCSParamFloat
    // See <IShader::SetCSParamFloat>
    //**********************************************************************
    void CShader::SetCSParamFloat(const std::wstring& paramName, float value)
    {
        std::any v(value);
        SetCSParam(paramName, v);
    }

    //**********************************************************************
    void CShader::SetCSParamFloat(const wchar_t* paramName, float value)
    {
        std::any v(value);
        SetCSParam(paramName, v);
    }

    //**********************************************************************
    // Method: SetCSParamInt
    // See <IShader::SetCSParamInt>
    //**********************************************************************
    void CShader::SetCSParamInt(const std::wstring& paramName, int value)
    {
        std::any v(Int((int)value));
        SetCSParam(paramName, v);
    }
    
    //**********************************************************************
    void CShader::SetCSParamInt(const wchar_t* paramName, int value)
    {
        std::any v(Int((int)value));
        SetCSParam(paramName, v);
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
    void CShader::SetCSParam(const std::wstring& paramName, int index, const std::any& value)
    {
        SetParam(paramName, index, value, m_csParams);
    }

    //**********************************************************************
    void CShader::SetCSParam(const wchar_t* paramName, int index, const std::any& value)
    {
        SetParam(paramName, index, value, m_csParams);
    }

    static Matrix D3DMatrixToMatrix(const DirectX::XMMATRIX &mat)
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

    void CShader::PushMatrix(const wchar_t* pParamName, std::any mat, uint32 psmask, uint32 vsmask)
    {
        if ((m_matricesAvail & vsmask) == vsmask)
            SetVSParam(pParamName, mat);
        if ((m_matricesAvail & psmask) == psmask)
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

    void CShader::DetermineMatricesUsed()
    {
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"world") ? PSMatrixAvail_world : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"worldInv") ? PSMatrixAvail_worldInv : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"worldInvTranspose") ? PSMatrixAvail_worldInvTranspose : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"view") ? PSMatrixAvail_view : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"viewInv") ? PSMatrixAvail_viewInv : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"proj") ? PSMatrixAvail_proj : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"projInv") ? PSMatrixAvail_projInv : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"worldView") ? PSMatrixAvail_worldView : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"worldViewInv") ? PSMatrixAvail_worldViewInv : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"worldViewProj") ? PSMatrixAvail_worldViewProj : 0;
        m_matricesAvail |= m_spShaderInfo->PSUsesVariable(L"worldViewProjInv") ? PSMatrixAvail_worldViewProjInv : 0;

        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"world") ? VSMatrixAvail_world : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"worldInv") ? VSMatrixAvail_worldInv : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"worldInvTranspose") ? VSMatrixAvail_worldInvTranspose : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"view") ? VSMatrixAvail_view : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"viewInv") ? VSMatrixAvail_viewInv : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"proj") ? VSMatrixAvail_proj : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"projInv") ? VSMatrixAvail_projInv : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"worldView") ? VSMatrixAvail_worldView : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"worldViewInv") ? VSMatrixAvail_worldViewInv : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"worldViewProj") ? VSMatrixAvail_worldViewProj : 0;
        m_matricesAvail |= m_spShaderInfo->VSUsesVariable(L"worldViewProjInv") ? VSMatrixAvail_worldViewProjInv : 0;
    }

    void CShader::PushMatrices(IRenderer *pRenderer, DirectX::XMMATRIX *pWorld)
    {
        DirectX::XMMATRIX view = pRenderer->GetCamera()->GetView();
        DirectX::XMMATRIX proj = pRenderer->GetCamera()->GetProjection();

        DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
        if (pWorld == nullptr)
            pWorld = &identity;
        if ((m_matricesAvail & PSMatrixAvail_world) == PSMatrixAvail_world || (m_matricesAvail & VSMatrixAvail_world) == VSMatrixAvail_world)
            PushMatrix(L"world", std::any(D3DMatrixToMatrix(*pWorld)), PSMatrixAvail_world, VSMatrixAvail_world);
        if ((m_matricesAvail & PSMatrixAvail_worldInv) == PSMatrixAvail_worldInv || (m_matricesAvail & VSMatrixAvail_worldInv) == VSMatrixAvail_worldInv)
            PushMatrix(L"worldInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, *pWorld))), PSMatrixAvail_worldInv, VSMatrixAvail_worldInv);
        if ((m_matricesAvail & PSMatrixAvail_worldInvTranspose) == PSMatrixAvail_worldInvTranspose || (m_matricesAvail & VSMatrixAvail_worldInvTranspose) == VSMatrixAvail_worldInvTranspose)
            PushMatrix(L"worldInvTranspose", std::any(D3DMatrixToMatrix(DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, *pWorld)))), PSMatrixAvail_worldInvTranspose, PSMatrixAvail_worldInvTranspose);
        if ((m_matricesAvail & PSMatrixAvail_view) == PSMatrixAvail_view || (m_matricesAvail & VSMatrixAvail_view) == VSMatrixAvail_view)
            PushMatrix(L"view", std::any(D3DMatrixToMatrix(view)), PSMatrixAvail_view, VSMatrixAvail_view);
        if ((m_matricesAvail & PSMatrixAvail_viewInv) == PSMatrixAvail_viewInv || (m_matricesAvail & VSMatrixAvail_viewInv) == VSMatrixAvail_viewInv)
            PushMatrix(L"viewInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, view))), PSMatrixAvail_viewInv, VSMatrixAvail_viewInv);
        if ((m_matricesAvail & PSMatrixAvail_proj) == PSMatrixAvail_proj || (m_matricesAvail & VSMatrixAvail_proj) == VSMatrixAvail_proj)
            PushMatrix(L"proj", std::any(D3DMatrixToMatrix(proj)), PSMatrixAvail_proj, VSMatrixAvail_proj);
        if ((m_matricesAvail & PSMatrixAvail_projInv) == PSMatrixAvail_projInv || (m_matricesAvail & VSMatrixAvail_projInv) == VSMatrixAvail_projInv)
            PushMatrix(L"projInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, proj))), PSMatrixAvail_projInv, VSMatrixAvail_projInv);
        DirectX::XMMATRIX wv = DirectX::XMMatrixMultiply(*pWorld, view);
        if ((m_matricesAvail & PSMatrixAvail_worldView) == PSMatrixAvail_worldView || (m_matricesAvail & VSMatrixAvail_worldView) == VSMatrixAvail_worldView)
            PushMatrix(L"worldView", std::any(D3DMatrixToMatrix(wv)), PSMatrixAvail_worldView, VSMatrixAvail_worldView);
        if ((m_matricesAvail & PSMatrixAvail_worldViewInv) == PSMatrixAvail_worldViewInv || (m_matricesAvail & VSMatrixAvail_worldViewInv) == VSMatrixAvail_worldViewInv)
            PushMatrix(L"worldViewInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, wv))), PSMatrixAvail_worldViewInv, VSMatrixAvail_worldViewInv);
        DirectX::XMMATRIX wvp = DirectX::XMMatrixMultiply(wv, proj);
        if ((m_matricesAvail & PSMatrixAvail_worldViewProj) == PSMatrixAvail_worldViewProj || (m_matricesAvail & VSMatrixAvail_worldViewProj) == VSMatrixAvail_worldViewProj)
            PushMatrix(L"worldViewProj", std::any(D3DMatrixToMatrix(wvp)), PSMatrixAvail_worldViewProj, VSMatrixAvail_worldViewProj);
        if ((m_matricesAvail & PSMatrixAvail_worldViewProjInv) == PSMatrixAvail_worldViewProjInv || (m_matricesAvail & VSMatrixAvail_worldViewProjInv) == VSMatrixAvail_worldViewProjInv)
            PushMatrix(L"worldViewProjInv", std::any(D3DMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, wvp))), PSMatrixAvail_worldViewProjInv, VSMatrixAvail_worldViewProjInv);
    }

    //**********************************************************************
    // Method: Dispatch
    // Runs the compute shader associated with a Caustic shader.
    //
    // Parameters:
    // xThreads - number of threads in X
    // yThreads - number of threads in Y
    // zThreads - number of threads in Z
    //**********************************************************************
    void CShader::Dispatch(IRenderer* pRenderer, int xThreads, int yThreads, int zThreads)
    {
        m_xThreads = xThreads;
        m_yThreads = yThreads;
        m_zThreads = zThreads;
        std::vector<CRefObj<ILight>> lights;
        BeginRender(pRenderer, nullptr, lights, nullptr);
        EndRender(pRenderer);
    }
    
    //**********************************************************************
    // Method: BeginRender
    // BeginRender is called before rendering using this shader occurs.
    // This call is responsible for setting up the pRenderer device to use the shader.
    //
    // Parameters:
    // pRenderer - D3D11 device/context to use
    //**********************************************************************
    void CShader::BeginRender(IRenderer* pRenderer, IRenderMaterial* pMaterial, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld)
    {
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(spCtx->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
        spCtx2->BeginEventInt(L"BeginRender", 0);
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
            if (pMaterial)
                pMaterial->Render(pRenderer, lights, nullptr, this);
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
            PopBuffers(pRenderer, m_csParams);
            spCtx->CSSetShader(nullptr, nullptr, 0);
            ///ID3D11UnorderedAccessView* uavNull[1] = { nullptr };
            ///ID3D11ShaderResourceView* srvNull[1] = { nullptr };
            ///spCtx->CSSetUnorderedAccessViews(0, 1, uavNull, NULL);
            ///spCtx->CSSetShaderResources(0, 1, srvNull);
        }
#ifdef _DEBUG
        spCtx2->EndEvent();
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
    void CShader::CreateConstantBuffer(ID3D11Device *pDevice, ShaderParamDef *pDefs, uint32 paramsSize, std::map<std::wstring, ShaderParamInstance> &params, SBuffer *pConstantBuffer)
    {
        uint32 s = ComputeParamSize(pDefs, paramsSize, params);
        CreateBuffer(pDevice, s, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC, 0, 0, 16, pConstantBuffer, &pConstantBuffer->m_spBuffer);
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
        DetermineMatricesUsed();
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
    void CGPUBuffer::CreateBuffer(ID3D11Device* pDevice, uint32 bufSize,
        uint32 bindFlags, uint32 cpuAccessFlags, D3D11_USAGE usage,
        uint32 miscFlags, uint32 stride, uint32 alignment, ID3D11Buffer** ppBuffer)
    {
        m_heapSize = 0;
        m_bufferSize = 0;
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
            m_heapSize = (uint32)buffDesc.ByteWidth;
            m_bufferSize = (uint32)bufSize;
        }
    }

    void CGPUBuffer::Create(IRenderer *pRenderer, EBufferType bufferType, uint32 numElems, uint32 elemSize, uint32 bindFlags)
    {
        m_numElems = numElems;
        m_elemSize = elemSize;
        m_bufferType = bufferType;
        uint32 miscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        uint32 bind = 0;
        uint32 access = 0;
        D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        uint32 stride, alignment = 1;
        switch (bufferType)
        {
        case AppendStructuredBuffer:
        case RWStructuredBuffer:
        case StructuredBuffer:
            miscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            bind = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
            stride = ((m_elemSize + 3) / 4) * 4;
            break;
        case RWByteAddressBuffer:
            miscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
            bind = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
        default:
            stride = elemSize;
            break;
        }
        bind |= bindFlags;
        uint32 bufSize = numElems * stride;
        if (bufferType == RWByteAddressBuffer)
            stride = 0;

        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        CreateBuffer(spDevice, bufSize, bind | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, access, usage, miscFlags, stride, alignment, &m_spBuffer.p);
        usage = D3D11_USAGE::D3D11_USAGE_STAGING;
        access = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
        bind = 0;
        miscFlags = 0;
        CreateBuffer(spDevice, bufSize, bind, access, usage, miscFlags, stride, alignment, &m_spStagingBuffer.p);

        if (bufferType == StructuredBuffer)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
            srvDesc.Buffer.NumElements = numElems;
            srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
            CT(spDevice->CreateShaderResourceView(m_spBuffer, &srvDesc, &m_spSRView.p));
        }
        else
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
            ZeroMemory(&uavDesc, sizeof(uavDesc));
            uavDesc.Buffer.FirstElement = 0;
            if (bufferType == RWStructuredBuffer)
            {
                uavDesc.Format = DXGI_FORMAT_UNKNOWN;
                uavDesc.Buffer.Flags = 0;
                uavDesc.Buffer.NumElements = numElems;
            }
            else if (bufferType == AppendStructuredBuffer)
            {
                uavDesc.Format = DXGI_FORMAT_UNKNOWN;
                uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
                uavDesc.Buffer.NumElements = numElems;
            }
            else // it.second.m_type == EShaderParamType::ShaderType_RWByteAddressBuffer
            {
                uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
                uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
                uavDesc.Buffer.NumElements = bufSize / sizeof(float); // NumElements is in sizeof DXGI_FORMAT_R32_TYPELESS (hence bufSize/sizeof(float))
            }
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
            CT(spDevice->CreateUnorderedAccessView(m_spBuffer, &uavDesc, &m_spUAView.p));
        }
    }

    void CGPUBuffer::CopyFromCPU(IRenderer* pRenderer, uint8* pData)
    {
        // Copy the data from the CPU memory to the buffer
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
        D3D11_MAPPED_SUBRESOURCE ms;
        CT(spCtx->Map(m_spStagingBuffer, 0, D3D11_MAP_WRITE, 0, &ms));
        BYTE* pb = reinterpret_cast<BYTE*>(ms.pData);
        uint32 alignedElemSize = ((m_elemSize + 3) / 4) * 4;
        if (alignedElemSize == m_elemSize)
            memcpy(pb, pData, m_numElems * m_elemSize);
        else
        {
            for (uint32 i = 0; i < m_numElems; i++)
            {
                memcpy(pb, pData, m_elemSize);
                pb += alignedElemSize;
                pData += m_elemSize;
            }
        }
        spCtx->Unmap(m_spStagingBuffer, 0);
        spCtx->CopyResource(m_spBuffer, m_spStagingBuffer);
    }

    void CGPUBuffer::CopyToCPU(IRenderer* pRenderer, uint8* pData)
    {
        if (pData)
        {
            CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
            spCtx->CopyResource(m_spStagingBuffer, m_spBuffer);
            D3D11_MAPPED_SUBRESOURCE ms;
            CT(spCtx->Map(m_spStagingBuffer, 0, D3D11_MAP_READ, 0, &ms));
            BYTE* pb = reinterpret_cast<BYTE*>(ms.pData);
            uint32 alignedElemSize = ((m_elemSize + 3) / 4) * 4;
            if (alignedElemSize == m_elemSize)
                memcpy(pData, pb, m_numElems * m_elemSize);
            else
            {
                for (uint32 i = 0; i < m_numElems; i++)
                {
                    memcpy(pData, pb, m_elemSize);
                    pb += alignedElemSize;
                    pData += m_elemSize;
                }
            }
            spCtx->Unmap(m_spStagingBuffer, 0);
        }
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
    CRefObj<IShader> CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName, IShaderInfo *pShaderInfo,
        ID3DBlob *pPSBlob, ID3DBlob *pVSBlob, ID3DBlob *pCSBlob)
    {
        std::unique_ptr<CShader> spShader(new CShader());
        spShader->Create(pRenderer, pShaderName, pShaderInfo, pPSBlob, pVSBlob, pCSBlob);
        return CRefObj<IShader>(spShader.release());
    }
};
