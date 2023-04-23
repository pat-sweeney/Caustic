//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <vector>

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
	// Defines an Component in our ECS Framework. Components are simple buckets
	// of data. They contain no code.
	//**********************************************************************
	struct Component
	{
		uint32 m_componentID;
	};

	//**********************************************************************
	// Class: Entity
	// Defines an Entity in our ECS Framework
	//**********************************************************************
	struct Entity : public IEntity, public CRefCount
	{
		uint32 m_objectID;
		std::string m_name;
		std::vector<Component> m_components;
		static inline uint32 s_nextObjID = 0;
	public:
		Entity() : m_objectID(s_nextObjID++)
		{
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IEntity
		//**********************************************************************
		virtual std::string GetEntityName() override { return m_name; }
		virtual bool HasComponent(uint32 componentID) { return false; };
	};

	//**********************************************************************
	// Class: System
	// Defines an System in our ECS Framework. Systems are where the code
	// exists.
	//**********************************************************************
	class System : public ISystem, public CRefCount
	{
		uint32 m_systemID;
		std::string m_systemName;
	public:
		System() : m_systemID(0)
		{
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ISystem
		//**********************************************************************
		virtual std::string GetSystemName() override { return m_systemName; }
	};
}