//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Rendering\SceneGraph\ISceneGraph.h"
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.SceneGraph.CubeElem;
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CRefObj<ISceneCubeElem> CreateCubeElem(Vector3 &center, float width, float height, float depth)
    {
        return CRefObj<ISceneCubeElem>(new CSceneCubeElem(center, width, height, depth));
    }
};
