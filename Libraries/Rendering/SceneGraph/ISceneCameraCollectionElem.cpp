//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneCameraCollectionElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.SceneGraph.SceneCameraCollectionElem;

namespace Caustic
{
    CRefObj<ISceneCameraCollectionElem> CreateCameraCollectionElem()
    {
        return CRefObj<ISceneCameraCollectionElem>(new CSceneCameraCollectionElem());
    }
}
