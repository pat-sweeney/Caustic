//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>
#include <windows.h>

module Rendering.RenderGraph.RGNCompute;
import Base.Core.ConvertStr;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Rendering.Caustic.IBuffer;
import Rendering.Caustic.ShaderInfo;
import Rendering.Caustic.Shader;
import Rendering.RenderGraph.IRGNCompute;
import Rendering.RenderGraph.RGNCompute;
import Rendering.RenderGraph.ILightCollection;

namespace Caustic
{
    CRenderGraphNode_Compute::CRenderGraphNode_Compute(IShader *pShader) :
        m_xThreads(0), m_yThreads(0), m_zThreads(0)
    {
        m_spComputeShader = pShader;
        CRefObj<IShaderInfo> spShaderInfo = m_spComputeShader->GetShaderInfo();
        std::vector<Caustic::ShaderParamDef> &defs = spShaderInfo->ComputeShaderParameterDefs();
        for (int i = 0; i < defs.size(); i++)
        {
            if (defs[i].m_type == ShaderType_RWStructuredBuffer ||
                defs[i].m_type == ShaderType_RWByteAddressBuffer)
            {
                std::any empty;
                CreatePin(this, false, wstr2str(defs[i].m_name), ERenderGraphDataType::Buffer, empty);
            }
            if (defs[i].m_type == ShaderType_StructuredBuffer ||
                defs[i].m_type == ShaderType_RWStructuredBuffer ||
                defs[i].m_type == ShaderType_RWByteAddressBuffer)
            {
                std::any empty;
                CreatePin(this, true, wstr2str(defs[i].m_name), ERenderGraphDataType::Buffer, empty);
            }
        }
    }

    std::any CRenderGraphNode_Compute::GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        // First pull each buffer from our input pins
        for (int i = 0; i < (int)m_inputs.size(); i++)
        {
            if (m_inputs[i]->GetDataType() == ERenderGraphDataType::Buffer)
            {
                CRefObj<IShaderInfo> spShaderInfo = m_spComputeShader->GetShaderInfo();
                std::vector<Caustic::ShaderParamDef>& defs = spShaderInfo->ComputeShaderParameterDefs();
                for (int i = 0; i < defs.size(); i++)
                {
                    if (defs[i].m_type == ShaderType_StructuredBuffer ||
                        defs[i].m_type == ShaderType_RWStructuredBuffer ||
                        defs[i].m_type == ShaderType_RWByteAddressBuffer)
                    {
                        std::string pinName = wstr2str(defs[i].m_name);
                        CRefObj<IRenderGraphPin> spPin = FindInputPin(pinName);
                        CRefObj<IBuffer> spBuffer = std::any_cast<CRefObj<IBuffer>>(spPin->GetValue(pRenderer, pRenderCtx));
                        std::any val(spBuffer);
                        m_spComputeShader->SetCSParam(defs[i].m_name, val);
                    }
                }
            }
        }

        m_spComputeShader->Dispatch(pRenderer, m_xThreads, m_yThreads, m_zThreads);
        return std::any(nullptr); // TODO: Return output buffer
    }

    void CRenderGraphNode_Compute::SetInputThreads(uint32_t width, uint32_t height, uint32_t depth /* = 1 */)
    {
        uint32_t xThreads, yThreads, zThreads;
        m_spComputeShader->GetShaderInfo()->GetThreadGroupSize(&xThreads, &yThreads, &zThreads);
        // Determine the correct number of thread groups so we process the entire image
        uint32_t xGroups = (width + xThreads - 1) / xThreads;
        uint32_t yGroups = (height + yThreads - 1) / yThreads;
        uint32_t zGroups = (depth + zThreads - 1) / zThreads;
        m_xThreads = xGroups;
        m_yThreads = yGroups;
        m_zThreads = zGroups;
    }

    void CRenderGraphNode_Compute::SetShaderParam(const wchar_t* pParamName, uint32_t value)
    {
        std::any wv = std::any(Int(value));
        m_spComputeShader->SetCSParam(pParamName, wv);
    }

    void CRenderGraphNode_Compute::SetShaderParam(const wchar_t* pParamName, float value)
    {
        std::any wv = std::any(Float(value));
        m_spComputeShader->SetCSParam(pParamName, wv);
    }


    void CRenderGraphNode_Compute::SetBuffer(IRenderer *pRenderer, const wchar_t* pBufferName, uint8* pData, uint32_t bufSize, uint32_t elemSize)
    {
        EBufferType bufferType;
        ShaderParamDef def;
        if (m_spComputeShader->GetShaderInfo()->GetComputeShaderParameterDef(pBufferName, &def))
        {
            switch (def.m_type)
            {
            case EShaderParamType::ShaderType_AppendStructuredBuffer: bufferType = AppendStructuredBuffer; break;
            default:
            case EShaderParamType::ShaderType_RWStructuredBuffer: bufferType = RWStructuredBuffer; break;
            case EShaderParamType::ShaderType_RWByteAddressBuffer: bufferType = RWByteAddressBuffer; break;
            case EShaderParamType::ShaderType_StructuredBuffer: bufferType = StructuredBuffer; break;
            }
            auto spGPUBuffer = Caustic::CreateGPUBuffer(pRenderer, bufferType, bufSize / elemSize, elemSize, 0);
            spGPUBuffer->CopyFromCPU(pRenderer, pData);
            std::any v(spGPUBuffer);
            m_spComputeShader->SetCSParam(pBufferName, v);
        }
    }
}
