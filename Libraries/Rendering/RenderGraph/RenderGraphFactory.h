//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Ray.h"
#include "Rendering\Caustic\Caustic.h"
#include "Cameras\CameraBase\ICamera.h"
#include "IRenderGraph.h"
#include "IRenderGraphFactory.h"
#include "IRGNMaterial.h"
#include "IRGNPhongMaterial.h"
#include <vector>
#include <string>
#include <any>
#include <map>

namespace Caustic
{
    //**********************************************************************
    // Class: CRenderGraphFactory
    // Implements <IRenderGraphFactory>
    //**********************************************************************
    class CRenderGraphFactory : public IRenderGraphFactory, public CRefCount
    {
    public:
        CRenderGraphFactory()
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraphFactory
        //**********************************************************************
        virtual CRefObj<IRenderGraph> CreateRenderGraph() override;
        virtual CRefObj<IRenderGraphNode_Compute> CreateComputeNode(IShader* pShader) override;
        virtual CRefObj<IRenderGraphNode_Mesh> CreateMeshNode() override;
        virtual CRefObj<IRenderGraphNode_LightCollection> CreateLightCollectionNode() override;
        virtual CRefObj<IRenderGraphNode_Group> CreateGroupNode() override;
        virtual CRefObj<IRenderGraphNode_Material> CreateMaterialNode(IShader* pShader) override;
        virtual CRefObj<IRenderGraphNode_PhongMaterial> CreatePhongMaterialNode() override;
        virtual CRefObj<IRenderGraphNode_DepthCameraSource> CreateDepthCameraSource(IDepthCameraDevice* pDevice) override;
        virtual CRefObj<IRenderGraphNode_ImageToTexture> CreateImageToTextureNode() override;
    };
}
