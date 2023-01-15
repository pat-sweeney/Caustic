﻿NDSummary.OnToolTipsLoaded("File:Geometry/GeomDS/Polygon.cpp",{296:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype296\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IPolygon2&gt; CreatePolygon2()</div></div><div class=\"TTSummary\">CreatePolygon2 creates a new 2D polygon</div></div>",297:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype297\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CPolygon2::SimplifyRecursive(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPolygon2&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pResult,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">start,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">end,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">err,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">maxLen</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">SimplifyRecursive is used internally for simplifying a subpiece of the polygon</div></div>",298:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype298\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IPolygon2&gt; CPolygon2::Simplify(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">err,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">maxLen</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Simplify Removes points from a polygon that allow the resulting polygon to still be with \'err\' units from the original polygon.&nbsp; This function uses the Ramer-Douglas-Peucker algorithm: https://en.wikipedia.org/wiki/Ramer�Douglas�Peucker_algorithm</div></div>",299:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype299\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> CPolygon2::ContainsPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">pos,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName\">pMinDist</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName\">pMaxDist</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">ContainsPoint determines whether the specified point is inside the polygon.</div></div>"});