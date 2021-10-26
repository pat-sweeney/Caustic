﻿NDSummary.OnToolTipsLoaded("File:Base/Math/Ray.ixx",{1537:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1537\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">RayIntersect2</div></div></div><div class=\"TTSummary\">Defines results of a 2D ray intersection</div></div>",45:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype45\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Ray2</div></div></div><div class=\"TTSummary\">Defines a 2 dimensional ray</div></div>",1570:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1570\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Ray2()</div></div><div class=\"TTSummary\">Default constructor for a 2D ray</div></div>",1571:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1571\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Ray2(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">_pos,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">_dir</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Constructor for a 2D from a point/direction vector</div></div>",1572:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1572\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> IntersectWithLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p0,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">RayIntersect2&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pIntersectInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Calculates the intersection of a 2D ray with a line.</div></div>",46:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype46\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">RayIntersect3</div></div></div><div class=\"TTSummary\">Defines results of a 3D ray intersection</div></div>",64:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype64\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Ray3</div></div></div><div class=\"TTSummary\">Defines a 3 dimensional ray</div></div>",1576:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1576\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Ray3()</div></div><div class=\"TTSummary\">Defines a 3D ray</div></div>",1577:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1577\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Ray3(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">_pos,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">_dir</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Defines a 3D ray</div></div>",1578:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1578\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> Intersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">BBox3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">bbox,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">Matrix4x4&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pInvTm,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">RayIntersect3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pIntersectInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Computes intersection of ray with 3D bounding box</div></div>",1579:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1579\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> Intersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">bottomRadius,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">topRadius,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">Matrix4x4&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pInvTm,</td></tr><tr><td class=\"PType first\">RayIntersect3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pIntersectInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Calculates intersection of a ray with a cone</div></div>",1580:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1580\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> Intersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">radius,</td></tr><tr><td class=\"PType first\">Matrix4x4&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pInvTm,</td></tr><tr><td class=\"PType first\">RayIntersect3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pIntersectInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Calculates the intersection of a ray with an sphere. If pIntersectInfo is not nullptr then the intersection data is returned via this pointer.</div></div>",1581:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1581\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> Intersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">radius,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">Matrix4x4*&nbsp;</td><td class=\"PName last\">pInvTm,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersectInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Calculates the intersection of a ray with an cylinder. If pIntersectInfo is not nullptr then the intersection data is returned via this pointer.</div></div>",1582:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1582\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> Intersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p0,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p1,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">p2,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersectInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Calculates the intersection of a ray with an triangle. If pIntersectInfo is not nullptr then the intersection data is returned via this pointer.</div></div>",1583:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1583\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> Intersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PName last\">n,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">d,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersectInfo</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Calculates the intersection of a ray with an plane. If pIntersectInfo is not nullptr then the intersection data is returned via this pointer.</div></div>"});