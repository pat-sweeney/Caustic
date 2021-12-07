﻿NDSummary.OnToolTipsLoaded("File:Base/Math/Vector.ixx",{78:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype78\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector2</div></div></div><div class=\"TTSummary\">Defines a simple vector with 2 components</div></div>",80:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype80\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Vector2() : x(<span class=\"SHNumber\">0.0</span>), y(<span class=\"SHNumber\">0.0</span>)</div></div><div class=\"TTSummary\">Default contructor</div></div>",81:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype81\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Vector2(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_y</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Contructor for a 2D vector</div></div>",82:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype82\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> IsEq(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns whether two points are equivalent (within some tolerance)</div></div>",83:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype83\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">float</span> Length() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns the L2 norm for the vector</div></div>",84:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype84\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Vector2 Normalize()</div></div><div class=\"TTSummary\">Normalizes the vector</div></div>",85:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype85\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">float</span> Cross(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Computes cross product between two vectors</div></div>",86:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype86\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">float</span> Dot(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Computes dot product between two vectors</div></div>",87:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype87\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector3</div></div></div><div class=\"TTSummary\">Defines a simple vector with 3 components</div></div>",89:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype89\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Vector3() : x(<span class=\"SHNumber\">0.0f</span>), y(<span class=\"SHNumber\">0.0f</span>), z(<span class=\"SHNumber\">0.0f</span>)</div></div><div class=\"TTSummary\">Default constructor</div></div>",90:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype90\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Vector3(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_x,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_y,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">_z</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Constructor</div></div>",91:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype91\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int</span> Sign() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns -1 or +1 depending on direction vector is pointing</div></div>",92:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype92\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> IsEq(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns whether two points are equivalent (within some tolerance)</div></div>",93:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype93\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Vector3 Cross(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the cross product between two vectors</div></div>",94:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype94\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">float</span> Dot(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the scalar product between two vectors</div></div>",95:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a variation of Vector3 that also supports swizzling. The reason this isn\'t part of Vector3 is that construction of all properties is far too heavy to place on the main class. This is only useful in non-critical path (in terms of speed) code. The usage generally looks like: Vector3 newv = ((Vector3Ext)v).zyx;</div></div>",96:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype96\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector4</div></div></div><div class=\"TTSummary\">Defines a simple vector with 4 components</div></div>",98:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype98\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> IsEq(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector4&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Returns whether two points are equivalent (within some tolerance)</div></div>",99:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a variation of Vector4 that also supports swizzling. The reason this isn\'t part of Vector4 is that construction of all properties is far too heavy to place on the main class. This is only useful in non-critical path (in terms of speed) code. The usage generally looks like: Vector4 newv = ((Vector4Ext)v).zyx;</div></div>"});