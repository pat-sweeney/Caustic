//**********************************************************************
// Copyright Patrick Sweeney 2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.ECS.MaterialComponent;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.Caustic.IShader;

export namespace Caustic
{
        struct MaterialComponent : public Component
    {
        friend class RenderSystem;
    public:
        static const uint32_t c_Flag_MaterialDirty = 0x1;
        CRefObj<IMaterialAttrib> m_spMaterial;
        CRefObj<IShader> m_spShader;
    };
}