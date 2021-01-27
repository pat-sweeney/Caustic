NDSummary.OnToolTipsLoaded("CClass:IRenderer",{789:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our basic renderer. IRenderer handles all rendering commands.&nbsp; It is generally expected that this object is running on its own thread.</div></div>",791:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype791\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11Device&gt; GetDevice() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">The underlying D3D11 device</div></div>",721:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype721\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> Freeze() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Freezes the renderer</div></div>",723:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype723\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> Unfreeze() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Unfreezes the renderer</div></div>",1311:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1311\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LoadShaders(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFolder</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Loads into our shader manager all the shaders in the specified folder</div></div>",1312:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Assigns a GPUImaging pipeline that is to run as a series of post effects on the final render buffer.</div></div>",1313:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1313\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawScreenQuad(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minV,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxV,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"PType first\">ISampler&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSampler</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders a quad on the display at the specified uv coordinates.&nbsp; A full screen quad runs from 0.0,0.0 =&gt; 1.0,1.0</div></div>",792:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype792\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11DeviceContext&gt; GetContext() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">The underlying D3D11 device context</div></div>",1068:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1068\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderable&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderable</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Adds a renderable object to the renderer</div></div>",793:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype793\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ICamera&gt; GetCamera() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">The camera associated with this device</div></div>",794:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype794\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ICamera&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCamera</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Assigns a camera to the renderer</div></div>",795:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype795\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShaderMgr&gt; GetShaderMgr() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the shader manager being used by the renderer</div></div>",811:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype811\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName\">hwnd,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName\">shaderFolder,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">createDebugDevice,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">startFrozen&nbsp;</td><td class=\"PDefaultValueSeparator\">=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Initializes the renderer. Applications need to call this method at startup.</div></div>",797:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype797\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders a mesh by adding it to the Renderable list that is drawn each time a frame is rendered</div></div>",647:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype647\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RenderLoop(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Defines the main entry point for our renderer</div></div>",648:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype648\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RenderFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback&nbsp;</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders the next frame</div></div>",1366:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1366\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetBackBufferWidth() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the width in pixels of the back buffer</div></div>",1367:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1367\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetBackBufferHeight() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the height in pixels of the back buffer</div></div>",1368:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1368\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CopyFrameBackBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pImage</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Copies the back buffer into an IImage (CPU based image)</div></div>",800:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype800\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Adds a point light which the renderer uses</div></div>",801:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype801\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderCtx&gt; GetRenderCtx() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current render context</div></div>",558:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype558\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> ClearDepth() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Erases the depth buffer</div></div>",802:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype802\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector4&nbsp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders a line</div></div>",803:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype803\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> PushShadowmapRT(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">whichShadowmap,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lightPos,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lightDir</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Setups up our shadow map as the current render target</div></div>",804:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype804\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> PopShadowmapRT() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Restores the default render target after PushShadowmapRT</div></div>",1210:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1210\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SelectShadowmap(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">whichShadowMap,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Selects which shadow map to use during shading</div></div>",806:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype806\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetShadowmapTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">whichShadowMap</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the shadow map as a texture</div></div>",928:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype928\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;IRenderer&gt; CreateRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName\">hwnd,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName\">shaderFolder,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">startFrozen&nbsp;</td><td class=\"PDefaultValueSeparator\">=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates a renderer</div></div>"});