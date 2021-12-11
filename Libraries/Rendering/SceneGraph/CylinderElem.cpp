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
import Rendering.SceneGraph.CylinderElem;
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius)
    {
        return CRefObj<ISceneCylinderElem>(new CSceneCylinderElem(center, height, topRadius, bottomRadius));
    }
};
