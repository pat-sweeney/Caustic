//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneGroupElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.SceneGroupElem;

namespace Caustic
{
    CRefObj<ISceneGroupElem> CreateGroupElem()
    {
        return CRefObj<ISceneGroupElem>(new CSceneGroupElem());
    }
}
