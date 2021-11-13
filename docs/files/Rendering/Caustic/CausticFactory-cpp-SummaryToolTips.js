﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/CausticFactory.cpp",{1264:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CCausticFactory object.</div></div>",1266:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1266\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ICausticFactory&gt; CreateCausticFactory()</div></div><div class=\"TTSummary\">Global function for creating the main Caustic factory.</div></div>",1267:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1267\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CCausticFactory::CCausticFactory()</div></div><div class=\"TTSummary\">Defines the implementation of ICausticFactory</div></div>",1268:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1268\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CCausticFactory::~CCausticFactory()</div></div><div class=\"TTSummary\">Implements the dtor for CCausticFactory</div></div>",1269:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1269\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IRenderer&gt; CCausticFactory::CreateRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">startFrozen ,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">desktopIndex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderer</div></div>",1270:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1270\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IPointCloud&gt; CCausticFactory::CreatePointCloud(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreatePointCloud</div></div>",1271:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1271\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IRenderMesh&gt; CCausticFactory::CreateRenderMesh()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderMesh</div></div>",1272:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1272\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IRenderSubMesh&gt; CCausticFactory::CreateRenderSubMesh()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderSubMesh</div></div>",1273:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1273\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IPointLight&gt; CCausticFactory::CreatePointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">intensity</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreatePointLight</div></div>",1274:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1274\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ISpotLight&gt; CCausticFactory::CreateSpotLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">dir,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">intensity,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">innerAngle,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">outerAngle,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">casts</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateSpotLight</div></div>",1275:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1275\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IDirectionalLight&gt; CCausticFactory::CreateDirectionalLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">dir,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">intensity</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateDirectionalLight</div></div>",1276:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1276\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ITrackball&gt; CCausticFactory::CreateTrackball()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateTrackball</div></div>",1277:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1277\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IRendererMarshaller&gt; CCausticFactory::CreateRendererMarshaller()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateRendererMarshaller</div></div>",1278:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1278\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IMaterialAttrib&gt; CCausticFactory::CreateMaterialAttrib()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateMaterialAttrib</div></div>",1279:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1279\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IRenderMaterial&gt; CCausticFactory::CreateRenderMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterialAttrib,</td></tr><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderMaterial</div></div>",1280:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1280\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IRenderable&gt; CCausticFactory::CreateRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSubMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFrontMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBackMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderable</div></div>",1281:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1281\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ISampler&gt; CCausticFactory::CreateSampler(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateSampler</div></div>",1282:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1282\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ICamera&gt; CCausticFactory::CreateCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateCamera</div></div>",1283:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1283\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td class=\"PName last\">format,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateTexture</div></div>",1284:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1284\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IImage*&nbsp;</td><td class=\"PName last\">pImage,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateTexture</div></div>",1285:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1285\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::CheckerboardTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CheckerboardTexture</div></div>",1286:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1286\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::CreateDesktopTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateDesktopTexture</div></div>",1287:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1287\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::LoadTexture</div></div>",1288:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1288\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::LoadVideoTexture</div></div>",1289:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1289\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::VideoTextureFromWebcam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::VideoTextureFromWebcam</div></div>",1290:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1290\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IShader&gt; CCausticFactory::CreateShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVertexShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob*&nbsp;</td><td></td><td class=\"PName last\">pPixelShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob*&nbsp;</td><td></td><td class=\"PName last\">pComputeShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShaderInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateShader</div></div>",1291:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1291\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IShaderInfo&gt; CCausticFactory::CreateShaderInfo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateShaderInfo</div></div>"});