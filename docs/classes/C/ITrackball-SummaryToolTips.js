﻿NDSummary.OnToolTipsLoaded("CClass:ITrackball",{896:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface to our trackball implementation</div></div>",898:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype898\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BeginTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenW,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenH</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Called to start tracking</div></div>",899:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype899\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> UpdateTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ETrackballConstraint&nbsp;</td><td></td><td class=\"PName last\">constraint,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMatrix</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Called when mouse moves during trackball tracking</div></div>",900:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype900\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> EndTracking() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Called when mouse is released during trackball tracking</div></div>",901:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype901\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ITrackball&gt; CreateTrackball()</div></div><div class=\"TTSummary\">Global function for creating a track ball. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>"});