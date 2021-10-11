//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "RenderGraphFactory.h"
#include "RenderGraph.h"
#include "RGNMesh.h"
#include "RGNCompute.h"
#include "RGNMesh.h"
#include "RGNLightCollection.h"
#include "RGNGroup.h"
#include "RGNMaterial.h"
#include "RGNPhongMaterial.h"
#include "RGNDepthCameraSource.h"
#include "RGNIMageToTexture.h"

namespace Caustic
{
    //**********************************************************************
    // Function: CreateRenderGraphFactory
    // Creates a new RenderGraph factory. This factory is used to create nodes
    // that will be added to the render graph
    //
    // Returns:
    // New render factory object
    //**********************************************************************
    CAUSTICAPI CRefObj<IRenderGraphFactory> CreateRenderGraphFactory()
    {
        return CRefObj<IRenderGraphFactory>(new CRenderGraphFactory());
    }

    //**********************************************************************
    // Function: CreateRenderGraph
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraph> CRenderGraphFactory::CreateRenderGraph()
    {
        return CRefObj<IRenderGraph>(new CRenderGraph());
    }

    //**********************************************************************
    // Function: CreateComputeNode
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_Compute> CRenderGraphFactory::CreateComputeNode(IShader* pShader)
    {
        return CRefObj<IRenderGraphNode_Compute>(new CRenderGraphNode_Compute(pShader));
    }

    //**********************************************************************
    // Function: CreateMeshNode
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_Mesh> CRenderGraphFactory::CreateMeshNode(CRefObj<IMesh> spMesh)
    {
        auto spMeshNode = CRefObj<IRenderGraphNode_Mesh>(new CRenderGraphNode_Mesh());
        if (spMesh)
            spMeshNode->FindInputPin("mesh")->SetDefaultValue(spMesh);
        return spMeshNode;
    }

    //**********************************************************************
    // Function: CreateMeshNode
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_Mesh> CRenderGraphFactory::CreateMeshNode()
    {
        return CRefObj<IRenderGraphNode_Mesh>(new CRenderGraphNode_Mesh());
    }

    //**********************************************************************
    // Function: CreateLightCollectionNode
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_LightCollection> CRenderGraphFactory::CreateLightCollectionNode()
    {
        return CRefObj<IRenderGraphNode_LightCollection>(new CRenderGraphNode_LightCollection());
    }
    
    //**********************************************************************
    // Function: CreateGroupNode
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_Group> CRenderGraphFactory::CreateGroupNode()
    {
        return CRefObj<IRenderGraphNode_Group>(new CRenderGraphNode_Group());
    }

    //**********************************************************************
    // Function: CreateMaterialNode
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_Material> CRenderGraphFactory::CreateMaterialNode()
    {
        return CRefObj<IRenderGraphNode_Material>(new CRenderGraphNode_Material());
    }

    //**********************************************************************
    // Function: CreatePhongMaterialNode
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_PhongMaterial> CRenderGraphFactory::CreatePhongMaterialNode()
    {
        return CRefObj<IRenderGraphNode_PhongMaterial>(new CRenderGraphNode_PhongMaterial());
    }

    //**********************************************************************
    // Function: CreateDepthCameraSource
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_DepthCameraSource> CRenderGraphFactory::CreateDepthCameraSource(IDepthCameraDevice *pDevice)
    {
        return CRefObj<IRenderGraphNode_DepthCameraSource>(new CRenderGraphNode_DepthCameraSource(pDevice));
    }
    
    //**********************************************************************
    // Function: CreateImageToTextureNode
    // See <IRenderGraphFactory>
    //**********************************************************************
    CRefObj<IRenderGraphNode_ImageToTexture> CRenderGraphFactory::CreateImageToTextureNode()
    {
        return CRefObj<IRenderGraphNode_ImageToTexture>(new CRenderGraphNode_ImageToTexture());
    }
}
