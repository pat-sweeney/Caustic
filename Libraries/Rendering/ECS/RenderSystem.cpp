//**********************************************************************
// Copyright Patrick Sweeney 2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>
#include <d3d11_4.h>

module Rendering.ECS.RenderSystem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;
import Rendering.ECS.BaseComponent;
import Rendering.ECS.MaterialComponent;
import Rendering.ECS.MeshComponent;
import Rendering.ECS.HierarchyComponent;
import Base.Core.ISerialize;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Geometry.Mesh.IMesh;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ILight;

namespace Caustic
{
    void RenderSystem::DrawSelected(IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
    }
    
    void RenderSystem::RenderMesh(IEntity* pEntity, MeshComponent *pMeshComponent, IShader *pShader, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        BaseComponent* pBaseComponent = (BaseComponent*)pEntity->HasComponent(c_BaseComponent);
        if (pBaseComponent->m_flags & c_EntityFlag_Selected)
            DrawSelected(pRenderer, pRenderCtx);
        if (!(pBaseComponent->m_passes & pRenderCtx->GetCurrentPass()))
            return;
        if (pBaseComponent->m_flags & c_EntityFlag_RenderableDirty)
        {
            pMeshComponent->m_spRenderMesh = pRenderer->ToRenderMesh(pMeshComponent->m_spMesh, pShader);
            pBaseComponent->m_flags &= ~c_EntityFlag_RenderableDirty;
        }
        bool oldDepthState = false;
        if (!(pBaseComponent->m_flags & c_EntityFlag_DepthTested))
            oldDepthState = pRenderer->EnableDepthTest(false);
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext4> spCtx;
        std::wstring entityName = pBaseComponent->m_name;
        if (!entityName.empty())
        {
            pRenderer->GetContext()->QueryInterface<ID3D11DeviceContext4>(&spCtx);
            spCtx->BeginEventInt(entityName.c_str(), 0);
        }
#endif
        std::vector<CRefObj<ILight>> lights;
        pMeshComponent->m_spRenderMesh->Render(pRenderer, pRenderCtx, (IShader*)nullptr, (IRenderMaterial*)nullptr, lights, &pBaseComponent->m_xm);
#ifdef _DEBUG
        if (spCtx)
            spCtx->EndEvent();
#endif
        if (!(pBaseComponent->m_flags & c_EntityFlag_DepthTested))
            pRenderer->EnableDepthTest(oldDepthState);
    }

    void RenderSystem::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        m_spHierarchySystem->Traverse(
            [this, pRenderer, pRenderCtx](IEntity* pEntity) -> void
            {
                MeshComponent *pMeshComponent = (MeshComponent*)pEntity->HasComponent(c_MeshComponent);
                if (pMeshComponent != nullptr)
                {
                    // Check if mesh entity has a material. If so, render it
                    MaterialComponent *pMaterialComponent = (MaterialComponent*)pEntity->HasComponent(c_MaterialComponent);
                    IShader* pShader = (pMaterialComponent != nullptr) ? pMaterialComponent->m_spShader : nullptr;
                    if (pShader == nullptr)
                        pShader = pRenderer->GetShaderMgr()->FindShader(L"Default");
                    if (pMaterialComponent->m_flags & MaterialComponent::c_Flag_MaterialDirty)
                    {
                        pRenderer->ToRenderMaterials(pMeshComponent->m_spMesh, pShader, pMeshComponent->m_spRenderMesh, pMaterialComponent->m_spMaterial);
                        pMaterialComponent->m_flags = pMaterialComponent->m_flags & ~MaterialComponent::c_Flag_MaterialDirty;
                    }
                    RenderMesh(pEntity, pMeshComponent, pMaterialComponent->m_spShader, pRenderer, pRenderCtx);
                }
            });
    }
}
