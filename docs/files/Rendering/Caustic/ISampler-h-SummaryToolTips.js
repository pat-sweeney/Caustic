﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/ISampler.h",{911:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for samplers</div></div>",912:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",914:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype914\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_FILTER GetFilter() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current filter state</div></div>",915:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype915\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFilter(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">D3D11_FILTER&nbsp;</td><td class=\"PName last\">filter</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Set the current filter state</div></div>",916:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype916\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressU() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current addressing mode in the U direction</div></div>",917:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype917\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAddressU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">D3D11_TEXTURE_ADDRESS_MODE&nbsp;</td><td class=\"PName last\">mode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Set the current addressing mode in the U direction</div></div>",918:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype918\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressV() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current addressing mode in the V direction</div></div>",919:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Set the current addressing mode in the V direction</div></div>",920:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype920\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">slot,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">isPixelShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets up the sampler for the current rendering</div></div>",921:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype921\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetTexture() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the texture associated with this sampler</div></div>",922:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype922\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ISampler&gt; CreateSampler(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">ITexture*&nbsp;</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a sampler. See ISampler</div></div>"});