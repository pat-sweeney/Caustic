//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "SphereElem.h"

namespace Caustic
{
    CAUSTICAPI CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius)
    {
        return CRefObj<ISceneSphereElem>(new CSceneSphereElem(center, radius));
    }
};
