//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>

export module Rendering.RenderGraph.IRGNCompute;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IRenderer;
import Rendering.RenderGraph.IRenderGraphNode;

export namespace Caustic
{
    class IRenderGraphNode_Compute : public IRenderGraphNode
    {
        virtual CRefObj<IShader> GetShader() = 0;
        virtual void SetNumberThreads(int xThreads, int yThreads, int zThreads) = 0;
        virtual void SetInputThreads(uint32 width, uint32 height, uint32 depth /* = 1 */) = 0;
        virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) = 0;
        virtual void SetShaderParam(const wchar_t* pParamName, float value) = 0;
        virtual void SetBuffer(IRenderer* pRenderer, const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride) = 0;
    };
}
