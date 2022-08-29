//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
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

export module Rendering.SceneImport.Collada;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Geometry.Mesh.IMeshConstructor;
import Rendering.SceneGraph.ISceneGraph;

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
            std::vector<SParam*> m_params;

            SAccessor()
            {
                m_count = 0;
                m_stride = 0;
            }

            ~SAccessor()
            {
                for (int i = 0; i < (int)m_params.size(); i++)
                {
                    delete m_params[i];
                    m_params[i] = nullptr;
                }
            }
        };

        struct STechnique
        {
            std::vector<SAccessor*> m_accessors;

            STechnique()
            {
            }

            ~STechnique()
            {
                for (int i = 0; i < (int)m_accessors.size(); i++)
                {
                    delete m_accessors[i];
                    m_accessors[i] = nullptr;
                }
            }
        };

        struct SSource
        {
            std::wstring m_id;
            STechnique *m_pTechnique;
            std::vector<float> m_data;

            SSource()
            {
                m_pTechnique = nullptr;
            }

            ~SSource()
            {
                delete m_pTechnique;
                m_pTechnique = nullptr;
            }

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
            std::vector<SInput*> m_inputs;

            SPolylist()
            {
                m_count = 0;
            }

            ~SPolylist()
            {
                for (int i = 0; i < (int)m_inputs.size(); i++)
                {
                    delete m_inputs[i];
                    m_inputs[i] = nullptr;
                }
            }

            void BuildMesh(Collada::SMesh *pMesh, IMeshConstructor *pMeshConstructor);
        };

        struct SVertex
        {
            std::wstring m_id;
            std::vector<SInput*> m_inputs;
        public:
            SVertex()
            {
            }

            ~SVertex()
            {
                for (int i = 0; i < (int)m_inputs.size(); i++)
                {
                    delete m_inputs[i];
                    m_inputs[i] = nullptr;
                }
            }

            Vector3 ReadElement(SMesh *pMesh, int index);
        };

        struct SMesh
        {
            std::map<std::wstring, SSource*> m_sources;
            std::map<std::wstring, SVertex*> m_vertices;
            std::vector<SPolylist*> m_polylists;

            SMesh()
            {
            }

            ~SMesh()
            {
                std::map<std::wstring, SSource*>::iterator it = m_sources.begin();
                while (it != m_sources.end())
                {
                    delete it->second;
                    it++;
                }
                std::map<std::wstring, SVertex*>::iterator it2 = m_vertices.begin();
                while (it2 != m_vertices.end())
                {
                    delete it2->second;
                    it2++;
                }
                m_sources.clear();
                m_vertices.clear();
                for (int i = 0; i < (int)m_polylists.size(); i++)
                    delete m_polylists[i];
            }

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

        struct SGeometry
        {
            std::wstring m_id;
            std::wstring m_name;
            SMesh *m_pMesh;

            SGeometry()
            {
                m_pMesh = nullptr;
            }

            ~SGeometry()
            {
                delete m_pMesh;
                m_pMesh = nullptr;
            }

            void BuildMesh(IMeshConstructor *pMeshConstructor) { m_pMesh->BuildMesh(pMeshConstructor); }
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
        };

        struct SVisualScene
        {
            std::wstring m_id;
            std::wstring m_name;
            std::map<std::wstring, SNode*> m_nodes;
        };

        struct SCollada
        {
            std::map<std::wstring, SCamera*> m_cameras;
            std::map<std::wstring, SGeometry*> m_geometries;
            std::map<std::wstring, SVisualScene*> m_visualScenes;

            SCollada()
            {
            }

            ~SCollada()
            {
                std::map<std::wstring, SGeometry*>::iterator it = m_geometries.begin();
                while (it != m_geometries.end())
                {
                    delete it->second;
                    it++;
                }
                std::map<std::wstring, SVisualScene*>::iterator it2 = m_visualScenes.begin();
                while (it2 != m_visualScenes.end())
                {
                    delete it2->second;
                    it2++;
                }
            }
        };
    }

    class CColladaImporter
    {
    public:
        struct ParseElements
        {
            const wchar_t *nodeName;
            std::function<void(IXMLDOMNode*)> func;
        };

        void ParseSubnode(IXMLDOMNode* pNode, ParseElements* _Elements, const wchar_t* pPrepend);
        void ParseSubnode(IXMLDOMNode* pNode, ParseElements* _Elements);
        void ParseCollada(IXMLDOMNode *pNode, Caustic::ISceneGraph **ppSceneGraph);
        void ParseLight(IXMLDOMNode *pChild);
        void ParseParam(IXMLDOMNode *pNode, std::vector<Collada::SParam*> *pParams);
        void ParseTechnique(IXMLDOMNode *pChild, Collada::SSource *pSource);
        void ParseAccessor(IXMLDOMNode *pChild, std::vector<Collada::SAccessor*> *pAccessors);
        void ParseVertices(IXMLDOMNode *pChild, std::map<std::wstring, Collada::SVertex*> *pVertices);
        void ParseInput(IXMLDOMNode *pNode, std::vector<Collada::SInput*> *pInputs);
        void ParsePolylist(IXMLDOMNode *pNode, Collada::SMesh *pMesh);
        void ParseVCount(IXMLDOMNode *pNode, int count);
        void ParseP(IXMLDOMNode *pNode, Collada::SPolylist *pPolylist);
        void ParseCamera(IXMLDOMNode* pNode, std::map<std::wstring, Collada::SCamera*>* cameras);
        void ParseGeometry(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SGeometry*> *geometries);
        void ParseLibraryGeometries(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SGeometry*> *geometries);
        void ParseFloatArray(IXMLDOMNode *pChild, Collada::SSource *pSource);
        void ParseMatrix(IXMLDOMNode *pNode, Matrix4x4 *pMat);
        void ParseInstance(IXMLDOMNode *pNode, std::wstring *pURL);
        void ParseNode(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SNode*> *pNodes);
        void ParseVisualSceneInstance(IXMLDOMNode *pNode, Collada::SCollada *pCollada, Caustic::ISceneGraph **ppSceneGraph);
        void ParseScene(IXMLDOMNode *pNode, Collada::SCollada *pCollada, Caustic::ISceneGraph **ppSceneGraph);
        void ParseLibraryVisualScenes(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SVisualScene*> *pVisualScenes);
        void ParseLibraryCameras(IXMLDOMNode* pNode, std::map<std::wstring, Collada::SCamera*>* cameras);
        void ParseVisualScene(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SVisualScene*> *pVisualScenes);
        void ParseMesh(IXMLDOMNode *pNode, Collada::SGeometry *pGeom);
        void ParseSource(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SSource*> *pSources);
        void ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, std::wstring &value);
        void ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, float &value);
        void ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, int &value);

    public:
        void Import(const char *fn, Caustic::ISceneGraph **ppSceneGraph);
    };
}
