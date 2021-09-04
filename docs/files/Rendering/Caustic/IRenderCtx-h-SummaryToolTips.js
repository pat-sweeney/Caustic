﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IRenderCtx.h",{997:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface the Render context.&nbsp; The render context defines top level states that are relevant to the entire frame.</div></div>",998:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype998\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">RenderCtxFlags</div></div></div><div class=\"TTSummary\">Defines flags for setting various render modes</div></div>",999:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",1001:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1001\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDebugFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets debug flags used for rendering</div></div>",1002:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1002\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetDebugFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current debug flags used for rendering</div></div>",1003:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1003\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetNormalScale(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">normalScale</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the scale used to determine how large debug normal vectors are drawn</div></div>",1004:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1004\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetNormalScale() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the scale used for determining how large debug normal vectors are drawn</div></div>",1005:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1005\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetCurrentPass() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current render pass</div></div>",1006:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1006\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> PassBlendable() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whether the current render pass supports alpha blending</div></div>",1007:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1007\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> IncrementEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Increment to the next epoch</div></div>",1008:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1008\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current epoch. The epoch is updated each time something changes</div></div>",1009:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1009\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetMostRecentEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the most recent epoch found while traversing the render graph</div></div>",1010:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1010\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMostRecentEpoch(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the most recent epoch found while traversing the render graph</div></div>"});