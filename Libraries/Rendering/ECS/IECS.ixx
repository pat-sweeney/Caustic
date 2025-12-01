//**********************************************************************
// Copyright Patrick Sweeney 2023-2025
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <vector>
#include <functional>
#include <DirectXMath.h>

export module Rendering.ECS.IECS;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;

export namespace Caustic
{
    typedef uint64_t SystemID;
    typedef uint64_t ComponentID;
    typedef uint64_t EntityID;

	// Inside our ECS, ids are created as <time><<32 | <incrementing counter>
	// For builtin components and systems, we just use small integers (where time == 0)
	const uint64_t c_UnassignedID = 0;

    // Component IDs
    const ComponentID c_BaseComponent = 1;
    const ComponentID c_TransformComponent = 2;
    const ComponentID c_HierarchyComponent = 3;
    const ComponentID c_MeshComponent = 4;
    const ComponentID c_MaterialComponent = 5;
    const ComponentID c_LightComponent = 6;
    const ComponentID c_CameraComponent = 7;
	const ComponentID c_MaxComponentID = 64;

    // System IDs
    const SystemID c_HierarchySystem = 1;
    const SystemID c_RenderSystem = 2;
    const SystemID c_LightingSystem = 3;
    const SystemID c_MaxSystemID = 64;

    // Entity flags
    const int c_EntityFlag_Selected = 0x1;
    const int c_EntityFlag_RenderableDirty = 0x2;
    const int c_EntityFlag_DepthTested = 0x4;
    const int c_EntityFlag_Enabled = 0x8;
    const int c_EntityFlag_Visible = 0x10;

    // Component flags
	const int c_ComponentFlag_Dirty = 0x1;

    struct IEntity;
    struct ISystem;
    struct IECSManager;

    //**********************************************************************
    // Interface: IComponent
    // Base interface for all components
    //**********************************************************************
    struct IComponent
    {
        virtual ~IComponent() = default;
        virtual ComponentID GetComponentID() const = 0;
        virtual std::wstring GetName() const = 0;
		virtual void SetName(const std::wstring& name) = 0;
        virtual EntityID GetEntity() const = 0;
        virtual void SetEntity(EntityID entityID) = 0;

        virtual uint32_t GetFlags() const = 0;
        virtual void SetFlags(uint32_t flags) = 0;
        virtual void SetFlag(uint32_t flag, bool value) = 0;
        virtual bool HasFlag(uint32_t flag) const = 0;
    };

    //**********************************************************************
    // Interface: IEntity
    // Defines the interface to the Entity in our ECS framework.
    //**********************************************************************
    struct IEntity
    {
        virtual EntityID GetEntityID() const = 0;
        virtual std::wstring GetName() const = 0;
        virtual void SetName(const std::wstring& name) = 0;

        virtual bool HasComponent(ComponentID componentID) = 0;
        virtual std::vector<ComponentID> GetComponents() = 0;
        virtual void AddComponent(ComponentID componentID) = 0;
        virtual void RemoveComponent(ComponentID componentID) = 0;

        virtual uint32_t GetFlags() const = 0;
        virtual void SetFlags(uint32_t flags) = 0;
        virtual void SetFlag(uint32_t flag, bool value) = 0;
        virtual bool HasFlag(uint32_t flag) const = 0;

        virtual void Destroy() = 0;
    };

    //**********************************************************************
    // Interface: ISystem
    // Defines the interface to the System in our ECS framework.
    //**********************************************************************
    struct ISystem
    {
        virtual SystemID GetSystemID() const = 0;
        virtual std::wstring GetName() const = 0;
        virtual void SetName(const std::wstring& name) = 0;
        virtual void Initialize(IECSManager* pManager) = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Shutdown() = 0;
        virtual bool IsEnabled() const = 0;
        virtual void SetEnabled(bool enabled) = 0;
    };

    //**********************************************************************
    // Interface: IECSManager
    // Main interface for managing the ECS
    //**********************************************************************
    struct IECSManager
    {
        // Entity management
        virtual EntityID CreateEntity(const std::wstring& name = L"") = 0;
        virtual void DestroyEntity(EntityID entityID) = 0;
        virtual IEntity* GetEntity(EntityID entityID) = 0;
        virtual std::vector<EntityID> GetEntitiesWithComponents(int componentMask) = 0;

        // System management
        virtual void RegisterSystem(SystemID systemID) = 0;
        virtual void UnregisterSystem(SystemID systemID) = 0;
        virtual ISystem* GetSystem(SystemID systemID) = 0;
        virtual void UpdateSystems(float deltaTime) = 0;
    };

    // Factory functions
    CRefObj<IECSManager> CreateECSManager();
}
