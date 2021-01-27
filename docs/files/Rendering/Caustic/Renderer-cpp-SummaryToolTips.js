NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/Renderer.cpp",{632:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains code for the main renderer</div></div>",634:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype634\" class=\"NDPrototype NoParameterForm\">CRenderer::CRenderer() : m_waitForShutdown(<span class=\"SHKeyword\">true</span>, <span class=\"SHKeyword\">true</span>), m_exitThread(<span class=\"SHKeyword\">false</span>)</div></div>",635:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype635\" class=\"NDPrototype NoParameterForm\">CRenderer::~CRenderer()</div></div>",636:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype636\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::InitializeD3D(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">HWND&nbsp;</td><td class=\"PName last\">hwnd</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Initializes the server side renderer. Clients should call this method indirectly (via IRenderer::Setup) at application startup.</div></div>",601:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype601\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">createDebugDevice,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">startFrozen</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::Setup.</div></div>",1327:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1327\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRenderer::Freeze()</div><div class=\"TTSummary\">See IRenderer::Freeze</div></div>",1328:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1328\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRenderer::Unfreeze()</div><div class=\"TTSummary\">See IRenderer::Unfreeze</div></div>",638:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype638\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::LoadShaderBlob(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">filename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppBlob</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Loads the shader into a blob from the specified file.</div></div>",639:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype639\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IShaderInfo&gt; CRenderer::LoadShaderInfo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">filename</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Loads the shader info (*.shi) file (describes the layout and parameters of a given shader)</div></div>",1314:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1314\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::DrawScreenQuad(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minV,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxV,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"PType first\">ISampler&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSampler</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::DrawScreenQuad</div></div>",1315:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1315\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::LoadShaders(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFolder</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Loads all the shaders found in the specified folder.</div></div>",641:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype641\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::DrawMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSubMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::DrawMesh</div></div>",642:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype642\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::AddPointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::AddPointLight</div></div>",643:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype643\" class=\"NDPrototype NoParameterForm\">CRefObj&lt;IRenderCtx&gt; CRenderer::GetRenderCtx()</div><div class=\"TTSummary\">See IRenderer::GetRenderCtx</div></div>",556:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype556\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CRenderer::ClearDepth()</div><div class=\"TTSummary\">See IRenderer::ClearDepth</div></div>",644:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype644\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector4&nbsp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::DrawLine</div></div>",645:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype645\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::DrawSceneObjects(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">pass,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">First calls the renderCallback provided. Then renders each Renderable.</div></div>",646:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype646\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::RenderScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Renders current scene (both scene graph and any renderables currently attached to the renderer)</div></div>",620:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype620\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::RenderFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::RenderFrame</div></div>",637:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype637\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::RenderLoop(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::RenderLoop</div></div>",1360:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1360\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CRenderer::CopyFrameBackBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pImage</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Copies the back buffer into an IImage (CPU based image)</div></div>",614:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype614\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;IRenderer&gt; CreateRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">startFrozen&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates a renderer</div></div>",650:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype650\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CGraphicsBase::SetCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ICamera&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCamera</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::SetCamera</div></div>",651:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype651\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CGraphicsBase::InitializeD3D(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">HWND&nbsp;</td><td class=\"PName last\">hwnd</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Called at the start of the application to initialize the server side of our renderer.</div></div>",652:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype652\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CGraphicsBase::Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">HWND&nbsp;</td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">createDebugDevice</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See IRenderer::Setup</div></div>"});