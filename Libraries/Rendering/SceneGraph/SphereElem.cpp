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
import Rendering.SceneGraph.SphereElem;
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius)
    {
        return CRefObj<ISceneSphereElem>(new CSceneSphereElem(center, radius));
    }
};
