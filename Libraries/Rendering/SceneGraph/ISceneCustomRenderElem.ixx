//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>

export module Rendering.SceneGraph.ISceneCustomRenderElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.SceneCtx;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISceneCustomRenderElem
    // Defines a custom render element in our scene
    //**********************************************************************
    struct ISceneCustomRenderElem : public ISceneElem
    {
        //**********************************************************************
        // Method: SetCallback
        // Updates the callback function associated with the custom render element
        //**********************************************************************
        virtual void SetCallback(std::function<void(IRenderer* pRenderer, IRenderCtx* pCtx, SceneCtx* pSceneCtx)> clientCallback) = 0;
    };

    CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)> clientCallback);
}
