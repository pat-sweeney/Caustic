//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>

export module Rendering.ECS.RenderSystem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;
import Rendering.ECS.IHierarchySystem;
import Rendering.ECS.IRenderSystem;
import Rendering.ECS.MaterialComponent;
import Rendering.ECS.MeshComponent;
import Rendering.ECS.HierarchyComponent;
import Base.Core.ISerialize;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IShader;
import Geometry.Mesh.IMesh;

export namespace Caustic
{
    class RenderSystem : public System, public IRenderSystem
    {
        CRefObj<IHierarchySystem> m_spHierarchySystem;

        void DrawSelected(IRenderer* pRenderer, IRenderCtx* pRenderCtx);
        void RenderMesh(IEntity* pEntity, MeshComponent* pMeshComponent, IShader* pShader, IRenderer* pRenderer, IRenderCtx* pRenderCtx);
    public:
        //**********************************************************************
        // Constructor: RenderSystem
        // Default constructor
        //**********************************************************************
        RenderSystem()
        {
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderSystem
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx) override;
    };
}