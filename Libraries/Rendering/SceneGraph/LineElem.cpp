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
import Rendering.SceneGraph.LineElem;
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CRefObj<ISceneLineElem> CreateLineElem(Vector3& p0, Vector3& p1)
    {
        return CRefObj<ISceneLineElem>(new CSceneLineElem(p0, p1));
    }
};
