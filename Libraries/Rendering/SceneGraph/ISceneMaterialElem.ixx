//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneMaterialElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneGroupElem;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISceneMaterialElem
    // Defines a material object. Materials define the vertex and pixel shader
    // associated with a given scene element.
    //**********************************************************************
    struct ISceneMaterialElem : public ISceneGroupElem
    {
        //**********************************************************************
        // Method: SetShader
        // Sets our shader
        //**********************************************************************
        virtual void SetShader(IShader* pShader) = 0;

        //**********************************************************************
        // Method: GetMaterial
        // Returns the material associated with this element
        //**********************************************************************
        virtual CRefObj<IMaterialAttrib> GetMaterial() = 0;

        //**********************************************************************
        // Method: SetMaterial
        // Sets the underlying material associated with the scene material
        //
        // Parameters:
        // pMaterial - material to associate with this scene material
        //**********************************************************************
        virtual void SetMaterial(IMaterialAttrib* pMaterial) = 0;
    };
    
    CRefObj<ISceneMaterialElem> CreateMaterialElem();
}
