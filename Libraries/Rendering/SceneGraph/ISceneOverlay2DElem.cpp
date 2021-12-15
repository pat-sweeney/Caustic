//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneOverlay2DElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IRenderer;
import Rendering.SceneGraph.SceneOverlay2DElem;

namespace Caustic
{
    CRefObj<ISceneOverlay2DElem> CreateOverlay2DElem(IShader* pShader /* = nullptr */)
    {
        return CRefObj<ISceneOverlay2DElem>(new CSceneOverlay2DElem(pShader));
    }
};
