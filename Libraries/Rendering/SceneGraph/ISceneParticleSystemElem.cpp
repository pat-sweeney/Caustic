//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneParticleSystemElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.SceneGraph.SceneParticleSystemElem;

namespace Caustic
{
    CRefObj<ISceneParticleSystemElem> CreateSceneParticleSystemElem(IParticleSystem* pParticleSystem)
    {
        return CRefObj<ISceneParticleSystemElem>(new CSceneParticleSystemElem(pParticleSystem));
    }
}
