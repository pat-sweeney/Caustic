﻿NDSummary.OnToolTipsLoaded("CClass:IRenderCtx",{596:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",598:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype598\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDebugFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets debug flags used for rendering</div></div>",599:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype599\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetDebugFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current debug flags used for rendering</div></div>",621:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype621\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetNormalScale(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">normalScale</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the scale used to determine how large debug normal vectors are drawn</div></div>",797:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype797\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetNormalScale() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the scale used for determining how large debug normal vectors are drawn</div></div>",800:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype800\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetCurrentPass() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current render pass</div></div>",864:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype864\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> PassBlendable() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whether the current render pass supports alpha blending</div></div>",867:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype867\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> IncrementEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Increment to the next epoch</div></div>",868:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype868\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current epoch. The epoch is updated each time something changes</div></div>",869:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype869\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetMostRecentEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the most recent epoch found while traversing the render graph</div></div>",870:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype870\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMostRecentEpoch(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the most recent epoch found while traversing the render graph</div></div>"});