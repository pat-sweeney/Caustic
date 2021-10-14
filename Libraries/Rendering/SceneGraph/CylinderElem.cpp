//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "CylinderElem.h"

namespace Caustic
{
    CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius)
    {
        return CRefObj<ISceneCylinderElem>(new CSceneCylinderElem(center, height, topRadius, bottomRadius));
    }
};
