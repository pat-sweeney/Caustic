//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneMeshElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.SceneGraph.SceneMeshElem;

namespace Caustic
{
    CRefObj<ISceneMeshElem> CreateMeshElem()
    {
        return CRefObj<ISceneMeshElem>(new CSceneMeshElem());
    }
}
