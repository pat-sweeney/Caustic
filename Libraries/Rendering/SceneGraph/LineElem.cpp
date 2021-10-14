//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "LineElem.h"

namespace Caustic
{
    CRefObj<ISceneLineElem> CreateLineElem(Vector3& p0, Vector3& p1)
    {
        return CRefObj<ISceneLineElem>(new CSceneLineElem(p0, p1));
    }
};
