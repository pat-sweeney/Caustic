//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneCameraCollectionElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.ILight;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneGroupElem;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISceneCameraCollectionElem
    // Defines a collection of cameras available for rendering the scene
    //**********************************************************************
    struct ISceneCameraCollectionElem : public ISceneElem
    {
        //**********************************************************************
        // Method: AddCamera
        // Adds a camera to the collection
        //**********************************************************************
        virtual void AddCamera(ICamera* pLight) = 0;

        //**********************************************************************
        // Method: RemoveCamera
        // Removes a camera from the collection
        //**********************************************************************
        virtual void RemoveCamera(ICamera* pLight) = 0;

        //**********************************************************************
        // Method: NumberCameras
        // Returns the number of cameras in the collection
        //**********************************************************************
        virtual uint32_t NumberCameras() = 0;

        //**********************************************************************
        // Method: GetCamera
        // Returns the Nth camera from the collection
        //**********************************************************************
        virtual CRefObj<ICamera> GetCamera(int index) = 0;
    };

    CRefObj<ISceneCameraCollectionElem> CreateCameraCollectionElem();
}
