﻿NDSummary.OnToolTipsLoaded("File:Geometry/Mesh/Submesh.cpp",{308:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CSubMesh object.</div></div>",323:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype323\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> CSubMesh::RayIntersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">Ray3&amp;&nbsp;</td><td class=\"PName last\">ray,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersection,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib**&nbsp;</td><td class=\"PName last\">ppMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IMaterialAttrib&gt;&gt;&nbsp;</td><td class=\"PName last\">materials</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ISubMesh::RayIntersect</div></div>",311:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype311\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CSubMesh::Normalize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">BBox3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">bbox</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ISubMesh::Normalize</div></div>",324:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype324\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CSubMesh::Triangulate(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ETriangulateMethod&nbsp;</td><td class=\"PName last\">method</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ISubMesh::Triangulate</div></div>",313:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype313\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CSubMesh::ComputeNormals()</div></div><div class=\"TTSummary\">See ISubMesh::ComputeNormals</div></div>",644:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype644\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">uint32</span> CSubMesh::VertexToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVertex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ISubMesh::VertexToIndex</div></div>",1558:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1558\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">uint32</span> CSubMesh::EdgeToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pEdge</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ISubMesh::EdgeToIndex</div></div>",1559:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1559\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">uint32</span> CSubMesh::FaceToIndex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CFace&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFace</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ISubMesh::FaceToIndex</div></div>",1587:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1587\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CFace *CSubMesh::AllocateFace()</div></div><div class=\"TTSummary\">See CSubMesh::AllocateFace</div></div>",1588:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1588\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CGeomVertex *CSubMesh::AllocateGeomVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">normal,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">uv</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See CSubMesh::AllocateGeomVertex</div></div>",1589:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1589\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *CSubMesh::AllocateEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pHead,</td></tr><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTail</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See CSubMesh::AllocateEdge</div></div>",1590:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1590\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *CSubMesh::FindEdge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pHead,</td></tr><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTail</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See CSubMesh::FindEdge</div></div>",1591:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1591\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CGeomVertex *CSubMesh::FindVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pNorm,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pUV</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See CSubMesh::FindVertex</div></div>",1592:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1592\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CHalfEdge *CSubMesh::FindNextEdgeAroundVertex(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVert,</td></tr><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pEdge,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">entering,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">clockwise</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See CSubMesh::FindNextEdgeAroundVertex</div></div>",1593:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1593\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CSubMesh::LinkEdges(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPrev,</td></tr><tr><td class=\"PType first\">CHalfEdge&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pCur</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See CSubMesh::LinkEdges</div></div>",1594:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1594\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CSubMesh::CheckConsistency()</div></div><div class=\"TTSummary\">See CSubMesh::CheckConsistency</div></div>",1595:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1595\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CSubMesh::Load(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;ISubMesh::Load&gt;</div></div>",1596:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">See &lt;ISubMesh::Store&gt;</div></div>"});