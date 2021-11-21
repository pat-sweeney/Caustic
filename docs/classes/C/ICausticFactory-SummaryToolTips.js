﻿NDSummary.OnToolTipsLoaded("CClass:ICausticFactory",{1222:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main factory for creating Caustic objects</div></div>",1224:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1224\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderer&gt; CreateRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName\">hwnd,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">shaderFolder,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">startFrozen</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span>,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName\">desktopIndex</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">0</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates our basic renderer object to be used by the client</div></div>",1225:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1225\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IPointCloud&gt; CreatePointCloud(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a point cloud</div></div>",1226:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1226\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMesh&gt; CreateRenderMesh() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a render mesh object</div></div>",1227:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1227\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderSubMesh&gt; CreateRenderSubMesh() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a render submesh object</div></div>",1228:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1228\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IPointLight&gt; CreatePointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">intensity</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a point light source</div></div>",1229:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1229\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISpotLight&gt; CreateSpotLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName\">pos,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName\">dir,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName\">color,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">intensity</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">1000.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">innerAngle</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">30.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">outerAngle</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">45.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">casts</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">true</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a spot light</div></div>",1230:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1230\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IDirectionalLight&gt; CreateDirectionalLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td></td><td class=\"PName last\">dir,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td></td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">intensity</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a directional light source</div></div>",1231:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1231\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITrackball&gt; CreateTrackball() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a new trackball object</div></div>",1232:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1232\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRendererMarshaller&gt; CreateRendererMarshaller() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates the marshalled version of the renderer. This renderer marshals all calls over to the render thread (i.e. can be called on any thread)</div></div>",1233:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1233\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IMaterialAttrib&gt; CreateMaterialAttrib() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a MaterialAttrib object.</div></div>",1234:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1234\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMaterial&gt; CreateRenderMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterialAttrib,</td></tr><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a RenderMaterial object.</div></div>",1235:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1235\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderable&gt; CreateRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSubMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFrontMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBackMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a renderable object. A Renderable is a mesh+material+shader.</div></div>",1236:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1236\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISampler&gt; CreateSampler(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a new sampler. Samplers defined how shaders read from textures.</div></div>",1237:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1237\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ICamera&gt; CreateCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a new camera object. This camera can then be passed to IRenderer to be used for rendering.</div></div>",1238:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1238\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td class=\"PName last\">format,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a new texture</div></div>",1239:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1239\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IImage&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pImage,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td></td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td></td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a new texture</div></div>",1240:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1240\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; CheckerboardTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a texture with a checkerboard pattern</div></div>",1241:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1241\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; CreateDesktopTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a texture displaying the windows desktop</div></div>",1242:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1242\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a texture from disk</div></div>",1243:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1243\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a video from disk</div></div>",1244:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1244\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; VideoTextureFromWebcam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a video texture using a webcam as the source</div></div>",1245:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1245\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShader&gt; CreateShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVertexShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob*&nbsp;</td><td></td><td class=\"PName last\">pPixelShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob*&nbsp;</td><td></td><td class=\"PName last\">pComputeShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShaderInfo</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a shader</div></div>",1246:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1246\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShaderInfo&gt; CreateShaderInfo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads the shader info associated with a shader</div></div>",1247:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1247\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ICausticFactory&gt; CreateCausticFactory()</div></div><div class=\"TTSummary\">Function used by clients to create a Caustic factory</div></div>"});