﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IRenderMaterial.ixx",{1008:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for render materials.</div></div>",1009:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a material that has its associated render element (as opposed to an IMaterialAttrib which only defines the attributes of the material).</div></div>",1011:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1011\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets shader to use for this material</div></div>",1012:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1012\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShader&gt; GetShader() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Gets shader used by this material</div></div>",1013:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1013\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the material definition</div></div>",1014:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1014\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IMaterialAttrib&gt; GetMaterial() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the material definition used by this material</div></div>",1015:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1015\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture*&nbsp;</td><td></td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">EShaderAccess&nbsp;</td><td></td><td class=\"PName last\">access</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a texture</div></div>",1016:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1016\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the texture with the specified name</div></div>",1017:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1017\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderCtx,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pOverrideShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the material (pushes it to the shader)</div></div>"});