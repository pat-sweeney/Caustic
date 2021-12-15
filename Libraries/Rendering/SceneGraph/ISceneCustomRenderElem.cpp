//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <functional>

module Rendering.SceneGraph.ISceneCustomRenderElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.SceneGraph.SceneCustomRenderElem;

namespace Caustic
{
    CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)> clientCallback)
    {
        return CRefObj<ISceneCustomRenderElem>(new CSceneCustomRenderElem(clientCallback));
    }
}