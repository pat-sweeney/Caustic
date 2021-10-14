//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "CubeElem.h"

namespace Caustic
{
    CRefObj<ISceneCubeElem> CreateCubeElem(Vector3 &center, float width, float height, float depth)
    {
        return CRefObj<ISceneCubeElem>(new CSceneCubeElem(center, width, height, depth));
    }
};
