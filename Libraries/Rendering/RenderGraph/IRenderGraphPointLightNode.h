//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
#include "Base\Core\RefCount.h"
#include "Base\Math\Vector.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    class IRenderGraphPointLightNode : public IRenderGraphNode
    {
        //**********************************************************************
        // Method: SetPosition
        // Sets the position of the light
        //
        // Parameters:
        // pos - position for light
        //**********************************************************************
        virtual void SetPosition(Caustic::Vector3& pos) = 0;

        //**********************************************************************
        // Method: SetColor
        // Sets the color of the light
        //
        // Parameters:
        // pos - color for light
        //**********************************************************************
        virtual void SetColor(Caustic::Vector3& clr) = 0;
    };
}
