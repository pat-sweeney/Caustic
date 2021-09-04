﻿NDSummary.OnToolTipsLoaded("File:Geometry/Mesh/IMesh.h",{330:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Defines interface for mesh.</div></div>",331:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype331\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">CFaceVertex</div></div></div><div class=\"TTSummary\">Defines a vertex data relative to a given face.&nbsp; See CGeomVertex for further information.</div></div>",332:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype332\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CHalfEdge</div></div></div><div class=\"TTSummary\">Defines a single half edge on our graph</div></div>",334:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype334\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CHalfEdge *GetNextEdge()</div></div><div class=\"TTSummary\">Returns the next edge in our edge loop</div></div>",335:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype335\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CHalfEdge *GetPrevEdge()</div></div><div class=\"TTSummary\">Returns the previous edge in our edge loop</div></div>",336:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype336\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CHalfEdge *GetOppositeEdge()</div></div><div class=\"TTSummary\">Returns the half edge opposite to the current edge</div></div>",337:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype337\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CGeomVertex *GetHeadVertex()</div></div><div class=\"TTSummary\">Returns the vertex at the head of this edge</div></div>",338:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype338\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CGeomVertex *GetTailVertex()</div></div><div class=\"TTSummary\">Returns the vertex at the tail of this edge</div></div>",339:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype339\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CFace *GetFace()</div></div><div class=\"TTSummary\">Returns the face this edge is a boundary of</div></div>",340:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype340\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">uint32</span> GetSmoothingGroup()</div></div><div class=\"TTSummary\">Returns the smoothing group this edge belongs to</div></div>",341:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype341\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CFace</div></div></div><div class=\"TTSummary\">Defines a face in our CMesh</div></div>",343:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Define flags describing what fields are available in a SVertex</div></div>",344:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines the method used for converting a polygonal mesh into a trianglar mesh.</div></div>",345:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype345\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">enum</span> </div><div class=\"CPName\">EMeshFlags</div></div></div><div class=\"TTSummary\">Defines various flags associated with an ISubMesh</div></div>",346:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">ISubMesh is used by clients for manipulating a submesh. A submesh contains actual mesh data. This is different from a mesh object which is just a collection of submesh objects.</div></div>",310:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype310\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> RayIntersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">Ray3&amp;&nbsp;</td><td class=\"PName last\">ray,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersection,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib**&nbsp;</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IMaterialAttrib&gt;&gt;&nbsp;</td><td class=\"PName last\">materials</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Computes the intersection between a ray and a mesh.</div></div>",312:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype312\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetName(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>*&nbsp;</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the name of the submesh</div></div>",1560:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1560\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of the submesh</div></div>",1561:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1561\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetNumberFaces() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of faces in the submesh</div></div>",1562:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1562\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CFace *GetFace(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns a pointer to the requested face</div></div>",1563:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1563\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetNumberVertices() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns number of vertices in submesh</div></div>",1564:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1564\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CGeomVertex *GetVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns a requested vertex</div></div>",1565:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1565\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetNumberEdges() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns number of edges in submesh</div></div>",1566:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1566\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CHalfEdge *GetEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the requested edge</div></div>",1567:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1567\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> EVertexFlags GetVertexFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns vertex flags (which describe what fields a vertex contains)</div></div>",1568:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1568\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVertexFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">EVertexFlags&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets which fields a vertex contains</div></div>",1569:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1569\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> EMeshFlags GetMeshFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns mesh flags (set of flags describing attributes of the submesh)</div></div>",1570:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1570\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMeshFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">EMeshFlags&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the mesh flags on the submesh</div></div>",1571:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1571\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetMaterialID() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the material ID associated with the submesh</div></div>",1572:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1572\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMaterialID(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">materialID</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the material ID for the submesh</div></div>",1573:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1573\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual const</span> BBox3 &amp;GetBBox() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the bounding box of the submesh</div></div>",348:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype348\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Normalize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">BBox3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">bbox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Rescales a mesh so fits inside a -0.5..+0.5 bounding box</div></div>",646:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype646\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ComputeNormals() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">This method will compute all the vertex normals on the mesh by summing and normalizing the face vertices around each vertex (via Newell\'s method). NOTE: This makes an implicit assumption that each face is planar. This is not always the case and can thus result in incorrect normals (for instance, the faces from CreateSphere() will not be planar).&nbsp; To prevent incorrect normals, it is first recommended that the mesh be triangulated first via CSubMesh::Triangulate().</div></div>",1574:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1574\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual uint32</span> VertexToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVertex</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Given a vertex this returns the index of that vertex</div></div>",1575:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1575\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual uint32</span> FaceToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CFace&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFace</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Given a face this returns the index of that face</div></div>",1576:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1576\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual uint32</span> EdgeToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pEdge</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Given a edge this returns the index of that edge</div></div>",1556:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1556\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Triangulate(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ETriangulateMethod&nbsp;</td><td class=\"PName last\">method</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Converts a mesh from arbitrary polygons into a triangulated mesh.</div></div>",1557:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1557\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderSubMesh&gt; ToRenderSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Converts a CSubMesh object into a renderable form</div></div>",349:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">IMesh is simply a collection of submesh objects (ISubMesh).</div></div>",1578:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1578\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CAUSTICAPI CRefObj&lt;ISubMesh&gt; CreateEmptySubMesh()</div></div><div class=\"TTSummary\">Creates a empty submesh.</div></div>"});