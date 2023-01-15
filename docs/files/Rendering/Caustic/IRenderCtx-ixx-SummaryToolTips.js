﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IRenderCtx.ixx",{1108:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface the Render context.&nbsp; The render context defines top level states that are relevant to the entire frame.</div></div>",1109:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1109\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">RenderCtxFlags</div></div></div><div class=\"TTSummary\">Defines flags for setting various render modes</div></div>",1110:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",1112:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1112\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDebugFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets debug flags used for rendering</div></div>",1113:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1113\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetDebugFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current debug flags used for rendering</div></div>",1114:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1114\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetNormalScale(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">normalScale</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the scale used to determine how large debug normal vectors are drawn</div></div>",1115:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1115\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetNormalScale() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the scale used for determining how large debug normal vectors are drawn</div></div>",1116:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1116\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetCurrentPass() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current render pass</div></div>",1117:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1117\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> PassBlendable() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whether the current render pass supports alpha blending</div></div>",1118:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1118\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> IncrementEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Increment to the next epoch</div></div>",1119:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1119\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current epoch. The epoch is updated each time something changes</div></div>",1120:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1120\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetMostRecentEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the most recent epoch found while traversing the render graph</div></div>",1121:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1121\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMostRecentEpoch(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the most recent epoch found while traversing the render graph</div></div>"});