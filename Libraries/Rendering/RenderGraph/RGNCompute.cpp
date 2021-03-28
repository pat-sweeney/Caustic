//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Vector.h"
#include "Base\Core\convertStr.h"
#include "IRGNCompute.h"
#include "RGNCompute.h"
#include "ILightCollection.h"
#include "Rendering\Caustic\IBuffer.h"
#include "Rendering\Caustic\Shader.h"
#include "Rendering\Caustic\ShaderInfo.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    CRenderGraphNode_Compute::CRenderGraphNode_Compute(IShader *pShader)
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

        std::vector<CRefObj<ILight>> lights;
        m_spComputeShader->BeginRender(pRenderer, nullptr, lights, nullptr);
        m_spComputeShader->EndRender(pRenderer);
        return std::any(nullptr); // TODO: Return output buffer
    }

    void CRenderGraphNode_Compute::SetInputThreads(uint32 width, uint32 height, uint32 depth /* = 1 */)
    {
        uint32 xThreads, yThreads, zThreads;
        m_spComputeShader->GetShaderInfo()->GetThreadGroupSize(&xThreads, &yThreads, &zThreads);
        // Determine the correct number of thread groups so we process the entire image
        uint32 xGroups = (width + xThreads - 1) / xThreads;
        uint32 yGroups = (height + yThreads - 1) / yThreads;
        uint32 zGroups = (depth + zThreads - 1) / zThreads;
        m_spComputeShader->SetThreadCounts(xGroups, yGroups, zGroups);
    }

    void CRenderGraphNode_Compute::SetShaderParam(const wchar_t* pParamName, uint32 value)
    {
        std::any wv = std::any(Int(value));
        m_spComputeShader->SetCSParam(pParamName, wv);
    }

    void CRenderGraphNode_Compute::SetShaderParam(const wchar_t* pParamName, float value)
    {
        std::any wv = std::any(Float(value));
        m_spComputeShader->SetCSParam(pParamName, wv);
    }


    void CRenderGraphNode_Compute::SetInputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride)
    {
   //     ClientBuffer s;
   //     s.m_dataSize = bufSize;
   //     s.m_stride = stride;
   //     uint8* pBuffer = new uint8[bufSize];
   //     memcpy(pBuffer, pData, bufSize);
   //     s.m_spInputData.reset(pBuffer);
   //     std::any v(s);
   //     m_spComputeShader->SetCSParam(pBufferName, v);
    }

    void CRenderGraphNode_Compute::SetOutputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride)
    {
   //   ClientBuffer s;
   //   s.m_dataSize = bufSize;
   //   s.m_stride = stride;
   //   s.m_wpOutputData = pData;
   //   std::any v(s);
   //   m_spComputeShader->SetCSParam(pBufferName, v);
    }

    void CRenderGraphNode_Compute::SetInputOutputBuffer(const wchar_t* pBufferName, uint8* pInputData, uint8* pOutputData, uint32 bufSize, uint32 stride)
    {
   //   std::vector<CRefObj<IRenderGraphPin>> m_inputs; // List of arbitrary inputs that Compute shader might take
   //   ClientBuffer s;
   //   s.m_dataSize = bufSize;
   //   s.m_stride = stride;
   //   uint8* pBuffer = new uint8[bufSize];
   //   memcpy(pBuffer, pInputData, bufSize);
   //   s.m_spInputData.reset(pBuffer);
   //   s.m_wpOutputData = pOutputData;
   //   std::any v(s);
   //   m_spComputeShader->SetCSParam(pBufferName, v);
    }
}
