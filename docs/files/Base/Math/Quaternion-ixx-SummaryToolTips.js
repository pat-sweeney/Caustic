﻿NDSummary.OnToolTipsLoaded("File:Base/Math/Quaternion.ixx",{111:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype111\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Quaternion</div></div></div><div class=\"TTSummary\">Defines a quaternion</div></div>",113:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype113\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Quaternion()</div></div><div class=\"TTSummary\">Default ctor</div></div>",114:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype114\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Quaternion(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Matrix4x4&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">tm</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Constructor from 4x4 matrix</div></div>",115:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype115\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Quaternion(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Matrix4x3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">tm</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Constructor from 4x3 matrix</div></div>",116:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype116\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Quaternion(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">vec,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">angle</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Constructor from vector and angle</div></div>",117:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype117\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Quaternion(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">_x,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">_y,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">_z,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">_w,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">isvec</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">This ctor creates a Quaternion for a list of floats.&nbsp; If \'isvec\' is TRUE then _x,_y,_z form a direction vector and _w is the angle of rotation about the direction vector.&nbsp; Otherwise, _x,_y,_z,_w are treated as the four components of a quaternion.</div></div>",118:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype118\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Quaternion <span class=\"SHKeyword\">operator</span> *(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Quaternion&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">q</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Multiplies two quaternions</div></div>",119:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype119\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Quaternion &amp;<span class=\"SHKeyword\">operator</span> *=(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Quaternion&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">q</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Multiplies two quaternions</div></div>",120:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype120\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Quaternion Conjugate()</div></div><div class=\"TTSummary\">Computes the conjugate of the quaternion</div></div>",121:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype121\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Quaternion Slerp(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Quaternion&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">q1,</td></tr><tr><td class=\"PType first\">Quaternion&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">q2,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">t</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Computes the spherical linear interpolation between two quaternions</div></div>"});