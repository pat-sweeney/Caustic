﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/ISpotLight.h",{1106:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for spot lights</div></div>",1107:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a spot light</div></div>",1109:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1109\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAngles(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">float2&nbsp;</td><td class=\"PName last\">angles</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the inner (angles.x) and outer (angles.y) angles for the spotlight.&nbsp; The inner angle defines the area around the direction vector where the light is full intensity. The light intensity then falls off from the inner angle to the outer angle. Areas outside the outer angle are not effected by the light.</div></div>",1110:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1110\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> float2 GetAngles() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Gets the inner (angles.x) and outer (angles.y) angles for the spotlight.&nbsp; The inner angle defines the area around the direction vector where the light is full intensity. The light intensity then falls off from the inner angle to the outer angle. Areas outside the outer angle are not effected by the light.</div></div>",1111:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1111\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ISpotLight&gt; CreateSpotLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td></td><td class=\"PName\">pos,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">dir,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">FRGBColor&amp;&nbsp;</td><td></td><td class=\"PName\">color,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName\">intensity</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">1000.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName\">innerAngle</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">30.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName\">outerAngle</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">45.0f</span>,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">casts</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">true</span></td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a spot light</div></div>"});