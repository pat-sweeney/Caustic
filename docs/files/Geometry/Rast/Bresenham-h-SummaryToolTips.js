NDSummary.OnToolTipsLoaded("File:Geometry/Rast/Bresenham.h",{303:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype303\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">Bresenham</div></div></div><div class=\"TTSummary\">Defines a class for walking a line using Bresenham\'s algorithm</div></div>",305:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype305\" class=\"NDPrototype NoParameterForm\">Bresenham()</div><div class=\"TTSummary\">Default constructor</div></div>",306:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype306\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">Bresenham(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">fx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">fy,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">tx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">ty</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Constructor</div></div>",307:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype307\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">int32</span> get_x()</div><div class=\"TTSummary\">Returns the X coordinate where the iterator is currently positioned</div></div>",308:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype308\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">int32</span> get_y()</div><div class=\"TTSummary\">Returns the Y coordinate where the iterator is currently positioned</div></div>",309:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype309\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">int32</span> get_endx()</div><div class=\"TTSummary\">Returns the ending pixel\'s X coordinate</div></div>",310:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype310\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">int32</span> get_endy()</div><div class=\"TTSummary\">Returns the ending pixel\'s Y coordinate</div></div>",311:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype311\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> eol()</div><div class=\"TTSummary\">Returns true if iterator has reached end of line. False otherwise.</div></div>",312:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype312\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> step()</div><div class=\"TTSummary\">Steps one unit along the line</div></div>",313:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype313\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> step_in_x()</div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the X direction</div></div>",314:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype314\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> step_in_y()</div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the Y direction</div></div>",315:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype315\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> step_in_minor()</div><div class=\"TTSummary\">Determines whether we are about to step along the minor axis.</div></div>",316:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype316\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_x(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">dx</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Moves the iterator \'dx\' units along the x axis</div></div>",317:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype317\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_y(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">dy</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Moves the iterator \'dy\' units along the y axis</div></div>",318:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype318\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_inc(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">delta</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Moves \'delta\' units along the major axis</div></div>"});