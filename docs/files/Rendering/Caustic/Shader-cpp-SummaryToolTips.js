NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/Shader.cpp",{1231:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1231\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IShader&gt; CShader::Clone(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ID3D11Device&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDevice</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IShader::Clone</div></div>",710:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype710\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">uint32</span> CShader::ComputeParamSize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">ShaderParamDef&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDefs,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">numParams,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">map&lt;std::wstring, ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Parses the definitions of each shader parameter that was read from the HLSL (using ParseShader.exe). For each definition we will compute the offset of the parameter within the constant buffer. This is used later when updating the constant buffer.</div></div>",711:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype711\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::PushConstants(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">SBuffer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBuffer,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">map&lt;std::wstring, ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Pushes each shader parameter into the D3D11 constant buffer.</div></div>",554:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype554\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::PopBuffers(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">PopBuffers copies GPU buffers back to the CPU (after a compute shader has written to them).</div></div>",712:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype712\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::PushBuffers(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">map&lt;std::wstring, ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Pushes buffers from the CPU to the GPU (for instance, from a CPU buffer to a GPU structured buffer).</div></div>",713:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype713\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">map&lt;std::wstring, ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets shader constant, texture or sampler</div></div>",1350:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1350\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">map&lt;std::wstring, ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets shader constant, texture or sampler</div></div>",1351:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1351\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader parameter</div></div>",1352:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1352\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetPSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IShader::SetPSParamFloat</div></div>",1353:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1353\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetPSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IShader::SetPSParamInt</div></div>",1354:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1354\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader array element parameter</div></div>",1355:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1355\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a vertex shader parameter</div></div>",1356:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1356\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetVSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IShader::SetVSParamFloat</div></div>",1357:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1357\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetVSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IShader::SetVSParamInt</div></div>",1432:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1432\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a vertex shader array element parameter</div></div>",1433:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1433\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a compute shader parameter</div></div>",1434:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1434\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetCSParamFloat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IShader::SetCSParamFloat</div></div>",1435:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1435\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetCSParamInt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IShader::SetCSParamInt</div></div>",1436:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1436\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span> std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a compute shader array element parameter</div></div>",564:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype564\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetThreadCounts(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">xThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">yThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">zThreads</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Specifies the number of thread groups to run when dispatching the shader\'s compute shader.</div></div>",889:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype889\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::BeginRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">BeginRender is called before rendering using this shader occurs.&nbsp; This call is responsible for setting up the pRenderer device to use the shader.</div></div>",798:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype798\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::EndRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PName first last\">IRenderer *&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">EndRender is called after rendering using this shader occurs.&nbsp; During this call the shader may clean up any state/memory it needed</div></div>",567:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype567\" class=\"NDPrototype NoParameterForm\">CRefObj&lt;IShaderInfo&gt; CShader::GetShaderInfo()</div><div class=\"TTSummary\">Returns information about the shader</div></div>",568:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype568\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::CreateBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ID3D11Device&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDevice,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">bufSize,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">bindFlags,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">cpuAccessFlags,</td></tr><tr><td class=\"PType first\">D3D11_USAGE&nbsp;</td><td></td><td class=\"PName last\">usage,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">miscFlags,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">stride,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">alignment,</td></tr><tr><td class=\"PType first\">SBuffer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBuffer,</td></tr><tr><td class=\"PType first\">ID3D11Buffer&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppBuffer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Helper function that creates a buffer for the shader (Constant buffer or StructuredBuffer)</div></div>",1437:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1437\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::CreateConstantBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">ID3D11Device&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDevice,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ShaderParamDef&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDefs,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">paramsSize,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">map&lt;std::wstring, ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">SBuffer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pConstantBuffer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">CreateConstantBuffer creates the constant buffer (pixel or vertex shader)</div></div>",571:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype571\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::Create(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderInfo,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPSBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pVSBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCSBlob</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates a shader</div></div>",572:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype572\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;IShader&gt; CreateShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderInfo,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPSBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pVSBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCSBlob</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">A helper function to create a shader</div></div>",573:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype573\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IShader&gt; CShaderMgr::FindShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Returns the requested shader</div></div>",574:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype574\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShaderMgr::RegisterShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Registers the specified shader with the shader manager</div></div>"});