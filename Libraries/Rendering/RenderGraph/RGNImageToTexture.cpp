//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "IRenderGraphNode.h"
#include "RGNImageToTexture.h"

namespace Caustic
{
    CRenderGraphNode_ImageToTexture::CRenderGraphNode_ImageToTexture()
    {
        std::any empty;
        m_spImagePin = CreatePin(this, true, "image", ERenderGraphDataType::Image, empty);
        m_spTexturePin = CreatePin(this, false, "texture", ERenderGraphDataType::Texture, empty);
    }
    
    std::any CRenderGraphNode_ImageToTexture::GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        if (pPin == m_spTexturePin)
        {
            CRefObj<IImage> spImage = std::any_cast<CRefObj<IImage>>(m_spImagePin->GetValue(pRenderer, pRenderCtx));
            CRefObj<IGraphics> spGraphics = pRenderer->GetGraphics();
            CRefObj<ITexture> spTexture;
            CRefObj<ICausticFactory> spCausticFactory;
            Caustic::CreateCausticFactory(&spCausticFactory);
            spCausticFactory->CreateTexture(spGraphics, spImage, D3D11_CPU_ACCESS_FLAG(0), D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &spTexture);
            return std::any(spTexture);
        }
        return std::any(nullptr);
    }
}
