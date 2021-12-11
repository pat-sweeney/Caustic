//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "CustomRenderElem.h"
#include <string>
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pRenderCtx, SceneCtx * pSceneCtx)> clientCallback)
    {
        return CRefObj<ISceneCustomRenderElem>(new CSceneCustomRenderElem(clientCallback));
    }
}