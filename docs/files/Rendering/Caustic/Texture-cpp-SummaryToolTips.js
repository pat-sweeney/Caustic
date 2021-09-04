﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/Texture.cpp",{691:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype691\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td class=\"PName last\">format,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates an empty texture.</div></div>",692:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype692\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td class=\"PName last\">format</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates an empty texture.</div></div>",693:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype693\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IImage*&nbsp;</td><td class=\"PName last\">pImage,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a texture from an &lt;IImage at Caustic::IImage&gt;.</div></div>",694:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype694\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; CheckerboardTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">w ,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">h ,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">blocksize</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a black and white checkboard texture</div></div>",695:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype695\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">LoadTexture loads a texture from a file using WIC</div></div>",696:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype696\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CTexture::CopyFromImage(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IImage*&nbsp;</td><td class=\"PName last\">pImage,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">generateMipMap</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Copies an image\'s pixels into a texture</div></div>",697:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype697\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IImage&gt; CTexture::CopyToImage(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Copies an texture\'s pixels into an IImage</div></div>",698:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype698\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CTexture::GenerateMips(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Generates the MIPMAP chain for a texture</div></div>",699:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype699\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CTexture::CTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td></td><td class=\"PName last\">format,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td></td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td></td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">) : m_Format(format), m_Width(width), m_Height(height)</td></tr></table></div></div><div class=\"TTSummary\">Ctor for our texture wrapper object</div></div>"});