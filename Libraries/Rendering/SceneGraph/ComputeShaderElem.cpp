//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "ComputeShaderElem.h"
#include <string>
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader *pComputeShader)
    {
        return CRefObj<ISceneComputeShaderElem>(new CSceneComputeShaderElem(pComputeShader));
    }

    void CSceneComputeShaderElem::SetInputThreads(uint32 width, uint32 height, uint32 depth /* = 1 */)
    {
        uint32 xThreads, yThreads, zThreads;
        m_spComputeShader->GetShaderInfo()->GetThreadGroupSize(&xThreads, &yThreads, &zThreads);
        // Determine the correct number of thread groups so we process the entire image
        uint32 xGroups = (width + xThreads - 1) / xThreads;
        uint32 yGroups = (height + yThreads - 1) / yThreads;
        uint32 zGroups = (depth + zThreads - 1) / zThreads;
        m_xThreads = xGroups;
        m_yThreads = yGroups;
        m_zThreads = zGroups;
    }

    void CSceneComputeShaderElem::SetShaderParam(const wchar_t* pParamName, uint32 value)
    {
        std::any wv = std::any(Int(value));
        m_spComputeShader->SetCSParam(pParamName, wv);
    }
    
    void CSceneComputeShaderElem::SetShaderParam(const wchar_t* pParamName, float value)
    {
        std::any wv = std::any(Float(value));
        m_spComputeShader->SetCSParam(pParamName, wv);
    }

    void CSceneComputeShaderElem::SetNumberThreads(int xThreads, int yThreads, int zThreads)
    {
        m_xThreads = xThreads;
        m_yThreads = yThreads;
        m_zThreads = zThreads;
    }

    void CSceneComputeShaderElem::SetBuffer(IRenderer *pRenderer, const wchar_t * pBufferName, uint8* pData, uint32 bufSize, uint32 elemSize)
    {
        EBufferType bufferType;
        ShaderParamDef def;
        if (m_spComputeShader->GetShaderInfo()->GetComputeShaderParameterDef(pBufferName, &def))
        {
            switch (def.m_type)
            {
            case EShaderParamType::ShaderType_AppendStructuredBuffer: bufferType = AppendStructuredBuffer; break;
            case EShaderParamType::ShaderType_RWStructuredBuffer: bufferType = RWStructuredBuffer; break;
            case EShaderParamType::ShaderType_RWByteAddressBuffer: bufferType = RWByteAddressBuffer; break;
            case EShaderParamType::ShaderType_StructuredBuffer: bufferType = StructuredBuffer; break;
            }
            auto spGPUBuffer = Caustic::CreateGPUBuffer(pRenderer, bufferType, bufSize / elemSize, elemSize, 0); // TODO: Change signature on this method so we don't compute number of elements
            spGPUBuffer->CopyFromCPU(pRenderer, pData);
            std::any v(spGPUBuffer);
            m_spComputeShader->SetCSParam(pBufferName, v);
        }
    }
}
