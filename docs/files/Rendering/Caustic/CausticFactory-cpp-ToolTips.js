﻿NDContentPage.OnToolTipsLoaded({94:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype94\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">template</span> &lt;typename T&gt; <span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">CRefObj</div></div></div><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",101:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype101\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">FRGBColor</div></div></div><div class=\"TTSummary\">FRGBColor defines an RGB tuple defined in floating point.&nbsp; Each color component is in the range of 0.0 to 1.0</div></div>",516:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Used for manipulating the materials assigned to a mesh.</div></div>",620:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface to our trackball implementation</div></div>",624:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a spot light</div></div>",626:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",771:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines primary interface used for getting information about pre-compiled shaders. This isn\'t the shader itself. It is simply the data that we load from disk to create a shader (i.e. data from .shi file)</div></div>",784:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",787:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",788:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a material that has its associated render element (as opposed to an IMaterialAttrib which only defines the attributes of the material).</div></div>",789:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the D3D counterpart to an ISubMesh object (i.e. the renderable form of a mesh, that is, a mesh converted from a half-edge structure to a vertex and index buffer).</div></div>",790:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">IRenderMesh is simply a collection of submesh objects (IRenderSubMesh).</div></div>",791:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Creates a renderer that runs on its own thread and method calls are marshalled from the client to render thread.&nbsp; The renderer is on a separate thread where the client is calling from.</div></div>",801:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a point light</div></div>",827:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">A Renderable is an mesh, front and back materials, and a shader. The renderer keeps a simple list of these objects which are rendered at each call to IRenderer::RenderFrame. The purpose of this object is to allow the renderer to render a set of objects without forcing the client to maintain an entire scene graph.</div></div>",853:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">A PointCloud renders a depth map as a set of points.</div></div>",865:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a directional light (i.e. the Sun)</div></div>",883:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our basic renderer. IRenderer handles all rendering commands.&nbsp; It is generally expected that this object is running on its own thread.</div></div>",947:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The CCamera object defines our camera. We pass this to our IRenderer to determine where to render from.</div></div>",986:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main factory for creating Caustic objects</div></div>",1010:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1010\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CCausticFactory</div></div></div><div class=\"TTSummary\">Implements ICausticFactory</div></div>",1027:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1027\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderer&gt; CreateRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName\">hwnd,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">shaderFolder,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">startFrozen</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span>,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName\">desktopIndex</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">0</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates our basic renderer object to be used by the client</div></div>",1028:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1028\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IPointCloud&gt; CreatePointCloud(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a point cloud</div></div>",1029:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1029\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMesh&gt; CreateRenderMesh() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a render mesh object</div></div>",1030:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1030\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderSubMesh&gt; CreateRenderSubMesh() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a render submesh object</div></div>",1031:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1031\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IPointLight&gt; CreatePointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">intensity</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a point light source</div></div>",1032:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1032\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISpotLight&gt; CreateSpotLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName\">pos,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName\">dir,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName\">color,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">intensity</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">1000.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">innerAngle</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">30.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">outerAngle</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">45.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">casts</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">true</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a spot light</div></div>",1033:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1033\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IDirectionalLight&gt; CreateDirectionalLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td></td><td class=\"PName last\">dir,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td></td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">intensity</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a directional light source</div></div>",1034:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1034\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITrackball&gt; CreateTrackball() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a new trackball object</div></div>",1035:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1035\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRendererMarshaller&gt; CreateRendererMarshaller() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates the marshalled version of the renderer. This renderer marshals all calls over to the render thread (i.e. can be called on any thread)</div></div>",1036:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1036\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IMaterialAttrib&gt; CreateMaterialAttrib() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a MaterialAttrib object.</div></div>",1037:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1037\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMaterial&gt; CreateRenderMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterialAttrib,</td></tr><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a RenderMaterial object.</div></div>",1038:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1038\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderable&gt; CreateRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSubMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFrontMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBackMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a renderable object. A Renderable is a mesh+material+shader.</div></div>",1039:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1039\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISampler&gt; CreateSampler(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a new sampler. Samplers defined how shaders read from textures.</div></div>",1040:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1040\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ICamera&gt; CreateCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a new camera object. This camera can then be passed to IRenderer to be used for rendering.</div></div>",1041:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1041\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td class=\"PName last\">format,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a new texture</div></div>",1043:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1043\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; CheckerboardTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a texture with a checkerboard pattern</div></div>",1044:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1044\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; CreateDesktopTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a texture displaying the windows desktop</div></div>",1045:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1045\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a texture from disk</div></div>",1046:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1046\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a video from disk</div></div>",1047:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1047\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; VideoTextureFromWebcam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a video texture using a webcam as the source</div></div>",1048:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1048\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShader&gt; CreateShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVertexShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob*&nbsp;</td><td></td><td class=\"PName last\">pPixelShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob*&nbsp;</td><td></td><td class=\"PName last\">pComputeShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShaderInfo</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a shader</div></div>",1049:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1049\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShaderInfo&gt; CreateShaderInfo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads the shader info associated with a shader</div></div>",1606:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1606\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector3</div></div></div><div class=\"TTSummary\">Defines a simple vector with 3 components</div></div>",1619:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an image (CPU based)</div></div>"});