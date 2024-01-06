//**********************************************************************
// Copyright Patrick Sweeney 2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <DirectXMath.h>
#include <string>

export module Rendering.ECS.BaseComponent;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;

export namespace Caustic
{
    struct BaseComponent : public Component
    {
        std::wstring m_name;
        uint32_t m_flags;
        uint32_t m_passes;
        DirectX::XMMATRIX m_xm;
    };
}