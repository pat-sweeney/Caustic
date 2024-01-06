//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>

module Rendering.SceneGraph.ISceneMarchingCubesElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IRenderer;
import Rendering.SceneGraph.SceneMarchingCubesElem;

namespace Caustic
{
    CRefObj<ISceneMarchingCubesElem> CreateMarchingCubesElem(IRenderer* pRenderer, uint32_t subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed)
    {
        return CRefObj<ISceneMarchingCubesElem>(new CSceneMarchingCubesElem(pRenderer, subdivisions, sdf, drawIndexed));
    }
}
