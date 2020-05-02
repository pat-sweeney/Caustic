//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\IRefCount.h"
#include "Base\Math\BBox.h"

//**********************************************************************
// File: IBuffer.h
// Defines interface for buffers.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Interface: IBuffer
    // Wrapper around arbitrary buffers (StructuredBuffer, RWStructuredBuffer, ...)
    // This is mostly used by compute shaders.
    //**********************************************************************
    struct IBuffer : public IRefCount
    {
    };
}
