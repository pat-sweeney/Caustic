﻿NDSummary.OnToolTipsLoaded("File:Geometry/GeomDS/Polygon.cpp",{321:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype321\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IPolygon2&gt; CreatePolygon2()</div></div><div class=\"TTSummary\">CreatePolygon2 creates a new 2D polygon</div></div>",322:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype322\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">void</span> CPolygon2::SimplifyRecursive(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IPolygon2&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">pResult,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">start,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/4/4/5\" data-NarrowGridArea=\"4/3/5/4\" style=\"grid-area:3/4/4/5\">end,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"4/2/5/3\" data-NarrowGridArea=\"5/1/6/2\" style=\"grid-area:4/2/5/3\"><span class=\"SHKeyword\">float</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/4/5/5\" data-NarrowGridArea=\"5/3/6/4\" style=\"grid-area:4/4/5/5\">err,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"5/2/6/3\" data-NarrowGridArea=\"6/1/7/2\" style=\"grid-area:5/2/6/3\"><span class=\"SHKeyword\">float</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"5/4/6/5\" data-NarrowGridArea=\"6/3/7/4\" style=\"grid-area:5/4/6/5\">maxLen</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"7/1/8/5\" style=\"grid-area:5/5/6/6\">)</div></div></div></div><div class=\"TTSummary\">SimplifyRecursive is used internally for simplifying a subpiece of the polygon</div></div>",323:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype323\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/3/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/3/2\">CRefObj&lt;IPolygon2&gt; CPolygon2::Simplify(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">float</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">err,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\"><span class=\"SHKeyword\">float</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">maxLen</div><div class=\"PAfterParameters\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"4/1/5/4\" style=\"grid-area:2/4/3/5\">)</div></div></div></div><div class=\"TTSummary\">Simplify Removes points from a polygon that allow the resulting polygon to still be with \'err\' units from the original polygon.&nbsp; This function uses the Ramer-Douglas-Peucker algorithm: https://en.wikipedia.org/wiki/Ramer�Douglas�Peucker_algorithm</div></div>",324:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype324\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"7\" data-NarrowColumnCount=\"6\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/4/2\" data-NarrowGridArea=\"1/1/2/7\" style=\"grid-area:1/1/4/2\"><span class=\"SHKeyword\">bool</span> CPolygon2::ContainsPoint(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">Vector2&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">&amp;</div><div class=\"PName\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">pos,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\"><span class=\"SHKeyword\">float</span>&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">*</div><div class=\"PName\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">pMinDist</div><div class=\"PDefaultValueSeparator\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">&nbsp=&nbsp;</div><div class=\"PDefaultValue InLastParameterColumn\" data-WideGridArea=\"2/6/3/7\" data-NarrowGridArea=\"3/5/4/6\" style=\"grid-area:2/6/3/7\">nullptr,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\"><span class=\"SHKeyword\">float</span>&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">*</div><div class=\"PName\" data-WideGridArea=\"3/4/4/5\" data-NarrowGridArea=\"4/3/5/4\" style=\"grid-area:3/4/4/5\">pMaxDist</div><div class=\"PDefaultValueSeparator\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">&nbsp=&nbsp;</div><div class=\"PDefaultValue InLastParameterColumn\" data-WideGridArea=\"3/6/4/7\" data-NarrowGridArea=\"4/5/5/6\" style=\"grid-area:3/6/4/7\">nullptr</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"3/7/4/8\" data-NarrowGridArea=\"5/1/6/7\" style=\"grid-area:3/7/4/8\">)</div></div></div></div><div class=\"TTSummary\">ContainsPoint determines whether the specified point is inside the polygon.</div></div>"});