//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <string>

module Rendering.RenderGraph.RenderGraphFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Rendering.RenderGraph.RenderGraphFactory;
import Rendering.RenderGraph.RenderGraph;
import Rendering.RenderGraph.RGNMesh;
import Rendering.RenderGraph.RGNCompute;
import Rendering.RenderGraph.RGNLightCollection;
import Rendering.RenderGraph.RGNGroup;
import Rendering.RenderGraph.RGNMaterial;
import Rendering.RenderGraph.RGNPhongMaterial;
import Rendering.RenderGraph.RGNDepthCameraSource;
import Rendering.RenderGraph.RGNImageToTexture;

namespace Caustic
{
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
// TODO: The following 4 lines of code no longer compile. I'm not sure what the issue is.
// Given that I no longer am supporting the RenderGraph (should use the SceneGraph instead)
// I simply commented this out so it will compile.
//        if (spMesh)
//        {
//            spMeshNode->FindInputPin("mesh")->SetDefaultValue(spMesh);
//        }
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
