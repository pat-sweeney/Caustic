//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.IBuffer;
import Base.Core.IRefCount;
import Base.Math.BBox;

//**********************************************************************
// File: IBuffer.h
// Defines interface for buffers.
//**********************************************************************
export namespace Caustic
{
    //**********************************************************************
    // Interface: IBuffer
    // Wrapper around arbitrary buffers (StructuredBuffer, RWStructuredBuffer, ...)
    // This is mostly used by compute shaders.
    //
    // Header:
    // {Link:#include "Rendering/Caustic/IBuffer.h"{Rendering/Caustic/IBuffer.h}}
    //**********************************************************************
    struct IBuffer : public IRefCount
    {
    };
}