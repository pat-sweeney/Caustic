﻿NDSummary.OnToolTipsLoaded("File:Geometry/Rast/Bresenham.ixx",{455:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype455\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">Bresenham</div></div></div><div class=\"TTSummary\">Defines a class for walking a line using Bresenham\'s algorithm</div></div>",457:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype457\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Bresenham()</div></div><div class=\"TTSummary\">Default constructor</div></div>",458:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype458\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">Bresenham(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">fx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">fy,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">tx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">ty</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Constructor</div></div>",459:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype459\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int32</span> get_x()</div></div><div class=\"TTSummary\">Returns the X coordinate where the iterator is currently positioned</div></div>",460:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype460\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int32</span> get_y()</div></div><div class=\"TTSummary\">Returns the Y coordinate where the iterator is currently positioned</div></div>",461:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype461\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int32</span> get_endx()</div></div><div class=\"TTSummary\">Returns the ending pixel\'s X coordinate</div></div>",462:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype462\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int32</span> get_endy()</div></div><div class=\"TTSummary\">Returns the ending pixel\'s Y coordinate</div></div>",463:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype463\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> eol()</div></div><div class=\"TTSummary\">Returns true if iterator has reached end of line. False otherwise.</div></div>",464:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype464\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> step()</div></div><div class=\"TTSummary\">Steps one unit along the line</div></div>",465:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype465\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_x()</div></div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the X direction</div></div>",466:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype466\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_y()</div></div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the Y direction</div></div>",467:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype467\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_minor()</div></div><div class=\"TTSummary\">Determines whether we are about to step along the minor axis.</div></div>",468:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype468\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_x(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">dx</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Moves the iterator \'dx\' units along the x axis</div></div>",469:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype469\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_y(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">dy</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Moves the iterator \'dy\' units along the y axis</div></div>",470:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype470\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_inc(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">delta</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Moves \'delta\' units along the major axis</div></div>"});