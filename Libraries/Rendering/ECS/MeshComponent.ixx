//**********************************************************************
// Copyright Patrick Sweeney 2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.ECS.MeshComponent;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.IShader;
import Geometry.Mesh.IMesh;

export namespace Caustic
{
    struct MeshComponent : public Component
    {
        CRefObj<IShader> m_spShader;            // Shader m_spRenderMesh was created with
        CRefObj<IMesh> m_spMesh;                // Mesh
        CRefObj<IRenderMesh> m_spRenderMesh;    // Renderable version of m_spMesh
    };
}