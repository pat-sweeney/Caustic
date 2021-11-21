﻿NDSummary.OnToolTipsLoaded("CClass:ISampler",{1006:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",1008:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1008\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_FILTER GetFilter() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current filter state</div></div>",1009:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1009\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFilter(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">D3D11_FILTER&nbsp;</td><td class=\"PName last\">filter</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Set the current filter state</div></div>",1010:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1010\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressU() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current addressing mode in the U direction</div></div>",1011:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1011\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAddressU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">D3D11_TEXTURE_ADDRESS_MODE&nbsp;</td><td class=\"PName last\">mode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Set the current addressing mode in the U direction</div></div>",1012:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1012\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressV() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current addressing mode in the V direction</div></div>",1013:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Set the current addressing mode in the V direction</div></div>",1014:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1014\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">slot,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">isPixelShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets up the sampler for the current rendering</div></div>",1015:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1015\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetTexture() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the texture associated with this sampler</div></div>",1016:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1016\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ISampler&gt; CreateSampler(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">ITexture*&nbsp;</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a sampler. See ISampler</div></div>"});