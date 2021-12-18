//**********************************************************************
// Copyright Patrick Sweeney 2018-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <Windows.h>
#include <functional>

module Rendering.RenderWindow.IRenderWindow;
import Base.Core.Core;
import Base.Core.IRefCount;
import Rendering.RenderWindow.RenderWindow;

namespace Caustic
{
    CRefObj<IRenderWindow> CreateRenderWindow(HWND hwnd, std::wstring& shaderFolder,
        std::function<void(IRenderer*, IRenderCtx*, int)> callback,
        std::function<void(IRenderer*)> prePresentCallback,
        bool startFrozen /* = false */, int desktopIndex /* = 0 */)
    {
        return CRefObj<IRenderWindow>(new CRenderWindow(hwnd, shaderFolder, callback, prePresentCallback, false, startFrozen, desktopIndex));
    }
}
