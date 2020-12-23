NDContentPage.OnToolTipsLoaded({61:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",85:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a simple vector with 3 components</div></div>",93:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a simple vector with 4 components</div></div>",558:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype558\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> ClearDepth() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Erases the depth buffer</div></div>",704:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",718:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",721:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype721\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> Freeze() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Freezes the renderer</div></div>",723:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype723\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> Unfreeze() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Unfreezes the renderer</div></div>",730:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",758:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the D3D counterpart to an ISubMesh object (i.e. the renderable form of a mesh, that is, a mesh converted from a half-edge structure to a vertex and index buffer).</div></div>",791:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype791\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11Device&gt; GetDevice() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">The underlying D3D11 device</div></div>",792:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype792\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11DeviceContext&gt; GetContext() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">The underlying D3D11 device context</div></div>",793:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype793\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ICamera&gt; GetCamera() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">The camera associated with this device</div></div>",794:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype794\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ICamera&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCamera</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Assigns a camera to the renderer</div></div>",796:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype796\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">createDebugDevice</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Initializes the renderer. Applications need to call this method at startup.</div></div>",797:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype797\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders a mesh by adding it to the Renderable list that is drawn each time a frame is rendered</div></div>",798:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype798\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RenderLoop(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Defines the main entry point for our renderer</div></div>",799:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype799\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RenderFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders the next frame</div></div>",800:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype800\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Adds a point light which the renderer uses</div></div>",801:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype801\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderCtx&gt; GetRenderCtx() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current render context</div></div>",802:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype802\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector4&nbsp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders a line</div></div>",803:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype803\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> PushShadowmapRT(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">whichShadowmap,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lightPos,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lightDir</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Setups up our shadow map as the current render target</div></div>",804:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype804\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> PopShadowmapRT() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Restores the default render target after PushShadowmapRT</div></div>",806:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype806\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetShadowmapTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">whichShadowMap</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the shadow map as a texture</div></div>",809:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Creates a renderer that runs on its own thread and method calls are marshalled from the client to render thread.&nbsp; renderer is on a separate thread where the client is calling from)</div></div>",822:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",835:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a point light</div></div>",839:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">A Renderable is an mesh, front and back materials, and a shader. The renderer keeps a simple list of these objects which are rendered at each call to IRenderer::RenderFrame. The purpose of this object is to allow the renderer to render a set of objects without forcing the client to maintain an entire scene graph.</div></div>",854:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a properties common to all lights</div></div>",878:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Used for manipulating the materials assigned to a mesh.</div></div>",898:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The CCamera object defines our camera. We pass this to our IRenderer to determine where to render from.</div></div>",1068:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1068\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderable&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderable</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Adds a renderable object to the renderer</div></div>",1170:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Our main interface for manipulating a scene graph</div></div>",1210:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1210\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SelectShadowmap(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">whichShadowMap,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Selects which shadow map to use during shading</div></div>",1311:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1311\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LoadShaders(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFolder</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Loads into our shader manager all the shaders in the specified folder</div></div>",1313:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1313\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawScreenQuad(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">minV,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxU,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxV,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"PType first\">ISampler&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSampler</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders a quad on the display at the specified uv coordinates.&nbsp; A full screen quad runs from 0.0,0.0 =&gt; 1.0,1.0</div></div>"});