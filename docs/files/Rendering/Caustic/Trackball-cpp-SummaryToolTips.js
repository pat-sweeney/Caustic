NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/Trackball.cpp",{465:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CTrackball object.</div></div>",467:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype467\" class=\"NDPrototype NoParameterForm\">CAUSTICAPI CRefObj&lt;ITrackball&gt; CreateTrackball()</div><div class=\"TTSummary\">Global function for creating a track ball. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>",468:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype468\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CTrackball::ComputeSurfacePosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPos</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Converts a screen coordinate into a point on our trackball surface</div></div>",469:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype469\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CTrackball::BeginTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenW,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenH</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Called at start of mouse drag</div></div>",470:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype470\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">bool</span> CTrackball::UpdateTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ETrackballConstraint&nbsp;</td><td></td><td class=\"PName last\">constraint,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMatrix</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Called each time mouse is moved</div></div>",471:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype471\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> CTrackball::EndTracking()</div><div class=\"TTSummary\">Called at end of mouse drag</div></div>"});