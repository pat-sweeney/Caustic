//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"

namespace Caustic
{
    struct ICamera;

    ///**********************************************************************
    /// \brief IGraphics acts as a simple wrapper around our D3D renderer
    ///**********************************************************************
    struct IGraphics : public IRefCount
    {
        virtual CComPtr<ID3D11Device> GetDevice() = 0;
        virtual CComPtr<ID3D11DeviceContext> GetContext() = 0;
        virtual CRefObj<ICamera> GetCamera() = 0;
        virtual void SetCamera(ICamera *pCamera) = 0;
    };
};
