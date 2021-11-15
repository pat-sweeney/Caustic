//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "SphereElem.h"
import Rendering.Caustic.CausticFactory;

namespace Caustic
{
    CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius)
    {
        return CRefObj<ISceneSphereElem>(new CSceneSphereElem(center, radius));
    }
};
