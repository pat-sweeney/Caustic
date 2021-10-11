//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "IRenderGraphNode.h"
#include "ILightCollection.h"

namespace Caustic
{
    struct IRenderGraphNode_Group : public IRenderGraphNode
    {
        virtual void AddChild(IRenderGraphNode* pChild) = 0;
        virtual CRefObj<IRenderGraphPin> AddInputPin(std::string pinName, ERenderGraphDataType dataType) = 0;
    };
}
