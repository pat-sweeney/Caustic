//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
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
#include <memory>

export module Rendering.SceneImport.Collada;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Geometry.Mesh.IMeshConstructor;
import Rendering.SceneGraph.ISceneGraph;
import Rendering.SceneGraph.ISceneLightCollectionElem;

export namespace Caustic
{
    namespace Collada
    {
        struct SParam
        {
            std::wstring m_name;
            std::wstring m_type;
        };

        struct SAccessor
        {
            std::wstring m_source;
            int m_count;
            int m_stride;
            std::vector<std::unique_ptr<SParam>> m_params;

            SAccessor()
            {
                m_count = 0;
                m_stride = 0;
            }
        };

        struct STechnique
        {
            std::vector<std::unique_ptr<SAccessor>> m_accessors;

            STechnique()
            {
            }
        };

        struct SSource
        {
            std::wstring m_id;
            std::unique_ptr<STechnique> m_spTechnique;
            std::vector<float> m_data;

            Vector3 ReadElement(int index);
        };

        struct SInput
        {
            std::wstring m_semantic;
            std::wstring m_source;
            int m_offset;
        };

        struct SceneConstruction
        {
            CRefObj<ISceneGraph> m_spSceneGraph;
        };
        
        struct SMesh;

        struct SPolylist
        {
            int m_count;
            std::vector<int> m_indices;
            std::vector<std::unique_ptr<SInput>> m_inputs;

            SPolylist()
            {
                m_count = 0;
            }

            void BuildMesh(Collada::SMesh *pMesh, IMeshConstructor *pMeshConstructor);
        };

        struct SVertex
        {
            std::wstring m_id;
            std::vector<std::unique_ptr<SInput>> m_inputs;
        public:
            SVertex()
            {
            }

            Vector3 ReadElement(SMesh *pMesh, int index);
        };

        struct SMesh
        {
            std::map<std::wstring, std::unique_ptr<SSource>> m_sources;
            std::map<std::wstring, std::unique_ptr<SVertex>> m_vertices;
            std::vector<std::unique_ptr<SPolylist>> m_polylists;

            void BuildMesh(IMeshConstructor *pMeshConstructor)
            {
                for (int i = 0; i < (int)m_polylists.size(); i++)
                    m_polylists[i]->BuildMesh(this, pMeshConstructor);
            }
        };

        struct SCamera
        {
            std::wstring m_id;
            std::wstring m_name;
            float m_znear;
            float m_zfar;
            float m_aspectRatio;
            float m_fov;
        };

        struct SLight
        {
            std::wstring m_id;
            std::wstring m_name;
            int m_type;
            Vector3 m_pos;
            Vector3 m_color;
            float m_energy;
            float m_attenuationConstant;
            float m_attenuationLinear;
            float m_attenuationQuadratic;
            float m_angle;
            uint32 m_mode;
        };

        struct SMaterial
        {
            std::wstring m_id;
            std::wstring m_name;
            std::wstring m_effectURL;
        };

        struct SEffect
        {
            std::wstring m_id;
            std::wstring m_name;
            Vector4 m_emission;
            Vector4 m_diffuse;
            float m_indexOfRefraction;
        };

        struct SGeometry
        {
            std::wstring m_id;
            std::wstring m_name;
            std::unique_ptr<SMesh> m_spMesh;
            
            void BuildMesh(IMeshConstructor *pMeshConstructor) { m_spMesh->BuildMesh(pMeshConstructor); }
        };
        
        struct SInstance
        {
            std::wstring m_url;
        };

        struct VertexBinding
        {
            std::wstring m_semantic;
            std::wstring m_inputSemantic;
            int m_inputSet;
        };
        
        struct SMaterialInstance : public SInstance
        {
            std::wstring m_symbol;
            std::wstring m_targetURL;
            std::vector<std::unique_ptr<VertexBinding>> m_vertexBindings;
        };

        const int c_NodeType_Camera = 0x1;
        const int c_NodeType_Light = 0x2;
        const int c_NodeType_Geometry = 0x3;

        struct SNode
        {
            std::wstring m_id;
            std::wstring m_name;
            std::wstring m_type;
            Matrix4x4 m_mat;
            int m_nodeType;
            std::wstring m_url;
            std::unique_ptr<SMaterialInstance> m_spMaterialInstance;
        };

        struct SVisualScene
        {
            std::wstring m_id;
            std::wstring m_name;
            std::map<std::wstring, std::unique_ptr<SNode>> m_nodes;
        };

        struct SCollada
        {
            std::map<std::wstring, std::unique_ptr<SCamera>> m_cameras;
            std::map<std::wstring, std::unique_ptr<SLight>> m_lights;
            std::map<std::wstring, std::unique_ptr<SMaterial>> m_materials;
            std::map<std::wstring, std::unique_ptr<SEffect>> m_effects;
            std::map<std::wstring, std::unique_ptr<SGeometry>> m_geometries;
            std::map<std::wstring, std::unique_ptr<SVisualScene>> m_visualScenes;
        };
    }

    class CColladaImporter
    {
        CRefObj<Caustic::ISceneLightCollectionElem> m_spLightCollection;
    public:
        struct ParseElements
        {
            const wchar_t *nodeName;
            std::function<void(IXMLDOMNode*)> func;
        };

        void ParseSubnode(IXMLDOMNode* pNode, ParseElements* _Elements, const wchar_t* pPrepend);
        void ParseSubnode(IXMLDOMNode* pNode, ParseElements* _Elements);
        void ParseCollada(IXMLDOMNode *pNode, Caustic::ISceneGraph **ppSceneGraph);
        void ParseLight(IXMLDOMNode *pChild, std::map<std::wstring, std::unique_ptr<Collada::SLight>>* pLights);
        void ParseParam(IXMLDOMNode *pNode, std::vector<std::unique_ptr<Collada::SParam>> *pParams);
        void ParseTechnique(IXMLDOMNode *pChild, Collada::SSource *pSource);
        void ParseAccessor(IXMLDOMNode *pChild, std::vector<std::unique_ptr<Collada::SAccessor>> *pAccessors);
        void ParseVertices(IXMLDOMNode *pChild, std::map<std::wstring, std::unique_ptr<Collada::SVertex>> *pVertices);
        void ParseInput(IXMLDOMNode *pNode, std::vector<std::unique_ptr<Collada::SInput>> *pInputs);
        void ParsePolylist(IXMLDOMNode *pNode, Collada::SMesh *pMesh);
        void ParseVCount(IXMLDOMNode *pNode, int count);
        void ParseP(IXMLDOMNode *pNode, Collada::SPolylist *pPolylist);
        void ParseTriangles(IXMLDOMNode* pNode, Collada::SMesh* pMesh);
        void ParseCamera(IXMLDOMNode* pNode, std::map<std::wstring, std::unique_ptr<Collada::SCamera>>* cameras);
        void ParseGeometry(IXMLDOMNode *pNode, std::map<std::wstring, std::unique_ptr<Collada::SGeometry>> *geometries);
        void ParseLibraryGeometries(IXMLDOMNode* pNode, std::map<std::wstring, std::unique_ptr<Collada::SGeometry>>* geometries);
        void ParseLibraryMaterials(IXMLDOMNode* pNode, std::map<std::wstring, std::unique_ptr<Collada::SMaterial>>* pMaterials);
        void ParseMaterial(IXMLDOMNode* pNode, std::map<std::wstring, std::unique_ptr<Collada::SMaterial>>* pMaterials);
        void ParseLibraryEffects(IXMLDOMNode* pNode, std::map<std::wstring, std::unique_ptr<Collada::SEffect>>* pEffects);
        void ParseEffect(IXMLDOMNode* pNode, std::map<std::wstring, std::unique_ptr<Collada::SEffect>>* pEffects);
        void ParseFloatArray(IXMLDOMNode *pChild, Collada::SSource *pSource);
        void ParseMatrix(IXMLDOMNode *pNode, Matrix4x4 *pMat);
        void ParseInstance(IXMLDOMNode *pNode, std::wstring *pURL);
        void ParseNode(IXMLDOMNode* pNode, Collada::SCollada* pCollada, std::map<std::wstring, std::unique_ptr<Collada::SNode>>* pNodes);
        void ParseVisualSceneInstance(IXMLDOMNode *pNode, Collada::SCollada *pCollada, Caustic::ISceneGraph **ppSceneGraph);
        void ParseScene(IXMLDOMNode *pNode, Collada::SCollada *pCollada, Caustic::ISceneGraph **ppSceneGraph);
        void ParseLibraryVisualScenes(IXMLDOMNode* pNode, Collada::SCollada* pCollada, std::map<std::wstring, std::unique_ptr<Collada::SVisualScene>>* pVisualScenes);
        void ParseLibraryCameras(IXMLDOMNode* pNode, std::map<std::wstring, std::unique_ptr<Collada::SCamera>>* cameras);
        void ParseLibraryLights(IXMLDOMNode* pNode, std::map<std::wstring, std::unique_ptr<Collada::SLight>>* pLights);
        void ParseVisualScene(IXMLDOMNode* pNode, Collada::SCollada* pCollada, std::map<std::wstring, std::unique_ptr<Collada::SVisualScene>>* pVisualScenes);
        void ParseMesh(IXMLDOMNode *pNode, Collada::SGeometry *pGeom);
        void ParseSource(IXMLDOMNode *pNode, std::map<std::wstring, std::unique_ptr<Collada::SSource>> *pSources);
        void ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, std::wstring &value);
        void ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, float &value);
        void ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, int &value);

    public:
        void Import(const char *fn, Caustic::ISceneGraph **ppSceneGraph);
    };
}
