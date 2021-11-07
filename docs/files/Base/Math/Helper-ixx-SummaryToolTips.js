﻿NDSummary.OnToolTipsLoaded("File:Base/Math/Helper.ixx",{19:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype19\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">export namespace</span> </div><div class=\"CPName\">Caustic</div></div></div></div>",59:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype59\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">inline uint32</span> isqrt(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">n</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Computes the sqrt of an integer via Newtons method without using floating point.&nbsp; See https://&#8203;en&#8203;.wikipedia&#8203;.org&#8203;/wiki&#8203;/Integer_square_root for details.</div></div>",99:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype99\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">float</span> DistancePointToLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector2&amp;&nbsp;</td><td class=\"PName last\">p0,</td></tr><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector2&amp;&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector2&amp;&nbsp;</td><td class=\"PName last\">pt</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Determines the distance of a given point from a line</div></div>",1084:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1084\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">extern float</span> Determinant2x2(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a00,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a01,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a10,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a11</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Computes the determinant of a 2x2 matrix.</div></div>",1085:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1085\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">extern float</span> Determinant3x3(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a00,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a01,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a02,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a10,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a11,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a12,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a20,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a21,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">a22</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Computes the determinant of a 3x3 matrix.</div></div>",1086:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1086\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">extern void</span> CircumCircle(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p0,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pCircumCenter,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRadius</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Given 3 points of a triangle this function will compute the center and radius of the circle passing through those points</div></div>",1087:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1087\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">extern bool</span> PointInTriangleCircumcircle(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p0,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pt</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Tests if a point is inside the specified circumcircle</div></div>"});