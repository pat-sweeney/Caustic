//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include <vector>
#include <string>
#include <any>
#include <atlbase.h>
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Cameras.CameraBase.ICamera;
import Rendering.Caustic.IRenderCtx;
#include "IRGNDepthCameraSource.h"
#include "RGNDepthCameraSource.h"

namespace Caustic
{
    CRenderGraphNode_DepthCameraSource::CRenderGraphNode_DepthCameraSource(IDepthCameraDevice* pDepthCamera)
    {
        std::any empty;
        m_spDepthCamera = pDepthCamera;
        m_spColorImagePin = CreatePin(this, false, "colorImage", ERenderGraphDataType::Image, empty);
        m_spDepthImagePin = CreatePin(this, false, "depthImage", ERenderGraphDataType::Image, empty);
        m_spIRImagePin = CreatePin(this, false, "irImage", ERenderGraphDataType::Image, empty);
    }

    std::any CRenderGraphNode_DepthCameraSource::GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        // Check if new images are available
        CRefObj<IImage> spColorImage;
        CRefObj<IImage> spDepthImage;
        CRefObj<IImage> spIRImage;
        if (m_spDepthCamera->NextFrame(&spColorImage, &spDepthImage, &spIRImage))
        {
            m_spColorImage = spColorImage;
            m_spDepthImage = spDepthImage;
            m_spIRImage = spIRImage;
            m_lastEpochModified = pRenderCtx->GetEpoch();
            pRenderCtx->SetMostRecentEpoch(m_lastEpochModified);
        }
        if (pPin == m_spColorImagePin)
            return std::any(m_spColorImage);
        if (pPin == m_spDepthImagePin)
            return std::any(m_spColorImage);
        if (pPin == m_spIRImagePin)
            return std::any(m_spIRImage);
        return std::any(nullptr);
        // if (spDepthImage)
        // {
        //     if (numVertices == 0)
        //     {
        //         numVertices = spDepthImage->GetWidth() * spDepthImage->GetHeight();
        //         vertices.reset(new Vertex[numVertices]);
        //     }
        //     OutputDebugString(L"Read frame\n");
        //     uint32 depthWidth2 = spDepthImage->GetWidth() / 2; // We need half the width since the shader does 2 depth points at a time
        //     pComputeElem->SetShaderParam(L"depthImageWidth", depthWidth2);
        //     pComputeElem->SetInputBuffer(L"DepthBuffer", spDepthImage->GetData(), spDepthImage->GetStride() * spDepthImage->GetHeight(), spDepthImage->GetBytesPerPixel() * 2);
        //     pComputeElem->SetInputBuffer(L"RayBuffer", spRayMap->GetData(), spRayMap->GetStride() * spRayMap->GetHeight(), spRayMap->GetBytesPerPixel());
        //     pComputeElem->SetInputThreads(depthWidth2, spDepthImage->GetHeight());
        //     pComputeElem->SetOutputBuffer(L"Points", (uint8*)vertices.get(), (uint32)(sizeof(Vertex) * numVertices), (uint32)sizeof(Vertex));
        // }
    }
}
