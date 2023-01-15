﻿NDSummary.OnToolTipsLoaded("CClass:CSubMesh",{366:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype366\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CSubMesh</div></div></div><div class=\"TTSummary\">Defines a submesh. A submesh is a child of a mesh object and contains the actual mesh data (collection of vertices and edges).</div></div>",368:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype368\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *FindEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pHead,</td></tr><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTail</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the edge from pTail-&gt;pHead. If the edge does not exist then nullptr is returned.</div></div>",369:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype369\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *FindNextEdgeAroundVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVert,</td></tr><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pEdge,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">entering,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">clockwise</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the next edge around the vertex in the given direction (clockwise or counter-clockwise) that is either entering or leaving the vertex.</div></div>",370:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype370\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CGeomVertex *FindVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pNorm,</td></tr><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pUV</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns a pointer to a vertex in our mesh that matches the specified values</div></div>",371:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype371\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CFace *AllocateFace()</div></div><div class=\"TTSummary\">Allocates a new face and places in our face list.</div></div>",372:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype372\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *AllocateEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pHead,</td></tr><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTail</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Allocates a new edge going from pTail-&gt;pHead. NOTE: This function does not insert the edge into our edge list. That occurs during the call to LinkEdges().</div></div>",373:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype373\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CGeomVertex *AllocateGeomVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">normal,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">uv</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Allocates a new vertex given a position, normal, and UVs and places it in our vertex list.</div></div>",374:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype374\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> LinkEdges(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPrev,</td></tr><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pCur</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Links two edges (full edges) together We start with the picture at the top. The solid lines represent HalfEdge\'s, while the dotted lines represent their m_pPrev/m_pNext links. We are trying to add newEdge and link it to oldEdge (an existing edge)</div></div>",375:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype375\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> BuildVertexBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">ID3D11Device*&nbsp;</td><td></td><td class=\"PName last\">pDevice,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;D3D11_INPUT_ELEMENT_DESC&gt;&amp;&nbsp;</td><td></td><td class=\"PName last\">vertexLayout,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">vertexSize,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;<span class=\"SHKeyword\">int</span>&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">vertexReferenced,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">MeshData&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMeshData</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a vertex buffer using the specified submesh.</div></div>",376:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype376\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> BuildIndexBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">ID3D11Device*&nbsp;</td><td></td><td class=\"PName last\">pDevice,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;<span class=\"SHKeyword\">int</span>&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">vertexReferenced,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">MeshData&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMeshData</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a index buffer using the specified submesh.</div></div>",377:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype377\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CheckConsistency()</div></div><div class=\"TTSummary\">Checks whether the links between edges in the mesh are consistent</div></div>"});