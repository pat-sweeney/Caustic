//**********************************************************************
// Copyright Patrick Sweeney 2022-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneLevelOfDetailElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.SceneLevelOfDetailElem;

namespace Caustic
{
    CRefObj<ISceneLevelOfDetailElem> CreateLevelOfDetailElem()
    {
        return CRefObj<ISceneLevelOfDetailElem>(new CSceneLevelOfDetailElem());
    }
}
