NDSummary.OnToolTipsLoaded("File:Libraries/Geometry/Mesh/IMesh.h",{792:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">IMaterial is used for manipulating the materials assigned to a mesh.&nbsp; Each ISubMesh may have a single material assigned to it.</div></div>",793:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype793\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> Vector3 GetAmbientColor() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the ambient color for the material</div></div>",794:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype794\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> Vector3 GetDiffuseColor() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the diffuse color for the material</div></div>",795:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype795\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> Vector3 GetSpecularColor() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the specular color for the material</div></div>",796:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype796\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual float</span> GetSpecularExp() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the specular color\'s exponent</div></div>",797:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype797\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual float</span> GetAlpha() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the alpha value for the material. 0.0 = transparent; 1.0 = opaque</div></div>",798:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype798\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetMaterialID() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns a unique Id associated with this material</div></div>",799:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype799\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAmbientColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the ambient color</div></div>",800:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype800\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDiffuseColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the diffuse color</div></div>",801:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype801\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetSpecularColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the specular color</div></div>",802:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype802\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetSpecularExp(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the specular exponent</div></div>",803:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype803\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAlpha(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the alpha component</div></div>",804:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype804\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMaterialID(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the material\'s unique ID</div></div>",805:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype805\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAmbientTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the ambient texture</div></div>",806:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype806\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDiffuseTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the diffuse texture</div></div>",807:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype807\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetSpecularTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the specular texture</div></div>",808:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype808\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetAmbientTexture() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the ambient texture filename</div></div>",809:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype809\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetDiffuseTexture() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the diffuse texture filename</div></div>",810:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype810\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetSpecularTexture() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the specular texture filename</div></div>",811:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype811\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CHalfEdge</div></div></div><div class=\"TTSummary\">Defines a single half edge on our graph</div></div>",812:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype812\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CFace</div></div></div><div class=\"TTSummary\">Defines a face in our CMesh</div></div>",814:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Define flags describing what fields are available in a SVertex</div></div>",815:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines the method used for converting a polygonal mesh into a trianglar mesh.</div></div>",816:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines various flags associated with an ISubMesh</div></div>",817:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">ISubMesh is used by clients for manipulating a submesh. A submesh contains actual mesh data. This is different from a mesh object which is just a collection of submesh objects.</div></div>",819:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">IMesh is simply a collection of submesh objects (ISubMesh).</div></div>"});