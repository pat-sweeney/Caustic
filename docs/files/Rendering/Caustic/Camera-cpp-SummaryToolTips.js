﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/Camera.cpp",{1251:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the methods used to implement the CCamera object.</div></div>",1253:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1253\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;ICamera&gt; CreateCameraInternal(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Global function for creating a camera. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>",1254:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1254\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CCamera::CCamera(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">leftHanded</td></tr></table></td><td class=\"PAfterParameters\">) : m_leftHanded(leftHanded)</td></tr></table></div></div><div class=\"TTSummary\">Defines the implementation of ICamera</div></div>",1255:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1255\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CCamera::~CCamera()</div></div><div class=\"TTSummary\">Implements the dtor for CCamera</div></div>",1256:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1256\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">DirectX::XMMATRIX CCamera::GetProjection()</div></div><div class=\"TTSummary\">See ICamera::GetProjection</div></div>",1257:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1257\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">DirectX::XMMATRIX CCamera::GetView()</div></div><div class=\"TTSummary\">See ICamera::GetView</div></div>",1258:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1258\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::GetUVN(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">u,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">v,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">n</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICamera::GetUVN</div></div>",1259:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1259\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::SetOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">offset</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICamera::SetOffset</div></div>",1260:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1260\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::GetOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">offset</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICamera::GetOffset</div></div>",1261:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1261\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::SetParams(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">fov,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">aspectRatio,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">nearZ,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">farZ</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICamera::SetParams</div></div>",1262:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1262\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::GetParams(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>*&nbsp;</td><td class=\"PName last\">fov,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>*&nbsp;</td><td class=\"PName last\">aspectRatio,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>*&nbsp;</td><td class=\"PName last\">nearZ,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>*&nbsp;</td><td class=\"PName last\">farZ</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICamera::GetParams</div></div>",1263:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1263\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::SetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">eye,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">look,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">up</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICamera::SetPosition</div></div>",1264:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1264\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::GetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pEye,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pLook,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pUp,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pU,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pV,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pN</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ICamera::GetPosition</div></div>",1265:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1265\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::Load(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ISerialize::Load</div></div>",1266:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1266\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> CCamera::Store(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IStream&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pStream</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See ISerialize::Store</div></div>"});