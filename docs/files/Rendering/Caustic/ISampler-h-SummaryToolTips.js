NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/ISampler.h",{592:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for samplers</div></div>",593:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",595:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype595\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_FILTER GetFilter() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current filter state</div></div>",596:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype596\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFilter(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">D3D11_FILTER&nbsp;</td><td class=\"PName last\">filter</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Set the current filter state</div></div>",597:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype597\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressU() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current addressing mode in the U direction</div></div>",598:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype598\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAddressU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">D3D11_TEXTURE_ADDRESS_MODE&nbsp;</td><td class=\"PName last\">mode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Set the current addressing mode in the U direction</div></div>",599:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype599\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressV() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current addressing mode in the V direction</div></div>",600:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Set the current addressing mode in the V direction</div></div>",601:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype601\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">slot,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">isPixelShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets up the sampler for the current rendering</div></div>",602:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype602\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetTexture() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the texture associated with this sampler</div></div>"});