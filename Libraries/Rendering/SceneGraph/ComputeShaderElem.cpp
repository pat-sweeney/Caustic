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

    void CSceneComputeShaderElem::SetNumberThreads(int xThreads, int yThreads, int zThreads)
    {
        m_spComputeShader->SetThreadCounts(xThreads, yThreads, zThreads);
    }

    void CSceneComputeShaderElem::SetInputBuffer(const wchar_t * pBufferName, uint8* pData, uint32 bufSize, uint32 stride)
    {
        StructuredBuffer s;
        s.m_dataSize = bufSize;
        s.m_stride = stride;
        s.m_isInputBuffer = true;
        uint8* pBuffer = new uint8[bufSize];
        memcpy(pBuffer, pData, bufSize);
        s.m_spData.reset(pBuffer);
        m_spComputeShader->SetCSParam(pBufferName, std::any(s));
    }

    void CSceneComputeShaderElem::SetOutputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride)
    {
        StructuredBuffer s;
        s.m_dataSize = bufSize;
        s.m_stride = stride;
        s.m_spData.reset(pData);
        s.m_isInputBuffer = false;
        m_spComputeShader->SetCSParam(pBufferName, std::any(s));
    }
}
