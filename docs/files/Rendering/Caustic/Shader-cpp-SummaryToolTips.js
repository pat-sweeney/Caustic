NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/Shader.cpp",{343:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype343\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">uint32</span> CShader::ComputeParamSize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">ShaderParamDef&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDefs,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">numParams,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Parses the definitions of each shader parameter that was read from the HLSL (using ParseShader.exe). For each definition we will compute the offset of the parameter within the constant buffer. This is used later when updating the constant buffer.</div></div>",801:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype801\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::PushConstants(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">SBuffer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBuffer,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Pushes each shader parameter into the D3D11 constant buffer.</div></div>",803:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype803\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::PopBuffers(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">PopBuffers copies GPU buffers back to the CPU (after a compute shader has written to them).</div></div>",804:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype804\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::PushBuffers(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Pushes buffers from the CPU to the GPU (for instance, from a CPU buffer to a GPU structured buffer).</div></div>",361:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype361\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td></td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets shader constant, texture or sampler</div></div>",362:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype362\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td></td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets shader constant, texture or sampler</div></div>",363:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype363\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td></td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader parameter</div></div>",805:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype805\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetPSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td></td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a pixel shader array element parameter</div></div>",806:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype806\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td></td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a vertex shader parameter</div></div>",807:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype807\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetVSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td></td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a vertex shader array element parameter</div></div>",813:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype813\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td></td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a compute shader parameter</div></div>",834:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype834\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetCSParam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td></td><td class=\"PName last\">paramName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Sets a compute shader array element parameter</div></div>",835:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype835\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::SetThreadCounts(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">xThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">yThreads,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">zThreads</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Specifies the number of thread groups to run when dispatching the shader\'s compute shader.</div></div>",349:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype349\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::BeginRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFrontMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBackMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IPointLight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">BeginRender is called before rendering using this shader occurs.&nbsp; This call is responsible for setting up the pGraphics device to use the shader.</div></div>",368:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype368\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::EndRender(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PName first last\">IGraphics *&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">EndRender is called after rendering using this shader occurs.&nbsp; During this call the shader may clean up any state/memory it needed</div></div>",836:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype836\" class=\"NDPrototype NoParameterForm\">CRefObj&lt;IShaderInfo&gt; CShader::GetShaderInfo()</div><div class=\"TTSummary\">Returns information about the shader</div></div>",837:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype837\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::CreateBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ID3D11Device&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDevice,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">bufSize,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">bindFlags,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">cpuAccessFlags,</td></tr><tr><td class=\"PType first\">D3D11_USAGE&nbsp;</td><td></td><td class=\"PName last\">usage,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">miscFlags,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">stride,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">alignment,</td></tr><tr><td class=\"PType first\">SBuffer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBuffer,</td></tr><tr><td class=\"PType first\">ID3D11Buffer&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppBuffer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Helper function that creates a buffer for the shader (Constant buffer or StructuredBuffer)</div></div>",838:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype838\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::CreateConstantBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">ID3D11Device&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDevice,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ShaderParamDef&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDefs,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">paramsSize,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;ShaderParamInstance&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">params,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">SBuffer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pConstantBuffer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">CreateConstantBuffer creates the constant buffer (pixel or vertex shader)</div></div>",839:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">CreateConstantBuffer creates the constant buffer (pixel or vertex shader)</div></div>",840:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype840\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShader::Create(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderInfo,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPSBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pVSBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCSBlob</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates a shader</div></div>",379:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype379\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;IShader&gt; CreateShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderInfo,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPSBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pVSBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCSBlob</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">A helper function to create a shader</div></div>",371:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype371\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShaderMgr::FindShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Returns the requested shader</div></div>",372:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype372\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CShaderMgr::RegisterShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Registers the specified shader with the shader manager</div></div>"});