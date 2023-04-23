//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>

module Rendering.ECS.Hierarchy;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;

namespace Caustic
{
	void HierarchySystem::SetParent(IEntity* pParent)
	{
		m_spParent = pParent;
	}
}
