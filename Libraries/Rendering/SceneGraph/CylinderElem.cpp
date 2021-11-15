//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "CylinderElem.h"
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius)
    {
        return CRefObj<ISceneCylinderElem>(new CSceneCylinderElem(center, height, topRadius, bottomRadius));
    }
};
