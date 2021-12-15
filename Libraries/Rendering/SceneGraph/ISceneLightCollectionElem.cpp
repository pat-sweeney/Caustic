//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneLightCollectionElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.SceneGraph.SceneLightCollectionElem;

namespace Caustic
{
    CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem()
    {
        return CRefObj<ISceneLightCollectionElem>(new CSceneLightCollectionElem());
    }
}
