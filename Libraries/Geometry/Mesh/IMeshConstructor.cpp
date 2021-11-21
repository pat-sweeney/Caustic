//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Geometry.Mesh.IMeshConstructor;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.MeshConstructor;

namespace Caustic
{
    CRefObj<IMeshConstructor> IMeshConstructor::Create()
    {
        return CRefObj<IMeshConstructor>(new CMeshConstructor());
    }
}
