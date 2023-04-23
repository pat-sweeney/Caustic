//**********************************************************************
// Copyright Patrick Sweeney 2023
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
    //**********************************************************************
    // Class: ISystem
    // Defines the interface to the System in our ECS framework.
    //**********************************************************************
    struct ISystem : public IRefCount
    {
        virtual std::string GetSystemName() = 0;
    };

    //**********************************************************************
    // Class: IEntity
    // Defines the interface to the Entity in our ECS framework.
    //**********************************************************************
    struct IEntity : public IRefCount
    {
        virtual std::string GetEntityName() = 0;
        virtual bool HasComponent(uint32 componentID) = 0;
    };
}
