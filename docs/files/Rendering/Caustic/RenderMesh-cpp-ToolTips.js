NDContentPage.OnToolTipsLoaded({61:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",704:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",748:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a material that has its associated render element (as opposed to an IMaterialAttrib which only defines the attributes of the material).</div></div>",758:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the D3D counterpart to an ISubMesh object (i.e. the renderable form of a mesh, that is, a mesh converted from a half-edge structure to a vertex and index buffer).</div></div>",770:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype770\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFrontMaterialOverride,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBackMaterialOverride,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders the mesh</div></div>",777:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype777\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderSubMesh&gt; GetSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Retrieves the Nth submesh</div></div>",778:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype778\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSubMesh</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Adds a submesh to the mesh</div></div>",779:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype779\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Gets the mesh\'s bounding box</div></div>",780:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype780\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMaterials(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IMaterialAttrib&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">materials</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Assigns list of materials that submesh\'s use</div></div>",781:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype781\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IMaterialAttrib&gt; GetMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">materialID</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Retrieves a material</div></div>",783:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype783\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFrontMaterialOverride,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBackMaterialOverride,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders the mesh</div></div>",789:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our basic renderer. IRenderer handles all rendering commands.&nbsp; It is generally expected that this object is running on its own thread.</div></div>",854:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a properties common to all lights</div></div>",878:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Used for manipulating the materials assigned to a mesh.</div></div>"});