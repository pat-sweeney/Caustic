//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\IRefCount.h"
#include "Rendering\Caustic\ITexture.h"
#include <atlbase.h>

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Interface: ITextureMgr
    // TextureMgr manages textures on the GPU. Textures will generally be kept
    // in backing store on the CPU and only moved to the GPU when required.
    // This interface manages that movement of texture data.
    //**********************************************************************
    struct ITextureMgr : public IRefCount
    {
        virtual CComPtr<ID3D12Resource> Activate(IRenderer *pRenderer, ITexture *pTexture) = 0;
        virtual void Deactivate(ITexture *pTexture) = 0;
    };
}