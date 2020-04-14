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

    void CSceneComputeShaderElem::SetInputBuffer(std::string& bufferName, uint8* pData, uint32 bufSize)
    {
    }

    void CSceneComputeShaderElem::SetOutputBuffer(std::string& bufferName, uint8* pData, uint32 bufSize)
    {
    }
}
