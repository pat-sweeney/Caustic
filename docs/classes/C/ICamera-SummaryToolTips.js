NDSummary.OnToolTipsLoaded("CClass:ICamera",{898:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The CCamera object defines our camera. We pass this to our IRenderer to determine where to render from.</div></div>",725:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype725\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetParams(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">fov,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">aspectRatio,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">nearZ,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">farZ</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the camera parameters</div></div>",1417:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1417\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetParams(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">fov,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">aspectRatio,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">nearZ,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">farZ</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Gets the camera parameters</div></div>",901:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype901\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">eye,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">look,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">up</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the camera\'s position.</div></div>",902:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype902\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pEye,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLook,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pUp,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pU,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pV,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pN</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Gets the camera\'s position.</div></div>",903:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype903\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">offset</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets an offset that is applied to the camera\'s eye and look point.</div></div>",904:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype904\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">offset</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the current offset</div></div>",905:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype905\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DirectX::XMMATRIX GetProjection() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the camera\'s projection matrix</div></div>",906:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype906\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DirectX::XMMATRIX GetView() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the camera\'s view matrix</div></div>",907:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype907\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual float</span> GetNear() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the camera\'s near plane distance</div></div>",908:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype908\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual float</span> GetFar() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the camera\'s far plane distance</div></div>",909:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype909\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetUVN(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">u,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">v,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">n</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the camera axis in world coordinates</div></div>",910:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype910\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ICamera&gt; CreateCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Global function for creating a camera. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>"});