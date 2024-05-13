﻿NDSummary.OnToolTipsLoaded("CClass:Bresenham",{522:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype522\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">Bresenham</div></div></div><div class=\"TTSummary\">Defines a class for walking a line using Bresenham\'s algorithm</div></div>",524:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype524\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">Bresenham()</div></div><div class=\"TTSummary\">Default constructor</div></div>",525:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype525\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/5/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/5/2\">Bresenham(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">int32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">fx,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">int32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">fy,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\">int32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">tx,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"4/2/5/3\" data-NarrowGridArea=\"5/1/6/2\" style=\"grid-area:4/2/5/3\">int32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">ty</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"4/4/5/5\" data-NarrowGridArea=\"6/1/7/4\" style=\"grid-area:4/4/5/5\">)</div></div></div></div><div class=\"TTSummary\">Constructor</div></div>",526:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype526\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">int32_t get_x()</div></div><div class=\"TTSummary\">Returns the X coordinate where the iterator is currently positioned</div></div>",527:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype527\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">int32_t get_y()</div></div><div class=\"TTSummary\">Returns the Y coordinate where the iterator is currently positioned</div></div>",528:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype528\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">int32_t get_endx()</div></div><div class=\"TTSummary\">Returns the ending pixel\'s X coordinate</div></div>",529:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype529\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">int32_t get_endy()</div></div><div class=\"TTSummary\">Returns the ending pixel\'s Y coordinate</div></div>",530:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype530\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> eol()</div></div><div class=\"TTSummary\">Returns true if iterator has reached end of line. False otherwise.</div></div>",531:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype531\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> step()</div></div><div class=\"TTSummary\">Steps one unit along the line</div></div>",532:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype532\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_x()</div></div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the X direction</div></div>",533:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype533\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_y()</div></div><div class=\"TTSummary\">Returns if a call to step() will result in a step in the Y direction</div></div>",534:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype534\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> step_in_minor()</div></div><div class=\"TTSummary\">Determines whether we are about to step along the minor axis.</div></div>",535:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype535\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">void</span> step_x(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">int32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">dx</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">)</div></div></div></div><div class=\"TTSummary\">Moves the iterator \'dx\' units along the x axis</div></div>",536:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype536\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">void</span> step_y(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">int32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">dy</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">)</div></div></div></div><div class=\"TTSummary\">Moves the iterator \'dy\' units along the y axis</div></div>",537:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype537\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">void</span> step_inc(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">int32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">delta</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">)</div></div></div></div><div class=\"TTSummary\">Moves \'delta\' units along the major axis</div></div>"});