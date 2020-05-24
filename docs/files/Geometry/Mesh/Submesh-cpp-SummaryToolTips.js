NDSummary.OnToolTipsLoaded("File:Geometry/Mesh/Submesh.cpp",{222:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CSubMesh object.</div></div>",224:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype224\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CSubMesh::Normalize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">BBox3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">bbox</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ISubMesh::Normalize</div></div>",225:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype225\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CSubMesh::Triangulate(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ETriangulateMethod&nbsp;</td><td class=\"PName last\">method</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Converts a mesh from arbitrary polygons into a triangulated mesh.</div></div>",226:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype226\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CSubMesh::ComputeNormals()</div><div class=\"TTSummary\">This method will compute all the vertex normals on the mesh by summing and normalizing the face vertices around each vertex (via Newell\'s method). NOTE: This makes an implicit assumption that each face is planar. This is not always the case and can thus result in incorrect normals (for instance, the faces from CreateSphere() will not be planar).&nbsp; To prevent incorrect normals, it is first recommended that the mesh be triangulated first via CSubMesh::Triangulate().</div></div>",227:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype227\" class=\"NDPrototype NoParameterForm\">CFace *CSubMesh::AllocateFace()</div><div class=\"TTSummary\">Allocates a new face and places in our face list.</div></div>",228:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype228\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CGeomVertex *CSubMesh::AllocateGeomVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">normal,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">uv</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Allocates a new vertex given a position, normal, and UVs and places it in our vertex list.</div></div>",229:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype229\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *CSubMesh::AllocateEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pHead,</td></tr><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTail</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Allocates a new edge going from pTail-&gt;pHead. NOTE: This function does not insert the edge into our edge list. That occurs during the call to LinkEdges().</div></div>",230:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype230\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *CSubMesh::FindEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pHead,</td></tr><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTail</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Returns the edge from pTail-&gt;pHead. If the edge does not exist then nullptr is returned.</div></div>",231:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype231\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CGeomVertex *CSubMesh::FindVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pNorm,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pUV</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Returns a pointer to a vertex in our mesh that matches the specified values</div></div>",232:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype232\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *CSubMesh::FindNextEdgeAroundVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pVert,</td></tr><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pEdge,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">entering,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">clockwise</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Returns the next edge around the vertex in the given direction (clockwise or counter-clockwise) that is either entering or leaving the vertex.</div></div>",233:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype233\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CSubMesh::LinkEdges(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPrev,</td></tr><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCur</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Links two edges (full edges) together We start with the picture at the top. The solid lines represent HalfEdge\'s, while the dotted lines represent their m_pPrev/m_pNext links. We are trying to add newEdge and link it to oldEdge (an existing edge)</div></div>"});