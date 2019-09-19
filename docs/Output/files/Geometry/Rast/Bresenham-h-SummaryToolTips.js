NDSummary.OnToolTipsLoaded("File:Geometry/Rast/Bresenham.h",{276:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype276\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">Bresenham</div></div></div><div class=\"TTSummary\">Defines a class for walking a line using Bresenham\'s algorithm</div></div>",278:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype278\" class=\"NDPrototype NoParameterForm\">Bresenham()</div><div class=\"TTSummary\">Default constructor</div></div>",279:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype279\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">Bresenham(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">fx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">fy,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">tx,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">ty</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Constructor</div></div>",280:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype280\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">int32</span> get_x()</div><div class=\"TTSummary\">Returns the X coordinate where the iterator is currently positioned</div></div>",281:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype281\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">int32</span> get_y()</div><div class=\"TTSummary\">Returns the Y coordinate where the iterator is currently positioned</div></div>",282:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype282\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">int32</span> get_endx()</div><div class=\"TTSummary\">Returns the ending pixel\'s X coordinate</div></div>",283:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype283\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">int32</span> get_endy()</div><div class=\"TTSummary\">Returns the ending pixel\'s Y coordinate</div></div>",284:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype284\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> eol()</div><div class=\"TTSummary\">Returns true if iterator has reached end of line. False otherwise.</div></div>",285:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype285\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> step()</div><div class=\"TTSummary\">Steps one unit along the line</div></div>",286:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype286\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> step_in_x()</div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the X direction</div></div>",287:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype287\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> step_in_y()</div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the Y direction</div></div>",288:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype288\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">bool</span> step_in_minor()</div><div class=\"TTSummary\">Determines whether we are about to step along the minor axis.</div></div>",289:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype289\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_x(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">dx</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Moves the iterator \'dx\' units along the x axis</div></div>",290:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype290\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_y(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">dy</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Moves the iterator \'dy\' units along the y axis</div></div>",291:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype291\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> step_inc(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int32</span>&nbsp;</td><td class=\"PName last\">delta</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Moves \'delta\' units along the major axis</div></div>"});