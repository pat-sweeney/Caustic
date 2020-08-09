NDSummary.OnToolTipsLoaded("CClass:ITrackball",{633:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface to our trackball implementation</div></div>",635:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype635\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BeginTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenW,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenH</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Called to start tracking</div></div>",636:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype636\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> UpdateTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ETrackballConstraint&nbsp;</td><td></td><td class=\"PName last\">constraint,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMatrix</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Called when mouse moves during trackball tracking</div></div>",637:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype637\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> EndTracking() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Called when mouse is released during trackball tracking</div></div>",1141:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1141\" class=\"NDPrototype NoParameterForm\">CAUSTICAPI CRefObj&lt;ITrackball&gt; CreateTrackball()</div><div class=\"TTSummary\">Global function for creating a track ball. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>"});