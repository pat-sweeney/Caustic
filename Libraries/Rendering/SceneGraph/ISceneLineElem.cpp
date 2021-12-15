//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneLineElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.SceneGraph.SceneLineElem;

namespace Caustic
{
    CRefObj<ISceneLineElem> CreateLineElem(Vector3& p0, Vector3& p1)
    {
        return CRefObj<ISceneLineElem>(new CSceneLineElem(p0, p1));
    }
}
