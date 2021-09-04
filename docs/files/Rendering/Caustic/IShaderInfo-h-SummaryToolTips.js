﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IShaderInfo.h",{893:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for IShaderInfo.&nbsp; Each shader has an associated IShaderInfo that defines which parameters and vertex formats the shader uses.</div></div>",895:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines the various types of shaders</div></div>",896:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines primary interface used for getting information about pre-compiled shaders. This isn\'t the shader itself. It is simply the data that we load from disk to create a shader (i.e. data from .shi file)</div></div>",898:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype898\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> PSUsesVariable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns whether the Pixel shader uses the specified variable</div></div>",899:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype899\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> VSUsesVariable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns whether the Vertex shader uses the specified variable</div></div>",900:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype900\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> HasShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">EShaderType&nbsp;</td><td class=\"PName last\">type</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns whether the Caustic shader contains the specified D3D shader type</div></div>",901:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype901\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_PRIMITIVE_TOPOLOGY GetTopologyType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the type of mesh this shader renders (triangles/points/lines/etc)</div></div>",902:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype902\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ShaderParamDef&gt;&amp; PixelShaderParameterDefs() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns list of pixel parameter definitions that the shader uses</div></div>",903:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype903\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ShaderParamDef&gt;&amp; VertexShaderParameterDefs() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns list of vertex parameter definitions that the shader uses</div></div>",904:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype904\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ShaderParamDef&gt;&amp; ComputeShaderParameterDefs() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns list of compute parameter definitions that the shader uses</div></div>",905:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype905\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> GetPixelShaderParameterDef(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pParamName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ShaderParamDef*&nbsp;</td><td class=\"PName last\">pDef</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns information about a specific variable found in the shader</div></div>",906:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype906\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> GetVertexShaderParameterDef(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pParamName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ShaderParamDef*&nbsp;</td><td class=\"PName last\">pDef</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns information about a specific variable found in the shader</div></div>",907:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype907\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> GetComputeShaderParameterDef(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pParamName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ShaderParamDef*&nbsp;</td><td class=\"PName last\">pDef</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns information about a specific variable found in the shader</div></div>",908:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype908\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;D3D11_INPUT_ELEMENT_DESC&gt; &amp;VertexLayout() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the vertex format for the shader</div></div>",909:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype909\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetVertexSize() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the size of each vertex in bytes</div></div>",910:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype910\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetThreadGroupSize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>*&nbsp;</td><td class=\"PName last\">pXThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>*&nbsp;</td><td class=\"PName last\">pYThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>*&nbsp;</td><td class=\"PName last\">pZThreads</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div></div>"});