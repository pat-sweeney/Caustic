//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <msxml6.h>
#include <atlbase.h>
#include <string>
#include <memory>
#include <any>
#include <map>

module Rendering.SceneImport.Json;
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.CausticFactory;
import Rendering.SceneGraph.SceneFactory;
import Rendering.SceneGraph.SceneGraph;
import Parsers.JSon.IJSonParser;
import Base.Core.ConvertStr;

using namespace std;

namespace Caustic
{
    //**********************************************************************
    // Method: ParseVector3
    // Parses the definition of a Vector3. A vector is defined as a JSon array
    // containing 3 float elements
    // 
    // Parameters:
    // pObj - JSon object describing the transform
    // pv - where to store the values read
    //**********************************************************************
    void CJsonImporter::ParseVector3(IJSonObj* pObj, Vector3* pv)
    {
        std::vector<CRefObj<IJSonObj>>* vec = std::any_cast<std::vector<CRefObj<IJSonObj>>*>(pObj->GetValue());
        if (vec->size() != 3)
            throw new std::exception("Vector wrong size.");
        for (int i = 0; i < 3; i++)
        {
            CJSonType t = (*vec)[i]->GetType();
            if (t != CJSonType::Number)
                throw new std::exception("Expected number");
            float fv = std::any_cast<float>((*vec)[i]->GetValue());
            switch (i)
            {
            case 0: pv->x = fv; break;
            case 1: pv->y = fv; break;
            case 2: pv->z = fv; break;
            }
        }
    }

    //**********************************************************************
    // Method: ParseFloat
    // Parses a float value
    // 
    // Parameters:
    // pObj - JSon object describing the transform
    // pv - where to store the value read
    //**********************************************************************
    void CJsonImporter::ParseFloat(IJSonObj* pObj, float* pv)
    {
        if (pObj->GetType() != CJSonType::Number)
            throw new std::exception("Expected number");
        *pv = std::any_cast<float>(pObj->GetValue());
    }

    //**********************************************************************
    // Method: ParseTransform
    // Parses the definition of a transform. Transform has the following syntax:
    //     transform : {
    //        scale : [ <float>, <float>, <float> ],
    //        rotate : [ <float>, <float>, <float>, <float> ],
    //        translate : [ <float>, <float>, <float> ],
    //        children : [ <nodes> ]
    //     }
    // where:
    //    scale - scaling vector
    //    rotate - axis, angle used to rotate
    //    translate - amount to translate by
    //    children - list of child nodes
    // 
    // Parameters:
    // pGroup - Scene element this transform should be added to
    // pObj - JSon object describing the transform
    //**********************************************************************
    void CJsonImporter::ParseTransform(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj)
    {
        CRefObj<ISceneGroupElem> spTransform = m_spSceneFactory->CreateGroupElem();
        std::map<std::string, CRefObj<IJSonObj>>* pXformMap = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(pObj->GetValue());
        Vector3 scale;
        Vector3 rotate;
        Vector3 translate;
        for (auto n : *pXformMap)
        {
            if (n.first == "scale")
                ParseVector3(n.second, &scale);
            else if (n.first == "rotate")
                ParseVector3(n.second, &rotate);
            else if (n.first == "translate")
                ParseVector3(n.second, &translate);
            else if (n.first == "children")
            {
                std::vector<CRefObj<IJSonObj>>* vec = std::any_cast<std::vector<CRefObj<IJSonObj>>*>(n.second->GetValue());
                for (size_t i = 0; i < vec->size(); i++)
                {
                    ParseNode(pGroup, (*vec)[i]);
                }
            }
        }
        pGroup->AddChild(spTransform);
    }

    //**********************************************************************
    // Method: ParseMaterial
    // Parses the definition of a material. Materialhas the following syntax:
    //     material : {
    //        shader : "<shader>",
    //        texture : <texture>,
    //     }
    // where:
    //    shader - pixel shader as a string
    //    texture - texture object
    // 
    // Parameters:
    // pObj - JSon object describing the sphere
    // pMaterial - material parsed
    //**********************************************************************
    void CJsonImporter::ParseMaterial(IJSonObj* pObj, JsonMaterial *pMaterial)
    {
    }

    //**********************************************************************
    // Method: ParseSphere
    // Parses the definition of a sphere. Sphere has the following syntax:
    //     sphere : {
    //        center : [ <float>, <float>, <float> ],
    //        radius : <float>,
    //        material : <material>
    //     }
    // where:
    //    center - local coordinate center for the sphere
    //    radius - radius in meters
    //    material - material definition to apply to the sphere
    // 
    // Parameters:
    // pGroup - Scene element this sphere should be added to
    // pObj - JSon object describing the sphere
    //**********************************************************************
    void CJsonImporter::ParseSphere(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj)
    {
        Vector3 center;
        float radius;
        std::map<std::string, CRefObj<IJSonObj>>* values = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(pObj->GetValue());
        for (auto name : *values)
        {
            if (name.first == "center")
            {
                ParseVector3(name.second, &center);
            }
            else if (name.first == "radius")
            {
                ParseFloat(name.second, &radius);
            }
            else if (name.first == "material")
            {
                JsonMaterial material;
                ParseMaterial(name.second, &material);
            }
        }
        auto spSphere = m_spSceneFactory->CreateSphereElem(center, radius);
        pGroup->AddChild(spSphere);
    }

    //**********************************************************************
    // Method: ParseCube
    // Parses the definition of a cube. Cube has the following syntax:
    //     cube : {
    //        center : [ <float>, <float>, <float> ],
    //        radius : <float>,
    //        material : <material>
    //     }
    // where:
    //    center - local coordinate center for the sphere
    //    radius - radius in meters
    //    material - material definition to apply to the sphere
    // 
    // Parameters:
    // pGroup - Scene element this sphere should be added to
    // pObj - JSon object describing the sphere
    //**********************************************************************
    void CJsonImporter::ParseCube(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj)
    {
        Vector3 center;
        Vector3 dimensions;
        std::map<std::string, CRefObj<IJSonObj>>* values = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(pObj->GetValue());
        for (auto name : *values)
        {
            if (name.first == "center")
            {
                ParseVector3(name.second, &center);
            }
            else if (name.first == "dimensions")
            {
                ParseVector3(name.second, &dimensions);
            }
            else if (name.first == "material")
            {
                JsonMaterial material;
                ParseMaterial(name.second, &material);
            }
        }
        auto spCube = m_spSceneFactory->CreateCubeElem(center, dimensions.x, dimensions.y, dimensions.z);
        pGroup->AddChild(spCube);
    }

    //**********************************************************************
    // Method: ParseCylinder
    // Parses the definition of a cylinder. Cylinder has the following syntax:
    //     cylinder : {
    //        center : [ <float>, <float>, <float> ],
    //        topRadius : <float>,
    //        bottomRadius : <float>,
    //        height : <float>,
    //        material : <material>
    //     }
    // where:
    //    center - local coordinate center for the sphere
    //    topRadius - radius at top of cylinder in meters
    //    bottomRadius - radius at top of cylinder in meters
    //    height - height of cylinder
    //    material - material definition to apply to the sphere
    // 
    // Parameters:
    // pGroup - Scene element this sphere should be added to
    // pObj - JSon object describing the sphere
    //**********************************************************************
    void CJsonImporter::ParseCylinder(Caustic::ISceneGroupElem* pGroup, IJSonObj* pObj)
    {
        Vector3 center;
        float topRadius, bottomRadius, height;
        std::map<std::string, CRefObj<IJSonObj>>* values = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(pObj->GetValue());
        for (auto name : *values)
        {
            if (name.first == "center")
            {
                ParseVector3(name.second, &center);
            }
            else if (name.first == "topRadius")
            {
                ParseFloat(name.second, &topRadius);
            }
            else if (name.first == "bottomRadius")
            {
                ParseFloat(name.second, &bottomRadius);
            }
            else if (name.first == "height")
            {
                ParseFloat(name.second, &height);
            }
            else if (name.first == "material")
            {
                JsonMaterial material;
                ParseMaterial(name.second, &material);
            }
        }
        auto spCylinder = m_spSceneFactory->CreateCylinderElem(center, height, topRadius, bottomRadius);
        pGroup->AddChild(spCylinder);
    }

    //**********************************************************************
    // Method: ParseNode
    // Parses a node from the JSon
    // 
    // Parameters:
    // pGroup - Scene element to add nodes to
    // pObj - JSon object we are parsing
    //**********************************************************************
    void CJsonImporter::ParseNode(Caustic::ISceneGroupElem *pGroup, IJSonObj* pObj)
    {
        std::string name = pObj->GetName();
        if (name == "transform")
            ParseTransform(pGroup, pObj);
        else if (name == "sphere")
            ParseSphere(pGroup, pObj);
        else if (name == "cube")
            ParseCube(pGroup, pObj);
        else if (name == "cylinder")
            ParseCylinder(pGroup, pObj);
//        else if (name == "spotlight")
//        else if (name == "pointlight")
//        else if (name == "directionallight")
    }

    //**********************************************************************
    // Method: Import
    // Imports a new scene from a JSon file
    // 
    // Parameters:
    // fn - file to parse from
    // ppSceneGraph - returns the new scene graph
    //**********************************************************************
    CRefObj<ISceneGraph> CJsonImporter::Import(std::wstring fn)
    {
        m_spSceneFactory = Caustic::CreateSceneFactory();

        CRefObj<ISceneGraph> spSceneGraph = Caustic::CreateSceneGraph();

        CRefObj<IJSonParser> spParser = CreateJSonParser();
        CRefObj<IJSonObj> spDom = spParser->LoadDOM(fn);

        if (spDom->GetType() != CJSonType::Object)
            throw new std::exception("Expected top level object");
        std::map<std::string, CRefObj<IJSonObj>>* pTopLevelObj = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(spDom->GetValue());
        for (auto n : *pTopLevelObj)
        {
            ParseNode(spSceneGraph, n.second);
        }
        return spSceneGraph;
    }
}
