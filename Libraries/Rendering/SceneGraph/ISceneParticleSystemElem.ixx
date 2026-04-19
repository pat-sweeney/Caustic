//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <string>

export module Rendering.SceneGraph.ISceneParticleSystemElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.SceneGraph.ISceneElem;
import Rendering.Caustic.IParticleSystem;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISceneParticleSystemElem
    // Defines a particle system element in the scene graph.
    //**********************************************************************
    struct ISceneParticleSystemElem : public ISceneElem
    {
        //**********************************************************************
        // Method: GetParticleSystem
        // Returns the underlying particle system.
        //**********************************************************************
        virtual CRefObj<IParticleSystem> GetParticleSystem() = 0;
    };

    CRefObj<ISceneParticleSystemElem> CreateSceneParticleSystemElem(IParticleSystem* pParticleSystem);
}
