﻿NDContentPage.OnToolTipsLoaded({89:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype89\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector2</div></div></div><div class=\"TTSummary\">Defines a simple vector with 2 components</div></div>",98:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype98\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector3</div></div></div><div class=\"TTSummary\">Defines a simple vector with 3 components</div></div>",137:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype137\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">RayIntersect3</div></div></div><div class=\"TTSummary\">Defines results of a 3D ray intersection</div></div>",138:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype138\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Ray3</div></div></div><div class=\"TTSummary\">Defines a 3 dimensional ray</div></div>",235:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype235\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">BBox3</div></div></div><div class=\"TTSummary\">Defines a axis-aligned bounding box (in 3D)</div></div>",298:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype298\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">CGeomVertex</div></div></div><div class=\"TTSummary\">Defines a vertex in our mesh.</div></div>",338:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype338\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *FindEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pHead,</td></tr><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTail</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the edge from pTail-&gt;pHead. If the edge does not exist then nullptr is returned.</div></div>",339:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype339\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *FindNextEdgeAroundVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVert,</td></tr><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pEdge,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">entering,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">clockwise</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the next edge around the vertex in the given direction (clockwise or counter-clockwise) that is either entering or leaving the vertex.</div></div>",340:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype340\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CGeomVertex *FindVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pNorm,</td></tr><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pUV</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns a pointer to a vertex in our mesh that matches the specified values</div></div>",341:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype341\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CFace *AllocateFace()</div></div><div class=\"TTSummary\">Allocates a new face and places in our face list.</div></div>",342:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype342\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *AllocateEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pHead,</td></tr><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTail</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Allocates a new edge going from pTail-&gt;pHead. NOTE: This function does not insert the edge into our edge list. That occurs during the call to LinkEdges().</div></div>",343:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype343\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CGeomVertex *AllocateGeomVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">normal,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">uv</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Allocates a new vertex given a position, normal, and UVs and places it in our vertex list.</div></div>",344:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype344\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> LinkEdges(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPrev,</td></tr><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pCur</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Links two edges (full edges) together We start with the picture at the top. The solid lines represent HalfEdge\'s, while the dotted lines represent their m_pPrev/m_pNext links. We are trying to add newEdge and link it to oldEdge (an existing edge)</div></div>",347:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype347\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CheckConsistency()</div></div><div class=\"TTSummary\">Checks whether the links between edges in the mesh are consistent</div></div>",390:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype390\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CHalfEdge</div></div></div><div class=\"TTSummary\">Defines a single half edge on our graph</div></div>",399:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype399\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CFace</div></div></div><div class=\"TTSummary\">Defines a face in our CMesh</div></div>",406:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype406\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> RayIntersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Ray3&amp;&nbsp;</td><td class=\"PName last\">ray,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersection,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>*&nbsp;</td><td class=\"PName last\">pMaterialID</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Computes the intersection between a ray and a mesh.</div></div>",422:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype422\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Normalize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">BBox3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">bbox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Rescales a mesh so fits inside a -0.5..+0.5 bounding box</div></div>",423:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype423\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ComputeNormals() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">This method will compute all the vertex normals on the mesh by summing and normalizing the face vertices around each vertex (via Newell\'s method). NOTE: This makes an implicit assumption that each face is planar. This is not always the case and can thus result in incorrect normals (for instance, the faces from CreateSphere() will not be planar).&nbsp; To prevent incorrect normals, it is first recommended that the mesh be triangulated first via CSubMesh::Triangulate().</div></div>",424:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype424\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual uint32</span> VertexToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVertex</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Given a vertex this returns the index of that vertex</div></div>",425:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype425\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual uint32</span> FaceToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CFace&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFace</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Given a face this returns the index of that face</div></div>",426:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype426\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual uint32</span> EdgeToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pEdge</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Given a edge this returns the index of that edge</div></div>",427:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype427\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Triangulate(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ETriangulateMethod&nbsp;</td><td class=\"PName last\">method</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Converts a mesh from arbitrary polygons into a triangulated mesh.</div></div>"});