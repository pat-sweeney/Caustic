﻿NDSummary.OnToolTipsLoaded("File:Base/Math/Vector.ixx",{114:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype114\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector2</div></div></div><div class=\"TTSummary\">Defines a simple vector with 2 components</div></div>",116:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype116\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Vector2() : x(<span class=\"SHNumber\">0.0</span>), y(<span class=\"SHNumber\">0.0</span>)</div></div><div class=\"TTSummary\">Default contructor</div></div>",117:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype117\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Vector2(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_y</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Contructor for a 2D vector</div></div>",118:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype118\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> IsEq(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns whether two points are equivalent (within some tolerance)</div></div>",119:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype119\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">float</span> Length() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns the L2 norm for the vector</div></div>",120:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype120\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Vector2 Normalize()</div></div><div class=\"TTSummary\">Normalizes the vector</div></div>",121:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype121\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">float</span> Cross(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Computes cross product between two vectors</div></div>",122:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype122\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">float</span> Dot(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Computes dot product between two vectors</div></div>",123:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype123\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector3</div></div></div><div class=\"TTSummary\">Defines a simple vector with 3 components</div></div>",125:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype125\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Vector3() : x(<span class=\"SHNumber\">0.0f</span>), y(<span class=\"SHNumber\">0.0f</span>), z(<span class=\"SHNumber\">0.0f</span>)</div></div><div class=\"TTSummary\">Default constructor</div></div>",126:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype126\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Vector3(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_x,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_y,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_z</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Constructor</div></div>",127:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype127\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int</span> Sign() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns -1 or +1 depending on direction vector is pointing</div></div>",128:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype128\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> IsEq(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns whether two points are equivalent (within some tolerance)</div></div>",129:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype129\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Vector3 Cross(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the cross product between two vectors</div></div>",130:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype130\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">float</span> Dot(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the scalar product between two vectors</div></div>",131:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a variation of Vector3 that also supports swizzling. The reason this isn\'t part of Vector3 is that construction of all properties is far too heavy to place on the main class. This is only useful in non-critical path (in terms of speed) code. The usage generally looks like: Vector3 newv = ((Vector3Ext)v).zyx;</div></div>",132:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype132\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector4</div></div></div><div class=\"TTSummary\">Defines a simple vector with 4 components</div></div>",134:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype134\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> IsEq(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector4&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns whether two points are equivalent (within some tolerance)</div></div>",135:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a variation of Vector4 that also supports swizzling. The reason this isn\'t part of Vector4 is that construction of all properties is far too heavy to place on the main class. This is only useful in non-critical path (in terms of speed) code. The usage generally looks like: Vector4 newv = ((Vector4Ext)v).zyx;</div></div>"});