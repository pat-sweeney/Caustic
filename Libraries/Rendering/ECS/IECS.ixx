//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>

export module Rendering.ECS.IECS;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;

export namespace Caustic
{
    typedef uint32_t SystemID;
    typedef uint32_t ComponentID;
    const ComponentID c_BaseComponent = 1;
    const ComponentID c_HierarchyComponent = 2;
    const ComponentID c_MeshComponent = 3;
    const ComponentID c_MaterialComponent = 4;
    const ComponentID c_UserDefinedComponent = 100;
    const SystemID c_HierarchySystem = 1;
    const SystemID c_RenderSystem = 2;
    const SystemID c_UserDefinedSystem = 100;

    //**********************************************************************
    // Class: ISystem
    // Defines the interface to the System in our ECS framework.
    //**********************************************************************
    struct ISystem : public IRefCount
    {
        virtual std::string GetSystemName() = 0;
    };

    struct Component;

    const int c_EntityFlag_Selected = 0x1;
    const int c_EntityFlag_RenderableDirty = 0x2;
    const int c_EntityFlag_DepthTested = 0x4;
    
    //**********************************************************************
    // Class: IEntity
    // Defines the interface to the Entity in our ECS framework.
    //**********************************************************************
    struct IEntity : public IRefCount
    {
        virtual Component* HasComponent(ComponentID componentID) = 0;
    };

    //**********************************************************************
    // Class: Component
    // Defines an Component in our ECS Framework. Components are simple buckets
    // of data. They contain no code.
    //**********************************************************************
    struct Component
    {
        uint32_t m_flags;
        CRefObj<IEntity> m_spEntity;
    };

    CRefObj<IEntity> CreateEntity(std::wstring name);
}
