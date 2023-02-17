﻿NDSummary.OnToolTipsLoaded("File:Rendering/GuiControls/GuiControls.ixx",{1356:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1356\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> ImGui_ScalableSlider(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pLabel,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">uniqueID,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">value,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">minValue,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">maxValue</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Builds a ImGui slider that can decrease/increase the minimum/maximum value allowed by the slider. This is useful for instance when we want to postion an object in a scene that currently only covers the range -1..+1. Now if we set the slider min/max to -1000/+1000 it would be nearly impossible to set a value of 1.5, but we still want to ultimately be able to set a max value of 1000. Instead, we grow the range of the allowed slider values by 5% each time the min or max value is set.</div></div>",1357:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1357\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> ImGui_Vector(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>*&nbsp;</td><td></td><td class=\"PName last\">pLabel,</td></tr><tr><td class=\"first\"></td><td></td><td></td><td class=\"PName last\">std::function&lt;Vector3()&gt;getFunc,</td></tr><tr><td class=\"first\"></td><td></td><td></td><td class=\"PName last\">std::function&lt;void(Vector3 v)&gt;setFunc,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">minValue,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">maxValue,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">scalableRange</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Builds ImGui UI for modifying a Vector3 using a scalable range.</div></div>"});