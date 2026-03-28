//**********************************************************************
// Copyright Patrick Sweeney 2015-2025
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <memory>
#include <atlbase.h>
#include <d3d11.h>
#include <string>

module Rendering.Caustic.IRenderer;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Rendering.Caustic.Renderer;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.ICamera;

namespace Caustic
{
    CRefObj<IRenderer> CreateRenderer(HWND hwnd, BBox2& viewport, std::wstring& shaderFolder, bool startFrozen, int desktopIndex)
    {
        std::unique_ptr<CRenderer> spRenderer(new CRenderer());
        spRenderer->Setup(hwnd, viewport, shaderFolder, true, startFrozen, desktopIndex);
        CRefObj<ICamera> spCamera = CCausticFactory::Instance()->CreateCamera(true);
        spRenderer->SetCamera(spCamera);
        return CRefObj<IRenderer>(spRenderer.release());
    }
}
