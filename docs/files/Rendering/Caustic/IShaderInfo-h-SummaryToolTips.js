NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IShaderInfo.h",{663:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for IShaderInfo.&nbsp; Each shader has an associated IShaderInfo that defines which parameters and vertex formats the shader uses.</div></div>",665:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines the various types of shaders</div></div>",666:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines primary interface used for getting information about pre-compiled shaders. This isn\'t the shader itself. It is simply the data that we load from disk to create a shader (i.e. data from .shi file)</div></div>",668:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype668\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> HasShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">EShaderType&nbsp;</td><td class=\"PName last\">type</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns whether the Caustic shader contains the specified D3D shader type</div></div>",669:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype669\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_PRIMITIVE_TOPOLOGY GetTopologyType() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the type of mesh this shader renders (triangles/points/lines/etc)</div></div>",670:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype670\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ShaderParamDef&gt; &amp;PixelShaderParameterDefs() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns list of pixel parameter definitions that the shader uses</div></div>",671:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype671\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ShaderParamDef&gt;&amp; VertexShaderParameterDefs() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns list of vertex parameter definitions that the shader uses</div></div>",672:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype672\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ShaderParamDef&gt;&amp; ComputeShaderParameterDefs() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns list of compute parameter definitions that the shader uses</div></div>",673:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype673\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;D3D11_INPUT_ELEMENT_DESC&gt; &amp;VertexLayout() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the vertex format for the shader</div></div>",674:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype674\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetVertexSize() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the size of each vertex in bytes</div></div>",675:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype675\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetThreadGroupSize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pXThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pYThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pZThreads</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div>"});