//**********************************************************************
// Copyright Patrick Sweeney 2025
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

export module Rendering.ECS.Archetype;
import Base.Core.Core;
import Base.Core.Error;
import Rendering.ECS.IECS;
import Rendering.ECS.BaseComponent;
import Rendering.ECS.TransformComponent;
import Rendering.ECS.HierarchyComponent;
import Rendering.ECS.MeshComponent;
import Rendering.ECS.MaterialComponent;

export namespace Caustic
{
    //**********************************************************************
    // Class: ArchetypeBlock
    // Represents a block of memory containing entities sharing the same archetype (component composition)
    // Each block can hold a fixed number of entities as defined by numEntities passed to the ctor.
    // For instance, if we have an archetype with Transform, Mesh, and Material components,
    // Then memory will be layed out as follows where each component data type is stored
    // as a contiguous array:
    //      Transform: T0, T1, T2, ...
    //      Mesh:      M0, M1, M2, ...
    //      Material:  Ma0, Ma1, Ma2, ...
    //**********************************************************************
    class ArchetypeBlock
    {
		int m_componentMask; // Bitmask representing which components are present
        std::vector<void*> m_componentData; // Pointers to component data arrays

    public:
        ArchetypeBlock(int compMask, int numEntities)
            : m_componentMask(compMask)
        {
            size_t componentSize = 0;
            for (int i = 0; i < c_MaxComponentID; i++)
            {
                if (compMask & (1 << i))
                {
                    switch (i)
                    {
                    case c_BaseComponent:
                        componentSize = sizeof(BaseComponent);
                        break;
                    case c_TransformComponent:
                        componentSize = sizeof(TransformComponent);
                        break;
                    case c_HierarchyComponent:
                        componentSize = sizeof(HierarchyComponent);
                        break;
                    case c_MeshComponent:
                        componentSize = sizeof(MeshComponent);
                        break;
                    case c_MaterialComponent:
                        componentSize = sizeof(MaterialComponent);
                        break;
                    case c_LightComponent:
                        componentSize = sizeof(LightComponent);
                        break;
                    case c_CameraComponent:
                        componentSize = sizeof(CameraComponent);
                        break;
                    }
                    m_componentData.push_back(new byte[componentSize * numEntities]);
                }
            }
        }

        template<typename T>
        T* GetComponent(int entityIndex, ComponentID id)
        {
            size_t componentSize = 0;
            int compIndex = 0;
            for (int i = 0; i < c_MaxComponent; i++)
            {
                if (compMask & (1 << i))
                {
                    compIndex++;
                }
            }
            switch (i)
            {
            case c_BaseComponent:
                return reinterpret_cast<BaseComponent*>(&m_componentData[compIndex][entityIndex * sizeof(BaseComponent)]);
            case c_TransformComponent:
                return reinterpret_cast<TransformComponent*>(&m_componentData[compIndex][entityIndex * sizeof(TransformComponent)]);
            case c_HierarchyComponent:
                return reinterpret_cast<HierarchyComponent*>(&m_componentData[compIndex][entityIndex * sizeof(HierarchyComponent)]);
            case c_MeshComponent:
                return reinterpret_cast<MeshComponent*>(&m_componentData[compIndex][entityIndex * sizeof(MeshComponent)]);
            case c_MaterialComponent:
                return reinterpret_cast<MaterialComponent*>(&m_componentData[compIndex][entityIndex * sizeof(MaterialComponent)]);
            case c_LightComponent:
                return reinterpret_cast<LightComponent*>(&m_componentData[compIndex][entityIndex * sizeof(LightComponent)]);
            case c_CameraComponent:
                return reinterpret_cast<CameraComponent*>(&m_componentData[compIndex][entityIndex * sizeof(CameraComponent)]);
            default:
                return nullptr;
            }
        }
    };

    struct ArchetypeBlockKey
    {
        int componentMask;
        int block;
        int id;
    };

    class ArchetypeManager
    {
        std::unordered_map<EntityID, ArchetypeBlockKey> m_entityMapping; // Map of entityID to block key
        std::vector<ArchetypeBlock> m_archetypes;
        size_t m_defaultBlockSize;

        ArchetypeBlock* FindMatchingArchetype(int componentMask)
        {
            for (size_t i = 0; i < m_archetypes.size(); i++)
            {
                if (m_archetypes[i].m_componentMask == componentMask &&
                    m_archetypes.size() < m_defaultBlockSize)
                {
                    // Found matching archetype
                    return &m_archetypes[i];
                }
            }
            return nullptr;
        }
    public:
        ArchetypeManager(int defaultBlockSize) :
            m_defaultBlockSize(defaultBlockSize)
        {
        }

        ~ArchetypeManager() {}

    private:
        AllocateInstance(int componentMask)
        {
            // First find which archetype matches our entities component usage 
            ArchetypeBlock* pArchetype = FindMatchingArchetype(componentMask);
            if (pArchetype == nullptr)
            {
                // Create new archetype block
                m_archetypes.push_back(ArchetypeBlock(componentMask, defaultBlockSize));
                pArchetype = &m_archetypes.back();
            }
            pArchetype->AddInstance();
        }
        bool CreateArchetype(int componentMask)
        {
            auto it = m_entityMapping.find(entityID);
            if (it != m_entityMapping.end())
            {
                const ArchetypeBlockKey& key = it->second;
                ArchetypeBlock& block = m_archetypes[key.block];
                // Add component to chunk
                // Update component mask
                // Reorganize chunk data if necessary
            }
            else
            {
                // Create new chunk for entity with this component
            }
        }

        template<typename T>
        T* GetComponent(EntityID entityID, ComponentID componentID)
        {
            auto it = m_entityMapping.find(entityID);
            if (it != m_entityMapping.end())
            {
                const ArchetypeBlockKey& key = it->second;
                ArchetypeBlock& block = m_archetypes[key.block];
                // Get the offset for the requested component
                auto offsetIt = block.m_componentOffsets.find(componentID);
                if (offsetIt != block.m_componentOffsets.end())
                {
                    size_t offset = offsetIt->second;
                    // Get the base pointer to the entity's data in the block
                    byte* pData = reinterpret_cast<byte*>(/* get entity data pointer in block, e.g. */ block.m_spComponentData.get() + key.id);
                    return reinterpret_cast<T*>(pData + offset);
                }
            }
            return nullptr;
        }
    };
}
