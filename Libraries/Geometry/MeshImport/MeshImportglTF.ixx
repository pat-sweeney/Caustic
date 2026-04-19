//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <string>

export module Geometry.MeshImport.glTF;
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
        // Function: LoadglTF
        // Loads a glTF 2.0 model file (.gltf + .bin or .glb binary).
        // Supports the PBR metallic-roughness material model.
        //
        // Parameters:
        // pFilename - path to the .gltf or .glb file
        //
        // Returns:
        // IMesh object containing all mesh primitives with PBR materials
        //**********************************************************************
        CRefObj<IMesh> LoadglTF(const wchar_t* pFilename);
    }
}
