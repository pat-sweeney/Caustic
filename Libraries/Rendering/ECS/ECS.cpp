//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <vector>

module Rendering.ECS.ECS;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.BaseComponent;

namespace Caustic
{
    CRefObj<IEntity> CreateEntity(std::wstring name)
    {
        auto spEntity = CRefObj((IEntity*)new Entity());
        BaseComponent *pBaseComponent = (BaseComponent*)spEntity->HasComponent(c_BaseComponent);
        pBaseComponent->m_name = name;
        return spEntity;
    }
}