﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/ITrackball.ixx",{457:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for talking to the track ball manipulator.</div></div>",618:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype618\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">enum</span> </div><div class=\"CPName\">ETrackballConstraint</div></div></div><div class=\"TTSummary\">Defines available constraints for the trackball object</div></div>",620:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface to our trackball implementation</div></div>",753:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype753\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BeginTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenW,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenH</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Called to start tracking</div></div>",756:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype756\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> UpdateTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ETrackballConstraint&nbsp;</td><td></td><td class=\"PName last\">constraint,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMatrix</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Called when mouse moves during trackball tracking</div></div>",757:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype757\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> EndTracking() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Called when mouse is released during trackball tracking</div></div>",758:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype758\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ITrackball&gt; CreateTrackball()</div></div><div class=\"TTSummary\">Global function for creating a track ball. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>"});