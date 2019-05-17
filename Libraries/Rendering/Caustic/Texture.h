//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include <d3d12.h>

namespace Caustic
{
    class CTexture :
        public ITexture,
        public CRefCount
    {
    protected:
        uint32 m_Width;
        uint32 m_Height;
        DXGI_FORMAT m_Format;
        CComPtr<ID3D12Resource> m_spTexture;
    public:
		//**********************************************************************
		// Constructor: CTexture
		// Defines our texture object
		//
		// Paramters:
		// pRenderer - Rendering device
		// width - Width in pixels of image
		// height - Height in pixels of image
		// format - Defines the pixel format for the image
		//**********************************************************************
		CTexture(IRenderer *pRenderer, uint32 width, uint32 height, DXGI_FORMAT format);

		//**********************************************************************
		// Destructor: CTexture
		//**********************************************************************
		~CTexture();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ITexture
        //**********************************************************************
		virtual uint32 GetWidth() override;
        virtual uint32 GetHeight() override;
        virtual DXGI_FORMAT GetFormat() override;
        virtual void Update(IRenderer * /*pRenderer*/) override {}
        virtual CComPtr<ID3D12Resource> GetD3DTexture() override { return m_spTexture; }
        virtual void GenerateMips(IRenderer *pRenderer) override;
    };
};
