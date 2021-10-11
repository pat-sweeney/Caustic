//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Cameras\CameraBase\ICamera.h"
#include "IRenderGraphNode.h"
#include "RenderGraphNode.h"
#include "IRGNDepthCameraSource.h"

namespace Caustic
{
    class CRenderGraphNode_DepthCameraSource : public IRenderGraphNode_DepthCameraSource, public CRenderGraphNode, public CRefCount
    {
    protected:
        CRefObj<IDepthCameraDevice> m_spDepthCamera;
        CRefObj<IRenderGraphPin> m_spColorImagePin;
        CRefObj<IRenderGraphPin> m_spDepthImagePin;
        CRefObj<IRenderGraphPin> m_spIRImagePin;
        CRefObj<IImage> m_spColorImage;
        CRefObj<IImage> m_spDepthImage;
        CRefObj<IImage> m_spIRImage;
    public:
        CRenderGraphNode_DepthCameraSource(IDepthCameraDevice* pDepthCamera);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraphNode
        //**********************************************************************
        virtual ERenderGraphNodeType GetType() override { return ERenderGraphNodeType::MeshNode; }
        virtual std::string GetName() override { return m_name; }
        virtual void SetName(std::string name) override { CRenderGraphNode::SetName(name); }
        virtual void SetProperty(std::string name, std::any value) override { CRenderGraphNode::SetProperty(name, value); }
        virtual uint32 NumberInputPins() override { return CRenderGraphNode::NumberInputPins(); }
        virtual uint32 NumberOutputPins() override { return CRenderGraphNode::NumberOutputPins(); }
        virtual CRefObj<IRenderGraphPin> GetInputPin(int index) override { return CRenderGraphNode::GetInputPin(index); }
        virtual CRefObj<IRenderGraphPin> GetOutputPin(int index) override { return CRenderGraphNode::GetOutputPin(index); }
        virtual CRefObj<IRenderGraphPin> FindInputPin(std::string name) override { return CRenderGraphNode::FindInputPin(name); }
        virtual CRefObj<IRenderGraphPin> FindOutputPin(std::string name) override { return CRenderGraphNode::FindOutputPin(name); }
        virtual CRefObj<IRenderGraphNode> GetParent() override { return CRenderGraphNode::GetParent(); }
        virtual void SetParent(IRenderGraphNode* pParent) override { CRenderGraphNode::SetParent(pParent); }
        virtual std::any GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx) override;

        //**********************************************************************
        // IRenderGraphNode_DepthCameraSource
        //**********************************************************************
    };
}
