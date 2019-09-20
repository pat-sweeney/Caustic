NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IShaderInfo.h",{401:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for IShaderInfo.&nbsp; Each shader has an associated IShaderInfo that defines which parameters and vertex formats the shader uses.</div></div>",399:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines primary interface used for getting information about pre-compiled shaders. This isn\'t the shader itself. It is simply the data that we load from disk to create a shader (i.e. data from .shi file)</div></div>",451:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype451\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_PRIMITIVE_TOPOLOGY GetTopologyType() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the type of mesh this shader renders (triangles/points/lines/etc)</div></div>",452:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype452\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ShaderParamDef&gt; &amp;PixelShaderParameterDefs() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns list of pixel parameter definitions that the shader uses</div></div>",708:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype708\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ShaderParamDef&gt; &amp;VertexShaderParameterDefs() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns list of vertex parameter definitions that the shader uses</div></div>",709:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype709\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;D3D11_INPUT_ELEMENT_DESC&gt; &amp;VertexLayout() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the vertex format for the shader</div></div>",710:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype710\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetVertexSize() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the size of each vertex in bytes</div></div>"});