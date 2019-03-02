NDSummary.OnToolTipsLoaded("CClass:BBox3",{212:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines a axis-aligned bounding box (in 3D)</div></div>",214:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype214\" class=\"NDPrototype NoParameterForm\">BBox3()</div><div class=\"TTSummary\">Default constructor</div></div>",215:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype215\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">BBox3(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">frontTopLeft,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">backBottomRight</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div>",216:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype216\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> Empty()</div><div class=\"TTSummary\">Determines if bbox is empty</div></div>",217:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype217\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> AddPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">y,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">z</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Adds a point to a bounding box. If the point is outside the bbox, the bbox is updated.</div></div>",218:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype218\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> AddPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">pt</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Adds a point to a bounding box. If the point is outside the bbox, the bbox is updated.</div></div>",219:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype219\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> PointInside(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">p</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Tests if the specified point is inside the bounding box</div></div>",220:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype220\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> PointInside(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">p,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">tolerance</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Tests if the specified point +/- tolerance is inside the bounding box</div></div>"});