﻿NDSummary.OnToolTipsLoaded("File:Geometry/Mesh/IMaterialAttrib.h",{197:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for render materials.</div></div>",515:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Indicates whether a material is used by a pixel or vertex shader or both</div></div>",516:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Used for manipulating the materials assigned to a mesh.</div></div>",518:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype518\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetName(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>*&nbsp;</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the name of the material (mostly used for debugging)</div></div>",519:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype519\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns name of material</div></div>",520:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype520\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCullMode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">D3D11_CULL_MODE&nbsp;</td><td class=\"PName last\">cullMode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the culling mode</div></div>",521:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype521\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_CULL_MODE GetCullMode() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns current culling mode</div></div>",522:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype522\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFillMode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">D3D11_FILL_MODE&nbsp;</td><td class=\"PName last\">renderMode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets fill mode for rendering</div></div>",523:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype523\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_FILL_MODE GetFillMode() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns current fill mode</div></div>",524:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype524\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> GetIsTransparent() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Indicates whether material is considered transparent</div></div>",525:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype525\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetIsTransparent(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">isTransparent</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Allows client to override whether material is considered transparent</div></div>",545:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype545\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> GetIsShadowReceiver() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Indicates whether the surface acts as a shadow receiver</div></div>",612:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype612\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetIsShadowReceiver(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">receivesShadow</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Allows client to set whether the surface is a shadow receiver</div></div>",613:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype613\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> FRGBColor GetColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the color associated with the specified name</div></div>",739:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype739\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the color associated with the specified property name</div></div>",740:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype740\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">FRGBAColor&amp;&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the color associated with the specified property name</div></div>",741:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype741\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual float</span> GetScalar(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Gets a float value associated with the specified property name</div></div>",742:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype742\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetScalar(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">s</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a float value associated with the specified property name</div></div>",743:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype743\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IImage&gt; GetTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the texture with the specified property name</div></div>",879:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype879\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IImage*&nbsp;</td><td></td><td class=\"PName last\">pImage,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">EShaderAccess&nbsp;</td><td></td><td class=\"PName last\">access</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a texture with the specified property name</div></div>",881:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype881\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTextureViaFilename(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">filename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">EShaderAccess&nbsp;</td><td class=\"PName last\">access</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a texture from the spcified file and sets it to the specified property name</div></div>",910:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype910\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> EnumerateColors(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(<span class=\"SHKeyword\">const wchar_t</span>* pName, FRGBAColor &amp; v)&gt;&nbsp;</td><td class=\"PName last\">func</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Enumerates each color currently assigned in this material</div></div>",917:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype917\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> EnumerateScalars(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(<span class=\"SHKeyword\">const wchar_t</span>* pName, <span class=\"SHKeyword\">float</span> s)&gt;&nbsp;</td><td class=\"PName last\">func</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Enumerates each scalar currently assigned in this material</div></div>",919:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype919\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> EnumerateTextures(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(<span class=\"SHKeyword\">const wchar_t</span>* pName, IImage * pTexture, EShaderAccess access)&gt;&nbsp;</td><td class=\"PName last\">func</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Enumerates each texture currently assigned in this material</div></div>",933:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype933\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetMaterialID() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns a unique Id associated with this material</div></div>",946:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype946\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMaterialID(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the material\'s unique ID</div></div>",1073:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1073\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IMaterialAttrib&gt; CreateStandardMaterialAttrib(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">FRGBColor&nbsp;</td><td class=\"PName last\">ambientColor,</td></tr><tr><td class=\"PType first\">FRGBColor&nbsp;</td><td class=\"PName last\">diffuseColor,</td></tr><tr><td class=\"PType first\">FRGBColor&nbsp;</td><td class=\"PName last\">specularColor,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">specularExp,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">alpha</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a standard material (as specified in the Phong lighting model)</div></div>",1074:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1074\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IMaterialAttrib&gt; CreateMaterialAttrib()</div></div><div class=\"TTSummary\">Creates an empty material object</div></div>"});