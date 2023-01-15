﻿NDSummary.OnToolTipsLoaded("CClass:BBox3",{245:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype245\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">BBox3</div></div></div><div class=\"TTSummary\">Defines a axis-aligned bounding box (in 3D)</div></div>",247:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype247\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">BBox3()</div></div><div class=\"TTSummary\">Default constructor</div></div>",248:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype248\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">BBox3(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">frontTopLeft,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">backBottomRight</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div></div>",249:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype249\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> Empty()</div></div><div class=\"TTSummary\">Determines if bbox is empty</div></div>",250:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype250\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Vector3 Center()</div></div><div class=\"TTSummary\">Returns the center of the bounding box.</div></div>",251:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype251\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> AddPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">y,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">z</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds a point to a bounding box. If the point is outside the bbox, the bbox is updated.</div></div>",252:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype252\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> AddPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pt</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds a point to a bounding box. If the point is outside the bbox, the bbox is updated.</div></div>",253:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype253\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> PointInside(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Tests if the specified point is inside the bounding box</div></div>",254:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype254\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> PointInside(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">tolerance</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Tests if the specified point +/- tolerance is inside the bounding box</div></div>"});