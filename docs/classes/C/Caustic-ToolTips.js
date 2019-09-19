NDContentPage.OnToolTipsLoaded({38:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",44:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a simple vector with 2 components</div></div>",53:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a simple vector with 3 components</div></div>",230:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">IMaterial is used for manipulating the materials assigned to a mesh.&nbsp; Each ISubMesh may have a single material assigned to it.</div></div>",300:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an image (CPU based)</div></div>",394:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface to our trackball implementation</div></div>",402:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",413:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",450:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Creates a renderer that runs on its own thread and method calls are marshalled from the client to render thread.&nbsp; renderer is on a separate thread where the client is calling from)</div></div>",470:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a material that has its associated render element (as opposed to an IMaterialAttrib which only defines the attributes of the material).</div></div>",473:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface clients use to talk to the renderer</div></div>",478:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",481:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a point light</div></div>",484:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">A Renderable is an mesh, front and back materials, and a shader. The renderer keeps a simple list of these objects which are rendered at each call to &lt;IRenderer::RenderFrame&gt;. The purpose of this object is to allow the renderer to render a set of objects without forcing the client to maintain an entire scene graph.</div></div>",491:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the D3D counterpart to an ISubMesh object (i.e. the renderable form of a mesh, that is, a mesh converted from a half-edge structure to a vertex and index buffer).</div></div>",492:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">IRenderMesh is simply a collection of submesh objects (IRenderSubMesh).</div></div>",494:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a simple wrapper around our D3D renderer</div></div>",497:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main factory for creating Caustic objects</div></div>",499:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype499\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates our basic renderer object to be used by the client</div></div>",500:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype500\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateGraphics(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppGraphics</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a wrapper around our D3D device.</div></div>",501:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype501\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateRenderMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderMesh&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppRenderMesh</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a render mesh object</div></div>",502:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype502\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateRenderSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppRenderSubMesh</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a render submesh object</div></div>",503:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype503\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreatePointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppLight</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a point light source</div></div>",504:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype504\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateTrackball(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ITrackball&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTrackball</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a new trackball object</div></div>",505:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype505\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateRendererMarshaller(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRendererMarshaller&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppMarshaller</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates the marshalled version of the renderer. This renderer marshals all calls over to the render thread (i.e. can be called on any thread)</div></div>",506:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype506\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateRenderMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\">IMaterialAttrib&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterialAttrib,</td></tr><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PType first\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppRenderMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a RenderMaterial object.</div></div>",507:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype507\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSubMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFrontMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBackMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderable&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppRenderable</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a renderable object. A Renderable is a mesh+material+shader.</div></div>",508:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype508\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateSampler(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"PType first\">ISampler&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppSampler</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a new sampler. Samplers defined how shaders read from textures.</div></div>",509:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype509\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">leftHanded,</td></tr><tr><td class=\"PType first\">ICamera&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppCamera</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a new camera object. This camera can then be passed to IRenderer to be used for rendering.</div></div>",510:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype510\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td></td><td class=\"PName last\">format,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">bindFlags,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a new texture</div></div>",511:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype511\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; CheckerboardTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a texture with a checkerboard pattern</div></div>",512:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype512\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Loads a texture from disk</div></div>",513:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype513\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Loads a video from disk</div></div>",517:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The CCamera object defines our camera. We pass this to our IRenderer to determine where to render from.</div></div>",530:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype530\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CCausticFactory</div></div></div><div class=\"TTSummary\">Implements ICausticFactory</div></div>"});