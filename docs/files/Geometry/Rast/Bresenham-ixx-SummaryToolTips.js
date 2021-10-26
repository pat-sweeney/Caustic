﻿NDSummary.OnToolTipsLoaded("File:Geometry/Rast/Bresenham.ixx",{289:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype289\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">Bresenham</div></div></div><div class=\"TTSummary\">Defines a class for walking a line using Bresenham\'s algorithm</div></div>",291:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype291\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Bresenham()</div></div><div class=\"TTSummary\">Default constructor</div></div>",292:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype292\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Bresenham(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">fx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">fy,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">tx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">ty</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Constructor</div></div>",293:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype293\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int32</span> get_x()</div></div><div class=\"TTSummary\">Returns the X coordinate where the iterator is currently positioned</div></div>",294:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype294\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int32</span> get_y()</div></div><div class=\"TTSummary\">Returns the Y coordinate where the iterator is currently positioned</div></div>",295:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype295\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int32</span> get_endx()</div></div><div class=\"TTSummary\">Returns the ending pixel\'s X coordinate</div></div>",296:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype296\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int32</span> get_endy()</div></div><div class=\"TTSummary\">Returns the ending pixel\'s Y coordinate</div></div>",297:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype297\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> eol()</div></div><div class=\"TTSummary\">Returns true if iterator has reached end of line. False otherwise.</div></div>",298:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype298\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> step()</div></div><div class=\"TTSummary\">Steps one unit along the line</div></div>",299:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype299\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_x()</div></div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the X direction</div></div>",300:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype300\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_y()</div></div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the Y direction</div></div>",301:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype301\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_minor()</div></div><div class=\"TTSummary\">Determines whether we are about to step along the minor axis.</div></div>",302:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype302\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_x(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">dx</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Moves the iterator \'dx\' units along the x axis</div></div>",303:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype303\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_y(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">dy</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Moves the iterator \'dy\' units along the y axis</div></div>",304:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype304\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_inc(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">delta</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Moves \'delta\' units along the major axis</div></div>"});