﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IShader.h",{826:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for a shader.</div></div>",827:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface to a buffer used by the GPU. When clients need to pass data to a compute shader (i.e. as either a StructuredBuffer, RWStructuredBuffer, AppendStructuredBuffer, or RWByteAddressBuffer) they will pass one of these objects via IShader::SetCSParam().</div></div>",829:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype829\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CopyFromCPU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>*&nbsp;</td><td class=\"PName last\">pData</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">CopyFromCPU() is used to move data from the CPU (i.e. client data) to the underlying GPU buffer.</div></div>",830:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype830\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CopyToCPU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>*&nbsp;</td><td class=\"PName last\">pData</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">CopyToCPU() is used to move data from the CPU (i.e. client data) to the underlying GPU buffer.</div></div>",831:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype831\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IGPUBuffer&gt; CreateGPUBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">EBufferType&nbsp;</td><td class=\"PName last\">type,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">numElems,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">elemSize,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a buffer on the GPU</div></div>",832:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",834:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype834\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShader&gt; Clone(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ID3D11Device*&nbsp;</td><td class=\"PName last\">pDevice</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Clones the shader. This is useful when you want to reuse a shader (such as those returned by IShaderMgr::FindShader) but want the shader to have its own set of parameters.</div></div>",835:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype835\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::wstring &amp;Name() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Name of shader</div></div>",836:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype836\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BeginRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Starts rendering.</div></div>",837:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype837\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader parameter</div></div>",838:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype838\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader float parameter</div></div>",839:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype839\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",840:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype840\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a vertex shader parameter</div></div>",841:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype841\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a vertex shader float parameter</div></div>",842:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype842\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",843:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype843\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a computer shader parameter</div></div>",844:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype844\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a vertex shader float parameter</div></div>",845:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype845\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader int parameter</div></div>",846:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype846\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel shader parameter that is part of an array</div></div>",847:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype847\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a vertex shader parameter that is part of an array</div></div>",848:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype848\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">any&amp;&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a compute shader parameter that is part of an array</div></div>",849:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype849\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> EndRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Ends rendering. Called after the rendering using the shader has completed.</div></div>",850:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype850\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShaderInfo&gt; GetShaderInfo() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the shader info associated with the shader</div></div>",851:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype851\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Dispatch(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">xThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">yThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">zThreads</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">This runs the compute shader, if any.</div></div>"});