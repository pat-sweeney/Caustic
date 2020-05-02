//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Vector.h"
#include "IRenderGraphNode.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    class IRenderGraphNode_Compute : public IRenderGraphNode
    {
        virtual CRefObj<IShader> GetShader() = 0;
        virtual void SetNumberThreads(int xThreads, int yThreads, int zThreads) = 0;
        virtual void SetInputThreads(uint32 width, uint32 height, uint32 depth /* = 1 */) = 0;
        virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) = 0;
        virtual void SetShaderParam(const wchar_t* pParamName, float value) = 0;
        virtual void SetInputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride) = 0;
        virtual void SetOutputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride) = 0;
        virtual void SetInputOutputBuffer(const wchar_t* pBufferName, uint8* pInputData, uint8* pOutputData, uint32 bufSize, uint32 stride) = 0;
    };
}
