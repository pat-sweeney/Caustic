﻿NDSummary.OnToolTipsLoaded("File:Base/Math/Matrix4x4.ixx",{170:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype170\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">export namespace</span> </div><div class=\"CPName\">Caustic</div></div></div></div>",171:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype171\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Matrix4x4</div></div></div><div class=\"TTSummary\">A 4x4 matrix</div></div>",173:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype173\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">float</span> Determinant()</div></div><div class=\"TTSummary\">Returns the determinant of the matrix</div></div>",174:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype174\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Matrix4x4 Adjoint()</div></div><div class=\"TTSummary\">Returns the adjoint of the matrix</div></div>",175:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype175\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> Transpose()</div></div><div class=\"TTSummary\">Computes the inplace transpose of the matrix</div></div>",176:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype176\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> Transpose(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Matrix4x4&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">tm</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the transpose of the matrix</div></div>",177:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype177\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> Inverse()</div></div><div class=\"TTSummary\">Computes the inplace inverse of the matrix</div></div>",178:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype178\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> Inverse(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Matrix4x4&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">tm</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns the inverse of the matrix</div></div>",179:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype179\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> Decompose(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">scale,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">shear,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">rotate,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">translate</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Decomposes the matrix into a scale, shear, rotate and translate components</div></div>",180:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype180\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static</span> Matrix4x4 ScalingMatrix(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">sx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">sy,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">sz</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a scaling matrix</div></div>",181:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype181\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static</span> Matrix4x4 RotationMatrix(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">ax,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">ay,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">az</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a rotation matrix</div></div>",182:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype182\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static</span> Matrix4x4 TranslationMatrix(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">tx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">ty,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">tz</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a translation matrix</div></div>"});