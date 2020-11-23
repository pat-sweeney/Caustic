NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/ISampler.h",{717:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for samplers</div></div>",718:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",720:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype720\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_FILTER GetFilter() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current filter state</div></div>",1329:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1329\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFilter(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">D3D11_FILTER&nbsp;</td><td></td><td class=\"PName last\">filter</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Set the current filter state</div></div>",722:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype722\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressU() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current addressing mode in the U direction</div></div>",1330:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1330\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAddressU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">D3D11_TEXTURE_ADDRESS_MODE&nbsp;</td><td></td><td class=\"PName last\">mode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Set the current addressing mode in the U direction</div></div>",724:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype724\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressV() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current addressing mode in the V direction</div></div>",1331:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Set the current addressing mode in the V direction</div></div>",726:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype726\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">slot,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">isPixelShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets up the sampler for the current rendering</div></div>",727:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype727\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetTexture() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the texture associated with this sampler</div></div>",728:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype728\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ISampler&gt; CreateSampler(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates a sampler. See ISampler</div></div>"});