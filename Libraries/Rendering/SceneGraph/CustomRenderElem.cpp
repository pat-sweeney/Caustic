//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "CustomRenderElem.h"
#include <string>

namespace Caustic
{
    CAUSTICAPI CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pRenderCtx, SceneCtx * pSceneCtx)> clientCallback)
    {
        return CRefObj<ISceneCustomRenderElem>(new CSceneCustomRenderElem(clientCallback));
    }
}