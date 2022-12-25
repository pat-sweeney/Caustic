//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneMeshElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Geometry.Mesh.IMesh;
import Rendering.Caustic.IShader;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISceneMeshElem
    // Defines a single mesh in our scene
    //**********************************************************************
    struct ISceneMeshElem : public ISceneElem
    {
        //**********************************************************************
        // Method: SetMesh
        // Updates the mesh elements underlying mesh object
        //**********************************************************************
        virtual void SetMesh(IMesh* pMesh) = 0;

        //**********************************************************************
        // Method: GetMesh
        // Returns the underlying mesh object
        //**********************************************************************
        virtual CRefObj<IMesh> GetMesh() = 0;

        //**********************************************************************
        // Method: SetShader
        // Assigns shader used for rendering this mesh
        //**********************************************************************
        virtual void SetShader(IShader* pShader) = 0;
    };
    
    CRefObj<ISceneMeshElem> CreateMeshElem();
}
