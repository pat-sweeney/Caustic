NDSummary.OnToolTipsLoaded("File:Geometry/GeomDS/IPolygon.h",{151:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a 2D polygon</div></div>",153:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype153\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetNumberPoints() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the number of points in the polygon</div></div>",154:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype154\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pt</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the requested point (by index)</div></div>",155:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype155\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">pt</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Adds a point to the polygon</div></div>",156:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype156\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox2&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the 2D bounding box of the polygon</div></div>",167:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype167\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IPolygon2&gt; Simplify(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">err,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">maxLen</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Removes points from a polygon that allow the resulting polygon to still be with \'err\' units from the original polygon.&nbsp; This function uses the Ramer-Douglas-Peucker algorithm:</div></div>",158:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype158\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> ContainsPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName\">pt,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName\">pMinDist&nbsp;</td><td class=\"PDefaultValueSeparator\">=&nbsp;</td><td class=\"PDefaultValue last\">nullptr,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName\">pMaxDist&nbsp;</td><td class=\"PDefaultValueSeparator\">=&nbsp;</td><td class=\"PDefaultValue last\">nullptr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Determines whether the specified point is inside the polygon.</div></div>"});