﻿NDContentPage.OnToolTipsLoaded({13:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype13\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">template</span> &lt;typename T&gt; <span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">CRefObj</div></div></div><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",52:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype52\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetViewport(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">x0,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">y0,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">x1,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">y1</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Specifies the viewport for the final render target</div></div>",53:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype53\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Setup(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName\">hwnd,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\">BBox2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">viewport,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">shaderFolder,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">createDebugDevice,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">startFrozen</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span>,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName\">desktopIndex</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">0</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Initializes the renderer. Applications need to call this method at startup.</div></div>",98:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype98\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector3</div></div></div><div class=\"TTSummary\">Defines a simple vector with 3 components</div></div>",107:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype107\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector4</div></div></div><div class=\"TTSummary\">Defines a simple vector with 4 components</div></div>",225:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype225\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">BBox2</div></div></div><div class=\"TTSummary\">Defines a axis-aligned bounding box (in 2D)</div></div>",433:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Used for manipulating the materials assigned to a mesh.</div></div>",629:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an image (CPU based)</div></div>",888:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype888\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DeviceWindowResized(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Called when device window has been resized</div></div>",941:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",958:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",979:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",1009:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",1022:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the D3D counterpart to an ISubMesh object (i.e. the renderable form of a mesh, that is, a mesh converted from a half-edge structure to a vertex and index buffer).</div></div>",1051:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">A Renderable is an mesh, front and back materials, and a shader. The renderer keeps a simple list of these objects which are rendered at each call to IRenderer::RenderFrame. The purpose of this object is to allow the renderer to render a set of objects without forcing the client to maintain an entire scene graph.</div></div>",1075:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a point light</div></div>",1079:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a properties common to all lights</div></div>",1102:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1102\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BeginMarker(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pLabel</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Add marker to display in graphics debugger</div></div>",1103:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1103\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> EndMarker() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Ends marker</div></div>",1104:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1104\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> EnableDepthTest(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">enable</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Turns on or off depth testing</div></div>",1105:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1105\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11Device&gt; GetDevice() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">The underlying D3D11 device</div></div>",1106:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1106\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;IDXGIOutputDuplication&gt; GetDuplication() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">The underlying windows duplication service</div></div>",1107:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1107\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Freeze() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Freezes the renderer</div></div>",1108:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1108\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Unfreeze() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Unfreezes the renderer</div></div>",1109:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1109\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LoadShaders(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFolder</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads into our shader manager all the shaders in the specified folder</div></div>",1111:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1111\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawScreenQuadWithCustomShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader*&nbsp;</td><td class=\"PName\">pShader,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">minU,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">minV,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">maxU,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">maxV,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">ITexture*&nbsp;</td><td class=\"PName\">pTexture,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">ISampler*&nbsp;</td><td class=\"PName\">pSampler,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">disableDepth</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a quad on the display at the specified uv coordinates.&nbsp; A full screen quad runs from 0.0,0.0 =&gt; 1.0,1.0</div></div>",1112:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1112\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawScreenQuad(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName\">minU,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName\">minV,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName\">maxU,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName\">maxV,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">ITexture*&nbsp;</td><td></td><td class=\"PName\">pTexture,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">ISampler&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName\">pSampler,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">disableDepth</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a quad on the display at the specified uv coordinates.&nbsp; A full screen quad runs from 0.0,0.0 =&gt; 1.0,1.0</div></div>",1113:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1113\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11DeviceContext&gt; GetContext() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">The underlying D3D11 device context</div></div>",1114:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1114\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderable*&nbsp;</td><td class=\"PName last\">pRenderable</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a renderable object to the renderer</div></div>",1115:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1115\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ICamera&gt; GetCamera() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">The camera associated with this device</div></div>",1116:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1116\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ICamera*&nbsp;</td><td class=\"PName last\">pCamera</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns a camera to the renderer</div></div>",1119:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1119\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ITexture&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTexture,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a mesh by adding it to the Renderable list that is drawn each time a frame is rendered</div></div>",1120:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1120\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RenderLoop(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Defines the main entry point for our renderer</div></div>",1121:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1121\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RenderFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName last\">renderCallback,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName last\">prePresentCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the next frame</div></div>",1122:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1122\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetBackBufferWidth() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the width in pixels of the back buffer</div></div>",1123:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1123\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetBackBufferHeight() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the height in pixels of the back buffer</div></div>",1124:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1124\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11Texture2D&gt; GetBackBuffer() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the texture that contains the back buffer</div></div>",1125:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1125\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CopyFrameBackBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage*&nbsp;</td><td class=\"PName last\">pImage</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Copies the back buffer into an IImage (CPU based image)</div></div>",1126:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1126\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPointLight&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a point light which the renderer uses</div></div>",1127:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1127\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderCtx&gt; GetRenderCtx() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current render context</div></div>",1128:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1128\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ClearDepth() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Erases the depth buffer</div></div>",1129:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1129\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector4&nbsp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a line</div></div>",1130:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1130\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> PushShadowmapRT(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">whichShadowmap,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&amp;&nbsp;</td><td class=\"PName last\">lightPos,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&amp;&nbsp;</td><td class=\"PName last\">lightDir</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Setups up our shadow map as the current render target</div></div>",1131:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1131\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> PopShadowmapRT() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Restores the default render target after PushShadowmapRT</div></div>",1132:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1132\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SelectShadowmap(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">whichShadowMap,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">lightMapIndex,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&amp;&nbsp;</td><td></td><td class=\"PName last\">lights,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Selects which shadow map to use during shading</div></div>",1133:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1133\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetShadowmapTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">whichShadowMap</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the shadow map as a texture</div></div>",1134:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1134\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFinalRenderTarget(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ID3D11Texture2D*&nbsp;</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the final render target. This allows for the final output to be something other than the default window.</div></div>",1135:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1135\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFinalRenderTargetUsingSharedTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IUnknown*&nbsp;</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the final render target. This method is only intended to be used by the WPF interop layer. It will assign the final render target using the shared textured that D3D11Image returns.</div></div>",1147:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The CCamera object defines our camera. We pass this to our IRenderer to determine where to render from.</div></div>",1282:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Creates a renderer that runs on its own thread and method calls are marshalled from the client to render thread.&nbsp; The renderer is on a separate thread where the client is calling from.</div></div>",1543:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Our main interface for manipulating a scene graph</div></div>"});