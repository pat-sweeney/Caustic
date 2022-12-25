//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneCubeElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISceneCubeElem
    // Defines a cube element in our scene graph.
    //**********************************************************************
    struct ISceneCubeElem : public ISceneElem
    {
        virtual void SetPosition(Vector3& center, float width, float height, float depth) = 0;
        virtual void GetPosition(Vector3* pCenter, float* pWidth, float* pHeight, float* pDepth) = 0;
    };

    CRefObj<ISceneCubeElem> CreateCubeElem(Vector3& center, float width, float height, float depth);
}
