﻿NDSummary.OnToolTipsLoaded("CClass:IPolygon2",{284:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a 2D polygon</div></div>",286:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype286\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetNumberPoints() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of points in the polygon</div></div>",287:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype287\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pt</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the requested point (by index)</div></div>",288:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype288\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pt</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a point to the polygon</div></div>",289:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype289\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox2&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the 2D bounding box of the polygon</div></div>",290:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype290\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IPolygon2&gt; Simplify(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">err,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">maxLen</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Removes points from a polygon that allow the resulting polygon to still be with \'err\' units from the original polygon.&nbsp; This function uses the Ramer-Douglas-Peucker algorithm:</div></div>",291:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype291\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> ContainsPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">pt,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName\">pMinDist</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName\">pMaxDist</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Determines whether the specified point is inside the polygon.</div></div>"});