//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Error;
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "DesktopTexture.h"
#include <memory>
#include <objbase.h>
#include <assert.h>
#include <dxgi1_6.h>
#include <functional>
#include <string>

namespace Caustic
{
    //**********************************************************************
    CDesktopTexture::CDesktopTexture(IRenderer* pRenderer)
    {
        ZeroMemory(&m_desc, sizeof(m_desc));
    }

    //**********************************************************************
    CDesktopTexture::~CDesktopTexture()
    {
    }

    //**********************************************************************
    // Method: GetWidth
    // Returns the width of the texture
    //**********************************************************************
    uint32 CDesktopTexture::GetWidth()
    {
        return m_desc.Width;
    }

    //**********************************************************************
    // Method: GetHeight
    // Returns the height of the texture
    //**********************************************************************
    uint32 CDesktopTexture::GetHeight()
    {
        return m_desc.Height;
    }

    //**********************************************************************
    // Method: Update
    // Updates the underlying texture
    //**********************************************************************
    void CDesktopTexture::Update(IRenderer* pRenderer)
    {
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        CComPtr<IDXGIResource> spResource;
        auto spDuplication = pRenderer->GetDuplication();
        if (spDuplication)
        {
            HRESULT hr = spDuplication->AcquireNextFrame(0, &frameInfo, &spResource);
            if (hr == S_OK)
            {
                CComPtr<ID3D11Texture2D> spTexture;
                CT(spResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&spTexture)));

                spTexture->GetDesc(&m_desc);
                if (m_spDesktopCopy == nullptr)
                    m_spDesktopCopy = Caustic::CreateTexture(pRenderer, m_desc.Width, m_desc.Height, m_desc.Format);
                pRenderer->GetContext()->CopyResource(m_spDesktopCopy->GetD3DTexture(), spTexture);
                CT(spDuplication->ReleaseFrame());
            }
        }
        else
        {
            auto spTex = Caustic::CheckerboardTexture(pRenderer, 1024, 1024, 32);
            if (m_spDesktopCopy == nullptr)
                m_spDesktopCopy = Caustic::CreateTexture(pRenderer, spTex->GetWidth(), spTex->GetHeight(), spTex->GetFormat());
            pRenderer->GetContext()->CopyResource(m_spDesktopCopy->GetD3DTexture(), spTex->GetD3DTexture());
        }
    }

    //**********************************************************************
    void CDesktopTexture::Render(IRenderer* pRenderer, int slot, bool isPixelShader)
    {
        if (m_spDesktopCopy)
            m_spDesktopCopy->Render(pRenderer, slot, isPixelShader);
    }
    
    //**********************************************************************
    void CDesktopTexture::CopyFromImage(IRenderer* pRenderer, IImage* pImage, bool generateMipMap /* = false */)
    {
        if (m_spDesktopCopy)
            m_spDesktopCopy->CopyFromImage(pRenderer, pImage, generateMipMap);
    }

    //**********************************************************************
    CRefObj<IImage> CDesktopTexture::CopyToImage(IRenderer* pRenderer)
    {
        if (m_spDesktopCopy)
            return m_spDesktopCopy->CopyToImage(pRenderer);
        else
            return CRefObj<IImage>(nullptr);
    }

    void CDesktopTexture::CopyToImage(IRenderer* pRenderer, IImage* pImage)
    {
        if (m_spDesktopCopy)
            m_spDesktopCopy->CopyToImage(pRenderer, pImage);
    }

    //**********************************************************************
    // Function: CreateDesktopTexture
    // Creates a texture that contains the windows desktop
    //
    // Parameters:
    // pRenderer - Renderer
    //
    // Returns:
    // Returns the new texture
    //**********************************************************************
    CRefObj<ITexture> CreateDesktopTexture(IRenderer* pRenderer)
    {
        std::unique_ptr<CDesktopTexture> spTexture(new CDesktopTexture(pRenderer));
        return CRefObj<ITexture>(spTexture.release());
    }
}
