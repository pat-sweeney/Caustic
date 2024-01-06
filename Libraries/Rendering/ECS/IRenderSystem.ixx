//**********************************************************************
// Copyright Patrick Sweeney 2023-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>

export module Rendering.ECS.IRenderSystem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.ECS.ECS;
import Rendering.ECS.IECS;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.ILight;

export namespace Caustic
{
    //**********************************************************************
    // Class: IRenderSystem
    // Defines the interface to the RenderSystem. The RenderSystem handles
    // rendering of entities.
    //**********************************************************************
    struct IRenderSystem
    {
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx) = 0;
    };
}
