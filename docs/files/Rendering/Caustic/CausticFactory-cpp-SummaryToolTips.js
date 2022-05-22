﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/CausticFactory.cpp",{1212:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CCausticFactory object.</div></div>",1214:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1214\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CCausticFactory::CCausticFactory()</div></div><div class=\"TTSummary\">Defines the implementation of ICausticFactory</div></div>",1215:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1215\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CCausticFactory::~CCausticFactory()</div></div><div class=\"TTSummary\">Implements the dtor for CCausticFactory</div></div>",1216:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1216\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IRenderer&gt; CCausticFactory::CreateRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName last\">hwnd,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">shaderFolder,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">startFrozen ,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">desktopIndex</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderer</div></div>",1217:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1217\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IPointCloud&gt; CCausticFactory::CreatePointCloud(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreatePointCloud</div></div>",1218:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1218\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IRenderMesh&gt; CCausticFactory::CreateRenderMesh()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderMesh</div></div>",1219:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1219\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IRenderSubMesh&gt; CCausticFactory::CreateRenderSubMesh()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderSubMesh</div></div>",1220:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1220\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IPointLight&gt; CCausticFactory::CreatePointLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">intensity</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreatePointLight</div></div>",1221:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1221\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ISpotLight&gt; CCausticFactory::CreateSpotLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">dir,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">intensity,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">innerAngle,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">outerAngle,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">casts</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateSpotLight</div></div>",1222:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1222\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IDirectionalLight&gt; CCausticFactory::CreateDirectionalLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">pos,</td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">dir,</td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td class=\"PName last\">color,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">intensity</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateDirectionalLight</div></div>",1223:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1223\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ITrackball&gt; CCausticFactory::CreateTrackball()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateTrackball</div></div>",1224:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1224\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IMaterialAttrib&gt; CCausticFactory::CreateMaterialAttrib()</div></div><div class=\"TTSummary\">See ICausticFactory::CreateMaterialAttrib</div></div>",1225:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1225\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IRenderMaterial&gt; CCausticFactory::CreateRenderMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterialAttrib,</td></tr><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderMaterial</div></div>",1226:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1226\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IRenderable&gt; CCausticFactory::CreateRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSubMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFrontMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBackMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateRenderable</div></div>",1227:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1227\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ISampler&gt; CCausticFactory::CreateSampler(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateSampler</div></div>",1228:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1228\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ICamera&gt; CCausticFactory::CreateCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateCamera</div></div>",1229:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1229\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td class=\"PName last\">format,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateTexture</div></div>",1230:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1230\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IImage*&nbsp;</td><td class=\"PName last\">pImage,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateTexture</div></div>",1231:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1231\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::CheckerboardTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CheckerboardTexture</div></div>",1232:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1232\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::CreateDesktopTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateDesktopTexture</div></div>",1233:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1233\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::LoadTexture</div></div>",1234:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1234\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::LoadVideoTexture</div></div>",1235:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1235\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ITexture&gt; CCausticFactory::VideoTextureFromWebcam(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::VideoTextureFromWebcam</div></div>",1236:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1236\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IShader&gt; CCausticFactory::CreateShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pVertexShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob*&nbsp;</td><td></td><td class=\"PName last\">pPixelShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ID3DBlob*&nbsp;</td><td></td><td class=\"PName last\">pComputeShaderBlob,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShaderInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateShader</div></div>",1237:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1237\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IShaderInfo&gt; CCausticFactory::CreateShaderInfo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICausticFactory::CreateShaderInfo</div></div>"});