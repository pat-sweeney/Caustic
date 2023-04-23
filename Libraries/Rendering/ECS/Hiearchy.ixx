//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>

export module Rendering.ECS.Hierarchy;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;
import Rendering.ECS.IHierarchy;

export namespace Caustic
{
    //**********************************************************************
    // Class: HiearchyComponent
    // Implements IHiearchyComponent. The HiearchyComponent holds data defining
    // the hierarchy of our Entity nodes.
    //**********************************************************************
    struct HiearchyComponent : public Component
    {
        CRefObj<IEntity> m_spParent;
    };

    //**********************************************************************
    // Class: HierarchySystem
    // Implements IHierarchySystem. The HierarchySystem handles
    // parenting entities to other entities via the HierarchyComponent.
    //**********************************************************************
    class HierarchySystem : public System, public IHierarchySystem
    {
        CRefObj<IEntity> m_spParent;

    public:
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IHierarchySystem
        //**********************************************************************
        virtual void SetParent(IEntity* pParent) override { m_spParent = pParent; }
    };
}
