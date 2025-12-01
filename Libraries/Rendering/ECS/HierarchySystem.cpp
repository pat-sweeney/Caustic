//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <functional>

module Rendering.ECS.HierarchySystem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IHierarchySystem;
import Rendering.ECS.HierarchyComponent;

namespace Caustic
{
    void HierarchySystem::TraverseRecursive(IEntity* pEntity, std::function<void(IEntity*)> func)
    {
        CRefObj<IComponent> spComponent = pEntity->GetComponent(c_HierarchyComponent);
        if (spComponent != nullptr)
        {
            func(pEntity);
            HierarchyComponent* pHierarchyComponent = (HierarchyComponent*)spComponent.p;
            for (uint32_t i = 0; i < pHierarchyComponent->m_children.size(); i++)
            {
                CRefObj<IEntity> spEntity = pHierarchyComponent->m_children[i];
                TraverseRecursive(spEntity, func);
            }
        }
    }

    void HierarchySystem::Traverse(std::function<void(IEntity*)> func)
    {
        TraverseRecursive(m_spRoot, func);
    }
}
