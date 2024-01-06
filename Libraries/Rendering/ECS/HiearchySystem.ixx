//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <vector>
#include <functional>

export module Rendering.ECS.HierarchySystem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;
import Rendering.ECS.IHierarchySystem;

export namespace Caustic
{
    //**********************************************************************
    // Class: HierarchySystem
    // Implements IHierarchySystem. The HierarchySystem handles
    // parenting entities to other entities via the HierarchyComponent.
    //**********************************************************************
    class HierarchySystem : public System, public IHierarchySystem
    {
        CRefObj<IEntity> m_spRoot;

        void TraverseRecursive(IEntity* pEntity, std::function<void(IEntity*)> func);
    public:
        //**********************************************************************
        // IRefCount
        //************************************************************* *********
        virtual uint32_t  AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t  Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IHierarchySystem
        //**********************************************************************
        virtual void Traverse(std::function<void(IEntity*)> func) override;
    };
}
