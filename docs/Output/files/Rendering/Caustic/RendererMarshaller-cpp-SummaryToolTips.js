NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/RendererMarshaller.cpp",{387:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Code related to the renderer marshaller. The RendererMarshaller marshalls rendering calls from some user thread to the render thread.</div></div>",389:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype389\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI <span class=\"SHKeyword\">void</span> CreateRendererMarshaller(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRendererMarshaller&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppClientServer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Global function for creating the RendererMarshaller. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>",390:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype390\" class=\"NDPrototype NoParameterForm\">CRendererMarshaller::CRendererMarshaller() : m_exit(<span class=\"SHKeyword\">false</span>), m_thread(nullptr)</div><div class=\"TTSummary\">Defines the implementation of IRendererMarshaller</div></div>",391:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype391\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">DWORD WINAPI RenderThreadProc(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">LPVOID&nbsp;</td><td class=\"PName last\">lpParameter</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Entry point for the render thread.</div></div>",392:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype392\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Initialize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::Initialize&gt;</div></div>",393:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype393\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Shutdown()</div><div class=\"TTSummary\">See &lt;IRenderer::Shutdown&gt;</div></div>",394:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype394\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SaveScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Saves the scene graph to the specified file.</div></div>",395:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype395\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::LoadScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Loads the scene graph from the specified file.</div></div>",396:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype396\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPath,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::LoadTexture&gt;</div></div>",397:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype397\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPath,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::LoadVideoTexture&gt;</div></div>",398:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype398\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SetSceneGraph(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISceneGraph&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::SetSceneGraph&gt;</div></div>",399:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype399\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::MainLoop()</div><div class=\"TTSummary\">See &lt;IRenderer::MainLoop&gt;</div></div>",400:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype400\" class=\"NDPrototype NoParameterForm\">CComPtr&lt;ID3D11Device&gt; CRendererMarshaller::GetDevice()</div><div class=\"TTSummary\">See &lt;IRenderer::GetDevice&gt;</div></div>",401:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype401\" class=\"NDPrototype NoParameterForm\">CComPtr&lt;ID3D11DeviceContext&gt; CRendererMarshaller::GetContext()</div><div class=\"TTSummary\">See &lt;IRenderer::GetContext&gt;</div></div>",402:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype402\" class=\"NDPrototype NoParameterForm\">CRefObj&lt;ICamera&gt; CRendererMarshaller::GetCamera()</div><div class=\"TTSummary\">See &lt;IRenderer::GetCamera&gt;</div></div>",403:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype403\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::AddLambda(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>()&gt;&nbsp;</td><td class=\"PName last\">func</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Adds the specified lambda into the render\'s instruction queue</div></div>",404:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype404\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">createDebugDevice</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::Setup</div></div>",405:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype405\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector4&nbsp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::DrawLine&gt;</div></div>",406:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype406\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::DrawMesh&gt;</div></div>",407:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype407\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::GetRenderCtx(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderCtx&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppCtx</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::GetRenderCtx&gt;</div></div>",408:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype408\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::AddPointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::AddPointLight&gt;</div></div>",409:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype409\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::RenderLoop()</div><div class=\"TTSummary\">See &lt;IRenderer::RenderLoop&gt;. It is not valid for the client to call this method.</div></div>",410:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype410\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::RenderFrame()</div><div class=\"TTSummary\">See &lt;IRenderer::RenderFrame&gt;</div></div>",411:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype411\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SetCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ICamera&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCamera</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::SetCamera&gt;</div></div>"});