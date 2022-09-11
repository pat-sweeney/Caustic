﻿NDSummary.OnToolTipsLoaded("CClass:IMesh",{429:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">IMesh is simply a collection of submesh objects (ISubMesh).</div></div>",1696:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1696\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> RayIntersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Ray3&amp;&nbsp;</td><td class=\"PName last\">ray,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersection,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>*&nbsp;</td><td class=\"PName last\">pMaterialID</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Computes the intersection of a ray with a mesh</div></div>",857:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype857\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberSubMeshes() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns number of submeshes in the mesh</div></div>",1697:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1697\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISubMesh&gt; GetSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth submesh</div></div>",1698:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1698\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSubMesh</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a submesh to the mesh</div></div>",1699:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1699\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the bounding box for the mesh</div></div>",1700:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1700\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Normalize() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Rescales a mesh so that it has unit size</div></div>",1701:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1701\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMaterials(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IMaterialAttrib&gt;&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">materials</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the mesh\'s material list</div></div>",1702:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1702\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetNumberMaterials() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns number of materials currently assigned to mesh</div></div>",1703:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1703\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IMaterialAttrib&gt; GetMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">materialID</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth material</div></div>",1704:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1704\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ComputeNormals() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Computes normal vectors for each vertex on the mesh</div></div>",1705:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1705\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;MeshData&gt; ToMeshData(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">ID3D11Device*&nbsp;</td><td class=\"PName last\">pDevice,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;D3D11_INPUT_ELEMENT_DESC&gt;&amp;&nbsp;</td><td class=\"PName last\">vertexLayout,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">vertexSize</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Converts a CMesh object into a renderable form</div></div>"});