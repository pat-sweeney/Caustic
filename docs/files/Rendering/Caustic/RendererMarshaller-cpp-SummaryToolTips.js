﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/RendererMarshaller.cpp",{819:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Code related to the renderer marshaller. The RendererMarshaller marshalls rendering calls from some user thread to the render thread.</div></div>",821:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype821\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IRendererMarshaller&gt; CreateRendererMarshaller()</div></div><div class=\"TTSummary\">Global function for creating the RendererMarshaller. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>",822:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype822\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRendererMarshaller::CRendererMarshaller() : m_exit(<span class=\"SHKeyword\">false</span>), m_thread(nullptr)</div></div><div class=\"TTSummary\">Defines the implementation of IRendererMarshaller</div></div>",823:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype823\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> CRendererMarshaller::EnableDepthTest(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">enable</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::EnableDepthTest</div></div>",824:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype824\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Freeze()</div></div><div class=\"TTSummary\">See IRenderer::Freeze</div></div>",825:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype825\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Unfreeze()</div></div><div class=\"TTSummary\">See IRenderer::Unfreeze</div></div>",826:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype826\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">DWORD WINAPI RenderThreadProc(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">LPVOID&nbsp;</td><td class=\"PName last\">lpParameter</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Entry point for the render thread.</div></div>",827:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype827\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Initialize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td></td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td></td><td class=\"PName last\">prePresentCallback,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">startFrozen ,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">desktopIndex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;IRenderer::Initialize&gt;</div></div>",828:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype828\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Shutdown()</div></div><div class=\"TTSummary\">See &lt;IRenderer::Shutdown&gt;</div></div>",829:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype829\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::PushShadowmapRT(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">whichShadowmap,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&amp;&nbsp;</td><td></td><td class=\"PName last\">lightPos,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">lightDir</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::PushShadowmapRT</div></div>",830:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype830\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::PopShadowmapRT()</div></div><div class=\"TTSummary\">See IRenderer::PopShadowmapRT</div></div>",831:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype831\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SelectShadowmap(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">whichShadowmap,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&amp;&nbsp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader*&nbsp;</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::SelectShadowmap</div></div>",832:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype832\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CRendererMarshaller::GetShadowmapTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">whichShadowmap</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::GetShadowmapTexture</div></div>",833:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype833\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SaveScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Saves the scene graph to the specified file.</div></div>",834:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype834\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::LoadScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Loads the scene graph from the specified file.</div></div>",835:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype835\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CRendererMarshaller::LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;IRenderer::LoadTexture&gt;</div></div>",836:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype836\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CRendererMarshaller::LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;IRenderer::LoadVideoTexture&gt;</div></div>",837:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype837\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::MainLoop()</div></div><div class=\"TTSummary\">See &lt;IRenderer::MainLoop&gt;</div></div>",838:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype838\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawScreenQuadWithCustomShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minV,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxV,</td></tr><tr><td class=\"PType first\">ITexture*&nbsp;</td><td></td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"PType first\">ISampler*&nbsp;</td><td></td><td class=\"PName last\">pSampler,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">disableDepth</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::DrawScreenQuadWithCustomShader</div></div>",839:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype839\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawScreenQuad(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minV,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxV,</td></tr><tr><td class=\"PType first\">ITexture*&nbsp;</td><td></td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"PType first\">ISampler&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSampler,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">disableDepth</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::DrawScreenQuad</div></div>",840:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype840\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CComPtr&lt;ID3D11Device&gt; CRendererMarshaller::GetDevice()</div></div><div class=\"TTSummary\">See IRenderer::GetDevice</div></div>",841:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype841\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CComPtr&lt;IDXGIOutputDuplication&gt; CRendererMarshaller::GetDuplication()</div></div><div class=\"TTSummary\">See IRenderer::GetDuplication</div></div>",842:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype842\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::BeginMarker(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pLabel</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::BeginMarker</div></div>",843:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype843\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::EndMarker()</div></div><div class=\"TTSummary\">See IRenderer::EndMarker</div></div>",844:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype844\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::LoadShaders(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFolder</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::LoadShaders</div></div>",845:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype845\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CComPtr&lt;ID3D11DeviceContext&gt; CRendererMarshaller::GetContext()</div></div><div class=\"TTSummary\">See IRenderer::GetContext</div></div>",846:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype846\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ICamera&gt; CRendererMarshaller::GetCamera()</div></div><div class=\"TTSummary\">See IRenderer::GetCamera</div></div>",847:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype847\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::AddRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderable*&nbsp;</td><td class=\"PName last\">pRenderable</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::AddRenderable</div></div>",848:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype848\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">createDebugDevice,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">startFrozen ,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">desktopIndex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::Setup</div></div>",849:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype849\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector4&nbsp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::DrawLine</div></div>",850:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype850\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::DrawMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::DrawMesh</div></div>",851:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype851\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IRenderCtx&gt; CRendererMarshaller::GetRenderCtx()</div></div><div class=\"TTSummary\">See IRenderer::GetRenderCtx</div></div>",852:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype852\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::ClearDepth()</div></div><div class=\"TTSummary\">See IRenderer::ClearDepth</div></div>",853:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype853\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::AddPointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::AddPointLight</div></div>",854:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype854\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::RenderLoop(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::RenderLoop. It is not valid for the client to call this method.</div></div>",855:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype855\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::RenderFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::RenderFrame</div></div>",856:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype856\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">uint32</span> CRendererMarshaller::GetBackBufferWidth()</div></div><div class=\"TTSummary\">See IRenderer::GetBackBufferWidth</div></div>",857:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype857\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">uint32</span> CRendererMarshaller::GetBackBufferHeight()</div></div><div class=\"TTSummary\">See IRenderer::GetBackBufferHeight</div></div>",858:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype858\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CComPtr&lt;ID3D11Texture2D&gt; CRendererMarshaller::GetBackBuffer()</div></div><div class=\"TTSummary\">See IRenderer::GetBackBuffer</div></div>",859:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype859\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::CopyFrameBackBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pImage</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::CopyFrameBackbuffer</div></div>",860:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype860\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SetCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ICamera&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pCamera</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::SetCamera</div></div>",861:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype861\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SetFinalRenderTarget(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ID3D11Texture2D*&nbsp;</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::SetFinalRenderTarget</div></div>",862:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype862\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRendererMarshaller::SetFinalRenderTargetUsingSharedTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IUnknown*&nbsp;</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See IRenderer::SetFinalRenderTargetUsingSharedTexture</div></div>"});