//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneSphereElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.SceneGraph.SceneSphereElem;

namespace Caustic
{
    CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius)
    {
        return CRefObj<ISceneSphereElem>(new CSceneSphereElem(center, radius));
    }
};
