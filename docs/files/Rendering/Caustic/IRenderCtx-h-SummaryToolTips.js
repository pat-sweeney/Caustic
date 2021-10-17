﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IRenderCtx.h",{864:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface the Render context.&nbsp; The render context defines top level states that are relevant to the entire frame.</div></div>",797:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype797\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">RenderCtxFlags</div></div></div><div class=\"TTSummary\">Defines flags for setting various render modes</div></div>",800:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",868:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype868\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDebugFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets debug flags used for rendering</div></div>",869:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype869\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetDebugFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current debug flags used for rendering</div></div>",870:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype870\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetNormalScale(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">normalScale</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the scale used to determine how large debug normal vectors are drawn</div></div>",871:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype871\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetNormalScale() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the scale used for determining how large debug normal vectors are drawn</div></div>",872:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype872\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetCurrentPass() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current render pass</div></div>",873:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype873\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> PassBlendable() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whether the current render pass supports alpha blending</div></div>",874:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype874\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> IncrementEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Increment to the next epoch</div></div>",875:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype875\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current epoch. The epoch is updated each time something changes</div></div>",876:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype876\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetMostRecentEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the most recent epoch found while traversing the render graph</div></div>",877:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype877\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMostRecentEpoch(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the most recent epoch found while traversing the render graph</div></div>"});