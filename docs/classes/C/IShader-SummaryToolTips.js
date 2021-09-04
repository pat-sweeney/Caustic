﻿NDSummary.OnToolTipsLoaded("CClass:IShader",{929:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",931:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype931\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShader&gt; Clone(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ID3D11Device*&nbsp;</td><td class=\"PName last\">pDevice</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Clones the shader. This is useful when you want to reuse a shader (such as those returned by IShaderMgr::FindShader) but want the shader to have its own set of parameters.</div></div>",932:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype932\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::wstring &amp;Name() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Name of shader</div></div>",933:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype933\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BeginRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Starts rendering.</div></div>",934:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype934\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader parameter</div></div>",935:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype935\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader float parameter</div></div>",936:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype936\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",937:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype937\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a vertex shader parameter</div></div>",938:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype938\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a vertex shader float parameter</div></div>",939:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype939\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",940:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype940\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a computer shader parameter</div></div>",941:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype941\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a vertex shader float parameter</div></div>",942:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype942\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",943:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype943\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader parameter that is part of an array</div></div>",944:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype944\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a vertex shader parameter that is part of an array</div></div>",945:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype945\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a compute shader parameter that is part of an array</div></div>",946:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype946\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> EndRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Ends rendering. Called after the rendering using the shader has completed.</div></div>",947:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype947\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShaderInfo&gt; GetShaderInfo() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the shader info associated with the shader</div></div>",948:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype948\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Dispatch(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">xThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">yThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">zThreads</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">This runs the compute shader, if any.</div></div>"});