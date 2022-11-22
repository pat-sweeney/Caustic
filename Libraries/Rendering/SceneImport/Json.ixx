//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <string>
#include <vector>
#include <any>
#include <map>
#include <msxml6.h>

export module Rendering.SceneImport.Json;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Geometry.Mesh.IMeshConstructor;
import Rendering.SceneGraph.ISceneGraph;
import Rendering.SceneGraph.ISceneLightCollectionElem;
import Rendering.Caustic.ITexture;
import Parsers.JSon.IJSonParser;
import Rendering.SceneGraph.ISceneFactory;

export namespace Caustic
{
    struct JsonMaterial
    {
        CRefObj<ITexture> m_spTexture;
        std::string shader;
    };

    class CJsonImporter
    {
        CRefObj<ISceneFactory> m_spSceneFactory;

        void ParseVector3(IJSonObj* pObj, Vector3* pv);
        void ParseFloat(IJSonObj* pObj, float* pv);
        void ParseTransform(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj);
        void ParseSphere(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj);
        void ParseCube(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj);
        void ParseCylinder(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj);
        void ParseNode(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj);
        void ParseMaterial(IJSonObj* pObj, JsonMaterial* pMaterial);
    public:
        CRefObj<ISceneGraph> Import(std::wstring fn);
    };
}
