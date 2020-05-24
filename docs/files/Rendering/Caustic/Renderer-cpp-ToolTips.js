NDContentPage.OnToolTipsLoaded({23:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",83:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a simple vector with 3 components</div></div>",91:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a simple vector with 4 components</div></div>",563:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype563\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CRenderer</div></div></div><div class=\"TTSummary\">See IRenderer</div></div>",628:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",668:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",682:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the D3D counterpart to an ISubMesh object (i.e. the renderable form of a mesh, that is, a mesh converted from a half-edge structure to a vertex and index buffer).</div></div>",710:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines primary interface used for getting information about pre-compiled shaders. This isn\'t the shader itself. It is simply the data that we load from disk to create a shader (i.e. data from .shi file)</div></div>",721:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our basic renderer. IRenderer handles all rendering commands.&nbsp; It is generally expected that this object is running on its own thread.</div></div>",723:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype723\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">createDebugDevice</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Initializes the renderer. Applications need to call this method at startup.</div></div>",724:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype724\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders a mesh by adding it to the Renderable list that is drawn each time a frame is rendered</div></div>",725:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype725\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RenderLoop(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Defines the main entry point for our renderer</div></div>",726:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype726\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RenderFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders the next frame</div></div>",727:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype727\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ICamera&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pCamera</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Assigns a camera to the renderer</div></div>",728:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype728\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Adds a point light which the renderer uses</div></div>",729:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype729\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderCtx&gt; GetRenderCtx() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current render context</div></div>",730:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype730\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector4&nbsp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Renders a line</div></div>",731:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype731\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGraphics&gt; GetGraphics() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current graphics device the renderer is using</div></div>",734:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",805:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Used for manipulating the materials assigned to a mesh.</div></div>",810:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a point light</div></div>",813:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our basic rendering device.</div></div>",820:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The CCamera object defines our camera. We pass this to our IRenderer to determine where to render from.</div></div>"});