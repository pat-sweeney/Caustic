//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "ComputeShaderElem.h"
#include "Rendering\Caustic\CausticFactory.h"
#include <string>

namespace Caustic
{
    CAUSTICAPI void CreateComputeShaderElem(IShader *pComputeShader, ISceneComputeShaderElem** ppElem)
    {
        std::unique_ptr<CSceneComputeShaderElem> spShaderElem(new CSceneComputeShaderElem(pComputeShader));
        *ppElem = spShaderElem.release();
        (*ppElem)->AddRef();
    }

    void CSceneComputeShaderElem::SetInputThreads(uint32 width, uint32 height, uint32 depth /* = 1 */)
    {
        uint32 xThreads, yThreads, zThreads;
        m_spComputeShader->GetShaderInfo()->GetThreadGroupSize(&xThreads, &yThreads, &zThreads);
        // Determine the correct number of thread groups so we process the entire image
        uint32 xGroups = (width + xThreads - 1) / xThreads;
        uint32 yGroups = (height + yThreads - 1) / yThreads;
        uint32 zGroups = (depth + zThreads - 1) / zThreads;
        m_spComputeShader->SetThreadCounts(xGroups, yGroups, zGroups);
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
        m_spComputeShader->SetThreadCounts(xThreads, yThreads, zThreads);
    }

    void CSceneComputeShaderElem::SetInputBuffer(const wchar_t * pBufferName, uint8* pData, uint32 bufSize, uint32 stride)
    {
        ClientBuffer s;
        s.m_dataSize = bufSize;
        s.m_stride = stride;
        uint8* pBuffer = new uint8[bufSize];
        memcpy(pBuffer, pData, bufSize);
        s.m_spInputData.reset(pBuffer);
        m_spComputeShader->SetCSParam(pBufferName, std::any(s));
    }

    void CSceneComputeShaderElem::SetOutputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride)
    {
        ClientBuffer s;
        s.m_dataSize = bufSize;
        s.m_stride = stride;
        s.m_wpOutputData = pData;
        m_spComputeShader->SetCSParam(pBufferName, std::any(s));
    }

    void CSceneComputeShaderElem::SetInputOutputBuffer(const wchar_t* pBufferName, uint8* pInputData, uint8* pOutputData, uint32 bufSize, uint32 stride)
    {
        ClientBuffer s;
        s.m_dataSize = bufSize;
        s.m_stride = stride;
        uint8* pBuffer = new uint8[bufSize];
        memcpy(pBuffer, pInputData, bufSize);
        s.m_spInputData.reset(pBuffer);
        s.m_wpOutputData = pOutputData;
        m_spComputeShader->SetCSParam(pBufferName, std::any(s));
    }
}
