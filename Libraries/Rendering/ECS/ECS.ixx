//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
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
	// Class: Entity
	// Defines an Entity in our ECS Framework
	//**********************************************************************
	struct Entity : public IEntity, public CRefCount
	{
		std::vector<Component*> m_components;
	public:
		Entity()
		{
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
		virtual uint32_t Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IEntity
		//**********************************************************************
		virtual Component* HasComponent(ComponentID componentID) { return nullptr; };
	};

	//**********************************************************************
	// Class: System
	// Defines an System in our ECS Framework. Systems are where the code
	// exists.
	//**********************************************************************
	class System : public ISystem, public CRefCount
	{
		SystemID m_systemID;
		std::string m_systemName;
	public:
		System() : m_systemID(0)
		{
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
		virtual uint32_t Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ISystem
		//**********************************************************************
		virtual std::string GetSystemName() override { return m_systemName; }
	};
}