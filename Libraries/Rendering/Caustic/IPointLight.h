//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"

//**********************************************************************
// File: Caustic.h
// This file defines the published interface for talking to the Caustic Renderer.
// This include is usually the top level include that clients will use.
//**********************************************************************

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Interface: IPointLight
    // Defines a point light
    //**********************************************************************
    struct IPointLight : public IRefCount
    {
        virtual void SetPosition(Vector3 &pos) = 0;
        virtual Vector3 GetPosition() = 0;
        virtual void SetColor(Vector3 &color) = 0;
        virtual Vector3 GetColor() = 0;
    };
}
