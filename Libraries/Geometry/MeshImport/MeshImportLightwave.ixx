//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>

export module Geometry.MeshImport.Lightwave;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.Mesh;
import Geometry.Mesh.IMesh;

export namespace Caustic
{
    namespace MeshImport
    {
        CRefObj<IMesh> LoadLightwave(const wchar_t* pFilename);
    }
}
