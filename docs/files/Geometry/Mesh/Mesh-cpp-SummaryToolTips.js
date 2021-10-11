﻿NDSummary.OnToolTipsLoaded("File:Geometry/Mesh/Mesh.cpp",{321:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CMesh object.</div></div>",323:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype323\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CFace::ComputeFaceNormal()</div></div><div class=\"TTSummary\">Computes the normal for a face using Newell\'s method (so convex polygons are handled correctly)</div></div>",324:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype324\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> CFace::IsDegenerate()</div></div><div class=\"TTSummary\">Returns whether the face is degenerate (based on the normal vector)</div></div>",325:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype325\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CMesh::CMesh()</div></div><div class=\"TTSummary\">Default ctor for our mesh object. A mesh is simply a collection of submeshes (submeshes define geometry and material attributes)</div></div>",326:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype326\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> CMesh::RayIntersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Ray3&amp;&nbsp;</td><td class=\"PName last\">ray,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersection,</td></tr><tr><td class=\"PType first\">IMaterialAttrib**&nbsp;</td><td class=\"PName last\">pMaterial</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Computes the intersection of a ray with a mesh</div></div>",327:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype327\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ISubMesh&gt; CMesh::GetSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth submesh</div></div>",328:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype328\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::AddSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSubMesh</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds a submesh to the mesh</div></div>",329:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype329\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the bounding box for the mesh</div></div>",330:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype330\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CMesh::Normalize()</div></div><div class=\"TTSummary\">Rescales a mesh so that it has unit size</div></div>",331:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype331\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::SetMaterials(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IMaterialAttrib&gt;&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">materials</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the mesh\'s material list</div></div>",332:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype332\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">uint32</span> CMesh::GetNumberMaterials()</div></div><div class=\"TTSummary\">Returns number of materials currently assigned to mesh</div></div>",333:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype333\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IMaterialAttrib&gt; CMesh::GetMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">materialID</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth material</div></div>",334:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype334\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::Load(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Loads a previously save Mesh</div></div>",335:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype335\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::Store(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Stores a mesh to a stream</div></div>",336:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype336\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CMesh::ComputeNormals()</div></div><div class=\"TTSummary\">Computes normal vectors for each vertex on the mesh</div></div>",337:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype337\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CAUSTICAPI CRefObj&lt;IMesh&gt; CreateEmptyMesh()</div></div><div class=\"TTSummary\">Returns an empty mesh</div></div>"});