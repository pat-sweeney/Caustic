//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include <msxml6.h>
#include <atlbase.h>
#include <string>
#include <memory>
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.CausticFactory;
import Rendering.SceneGraph.SceneFactory;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneImport.Collada;

namespace Caustic
{
    void CColladaImporter::ParseSubnode(IXMLDOMNode* pNode, ParseElements* _Elements)
    {
        long numChildren;
        CComPtr<IXMLDOMNodeList> spChildren;
        CT(pNode->get_childNodes(&spChildren));
        CT(spChildren->get_length(&numChildren));
        for (long i = 0; i < numChildren; i++)
        {
            CComPtr<IXMLDOMNode> spChild;
            CT(spChildren->get_item(i, &spChild));
            BSTR bstrNodeName;
            CT(spChild->get_nodeName(&bstrNodeName));
            int indx = 0;
            while (_Elements[indx].nodeName)
            {
                if (_wcsicmp(_Elements[indx].nodeName, bstrNodeName) == 0)
                {
                    _Elements[indx].func(spChild);
                    break;
                }
                indx++;
            }
        }
    }
    
    void CColladaImporter::ParseSubnode(IXMLDOMNode *pNode, ParseElements *_Elements, const wchar_t *pPrepend)
    {
        long numChildren;
        CComPtr<IXMLDOMNodeList> spChildren;
        CT(pNode->get_childNodes(&spChildren));
        CT(spChildren->get_length(&numChildren));
        bool found = false;
        for (long i = 0; i < numChildren; i++)
        {
            CComPtr<IXMLDOMNode> spChild;
            CT(spChildren->get_item(i, &spChild));
            BSTR bstrNodeName;
            CT(spChild->get_nodeName(&bstrNodeName));
            std::wstring curNodeName = std::wstring(bstrNodeName);
            if (pPrepend != nullptr)
                curNodeName = std::wstring(pPrepend) + L"." + curNodeName;

            int indx = 0;
            while (_Elements[indx].nodeName)
            {
                // We allow _Elements[index].nodeName to take the form:
                //     nodeName.subNodeName. ...
                // This allows for parsing subnodes without having to implement
                // individual node parsing methods.
                if (_wcsicmp(_Elements[indx].nodeName, curNodeName.c_str()) == 0)
                {
                    _Elements[indx].func(spChild);
                }
                indx++;
            }
            ParseSubnode(spChild, _Elements, curNodeName.c_str());
        }
    }

    void CColladaImporter::ParseCamera(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SCamera*>* cameras)
    {
        Collada::SCamera* pCamera = new Collada::SCamera();
        std::unique_ptr<Collada::SCamera> spCamera(pCamera);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", pCamera->m_id);
        ParseAttribute(spAttributes, L"name", pCamera->m_name);

        CColladaImporter::ParseElements _elems[] =
        {
            {
                L"optics.technique_common.perspective.xfov",
                [this, pCamera](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pCamera->m_fov = (float)_wtof(bstr);
                }
            },
            {
                L"optics.technique_common.perspective.aspect_ratio",
                [this, pCamera](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pCamera->m_aspectRatio = (float)_wtof(bstr);
                }
            },
            {
                L"optics.technique_common.perspective.znear",
                [this, pCamera](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pCamera->m_znear = (float)_wtof(bstr);
                }
            },
            {
                L"optics.technique_common.perspective.zfar",
                [this, pCamera](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pCamera->m_zfar = (float)_wtof(bstr);
                }
            },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems, nullptr);
        cameras->insert(std::pair<std::wstring, Collada::SCamera*>(pCamera->m_id, pCamera));
        spCamera.release();
    }

    void CColladaImporter::ParseLight(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SLight*>* pLights)
    {
        Collada::SLight* pLight = new Collada::SLight();
        std::unique_ptr<Collada::SLight> spLight(pLight);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", pLight->m_id);
        ParseAttribute(spAttributes, L"name", pLight->m_name);

        CColladaImporter::ParseElements _elems[] =
        {
            {
                L"extra.technique.type",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pLight->m_type = _wtoi(bstr);
                }
            },
            {
                L"extra.technique.energy",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pLight->m_energy = (float)_wtof(bstr);
                }
            },
            {
                L"technique_common.point.color",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    float r, g, b;
                    swscanf_s(bstr, L"%f %f %f", &r, &g, &b);
                    pLight->m_color = Vector3(r, g, b);
                }
            },
            {
                L"technique_common.directional.color",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    float r, g, b;
                    swscanf_s(bstr, L"%f %f %f", &r, &g, &b);
                    pLight->m_color = Vector3(r, g, b);
                }
            },
            {
                L"technique_common.directional.color",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    float r, g, b;
                    swscanf_s(bstr, L"%f %f %f", &r, &g, &b);
                    pLight->m_color = Vector3(r, g, b);
                }
            },
            {
                L"technique_common.spot.color",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    float r, g, b;
                    swscanf_s(bstr, L"%f %f %f", &r, &g, &b);
                    pLight->m_color = Vector3(r, g, b);
                }
            },
            {
                L"technique_common.spot.falloff_angle",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pLight->m_angle = (float)_wtof(bstr);
                }
            },
            {
                L"extra.technique.mode",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pLight->m_mode = (uint32)_wtoi(bstr);
                }
            },
            {
                L"technique_common.point.constant_attenuation",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pLight->m_attenuationConstant = (float)_wtof(bstr);
                }
            },
            {
                L"technique_common.point.linear_attenuation",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pLight->m_attenuationLinear = (float)_wtof(bstr);
                }
            },
            {
                L"technique_common.point.quadratic_attenuation",
                [this, pLight](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pLight->m_attenuationQuadratic = (float)_wtof(bstr);
                }
            },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems, nullptr);
        pLights->insert(std::pair<std::wstring, Collada::SLight*>(pLight->m_id, pLight));
        spLight.release();
    }

    void CColladaImporter::ParseMaterial(IXMLDOMNode* pNode, std::map<std::wstring, Collada::SMaterial*>* pMaterials)
    {
        Collada::SMaterial* pMaterial = new Collada::SMaterial();
        std::unique_ptr<Collada::SMaterial> spMaterial(pMaterial);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", pMaterial->m_id);
        ParseAttribute(spAttributes, L"name", pMaterial->m_name);

        CColladaImporter::ParseElements _elems[] =
        {
            {
                L"instance_effect",
                [this, pMaterial](IXMLDOMNode* pNode)
                {
                    CComPtr<IXMLDOMNamedNodeMap> spNodeAttributes;
                    CT(pNode->get_attributes(&spNodeAttributes));
                    ParseAttribute(spNodeAttributes, L"url", pMaterial->m_effectURL);
                }
            },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems, nullptr);
        pMaterials->insert(std::pair<std::wstring, Collada::SMaterial*>(pMaterial->m_id, pMaterial));
        spMaterial.release();
    }

    void CColladaImporter::ParseLibraryMaterials(IXMLDOMNode* pNode, std::map<std::wstring, Collada::SMaterial*>* pMaterials)
    {
        CColladaImporter::ParseElements _elems[] =
        {
            { L"material",
                [this, pMaterials](IXMLDOMNode* pNode)
                {
                    ParseMaterial(pNode, pMaterials);
                }
            },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
    }
    
    void CColladaImporter::ParseLibraryLights(IXMLDOMNode* pNode, std::map<std::wstring, Collada::SLight*>* pLights)
    {
        CColladaImporter::ParseElements _elems[] =
        {
            { L"light",
                [this, pLights](IXMLDOMNode* pNode)
                {
                    ParseLight(pNode, pLights);
                }
            },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
    }

    void CColladaImporter::ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, std::wstring &value)
    {
        CComPtr<IXMLDOMNode> spNode;
        CComBSTR idBSTR(id);
        CT(pAttributes->getNamedItem(idBSTR, &spNode));
        if (spNode != nullptr)
        {
            CComVariant val;
            CT(spNode->get_nodeValue(&val));
            value = std::wstring(val.bstrVal);
        }
        else
            value = std::wstring(L"");
    }

    void CColladaImporter::ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, float &value)
    {
        CComPtr<IXMLDOMNode> spNode;
        CComBSTR idBSTR(id);
        CT(pAttributes->getNamedItem(idBSTR, &spNode));
        if (spNode != nullptr)
        {
            CComVariant val;
            CT(spNode->get_nodeValue(&val));
            value = (float)_wtof(val.bstrVal);
        }
        else
            value = 0.0f;
    }

    void CColladaImporter::ParseAttribute(IXMLDOMNamedNodeMap *pAttributes, const wchar_t *id, int &value)
    {
        CComPtr<IXMLDOMNode> spNode;
        CComBSTR idBSTR(id);
        CT(pAttributes->getNamedItem(idBSTR, &spNode));
        CComVariant val;
        if (spNode)
        {
            CT(spNode->get_nodeValue(&val));
            value = _wtoi(val.bstrVal);
        }
        else
            value = 0;
    }

    void CColladaImporter::ParseFloatArray(IXMLDOMNode *pNode, Collada::SSource *pSource)
    {
        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        std::wstring id; // Currently unused (since we don't create the float array but instead store in m_data on SSource)
        ParseAttribute(spAttributes, L"id", id);
        int numFloats;
        ParseAttribute(spAttributes, L"count", numFloats);
        CComBSTR bstr;
        pNode->get_text(&bstr);
        wchar_t *p = bstr;
        for (int i = 0; i < numFloats; i++)
        {
            float f = (float)_wtof(p);
            while (*p && !iswspace(*p))
                p++;
            while (*p && iswspace(*p))
                p++;
            pSource->m_data.push_back(f);
        }
    }

    void CColladaImporter::ParseInput(IXMLDOMNode *pNode, std::vector<Collada::SInput*> *pInputs)
    {
        std::unique_ptr<Collada::SInput> spInput(new Collada::SInput());
        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"semantic", spInput->m_semantic);
        ParseAttribute(spAttributes, L"source", spInput->m_source);
        ParseAttribute(spAttributes, L"offset", spInput->m_offset);
        pInputs->push_back(spInput.release());
    }
    
    void CColladaImporter::ParseVertices(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SVertex*> *pVertices)
    {
        Collada::SVertex *pVertex = new Collada::SVertex();
        std::unique_ptr<Collada::SVertex> spVertex(pVertex);
        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", pVertex->m_id);
        CColladaImporter::ParseElements _elems[] =
        {
            { L"input", [this, pVertex](IXMLDOMNode *pNode) { ParseInput(pNode, &pVertex->m_inputs); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
        pVertices->insert(std::pair<std::wstring, Collada::SVertex*>(pVertex->m_id, pVertex));
        spVertex.release();
    }

    void CColladaImporter::ParseVCount(IXMLDOMNode *pNode, int count)
    {
        CComBSTR bstr;
        pNode->get_text(&bstr);
        wchar_t *p = bstr;
        for (int i = 0; i < count; i++)
        {
            int n = _wtoi(p);
            _ASSERTE(n == 3);
            while (*p && !iswspace(*p))
                p++;
            while (*p && iswspace(*p))
                p++;
        }
    }

    Vector3 Collada::SSource::ReadElement(int index)
    {
        // Here we assume all our accessors describe Vector3
        _ASSERTE(m_pTechnique->m_accessors.size() == 1);
        _ASSERTE(m_pTechnique->m_accessors[0]->m_stride == 3);
        Vector3 v;
        v.x = m_data[index * 3 + 0];
        v.y = m_data[index * 3 + 1];
        v.z = m_data[index * 3 + 2];
        return v;
    }

    Vector3 Collada::SVertex::ReadElement(Collada::SMesh *pMesh, int index)
    {
        // Assume vertices only have a position
        _ASSERTE(m_inputs.size() == 1);
        return pMesh->m_sources[m_inputs[0]->m_source.substr(1)]->ReadElement(index);
    }
    
    void Collada::SPolylist::BuildMesh(Collada::SMesh *pMesh, IMeshConstructor *pMeshConstructor)
    {
        // First determine order of inputs
        std::vector<Collada::SInput*> inputs(m_inputs.size(), nullptr);
        for (int j = 0; j < (int)m_inputs.size(); j++)
        {
            if (m_inputs[j]->m_offset >= inputs.size())
                inputs.resize(m_inputs[j]->m_offset, nullptr);
            inputs[m_inputs[j]->m_offset] = m_inputs[j];
        }

        pMeshConstructor->SubMeshOpen();
        int index = 0;
        CGeomVertex verts[3];
        for (int i = 0; i < m_count; i++)
        {
            for (int k = 0; k < 3; k++)
            {
                CGeomVertex vert;
                for (int j = 0; j < inputs.size(); j++)
                {
                    int n = m_indices[index++];
                    if (inputs[j])
                    {
                        std::wstring sourceName = inputs[j]->m_source.substr(1);
                        if (inputs[j]->m_semantic == L"VERTEX")
                        {
                            // NOTE: Here we assume the source is a <vertices/> tag
                            vert.pos = pMesh->m_vertices[sourceName]->ReadElement(pMesh, n);
                        }
                        else if (inputs[j]->m_semantic == L"NORMAL")
                        {
                            // NOTE: Here we assume the source is a <source/> tag
                            vert.norm = pMesh->m_sources[sourceName]->ReadElement(n);
                        }
                        else if (inputs[j]->m_semantic == L"TEXCOORD")
                        {
                            vert.uvs[0] = Vector2(
                                pMesh->m_sources[sourceName]->m_data[2 * n + 0],
                                pMesh->m_sources[sourceName]->m_data[2 * n + 1]);
                        }
                    }
                }
                verts[k] = vert;
            }
            pMeshConstructor->FaceOpen();
//#define RESPECT_WINDING_ORDER
#ifdef RESPECT_WINDING_ORDER
            Vector3 n1(verts[0].pos - verts[1].pos);
            Vector3 n2(verts[2].pos - verts[1].pos);
            if (n1.Cross(n2).Sign() < 0)
            {
                pMeshConstructor->VertexAdd(verts[0].pos, verts[0].norm, verts[0].uvs[0]);
                pMeshConstructor->VertexAdd(verts[1].pos, verts[1].norm, verts[1].uvs[0]);
                pMeshConstructor->VertexAdd(verts[2].pos, verts[2].norm, verts[2].uvs[0]);
            }
            else
            {
                pMeshConstructor->VertexAdd(verts[0].pos, verts[0].norm, verts[0].uvs[0]);
                pMeshConstructor->VertexAdd(verts[2].pos, verts[2].norm, verts[2].uvs[0]);
                pMeshConstructor->VertexAdd(verts[1].pos, verts[1].norm, verts[1].uvs[0]);
            }
#else // RESPECT_WINDING_ORDER
            pMeshConstructor->VertexAdd(verts[0].pos, verts[0].norm, verts[0].uvs[0]);
            pMeshConstructor->VertexAdd(verts[2].pos, verts[2].norm, verts[2].uvs[0]);
            pMeshConstructor->VertexAdd(verts[1].pos, verts[1].norm, verts[1].uvs[0]);
#endif // RESPECT_WINDING_ORDER
            pMeshConstructor->FaceClose();
        }
        pMeshConstructor->SubMeshClose();
    }

    void CColladaImporter::ParseP(IXMLDOMNode *pNode, Collada::SPolylist *pPolylist)
    {
        CComBSTR bstr;
        pNode->get_text(&bstr);
        wchar_t *p = bstr;
        while(*p)//        for (int i = 0; i < pPolylist->m_count; i++)
        {
            int indx = _wtoi(p);
            pPolylist->m_indices.push_back(indx);
            while (*p && !iswspace(*p))
                p++;
            while (*p && iswspace(*p))
                p++;
        }
    }

    void CColladaImporter::ParsePolylist(IXMLDOMNode* pNode, Collada::SMesh* pMesh)
    {
        Collada::SPolylist* pPolylist = new Collada::SPolylist();
        std::unique_ptr<Collada::SPolylist> spPolylist(pPolylist);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"count", spPolylist->m_count);
        CColladaImporter::ParseElements _elems[] =
        {
            { L"input", [this, pPolylist](IXMLDOMNode* pNode) { ParseInput(pNode, &pPolylist->m_inputs); } },
            { L"vcount", [this, pPolylist](IXMLDOMNode* pNode) { ParseVCount(pNode, pPolylist->m_count); } },
            { L"p", [this, pPolylist](IXMLDOMNode* pNode) { ParseP(pNode, pPolylist); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
        pMesh->m_polylists.push_back(spPolylist.release());
    }

    void CColladaImporter::ParseTriangles(IXMLDOMNode* pNode, Collada::SMesh* pMesh)
    {
        Collada::SPolylist* pPolylist = new Collada::SPolylist();
        std::unique_ptr<Collada::SPolylist> spPolylist(pPolylist);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"count", spPolylist->m_count);
        CColladaImporter::ParseElements _elems[] =
        {
            { L"input", [this, pPolylist](IXMLDOMNode* pNode) { ParseInput(pNode, &pPolylist->m_inputs); } },
            { L"p", [this, pPolylist](IXMLDOMNode* pNode) { ParseP(pNode, pPolylist); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
        pMesh->m_polylists.push_back(spPolylist.release());
    }

    void CColladaImporter::ParseParam(IXMLDOMNode *pNode, std::vector<Collada::SParam*> *pParams)
    {
        Collada::SParam *pParam = new Collada::SParam();
        std::unique_ptr<Collada::SParam> spParam(pParam);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"name", pParam->m_name);
        ParseAttribute(spAttributes, L"type", pParam->m_type);

        pParams->push_back(spParam.release());
    }

    void CColladaImporter::ParseAccessor(IXMLDOMNode *pNode, std::vector<Collada::SAccessor*> *pAccessors)
    {
        Collada::SAccessor *pAccessor = new Collada::SAccessor();
        std::unique_ptr<Collada::SAccessor> spAccessor(pAccessor);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"source", pAccessor->m_source);
        ParseAttribute(spAttributes, L"count", pAccessor->m_count);
        ParseAttribute(spAttributes, L"stride", pAccessor->m_stride);

        CColladaImporter::ParseElements _elems[] =
        {
            { L"param", [this, pAccessor](IXMLDOMNode *pNode) { ParseParam(pNode, &pAccessor->m_params); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
        pAccessors->push_back(spAccessor.release());
    }

    void CColladaImporter::ParseTechnique(IXMLDOMNode *pNode, Collada::SSource *pSource)
    {
        Collada::STechnique *pTechnique = new Collada::STechnique();
        std::unique_ptr<Collada::STechnique> spTechnique(pTechnique);
        CColladaImporter::ParseElements _elems[] =
        {
            { L"accessor", [this, pTechnique](IXMLDOMNode *pNode) { ParseAccessor(pNode, &pTechnique->m_accessors); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
        pSource->m_pTechnique = spTechnique.release();
    }

    void CColladaImporter::ParseSource(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SSource*> *pSources)
    {
        Collada::SSource *pSource = new Collada::SSource();
        std::unique_ptr<Collada::SSource> spSource(pSource);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", spSource->m_id);

        CColladaImporter::ParseElements _elems[] =
        {
            { L"float_array", [this, pSource](IXMLDOMNode *pNode) { ParseFloatArray(pNode, pSource); } },
            { L"technique_common", [this, pSource](IXMLDOMNode *pNode) { ParseTechnique(pNode, pSource); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
        pSources->insert(std::pair<std::wstring, Collada::SSource*>(spSource->m_id, spSource.get()));
        spSource.release();
    }

    void CColladaImporter::ParseMesh(IXMLDOMNode *pNode, Collada::SGeometry *pGeom)
    {
        Collada::SMesh *pMesh = new Collada::SMesh();
        std::unique_ptr<Collada::SMesh> spMesh(pMesh);
        CColladaImporter::ParseElements _elems[] =
        {
            { L"source", [this, pMesh](IXMLDOMNode *pNode)
                { 
                    ParseSource(pNode, &pMesh->m_sources); 
                } },
            { L"vertices", [this, pMesh](IXMLDOMNode *pNode)
                { 
                    ParseVertices(pNode, &pMesh->m_vertices);
                } },
            { L"polylist", [this, pMesh](IXMLDOMNode* pNode)
                {
                    ParsePolylist(pNode, pMesh);
                } },
            { L"triangles", [this, pMesh](IXMLDOMNode* pNode)
                {
                    ParseTriangles(pNode, pMesh);
                } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
        pGeom->m_pMesh = spMesh.release();
    }

    void CColladaImporter::ParseGeometry(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SGeometry*> *geometries)
    {
        Collada::SGeometry *pGeom = new Collada::SGeometry();
        std::unique_ptr<Collada::SGeometry> spGeom(pGeom);
        
        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", pGeom->m_id);
        ParseAttribute(spAttributes, L"name", pGeom->m_name);

        CColladaImporter::ParseElements _elems[] =
        {
            { L"mesh", [this, pGeom](IXMLDOMNode *pNode) { ParseMesh(pNode, pGeom); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
        geometries->insert(std::pair<std::wstring, Collada::SGeometry*>(pGeom->m_id, pGeom));
        spGeom.release();
    }

    void CColladaImporter::ParseLibraryGeometries(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SGeometry*> *geometries)
    {
        CColladaImporter::ParseElements _elems[] = 
        {
            { L"geometry", [this, geometries](IXMLDOMNode *pNode) 
                { 
                    ParseGeometry(pNode, geometries); 
                } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
    }

    void CColladaImporter::ParseMatrix(IXMLDOMNode *pNode, Matrix4x4 *pMat)
    {
        Collada::SNode *pColladaNode = new Collada::SNode();
        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        std::wstring sid;
        ParseAttribute(spAttributes, L"sid", sid);
        CComBSTR bstr;
        pNode->get_text(&bstr);
        wchar_t *p = bstr;
        for (int i = 0; i < 16; i++)
        {
            float f = (float)_wtof(p);
            pMat->v[i % 4][i / 4] = f;
            while (*p && !iswspace(*p))
                p++;
            while (*p && iswspace(*p))
                p++;
        }
    }

    void CColladaImporter::ParseInstance(IXMLDOMNode *pNode, std::wstring *pURL)
    {
        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"url", *pURL);
    }

    void CColladaImporter::ParseNode(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SNode*> *pNodes)
    {
        Collada::SNode *pColladaNode = new Collada::SNode();
        std::unique_ptr<Collada::SNode> spColladaNode(pColladaNode);
        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", pColladaNode->m_id);
        ParseAttribute(spAttributes, L"name", pColladaNode->m_name);
        ParseAttribute(spAttributes, L"type", pColladaNode->m_type);

        CColladaImporter::ParseElements _Elements[] =
        {
            { L"matrix", [this, pColladaNode](IXMLDOMNode *pNode) { ParseMatrix(pNode, &pColladaNode->m_mat); } },
            { L"instance_camera", [this, pColladaNode](IXMLDOMNode *pNode) { pColladaNode->m_nodeType = Collada::c_NodeType_Camera; ParseInstance(pNode, &pColladaNode->m_url); } },
            { L"instance_light", [this, pColladaNode](IXMLDOMNode *pNode) { pColladaNode->m_nodeType = Collada::c_NodeType_Light; ParseInstance(pNode, &pColladaNode->m_url); } },
            { L"instance_geometry", [this, pColladaNode](IXMLDOMNode *pNode)
            {
                pColladaNode->m_nodeType = Collada::c_NodeType_Geometry; 
                ParseInstance(pNode, &pColladaNode->m_url); 
            } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _Elements);
        pNodes->insert(std::pair<std::wstring, Collada::SNode*>(pColladaNode->m_id, pColladaNode));
        spColladaNode.release();
    }

    void CColladaImporter::ParseVisualScene(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SVisualScene*> *pVisualScenes)
    {
        Collada::SVisualScene *pVisualScene = new Collada::SVisualScene();
        std::unique_ptr<Collada::SVisualScene> spVisualScene(pVisualScene);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", pVisualScene->m_id);
        ParseAttribute(spAttributes, L"name", pVisualScene->m_name);

        CColladaImporter::ParseElements _Elements[] =
        {
            { L"node", [this, pVisualScene](IXMLDOMNode *pNode) { ParseNode(pNode, &pVisualScene->m_nodes); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _Elements);

        pVisualScenes->insert(std::pair<std::wstring,Collada::SVisualScene*>(pVisualScene->m_id, pVisualScene));
        spVisualScene.release();
    }

    void CColladaImporter::ParseLibraryVisualScenes(IXMLDOMNode *pNode, std::map<std::wstring, Collada::SVisualScene*> *pVisualScenes)
    {
        CColladaImporter::ParseElements _Elements[] =
        {
            { L"visual_scene", [this, pVisualScenes](IXMLDOMNode *pNode) { ParseVisualScene(pNode, pVisualScenes); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _Elements);
    }

    void CColladaImporter::ParseVisualSceneInstance(IXMLDOMNode *pNode, Collada::SCollada *pCollada, Caustic::ISceneGraph **ppSceneGraph)
    {
        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        std::wstring url;
        ParseAttribute(spAttributes, L"url", url);

        CRefObj<ISceneGraph> spSceneGraph = Caustic::CSceneFactory::Instance()->CreateSceneGraph();
        m_spLightCollection = CSceneFactory::Instance()->CreateLightCollectionElem();

        Collada::SVisualScene *pScene = pCollada->m_visualScenes[url.substr(1)];
        std::map<std::wstring, Collada::SNode*>::iterator it = pScene->m_nodes.begin();
        while (it != pScene->m_nodes.end())
        {
            Collada::SNode *pNode = it->second;
            switch (pNode->m_nodeType)
            {
            case Collada::c_NodeType_Camera:
                {
                    CRefObj<ICamera> spCamera = CCausticFactory::Instance()->CreateCamera(true);
                    spSceneGraph->GetCameras()->AddCamera(spCamera);
                }
                break;
            case Collada::c_NodeType_Light:
                {
                    Collada::SLight* pLight = pCollada->m_lights[pNode->m_url.substr(1)];
                    Vector3 dir(0.0f, 0.0f, -1.0f);

                    // Transform the position and direction
                    Vector4 pos4 = Vector4(pLight->m_pos.x, pLight->m_pos.y, pLight->m_pos.z, 1.0f) * pNode->m_mat;
                    Vector4 dir4 = Vector4(dir.x, dir.y, dir.z, 0.0f) * pNode->m_mat;
                    Vector3 pos = Vector3(pos4.x, pos4.y, pos4.z);
                    dir = Vector3(dir4.x, dir4.y, dir4.z);

                    CRefObj<ILight> spLight;
                    FRGBColor color(pLight->m_color.x / pLight->m_energy, pLight->m_color.y / pLight->m_energy, pLight->m_color.z / pLight->m_energy);
                    bool castsShadows = (pLight->m_mode & 0x1) ? true : false;
                    switch (pLight->m_type)
                    {
                    case 0:
                        spLight = CRefObj<ILight>(CCausticFactory::Instance()->CreatePointLight(pos, color, pLight->m_energy, castsShadows).p);
                    break;
                    case 1:
                        spLight = CCausticFactory::Instance()->CreateDirectionalLight(pos, dir, color, pLight->m_energy, castsShadows);
                    break;
                    case 2:
                        spLight = CCausticFactory::Instance()->CreateSpotLight(pos, dir, color, pLight->m_energy, pLight->m_angle, pLight->m_angle, castsShadows);
                    break;
                    }
                    m_spLightCollection->AddLight(spLight);
                }
                break;
            case Collada::c_NodeType_Geometry:
                {
                    CRefObj<Caustic::ISceneGroupElem> spXform = CSceneFactory::Instance()->CreateGroupElem();
                    spXform->SetTransform(pNode->m_mat);
                    spXform->SetName((std::wstring(L"Mesh - ") + pNode->m_name).c_str());

                    CRefObj<Caustic::ISceneMeshElem> spMeshElem = CSceneFactory::Instance()->CreateMeshElem();

                    Collada::SGeometry *pGeometry = pCollada->m_geometries[pNode->m_url.substr(1)];
                    CRefObj<IMeshConstructor> spMeshConstructor = IMeshConstructor::Create();
                    spMeshConstructor->MeshOpen();
                    pGeometry->BuildMesh(spMeshConstructor);
                    CRefObj<IMesh> spMesh = spMeshConstructor->MeshClose();
                    spMeshElem->SetMesh(spMesh);

                    spXform->AddChild(spMeshElem);
                    m_spLightCollection->AddChild(spXform);
                }
                break;
            }
            it++;
        }

        spSceneGraph->AddChild(m_spLightCollection);
        *ppSceneGraph = spSceneGraph;
        (*ppSceneGraph)->AddRef();
    }

    void CColladaImporter::ParseScene(IXMLDOMNode *pNode, Collada::SCollada *pCollada, Caustic::ISceneGraph **ppSceneGraph)
    {
        CColladaImporter::ParseElements _Elements[] =
        {
            { L"instance_visual_scene", [this, pCollada, ppSceneGraph](IXMLDOMNode *pNode) { ParseVisualSceneInstance(pNode, pCollada, ppSceneGraph); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _Elements);
    }
    
    void CColladaImporter::ParseLibraryCameras(IXMLDOMNode* pNode, std::map<std::wstring, Collada::SCamera*>* cameras)
    {
        CColladaImporter::ParseElements _elems[] =
        {
            { L"camera",
                [this, cameras](IXMLDOMNode* pNode)
                {
                    ParseCamera(pNode, cameras);
                }
            },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
    }

    void CColladaImporter::ParseEffect(IXMLDOMNode* pNode, std::map<std::wstring, Collada::SEffect*>* pEffects)
    {
        Collada::SEffect* pEffect = new Collada::SEffect();
        std::unique_ptr<Collada::SEffect> spEffect(pEffect);

        CComPtr<IXMLDOMNamedNodeMap> spAttributes;
        CT(pNode->get_attributes(&spAttributes));
        ParseAttribute(spAttributes, L"id", pEffect->m_id);
        ParseAttribute(spAttributes, L"name", pEffect->m_name);

        CColladaImporter::ParseElements _elems[] =
        {
            {
                L"profile_COMMON.technique.lambert.emission.color",
                [this, pEffect](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    swscanf_s(bstr, L"%f %f %f %f",
                        &pEffect->m_emission.x,
                        &pEffect->m_emission.y,
                        &pEffect->m_emission.z,
                        &pEffect->m_emission.w);
                }
            },
            {
                L"profile_COMMON.technique.lambert.diffuse.color",
                [this, pEffect](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    swscanf_s(bstr, L"%f %f %f %f",
                        &pEffect->m_diffuse.x,
                        &pEffect->m_diffuse.y,
                        &pEffect->m_diffuse.z,
                        &pEffect->m_diffuse.w);
                }
            },
            {
                L"profile_COMMON.technique.lambert.index_of_refraction.float",
                [this, pEffect](IXMLDOMNode* pNode)
                {
                    CComBSTR bstr;
                    pNode->get_text(&bstr);
                    pEffect->m_indexOfRefraction = (float)_wtof(bstr);
                }
            },
            {nullptr, nullptr}
        };
        ParseSubnode(pNode, _elems, nullptr);

        pEffects->insert(std::pair<std::wstring, Collada::SEffect*>(pEffect->m_id, pEffect));
        spEffect.release();
    }
    
    void CColladaImporter::ParseLibraryEffects(IXMLDOMNode* pNode, std::map<std::wstring, Collada::SEffect*>* pEffects)
    {
        CColladaImporter::ParseElements _elems[] =
        {
            { L"effect",
                [this, pEffects](IXMLDOMNode* pNode)
                {
                    ParseEffect(pNode, pEffects);
                }
            },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _elems);
    }

    void CColladaImporter::ParseCollada(IXMLDOMNode *pNode, Caustic::ISceneGraph **ppSceneGraph)
    {
        Collada::SCollada *pCollada = new Collada::SCollada();
        std::unique_ptr<Collada::SCollada> spCollada(pCollada);
        CColladaImporter::ParseElements _Elements[] =
        {
            { L"library_cameras", [this, pCollada](IXMLDOMNode* pNode) { ParseLibraryCameras(pNode, &pCollada->m_cameras); } },
            { L"library_lights", [this, pCollada](IXMLDOMNode* pNode) { ParseLibraryLights(pNode, &pCollada->m_lights); } },
            { L"library_effects", [this, pCollada](IXMLDOMNode* pNode) { ParseLibraryEffects(pNode, &pCollada->m_effects); } },
            { L"library_images", [this](IXMLDOMNode* pNode) {} },
            { L"library_materials", [this, pCollada](IXMLDOMNode* pNode) { ParseLibraryMaterials(pNode, &pCollada->m_materials); } },
            { L"library_geometries", [this, pCollada](IXMLDOMNode *pNode) { ParseLibraryGeometries(pNode, &pCollada->m_geometries); } },
            { L"library_visual_scenes", [this, pCollada](IXMLDOMNode *pNode) { ParseLibraryVisualScenes(pNode, &pCollada->m_visualScenes); } },
            { L"scene", [this, pCollada, ppSceneGraph](IXMLDOMNode *pNode) { ParseScene(pNode, pCollada, ppSceneGraph); } },
            { nullptr, nullptr }
        };
        ParseSubnode(pNode, _Elements);
    }

    void CColladaImporter::Import(const char *fn, Caustic::ISceneGraph **ppSceneGraph)
    {
        CComPtr<IXMLDOMDocument> spXMLDom;
        CT(CoCreateInstance(__uuidof(DOMDocument), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&spXMLDom));
        CT(spXMLDom->put_async(VARIANT_FALSE));
        CT(spXMLDom->put_validateOnParse(VARIANT_FALSE));
        CT(spXMLDom->put_resolveExternals(VARIANT_FALSE));
        CComBSTR bstrFn(fn);
        CComVariant varFn = bstrFn;
        VARIANT_BOOL varResult;
        CT(spXMLDom->load(varFn, &varResult));
        if (varResult == VARIANT_TRUE)
        {
            CComPtr<IXMLDOMElement> spElem;
            CT(spXMLDom->get_documentElement(&spElem));
            ParseCollada(spElem, ppSceneGraph);
        }
        else
        {
            CComPtr<IXMLDOMParseError> spErr;
            spXMLDom->get_parseError(&spErr);
            CComBSTR bstrErr;
            spErr->get_reason(&bstrErr);
#ifdef _DEBUG
            OutputDebugString(bstrErr);
#endif
        }
    }
}
