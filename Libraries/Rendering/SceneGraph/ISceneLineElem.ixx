//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneLineElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISceneLineElem
    // Defines a line element in our scene graph.
    //**********************************************************************
    struct ISceneLineElem : public ISceneElem
    {
        virtual void SetPosition(Vector3& p0, Vector3& p1) = 0;
        virtual void GetPosition(Vector3* p0, Vector3* p1) = 0;
    };

    CRefObj<ISceneLineElem> CreateLineElem(Vector3& p0, Vector3& p1);
}
