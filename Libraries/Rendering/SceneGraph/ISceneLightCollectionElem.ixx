//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneLightCollectionElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.ILight;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneGroupElem;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISceneLightCollectionElem
    // Defines a collection of lights. These lights only effect the children of this group
    //**********************************************************************
    struct ISceneLightCollectionElem : public ISceneGroupElem
    {
        //**********************************************************************
        // Method: AddLight
        // Adds a light to the collection
        //**********************************************************************
        virtual void AddLight(ILight* pLight) = 0;

        //**********************************************************************
        // Method: RemoveLight
        // Removes a light from the collection
        //**********************************************************************
        virtual void RemoveLight(ILight* pLight) = 0;

        //**********************************************************************
        // Method: NumberLights
        // Returns the number of lights in the collection
        //**********************************************************************
        virtual uint32 NumberLights() = 0;

        //**********************************************************************
        // Method: GetLight
        // Returns the Nth light from the collection
        //**********************************************************************
        virtual CRefObj<ILight> GetLight(int index) = 0;
    };

    CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem();
}
