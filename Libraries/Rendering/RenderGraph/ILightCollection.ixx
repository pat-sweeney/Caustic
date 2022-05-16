//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Rendering.RenderGraph.ILightCollection;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Error;
import Rendering.Caustic.ILight;
import Rendering.RenderGraph.RenderGraphNode;

export namespace Caustic
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
        virtual void AddLight(ILight* pLight) = 0;

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
        virtual std::vector<CRefObj<ILight>> ToVector() = 0;
    };
}