NDSummary.OnToolTipsLoaded("File:Geometry/Mesh/Mesh.cpp",{306:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CMesh object.</div></div>",308:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype308\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CFace::ComputeFaceNormal()</div><div class=\"TTSummary\">Computes the normal for a face using Newell\'s method (so convex polygons are handled correctly)</div></div>",309:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype309\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> CFace::IsDegenerate()</div><div class=\"TTSummary\">Returns whether the face is degenerate (based on the normal vector)</div></div>",310:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype310\" class=\"NDPrototype NoParameterForm\">CMesh::CMesh()</div><div class=\"TTSummary\">Default ctor for our mesh object. A mesh is simply a collection of submeshes (submeshes define geometry and material attributes)</div></div>",311:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype311\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ISubMesh&gt; CMesh::GetSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Returns the Nth submesh</div></div>",312:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype312\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::AddSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSubMesh</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Adds a submesh to the mesh</div></div>",313:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype313\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Returns the bounding box for the mesh</div></div>",314:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype314\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CMesh::Normalize()</div><div class=\"TTSummary\">Rescales a mesh so that it has unit size</div></div>",315:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype315\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::SetMaterials(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IMaterialAttrib&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">materials</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets the mesh\'s material list</div></div>",316:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype316\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">uint32</span> CMesh::GetNumberMaterials()</div><div class=\"TTSummary\">Returns number of materials currently assigned to mesh</div></div>",317:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype317\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IMaterialAttrib&gt; CMesh::GetMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">materialID</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Returns the Nth material</div></div>",318:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype318\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::Load(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Loads a previously save Mesh</div></div>",319:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype319\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CMesh::Store(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Stores a mesh to a stream</div></div>",320:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype320\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CMesh::ComputeNormals()</div><div class=\"TTSummary\">Computes normal vectors for each vertex on the mesh</div></div>",321:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype321\" class=\"NDPrototype NoParameterForm\">CAUSTICAPI CRefObj&lt;IMesh&gt; CreateEmptyMesh()</div><div class=\"TTSummary\">Returns an empty mesh</div></div>"});