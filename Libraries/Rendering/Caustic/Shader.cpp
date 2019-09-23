//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
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

namespace Caustic
{
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
    uint32 CShader::ComputeParamSize(ShaderParamDef *pDefs, uint32 numParams, std::vector<ShaderParamInstance> &params)
    {
        if (pDefs == nullptr || numParams == 0)
            return 0;
        uint32 s = 0;
        params.resize(numParams);
        for (size_t i = 0; i < numParams; i++)
        {
            if (pDefs[i].m_name.length() == 0)
                continue;
            ShaderParamDef &d = params[i];
            d = pDefs[i];
            params[i].m_dirty = true;
            params[i].m_offset = s;
            if (pDefs[i].m_type == EShaderParamType::ShaderType_Float)
            {
                params[i].m_value = std::any(0.0f);
                s += sizeof(float);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Float(0.0f));
                s += params[i].m_members * sizeof(float);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float2)
            {
                params[i].m_value = std::any(Float2(0.0f, 0.0f));
                s += sizeof(float) * 2;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float2_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Float2(0.0f, 0.0f));
                s += params[i].m_members * sizeof(float) * 2;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float3)
            {
                params[i].m_value = std::any(Float3(0.0f, 0.0f, 0.0f));
                s += sizeof(float) * 3;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float3_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Float3(0.0f, 0.0f, 0.0f));
                s += params[i].m_members * sizeof(float) * 3;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float4)
            {
                params[i].m_value = std::any(Float4(0.0f, 0.0f, 0.0f, 0.0f));
                s += sizeof(float) * 4;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Float4_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Float4(0.0f, 0.0f, 0.0f, 0.0f));
                s += params[i].m_members * sizeof(float) * 4;
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Int)
            {
                params[i].m_value = std::any(Int(0));
                s += sizeof(int);
            }
            else if (pDefs[i].m_type == EShaderParamType::ShaderType_Int_Array)
            {
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(Int(0));
                s += params[i].m_members * sizeof(int);
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
                for (size_t j = 0; j < params[i].m_members; j++)
                    params[i].m_values.push_back(m);
                s += params[i].m_members * 16 * sizeof(float);
            }
        }
        return s;
    }

    //**********************************************************************
    // Method: PushConstants
    // Pushes each shader parameter into the D3D11 constant buffer.
    //
    // Parameters:
    // pGraphics - D3D11 device/context to use
    // pBuffer - Constant buffer to push values into
    // params - List of parameters to push
    //**********************************************************************
    void CShader::PushConstants(IGraphics *pGraphics, SConstantBuffer *pBuffer, std::vector<ShaderParamInstance> &params)
    {
        if (pBuffer->m_bufferSize == 0)
            return;

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
        CT(pGraphics->GetContext()->Map(pBuffer->m_spBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
        BYTE *pb = reinterpret_cast<BYTE*>(ms.pData);
        for (size_t i = 0; i < params.size(); i++)
        {
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
                for (size_t j = 0; j < params[i].m_members; j++)
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
                for (size_t j = 0; j < params[i].m_members; j++)
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
                for (size_t j = 0; j < params[i].m_members; j++)
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
                for (size_t j = 0; j < params[i].m_members; j++)
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
                for (size_t j = 0; j < params[i].m_members; j++)
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
                for (size_t j = 0; j < params[i].m_members; j++)
                {
                        Matrix m = std::any_cast<Matrix>(params[i].m_values[j]);
                        memcpy(pb, m.x, 16 * sizeof(float));
                        pb += 16 * sizeof(float);
                    }
                }
                break;
            }
        }
        pGraphics->GetContext()->Unmap(pBuffer->m_spBuffer, 0);
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
    // Method: SetParam
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
    void CShader::SetVSParam(std::wstring paramName, std::any &value)
    {
        SetParam(paramName, value, m_vsParams);
    }

    //**********************************************************************
    // Method: SetPSParam
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

    void CShader::PushLights(std::vector<CRefObj<IPointLight>> &lights)
    {
        uint32 numLights = (uint32)lights.size();
        for (uint32 i = 0; i < numLights; i++)
        {
            Caustic::Vector3 color = lights[i]->GetColor();
            Float4 lightColor(color.x, color.y, color.z, 1.0f);
            SetParam(L"lightColor", i, std::any(lightColor), m_psParams);
            Caustic::Vector3 pos = lights[i]->GetPosition();
            Float4 lightPos(pos.x, pos.y, pos.z, 1.0f);
            SetParam(L"lightPosWS", i, std::any(lightPos), m_psParams);
        }
        SetPSParam(L"numLights", std::any((Int)numLights));
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
    // Method: BeginRender
    // BeginRender is called before rendering using this shader occurs.
    // This call is responsible for setting up the pGraphics device to use the shader.
    //
    // Parameters:
    // pGraphics - D3D11 device/context to use
    //**********************************************************************
    void CShader::BeginRender(IGraphics *pGraphics, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, std::vector<CRefObj<IPointLight>> &lights, DirectX::XMMATRIX *pWorld)
    {
        pGraphics->GetContext()->IASetInputLayout(m_spLayout);
        pGraphics->GetContext()->PSSetShader(m_spPixelShader, nullptr, 0);
        pGraphics->GetContext()->VSSetShader(m_spVertexShader, nullptr, 0);
        
        if (pFrontMaterial)
            pFrontMaterial->Render(pGraphics, lights, nullptr, this);

        PushLights(lights);
        PushMatrices(pGraphics, pWorld);
        PushConstants(pGraphics, &m_vertexConstants, m_vsParams);
        PushConstants(pGraphics, &m_pixelConstants, m_psParams);

        pGraphics->GetContext()->VSSetConstantBuffers(0, 1, &m_vertexConstants.m_spBuffer.p);
        pGraphics->GetContext()->PSSetConstantBuffers(0, 1, &m_pixelConstants.m_spBuffer.p);
    }

    //**********************************************************************
    // Method: EndRender
    // EndRender is called after rendering using this shader occurs.
    // During this call the shader may clean up any state/memory it needed
    //
    // Parameters:
    // pGraphics - D3D11 device/context to use
    //**********************************************************************
    void CShader::EndRender(IGraphics * /*pGraphics*/)
    {
    }

	CRefObj<IShaderInfo> CShader::GetShaderInfo()
	{
		return m_spShaderInfo;
	}
	
	//**********************************************************************
    // CreateConstantBuffer creates the constant buffer (pixel or vertex shader)
    //
    // Parameters:
    // pDevice - D3D device
    // pDefs - List of parameter definitions (parsed from HLSL)
    // paramsSize - Length of pDefs
    // params - Generated parameter list
    // ppBuffer - Returns the created constant buffer
    //**********************************************************************
    void CShader::CreateConstantBuffer(ID3D11Device *pDevice, ShaderParamDef *pDefs, uint32 paramsSize, std::vector<ShaderParamInstance> &params, SConstantBuffer *pConstantBuffer)
    {
        pConstantBuffer->m_heapSize = 0;
        pConstantBuffer->m_bufferSize = 0;
        uint32 s = ComputeParamSize(pDefs, paramsSize, params);
        if (s > 0)
        {
            D3D11_BUFFER_DESC buffDesc;
            buffDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
            buffDesc.ByteWidth = ((s + 15) / 16) * 16;
            buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
            buffDesc.MiscFlags = 0;
            buffDesc.StructureByteStride = 0;
            buffDesc.Usage = D3D11_USAGE_DYNAMIC;
            CT(pDevice->CreateBuffer(&buffDesc, nullptr, &pConstantBuffer->m_spBuffer));
	        pConstantBuffer->m_heapSize = (uint32)buffDesc.ByteWidth;
	        pConstantBuffer->m_bufferSize = (uint32)s;
        }
    }

    //**********************************************************************
    // Method: Create
    // Creates a shader
    //
    // Parameters:
    // pGraphics - Graphics device to use
    // pShaderName - Name of shader
    // pShaderInfo - Shader description
    // pPSBlob - Compiled binary for the pixel shader
    // pVSBlob - Compiled binary for the vertexshader
    //**********************************************************************
	void CShader::Create(IGraphics *pGraphics, const wchar_t *pShaderName, IShaderInfo *pShaderInfo, ID3DBlob *pPSBlob, ID3DBlob *pVSBlob)
    {
        if (pShaderName)
            m_name = std::wstring(pShaderName);
        
        std::vector<ShaderParamDef> &pixelShaderDefs = pShaderInfo->PixelShaderParameterDefs();
        std::vector<ShaderParamDef> &vertexShaderDefs = pShaderInfo->VertexShaderParameterDefs();
        std::vector<D3D11_INPUT_ELEMENT_DESC> &vertexLayout = pShaderInfo->VertexLayout();
        D3D11_INPUT_ELEMENT_DESC *pVertexLayout = vertexLayout.data();

        m_spShaderInfo = pShaderInfo;
        const byte *pPSByteCodes = (const byte*)pPSBlob->GetBufferPointer();
        uint32 psBufferLen = (uint32)pPSBlob->GetBufferSize();
        const byte *pVSByteCodes = (const byte*)pVSBlob->GetBufferPointer();
        uint32 vsBufferLen = (uint32)pVSBlob->GetBufferSize();

        //**********************************************************************
        ID3D11Device *pDevice = pGraphics->GetDevice();
        CT(pDevice->CreateInputLayout(pShaderInfo->VertexLayout().data(), (UINT)pShaderInfo->VertexLayout().size(),
            pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_spLayout));

        CreateConstantBuffer(pDevice, pShaderInfo->VertexShaderParameterDefs().data(),
            (uint32)pShaderInfo->VertexShaderParameterDefs().size(), m_vsParams, &m_vertexConstants);
        CreateConstantBuffer(pDevice, pShaderInfo->PixelShaderParameterDefs().data(),
            (uint32)pShaderInfo->PixelShaderParameterDefs().size(), m_psParams, &m_pixelConstants);

        CT(pDevice->CreateVertexShader(pVSByteCodes, vsBufferLen, nullptr, &m_spVertexShader));
        CT(pDevice->CreatePixelShader(pPSByteCodes, psBufferLen, nullptr, &m_spPixelShader));

        CD3D11_SAMPLER_DESC sdesc(D3D11_DEFAULT);
        CT(pDevice->CreateSamplerState(&sdesc, &m_spSamplerState));
    }
    
    //**********************************************************************
    // Method: CreateShader
    // A helper function to create a shader
    //
    // Parameters:
    // pGraphics - Graphics device to use
    // pShaderName - Name of shader
    // pShaderInfo - Shader description
    // pPSBlob - Compiled binary for the pixel shader
    // pVSBlob - Compiled binary for the vertexshader
    // ppShader - Returns the created shader
    //**********************************************************************
    CAUSTICAPI void CreateShader(IGraphics *pGraphics, const wchar_t *pShaderName, IShaderInfo *pShaderInfo,
        ID3DBlob *pPSBlob, ID3DBlob *pVSBlob, IShader **ppShader)
    {
        std::unique_ptr<CShader> spShader(new CShader());
        spShader->Create(pGraphics, pShaderName, pShaderInfo, pPSBlob, pVSBlob);
        *ppShader = spShader.release();
        (*ppShader)->AddRef();
    }
    
    CShaderMgr CShaderMgr::s_ShaderMgr;

    //**********************************************************************
    // Method: FindShader
    // Returns the requested shader
    //
    // Parameters:
    // pShaderName - Name of shader to locate
    // ppShader - Returns the requested shader
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
            *ppShader = it->second;
            (*ppShader)->AddRef();
        }
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
        else
        {
            CT(E_FAIL);
        }
    }
};
