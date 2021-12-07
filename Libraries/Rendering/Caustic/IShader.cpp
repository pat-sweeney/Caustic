//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>

module Rendering.Caustic.IShader;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IRenderer;

//**********************************************************************
// File: IShader.cpp
// This file contains the implementation of the public portions of Shader
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Method: CreateGPUBuffer
    // A helper function to create a shader
    //
    // Parameters:
    // pRenderer - Graphics device to use
    // type - which type of buffer are we creating
    // elemSize - size in bytes of each element in the buffer
    //
    // Returns:
    // Returns the created buffer
    // 
    // Module:
    // {Link:import Rendering.Caustic.IShader;{Rendering/Caustic/IShader.cpp}}
    //**********************************************************************
    CRefObj<IGPUBuffer> CreateGPUBuffer(IRenderer* pRenderer, EBufferType type, uint32 numElems, uint32 elemSize, uint32 bindFlags)
    {
        std::unique_ptr<CGPUBuffer> spBuffer(new CGPUBuffer());
        spBuffer->Create(pRenderer, type, numElems, elemSize, bindFlags);
        return CRefObj<IGPUBuffer>(spBuffer.release());
    }
}