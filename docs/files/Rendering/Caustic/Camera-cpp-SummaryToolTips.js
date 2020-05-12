NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/Camera.cpp",{646:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CCamera object.</div></div>",206:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype206\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ICamera&gt; CreateCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Global function for creating a camera. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>",649:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype649\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CCamera::CCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">) : m_leftHanded(leftHanded)</td></tr></table></div><div class=\"TTSummary\">Defines the implementation of ICamera</div></div>",650:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype650\" class=\"NDPrototype NoParameterForm\">CCamera::~CCamera()</div><div class=\"TTSummary\">Implements the dtor for CCamera</div></div>",651:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype651\" class=\"NDPrototype NoParameterForm\">DirectX::XMMATRIX CCamera::GetProjection()</div><div class=\"TTSummary\">See ICamera::GetProjection</div></div>",652:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype652\" class=\"NDPrototype NoParameterForm\">DirectX::XMMATRIX CCamera::GetView()</div><div class=\"TTSummary\">See ICamera::GetView</div></div>",653:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype653\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::GetUVN(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">u,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">v,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">n</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ICamera::GetUVN</div></div>",654:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype654\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::SetOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">offset</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ICamera::SetOffset</div></div>",655:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype655\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::GetOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">offset</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ICamera::GetOffset</div></div>",656:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype656\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::SetParams(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">fov,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">aspectRatio,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">nearZ,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">farZ</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ICamera::SetParams</div></div>",657:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype657\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::SetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">eye,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">look,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">up</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ICamera::SetPosition</div></div>",658:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype658\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::GetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pEye,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLook,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pUp,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pU,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pV,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pN</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ICamera::GetPosition</div></div>",659:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype659\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::Load(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ISerialize::Load</div></div>",660:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype660\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::Store(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">See ISerialize::Store</div></div>"});