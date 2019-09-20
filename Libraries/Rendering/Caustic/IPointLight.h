//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"

//**********************************************************************
// File: IPointLight.h
// This file defines the published interface for dealing with point lights
//**********************************************************************

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
