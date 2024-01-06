//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <functional>

export module Rendering.ECS.IHierarchySystem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;

export namespace Caustic
{
    //**********************************************************************
    // Class: IHierarchySystem
    // Defines the interface to the HierarchySystem. The HierarchySystem handles
    // parenting entities to other entities via the HierarchyComponent.
    //**********************************************************************
    struct IHierarchySystem : public ISystem
    {
        virtual void Traverse(std::function<void(IEntity*)> func) = 0;
    };
}
