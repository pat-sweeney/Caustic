//**********************************************************************
// Copyright Patrick Sweeney 2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <DirectXMath.h>

export module Rendering.ECS.HierarchyComponent;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;

export namespace Caustic
{
    struct HierarchyComponent : public Component
    {
        CRefObj<IEntity> m_parent;                  // Entity that acts as this entity's parent
        std::vector<CRefObj<IEntity>> m_children;   // List of entity which are children under this entity
    };
}