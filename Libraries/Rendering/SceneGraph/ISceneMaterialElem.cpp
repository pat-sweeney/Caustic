//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>

module Rendering.SceneGraph.ISceneMaterialElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.IMaterialAttrib;
import Geometry.Mesh.MaterialAttrib;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.SceneMaterialElem;

namespace Caustic
{
    CRefObj<ISceneMaterialElem> CreateMaterialElem()
    {
        std::unique_ptr<CSceneMaterialElem> spMaterialElem(new CSceneMaterialElem());
        CRefObj<IMaterialAttrib> spMaterial = Caustic::CreateMaterialAttrib();
        spMaterialElem->SetMaterial(spMaterial);
        return CRefObj<ISceneMaterialElem>(spMaterialElem.release());
    }
}