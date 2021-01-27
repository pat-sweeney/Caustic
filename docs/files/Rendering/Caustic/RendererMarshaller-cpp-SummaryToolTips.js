NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/RendererMarshaller.cpp",{596:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Code related to the renderer marshaller. The RendererMarshaller marshalls rendering calls from some user thread to the render thread.</div></div>",598:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype598\" class=\"NDPrototype NoParameterForm\">CAUSTICAPI CRefObj&lt;IRendererMarshaller&gt; CreateRendererMarshaller()</div><div class=\"TTSummary\">Global function for creating the RendererMarshaller. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>",599:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype599\" class=\"NDPrototype NoParameterForm\">CRendererMarshaller::CRendererMarshaller() : m_exit(<span class=\"SHKeyword\">false</span>), m_thread(nullptr)</div><div class=\"TTSummary\">Defines the implementation of IRendererMarshaller</div></div>",1325:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1325\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Freeze()</div><div class=\"TTSummary\">See IRenderer::Freeze</div></div>",1326:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1326\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Unfreeze()</div><div class=\"TTSummary\">See IRenderer::Unfreeze</div></div>",600:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype600\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">DWORD WINAPI RenderThreadProc(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">LPVOID&nbsp;</td><td class=\"PName last\">lpParameter</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Entry point for the render thread.</div></div>",960:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype960\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Initialize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td></td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td></td><td class=\"PName last\">prePresentCallback,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">startFrozen&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::Initialize&gt;</div></div>",602:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype602\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Shutdown()</div><div class=\"TTSummary\">See &lt;IRenderer::Shutdown&gt;</div></div>",603:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype603\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::PushShadowmapRT(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">whichShadowmap,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lightPos,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lightDir</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::PushShadowmapRT</div></div>",604:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype604\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::PopShadowmapRT()</div><div class=\"TTSummary\">See IRenderer::PopShadowmapRT</div></div>",1209:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1209\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SelectShadowmap(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">whichShadowmap,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::SelectShadowmap</div></div>",605:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype605\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CRendererMarshaller::GetShadowmapTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">whichShadowmap</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::GetShadowmapTexture</div></div>",606:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype606\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SaveScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Saves the scene graph to the specified file.</div></div>",607:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype607\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::LoadScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Loads the scene graph from the specified file.</div></div>",608:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype608\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CRendererMarshaller::LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::LoadTexture&gt;</div></div>",609:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype609\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CRendererMarshaller::LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See &lt;IRenderer::LoadVideoTexture&gt;</div></div>",610:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype610\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::MainLoop()</div><div class=\"TTSummary\">See &lt;IRenderer::MainLoop&gt;</div></div>",371:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype371\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawScreenQuad(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minV,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxV,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"PType first\">ISampler&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSampler</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::DrawScreenQuad</div></div>",611:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype611\" class=\"NDPrototype NoParameterForm\">CComPtr&lt;ID3D11Device&gt; CRendererMarshaller::GetDevice()</div><div class=\"TTSummary\">See IRenderer::GetDevice</div></div>",462:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype462\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::LoadShaders(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFolder</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::LoadShaders</div></div>",612:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype612\" class=\"NDPrototype NoParameterForm\">CComPtr&lt;ID3D11DeviceContext&gt; CRendererMarshaller::GetContext()</div><div class=\"TTSummary\">See IRenderer::GetContext</div></div>",613:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype613\" class=\"NDPrototype NoParameterForm\">CRefObj&lt;ICamera&gt; CRendererMarshaller::GetCamera()</div><div class=\"TTSummary\">See IRenderer::GetCamera</div></div>",1226:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1226\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::AddRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderable&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderable</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::AddRenderable</div></div>",1361:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1361\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">createDebugDevice,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">startFrozen&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::Setup</div></div>",615:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype615\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector4&nbsp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::DrawLine</div></div>",616:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype616\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::DrawMesh</div></div>",617:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype617\" class=\"NDPrototype NoParameterForm\">CRefObj&lt;IRenderCtx&gt; CRendererMarshaller::GetRenderCtx()</div><div class=\"TTSummary\">See IRenderer::GetRenderCtx</div></div>",557:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype557\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::ClearDepth()</div><div class=\"TTSummary\">See IRenderer::ClearDepth</div></div>",618:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype618\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::AddPointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::AddPointLight</div></div>",1224:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1224\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::RenderLoop(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::RenderLoop. It is not valid for the client to call this method.</div></div>",1362:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1362\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::RenderFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::RenderFrame</div></div>",1363:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1363\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">uint32</span> CRendererMarshaller::GetBackBufferWidth()</div><div class=\"TTSummary\">See IRenderer::GetBackBufferWidth</div></div>",1364:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1364\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">uint32</span> CRendererMarshaller::GetBackBufferHeight()</div><div class=\"TTSummary\">See IRenderer::GetBackBufferHeight</div></div>",1365:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1365\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::CopyFrameBackBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pImage</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::CopyFrameBackbuffer</div></div>",621:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype621\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SetCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ICamera&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCamera</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::SetCamera</div></div>"});