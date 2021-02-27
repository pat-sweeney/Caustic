NDSummary.OnToolTipsLoaded("CClass:IShader",{704:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",559:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype559\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShader&gt; Clone(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ID3D11Device&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDevice</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Clones the shader. This is useful when you want to reuse a shader (such as those returned by IShaderMgr::FindShader) but want the shader to have its own set of parameters.</div></div>",706:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype706\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::wstring &amp;Name() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Name of shader</div></div>",565:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype565\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BeginRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Starts rendering.</div></div>",707:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype707\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader parameter</div></div>",1404:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1404\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader float parameter</div></div>",1422:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1422\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",1423:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1423\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a vertex shader parameter</div></div>",1424:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1424\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a vertex shader float parameter</div></div>",1425:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1425\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",1426:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1426\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a computer shader parameter</div></div>",1427:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1427\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a vertex shader float parameter</div></div>",1428:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1428\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",1429:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1429\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader parameter that is part of an array</div></div>",1430:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1430\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a vertex shader parameter that is part of an array</div></div>",1431:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1431\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets a compute shader parameter that is part of an array</div></div>",714:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype714\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> EndRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Ends rendering. Called after the rendering using the shader has completed.</div></div>",715:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype715\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShaderInfo&gt; GetShaderInfo() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the shader info associated with the shader</div></div>",716:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype716\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetThreadCounts(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">xThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">yThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">zThreads</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">This is a bit of a hack. We need some way to specify how many threads to execute for the compute shader. This needs to be rethought (maybe make specific class for shader with underlying compute shader??)</div></div>"});