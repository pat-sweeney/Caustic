//**********************************************************************
// Copyright Patrick Sweeney 2023-2025
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <chrono>
#include <atomic>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

export module Rendering.ECS.ECS;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.IECS;

export namespace Caustic
{
    //**********************************************************************
    // Class: Component
    // Base implementation for components
    //**********************************************************************
	class Component : public IComponent
    {
    protected:
		ComponentID m_componentID;      // ID of this component
		std::wstring m_componentName;   // Name of this component
		EntityID  m_entityID;           // ID of entity this component is attached to
		uint32_t m_flags;               // Component-specific flags

    public:
        Component(ComponentID id) : m_componentID(id), m_entityID(c_UnassignedID), m_flags(0) {}
        virtual ~Component() = default;

        //**********************************************************************
        // IComponent
        //**********************************************************************
        virtual ComponentID GetComponentID() const override { return m_componentID; }
        virtual std::wstring GetName() const override { return m_componentName; }
		virtual void SetName(const std::wstring& name) override { m_componentName = name; }
		virtual EntityID GetEntity() const override { return m_entityID; }
		virtual void SetEntity(EntityID entityID) override { m_entityID = entityID; }
		virtual uint32_t GetFlags() const override { return m_flags; }
		virtual void SetFlags(uint32_t flags) override { m_flags = flags; }
		virtual void SetFlag(uint32_t flag, bool value) override
        {
			if (value)
				m_flags |= flag;
			else
				m_flags &= ~flag;
		}
		virtual bool HasFlag(uint32_t flag) const override { return (m_flags & flag) != 0; }
    };

    //**********************************************************************
    // Class: Entity
    // Implementation of IEntity
    //**********************************************************************
    class Entity : public IEntity
    {
        EntityID m_entityID;
        std::wstring m_name;
        int m_componentMask;
        uint32_t m_flags;
		bool m_destroyed;

    public:
        Entity(const std::wstring& name = L"");
        virtual ~Entity();

        //**********************************************************************
        // IEntity
        //**********************************************************************
        virtual EntityID GetEntityID() const override { return m_entityID; }
        virtual std::wstring GetName() const override { return m_name; }
        virtual void SetName(const std::wstring& name) override { m_name = name; }

        virtual bool HasComponent(ComponentID componentID) override;
        virtual std::vector<ComponentID> GetComponents() override;
        virtual void AddComponent(ComponentID componentID) override;
        virtual void RemoveComponent(ComponentID componentID) override;

        virtual uint32_t GetFlags() const override { return m_flags; }
        virtual void SetFlags(uint32_t flags) override { m_flags = flags; }
        virtual void SetFlag(uint32_t flag, bool value) override;
        virtual bool HasFlag(uint32_t flag) const override { return (m_flags & flag) != 0; }

        virtual void Destroy() override { m_destroyed = true; }
    };

    //**********************************************************************
    // Class: System
    // Base implementation for systems
    //**********************************************************************
    class System : public ISystem
    {
    protected:
        SystemID m_systemID;
        std::wstring m_systemName;
        bool m_enabled;

    public:
        System(SystemID id, const std::string& name);
        virtual ~System() = default;

        //**********************************************************************
        // ISystem
        //**********************************************************************
        virtual SystemID GetSystemID() const override { return m_systemID; }
		virtual std::wstring GetName() const override { return m_systemName; }
		virtual void SetName(const std::wstring& name) override { m_systemName = name; }
        virtual void Initialize(IECSManager* pManager) override {}
        virtual void Update(float deltaTime) override {} // Default empty implementation
        virtual void Shutdown() override {} // Default empty implementation
        virtual bool IsEnabled() const override { return m_enabled; }
        virtual void SetEnabled(bool enabled) override { m_enabled = enabled; }
    };


    //**********************************************************************
    // Class: ECSManager
    // Main ECS manager implementation
    //**********************************************************************
    class ECSManager : public IECSManager
    {
        std::vector<Component> m_components;
		std::vector<Entity> m_entities;
		std::vector<System> m_systems;
        std::vector<EntityID> m_entitiesToDestroy;

        void ProcessDestroyedEntities();

    public:
        ECSManager();
        virtual ~ECSManager();

		static ECSManager& GetInstance()
		{
			static ECSManager instance;
			return instance;
		}

        static uint64_t GenerateECSId()
        {
            static std::atomic<uint32_t> counter{ 0 };
            uint64_t timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
            uint64_t id = (timestamp << 32) | (counter.fetch_add(1) & 0xFFFFFFFF);
            return id;
        }

        //**********************************************************************
        // IECSManager
        //**********************************************************************
        virtual EntityID CreateEntity(const std::wstring& name = L"") override;
        virtual void DestroyEntity(EntityID entityID) override;
        virtual IEntity* GetEntity(EntityID entityID) override;
        virtual std::vector<EntityID> GetEntitiesWithComponents(int componentMask) override;

        virtual void RegisterSystem(SystemID systemID) override;
        virtual void UnregisterSystem(SystemID systemID) override;
        virtual ISystem* GetSystem(SystemID systemID) override;
        virtual void UpdateSystems(float deltaTime) override;
    };
}
