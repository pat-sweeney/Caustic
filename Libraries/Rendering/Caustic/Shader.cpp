//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Shader.h"
#include "Sampler.h"
#include <DirectXMath.h>
#include <memory>
#include "d3dx12.h"

namespace Caustic
{
	//**********************************************************************
    // CShader::ComputeParamSize parses the definitions of each shader
    // parameter that was read from the HLSL (using ParseShader.exe). For each
    // definition we will compute the offset of the parameter within the constant
    // buffer. This is used later when updating the constant buffer.
    // pDefs List of shader definitions defining each parameter.
    // numParams Number of parameters in params
    // \param[out] params Parameter list we will copy definitions into
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
    // CShader::PushConstants pushes each shader parameter into the D3D11 constant buffer.
    // pRenderer - Rendering device to use
    // pBuffer - Constant buffer to push values into
    // params - List of parameters to push
    //**********************************************************************
    void CShader::PushConstants(IRenderer *pRenderer, SConstantBuffer *pBuffer, std::vector<ShaderParamInstance> &params)
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
                    v.m_spSampler->Render(pRenderer, params[i].m_offset);
                }
                break;
            }
        }

        // Next push constants
        if (pBuffer == nullptr)
            return;
		D3D12_RANGE range;
		range.Begin = 0;
		range.End = 0;
		BYTE *pb;
		uint32 frameIndex = pRenderer->GetFrameIndex();
		CT(pBuffer->m_spBuffer[frameIndex]->Map(0, &range, (void**)&pb));
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
		pBuffer->m_spBuffer[frameIndex]->Unmap(0, &range);
	}

    //**********************************************************************
    // SetParam sets shader constant, texture or sampler
    // paramName Name of the parameter
    // value Value of parameter
    // params List of parameters to update
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
    // SetParam sets shader constant, texture or sampler
    // paramName Name of the parameter
    // value Value of parameter
    // params List of parameters to update
    //**********************************************************************
    void CShader::SetParam(std::wstring paramName, int index, std::any &value, std::vector<ShaderParamInstance> &params)
    {
        for (size_t i = 0; i < params.size(); i++)
        {
            if (params[i].m_name == paramName)
            {
                if (index >= params[i].m_values.size())
                    params[i].m_values.resize(index + 1);
                params[i].m_values[index] = value;
                params[i].m_dirty = true;
                break;
            }
        }
    }

    //**********************************************************************
    // SetPSParam sets a pixel shader parameter
    // paramName Name of parameter
    // value Value of parameter
    //**********************************************************************
    void CShader::SetPSParam(std::wstring paramName, std::any &value)
    {
        SetParam(paramName, value, m_psParams);
    }

    //**********************************************************************
    // SetPSParam sets a pixel shader array element parameter
    // paramName Name of parameter
    // index Index into array
    // value Value of parameter
    //**********************************************************************
    void CShader::SetPSParam(std::wstring paramName, int index, std::any &value)
    {
        SetParam(paramName, index, value, m_psParams);
    }

    //**********************************************************************
    // SetVSParam sets a vertex shader parameter
    // paramName Name of parameter
    // value Value of parameter
    //**********************************************************************
    void CShader::SetVSParam(std::wstring paramName, std::any &value)
    {
        SetParam(paramName, value, m_vsParams);
    }

    //**********************************************************************
    // SetPSParam sets a vertex shader array element parameter
    // paramName Name of parameter
    // index Index into array
    // value Value of parameter
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
    // BeginRender is called before rendering using this shader occurs.
    // This call is responsible for setting up the pGraphics device to use the shader.
    // pGraphics D3D11 device/context to use
    //**********************************************************************
    void CShader::BeginRender(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights, DirectX::XMMATRIX *pWorld)
    {
        auto spCmdList = pRenderer->GetCommandList();
        spCmdList->SetPipelineState(m_spPipelineState);

        PushLights(lights);
        PushMatrices(pRenderer, pWorld);
        PushConstants(pRenderer, &m_vertexConstants, m_vsParams);
        PushConstants(pRenderer, &m_pixelConstants, m_psParams);

        uint32 frameIndex = pRenderer->GetFrameIndex();

        ID3D12DescriptorHeap *ppHeaps[] = { m_spDescriptorHeap[frameIndex] };
        spCmdList->SetDescriptorHeaps(1, ppHeaps);
        spCmdList->SetGraphicsRootDescriptorTable(1, m_spDescriptorHeap[frameIndex]->GetGPUDescriptorHandleForHeapStart());
    }

    //**********************************************************************
    // EndRender is called after rendering using this shader occurs.
    // During this call the shader may clean up any state/memory it needed
    // pGraphics D3D11 device/context to use
    //**********************************************************************
    void CShader::EndRender(IRenderer * /*pRenderer*/)
    {
    }

	CRefObj<IShaderInfo> CShader::GetShaderInfo()
	{
		return m_spShaderInfo;
	}
	
	//**********************************************************************
    // CreateConstantBuffer creates the constant buffer (pixel or vertex shader)
    // pDevice D3D device
    // pDefs - List of parameter definitions (parsed from HLSL)
    // paramsSize - Length of pDefs
    // params - Generated parameter list
    // ppBuffer - Returns the created constant buffer
    //**********************************************************************
    void CShader::CreateConstantBuffer(ID3D12Device *pDevice, ShaderParamDef *pDefs, uint32 paramsSize, std::vector<ShaderParamInstance> &params, SConstantBuffer *pConstantBuffer)
    {
        pConstantBuffer->m_heapSize = 0;
        pConstantBuffer->m_bufferSize = 0;
        uint32 s = ComputeParamSize(pDefs, paramsSize, params);
        if (s > 0)
        {
            // Constant buffers must be 64KB aligned (which seems widly inefficient for our approach)
            UINT64 heapSize = ((s + 64 * 1024 - 1) / (64 * 1024)) * (64 * 1024);
            
            for (int i = 0; i < c_MaxFrames; i++)
			{
				CT(pDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(heapSize), D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr, __uuidof(ID3D12Resource), (void**)&pConstantBuffer->m_spBuffer[i]));
				CT(pConstantBuffer->m_spBuffer[i]->SetName((i == 0) ? L"m_spBuffer[0]" : L"m_spBuffer[1]"));
            }
            pConstantBuffer->m_heapSize = (uint32)heapSize;
            pConstantBuffer->m_bufferSize = (uint32)s;
        }
    }

    //**********************************************************************
    // Create creates a shader
    // pRenderer - Rendering device to use
    // pShaderName - Name of shader
    // pPSParams - List of vertex shader definitions (from ParseShader.exe)
    // psParamsSize - Length of pPSParams
    // pVSParams - List of pixel shader definitions (from ParseShader.exe)
    // vsParamsSize - Length of pVSParams
    // pPSByteCodes - Byte code for pixel shader
    // psBufferLen - Length of pPSByteCodes in bytes
    // pVSByteCodes - Byte code for vertex shader
    // vsBufferLen - Length of pVSByteCodes in bytes
    // pLayout - Vertex layout
    // numLayoutElems - Length of pLayout (in elements)
    //**********************************************************************
	void CShader::Create(IRenderer *pRenderer, const wchar_t *pShaderName, IShaderInfo *pShaderInfo, ID3DBlob *pPSBlob, ID3DBlob *pVSBlob)
    {
        auto spDevice = pRenderer->GetDevice();
        
        if (pShaderName)
            m_name = std::wstring(pShaderName);

		std::vector<ShaderParamDef> &pixelShaderDefs = pShaderInfo->PixelShaderParameterDefs();
		std::vector<ShaderParamDef> &vertexShaderDefs = pShaderInfo->VertexShaderParameterDefs();
		std::vector<D3D12_INPUT_ELEMENT_DESC> &vertexLayout = pShaderInfo->VertexLayout();
		D3D12_INPUT_ELEMENT_DESC *pVertexLayout = vertexLayout.data();

		m_spShaderInfo = pShaderInfo;

		const byte *pPSByteCodes = (const byte*)pPSBlob->GetBufferPointer();
		uint32 psBufferLen = (uint32)pPSBlob->GetBufferSize();
		const byte *pVSByteCodes = (const byte*)pVSBlob->GetBufferPointer();
		uint32 vsBufferLen = (uint32)pVSBlob->GetBufferSize();

		//**********************************************************************
		// Create pipeline state object
		//**********************************************************************
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
		inputLayoutDesc.NumElements = (UINT)vertexLayout.size();
		inputLayoutDesc.pInputElementDescs = &vertexLayout[0];

		for (uint32 i = 0; i < inputLayoutDesc.NumElements; i++)
			m_layout.push_back(inputLayoutDesc.pInputElementDescs[i]);

		D3D12_RASTERIZER_DESC rastDesc = {};
		rastDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		rastDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.SlopeScaledDepthBias = 0.0f;
		rastDesc.DepthClipEnable = true;
		rastDesc.MultisampleEnable = false;
		rastDesc.AntialiasedLineEnable = true;
		rastDesc.ForcedSampleCount = 0;
		rastDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		D3D12_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_COPY;
		blendDesc.RenderTarget[0].LogicOpEnable = false;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = inputLayoutDesc;
		psoDesc.pRootSignature = pRenderer->GetRootSignature();
		psoDesc.VS.BytecodeLength = vsBufferLen;
		psoDesc.VS.pShaderBytecode = pVSByteCodes;
		psoDesc.PS.BytecodeLength = psBufferLen;
		psoDesc.PS.pShaderBytecode = pPSByteCodes;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.RTVFormats[0] = pRenderer->GetFormat();
		psoDesc.SampleDesc = pRenderer->GetSampleDesc();
		psoDesc.SampleMask = 0xffffffff;
		psoDesc.RasterizerState = rastDesc;
		psoDesc.BlendState = blendDesc;
		psoDesc.NumRenderTargets = 1;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = true;
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		CT(spDevice->CreateGraphicsPipelineState(&psoDesc, __uuidof(ID3D12PipelineState), (void**)&m_spPipelineState));
		
		m_spVertexShader = pVSBlob;
		m_spPixelShader = pPSBlob;

        CreateConstantBuffer(spDevice, pShaderInfo->VertexShaderParameterDefs().data(),
			(uint32)pShaderInfo->VertexShaderParameterDefs().size(), m_vsParams, &m_vertexConstants);
        CreateConstantBuffer(spDevice, pShaderInfo->PixelShaderParameterDefs().data(),
			(uint32)pShaderInfo->PixelShaderParameterDefs().size(), m_psParams, &m_pixelConstants);

        // Allocate heap for descriptors and create constant buffer views
        for (int j = 0; j < c_MaxFrames; j++)
        {
            D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
            heapDesc.NumDescriptors = 2;
            heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            heapDesc.NodeMask = 0;
            CT(spDevice->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_spDescriptorHeap[j]));

            for (int i = 0; i < 2; i++)
            {
                SConstantBuffer *pBuffer = (i == 0) ? &m_vertexConstants : &m_pixelConstants;
                if (pBuffer->m_bufferSize == 0)
                    continue;
                D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
                cbvDesc.BufferLocation = pBuffer->m_spBuffer[j]->GetGPUVirtualAddress();
                cbvDesc.SizeInBytes = (sizeof(pBuffer->m_bufferSize) + 255) & ~255;    // CB size is required to be 256-byte aligned.
                D3D12_CPU_DESCRIPTOR_HANDLE h = m_spDescriptorHeap[j]->GetCPUDescriptorHandleForHeapStart();
                UINT offset = spDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                h.ptr += i * offset;
                spDevice->CreateConstantBufferView(&cbvDesc, h);
            }
        }
    }
    
    CShaderMgr CShaderMgr::s_ShaderMgr;

    //**********************************************************************
    // FindShader returns the requested shader
    // pShaderName Name of shader to locate
    // ppShader Returns the requested shader
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
    // RegisterShader registers the specified shader with the shader manager
    // pShaderName Name of shader
    // pShader Shader to register
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
