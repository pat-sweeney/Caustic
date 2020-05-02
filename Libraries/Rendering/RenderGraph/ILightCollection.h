//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Core\error.h"
#include "RenderGraphNode.h"
#include "ILightCollection.h"
#include <vector>

namespace Caustic
{
    //**********************************************************************
    // Interface: ILightCollection
    // Interface for managing a collection of lights used by the render graph
    //**********************************************************************
    struct ILightCollection : public IRefCount
    {
        //**********************************************************************
        // Method: AddLight
        // Adds a new light to the collection
        //
        // Parameters:
        // pLight - Light to add to the collection
        //**********************************************************************
        virtual void AddLight(IPointLight* pLight) = 0;

        //**********************************************************************
        // Method: Clear
        // Deletes all the lights from the collection
        //**********************************************************************
        virtual void Clear() = 0;

        //**********************************************************************
        // Method: ToVector
        // Returns the collection as a std::vector of <IPointLight>s
        //
        // Returns:
        // Returns a vector of the lights in the collection
        //**********************************************************************
        virtual std::vector<CRefObj<IPointLight>> ToVector() = 0;
    };
}