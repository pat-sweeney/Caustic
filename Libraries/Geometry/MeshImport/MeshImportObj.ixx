//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <string>
#include <any>
#include <map>

export module Geometry.MeshImport.Obj;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.Mesh;
import Geometry.Mesh.IMesh;

// Namespace: Caustic
export namespace Caustic
{
    // Namespace: MeshImport
    namespace MeshImport
    {
        //**********************************************************************
        // Function: LoadObj
        // Loads a OBJ model file.
        // 
        // Parameters:
        // pFilename - name of file to load
        // pDefaultMaterialValues - list of default material values
        //
        // Returns:
        // IMesh object
        //**********************************************************************
        CRefObj<IMesh> LoadObj(const wchar_t* pFilename, std::map<std::wstring, std::any>* pDefaultMaterialValues = nullptr);
    }
}
