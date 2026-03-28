//**********************************************************************
// Copyright Patrick Sweeney 2023-2025
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

module Rendering.ECS.ECS;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.BaseComponent;
import Rendering.ECS.TransformComponent;
import Rendering.ECS.HierarchyComponent;
import Rendering.ECS.MeshComponent;
import Rendering.ECS.MaterialComponent;

namespace Caustic
{
    //**********************************************************************
    // Entity Implementation
    //**********************************************************************
    Entity::Entity(const std::wstring& name)
        : m_entityID(s_nextEntityID++), m_name(name), m_flags(c_EntityFlag_Enabled | c_EntityFlag_Visible), m_destroyed(false)
    {
        // Every entity gets a base component by default
        auto baseComponent = std::make_unique<BaseComponent>();
        baseComponent->SetEntity(this);
        baseComponent->m_name = name;
        m_components[c_BaseComponent] = std::move(baseComponent);
    }

    Entity::~Entity()
    {
        m_components.clear();
    }

    bool Entity::HasComponent(ComponentID componentID)
    {
		for (auto& comp : m_components)
		{
			if (comp == componentID)
				return true;
		}
        return false;
    }

    std::vector<ComponentID> Entity::GetComponents()
    {
        return m_components;
    }

    void Entity::AddComponent(ComponentID componentID)
    {
        ECSManager::GetInstance()->GetComponent(componentID)->SetEntity(this->m_entityID);
        m_components[componentID] = std::unique_ptr<IComponent>(pComponent);
    }

    void Entity::RemoveComponent(ComponentID componentID)
    {
        // Don't allow removal of base component
        if (componentID != c_BaseComponent)
        {
            m_components.erase(componentID);
        }
    }

    void Entity::SetFlag(uint32_t flag, bool value)
    {
        if (value)
            m_flags |= flag;
        else
            m_flags &= ~flag;
    }

    //**********************************************************************
    // System Implementation
    //**********************************************************************
    System::System(SystemID id, const std::string& name)
        : m_systemID(id), m_systemName(name), m_pManager(nullptr), m_enabled(true)
    {
    }

    //**********************************************************************
    // ECSManager Implementation
    //**********************************************************************
    ECSManager::ECSManager()
    {
    }

    ECSManager::~ECSManager()
    {
        Clear();
    }

    CRefObj<IEntity> ECSManager::CreateEntity(const std::wstring& name)
    {
        auto spEntity = CRefObj<IEntity>(new Entity(name));
        m_entities[spEntity->GetEntityID()] = spEntity;
        return spEntity;
    }

    void ECSManager::DestroyEntity(EntityID entityID)
    {
        auto it = m_entities.find(entityID);
        if (it != m_entities.end())
        {
            it->second->Destroy();
            m_entitiesToDestroy.push_back(entityID);
        }
    }

    CRefObj<IEntity> ECSManager::GetEntity(EntityID entityID)
    {
        auto it = m_entities.find(entityID);
        return (it != m_entities.end()) ? it->second : CRefObj<IEntity>(nullptr);
    }

    std::vector<CRefObj<IEntity>> ECSManager::GetEntitiesWithComponent(ComponentID componentID)
    {
        std::vector<CRefObj<IEntity>> result;
        for (auto& pair : m_entities)
        {
            if (!pair.second->IsDestroyed() && pair.second->HasComponent(componentID))
            {
                result.push_back(pair.second);
            }
        }
        return result;
    }

    std::vector<CRefObj<IEntity>> ECSManager::GetEntitiesWithComponents(const std::vector<ComponentID>& componentIDs)
    {
        std::vector<CRefObj<IEntity>> result;
        for (auto& pair : m_entities)
        {
            if (pair.second->IsDestroyed())
                continue;

            bool hasAllComponents = true;
            for (ComponentID id : componentIDs)
            {
                if (!pair.second->HasComponent(id))
                {
                    hasAllComponents = false;
                    break;
                }
            }

            if (hasAllComponents)
            {
                result.push_back(pair.second);
            }
        }
        return result;
    }

    void ECSManager::RegisterSystem(ISystem* pSystem)
    {
        if (pSystem)
        {
            pSystem->Initialize(this);
            m_systems[pSystem->GetSystemID()] = CRefObj<ISystem>(pSystem);
        }
    }

    void ECSManager::UnregisterSystem(SystemID systemID)
    {
        auto it = m_systems.find(systemID);
        if (it != m_systems.end())
        {
            it->second->Shutdown();
            m_systems.erase(it);
        }
    }

    ISystem* ECSManager::GetSystem(SystemID systemID)
    {
        auto it = m_systems.find(systemID);
        return (it != m_systems.end()) ? it->second.get() : nullptr;
    }

    void ECSManager::UpdateSystems(float deltaTime)
    {
        ProcessDestroyedEntities();

        for (auto& pair : m_systems)
        {
            if (pair.second->IsEnabled())
            {
                pair.second->Update(deltaTime);
            }
        }
    }

    IComponent* ECSManager::CreateComponent(ComponentID componentID)
    {
        switch (componentID)
        {
        case c_BaseComponent:
            return new BaseComponent();
        case c_TransformComponent:
            return new TransformComponent();
        case c_HierarchyComponent:
            return new HierarchyComponent();
        case c_MeshComponent:
            return new MeshComponent();
        case c_MaterialComponent:
            return new MaterialComponent();
        default:
            return nullptr;
        }
    }

    void ECSManager::ProcessDestroyedEntities()
    {
        for (EntityID id : m_entitiesToDestroy)
        {
            m_entities.erase(id);
        }
        m_entitiesToDestroy.clear();
    }

    void ECSManager::Clear()
    {
        for (auto& pair : m_systems)
        {
            pair.second->Shutdown();
        }
        m_systems.clear();
        m_entities.clear();
        m_entitiesToDestroy.clear();
    }

    //**********************************************************************
    // Factory Functions
    //**********************************************************************
    CRefObj<IECSManager> CreateECSManager()
    {
        return CRefObj<IECSManager>(new ECSManager());
    }

    CRefObj<IEntity> CreateEntity(const std::wstring& name)
    {
        return CRefObj<IEntity>(new Entity(name));
    }
}
