//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Cameras.CameraBase.ICamera;
import Geometry.Mesh.IMesh;
#include "Rendering\RenderGraph\IRenderGraphNode.h"
#include "Rendering\RenderGraph\IRGNCompute.h"
#include "Rendering\RenderGraph\IRGNMesh.h"
#include "Rendering\RenderGraph\IRGNLightCollection.h"
#include "Rendering\RenderGraph\IRGNGroup.h"
#include "Rendering\RenderGraph\IRGNMaterial.h"
#include "Rendering\RenderGraph\IRGNPhongMaterial.h"
#include "Rendering\RenderGraph\IRGNDepthCameraSource.h"
#include "Rendering\RenderGraph\IRGNImageToTexture.h"

namespace Caustic
{
    //**********************************************************************
    // Interface: IRenderGraphFactory
    // Interface to object used to create <IRenderGraphNode> objects
    //**********************************************************************
    struct IRenderGraphFactory : public IRefCount
    {
        //**********************************************************************
        // Method: CreateRenderGraph
        // Creates a new render graph
        //**********************************************************************
        virtual CRefObj<IRenderGraph> CreateRenderGraph() = 0;

        //**********************************************************************
        // Method: CreateComputeNode
        // Creates a new compute node.
        //
        // --- Code
        // A compute node exposes the an arbitrary number of input and outpu buffers
        // based on what is found in the shader:
        //                     Compute
        //                  +---------+    
        // <bufferName> o---|         |---o <bufferName>
        //    ...       o---|         |---o ...
        // <bufferName> o---|         |---o <bufferName>
        //                  +---------+    
        // ---
        //
        // Parameters:
        // pShader - pointer to the compute shader
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode_Compute> CreateComputeNode(IShader* pShader) = 0;

        //**********************************************************************
        // Method: CreateMeshNode
        // Creates a new mesh node.
        //
        // --- Code
        // A mesh node exposes the following pins:
        //                       Mesh
        //                   +---------+    
        //          mesh o---|         |---o render
        // frontMaterial o---|         |
        //  backMaterial o---|         |
        //        shader o---|         |
        //        lights o---|         |
        //                   +---------+    
        // ---
        //
        // Parameters:
        // pMesh - Default mesh
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode_Mesh> CreateMeshNode(CRefObj<IMesh> spMesh) = 0;
        virtual CRefObj<IRenderGraphNode_Mesh> CreateMeshNode() = 0;

        //**********************************************************************
        // Method: CreateLightCollectionNode
        // Creates a new light collection node.
        //
        //
        // --- Code
        // A light collection node exposes the following pins:
        //   LightCollection
        //  +---------+    
        //  |         |---o lights
        //  +---------+    
        // ---
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode_LightCollection> CreateLightCollectionNode() = 0;

        //**********************************************************************
        // Method: CreateGroupNode
        // Creates a new group node.
        // --- Code
        // A group node exposes a single "render" pin as output. It exposes all
        // it's children node's inputs as its own inputs. This way you can wire
        // a single input that will be connected to all the children accepting
        // that input without having to explicitly connect up all the nodes.
        //
        //                       Group
        //                   +---------+    
        //  <inner pins> o---|         |---o render
        //      ...      o---|         |
        //  <inner pins> o---|         |
        //                   +---------+    
        // ---
        //
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode_Group> CreateGroupNode() = 0;

        //**********************************************************************
        // Method: CreateMaterialNode
        // Creates a new material node.
        //
        // --- Code
        // A material node exposes the following pins:
        //                      Material
        //                    +---------+
        //           mesh o---|         |---o material
        // materialAttrib o---|         |
        //         shader o---|         |
        //                    +---------+
        // ---
        //
        // Parameters:
        // pShader - shader that material will use
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode_Material> CreateMaterialNode() = 0;

        //**********************************************************************
        // Method: CreatePhongMaterialNode
        // Creates a new Phong lighting model material node.
        //
        // --- Code
        // A Phong material node exposes the following pins:
        //                      Phong Material
        //                     +---------+    
        //    diffuseColor o---|         |---o material
        //   specularColor o---|         |
        //   specularPower o---|         |
        //    ambientColor o---|         |
        //  diffuseTexture o---|         |
        // specularTexture o---|         |
        //  ambientTexture o---|         |
        //                     +---------+    
        // ---
        //
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode_PhongMaterial> CreatePhongMaterialNode() = 0;

        //**********************************************************************
        // Method: CreateDepthCameraSource
        // Creates a new depth camera that delivers images
        //
        // --- Code
        // A DepthCameraSource node exposes the following pins:
        //     DepthCameraSource
        //   +---------+    
        //   |         |---o depthImage
        //   |         |---o colorImage
        //   |         |---o irImage
        //   +---------+    
        // ---
        //
        // Parameters:
        // pDevice - Underlying depth camera to use
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode_DepthCameraSource> CreateDepthCameraSource(IDepthCameraDevice* pDevice) = 0;

        //**********************************************************************
        // Method: CreateImageToTextureNode
        // Creates a render graph node that accepts an <IImage> as input and outputs
        // and <ITexture>
        //
        // --- Code
        // A ImageToTexture node exposes the following pins:
        //             ImageToTexture
        //           +---------+    
        // image o---|         |---o texture
        //           +---------+    
        // ---
        //
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode_ImageToTexture> CreateImageToTextureNode() = 0;
    };
    CRefObj<IRenderGraphFactory> CreateRenderGraphFactory();
}
