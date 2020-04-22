NDSummary.OnToolTipsLoaded("CClass:IRendererMarshaller",{534:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Creates a renderer that runs on its own thread and method calls are marshalled from the client to render thread.&nbsp; renderer is on a separate thread where the client is calling from)</div></div>",536:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype536\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Initialize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td></td><td class=\"PName last\">renderCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Initializes the renderer</div></div>",537:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype537\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> Shutdown() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Shuts down the renderer</div></div>",416:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype416\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RunOnRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer*,<span class=\"SHKeyword\">void</span>*clientData)&gt;&nbsp;</td><td></td><td class=\"PName last\">callback,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">void</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">clientData</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Runs the specified callback function on the renderer\'s thread</div></div>",539:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype539\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPath,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Loads a texture from the specified path</div></div>",540:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype540\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPath,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Loads a video texture from the specified path</div></div>",541:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype541\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SaveScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Saves the scene graph to the specified file</div></div>",542:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Loads the scene graph from the specified file</div></div>",421:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype421\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderer&gt; GetRenderer() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the renderer this object is marshalling calls to</div></div>"});