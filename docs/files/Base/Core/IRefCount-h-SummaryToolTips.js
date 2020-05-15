NDSummary.OnToolTipsLoaded("File:Base/Core/IRefCount.h",{6:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines the basic interface used by reference counted objects</div></div>",8:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype8\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> AddRef() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Increments the reference count</div></div>",9:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype9\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> Release() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Decrements the reference count. If the reference count goes to zero the underlying object is destroyed.</div></div>",10:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",12:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype12\" class=\"NDPrototype NoParameterForm\">CRefObj()</div><div class=\"TTSummary\">Default constructor</div></div>",13:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype13\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">CRefObj&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Copy constructor</div></div>",14:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype14\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">T&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Constructor from base type</div></div>",15:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype15\" class=\"NDPrototype NoParameterForm\">~CRefObj()</div><div class=\"TTSummary\">Destructor</div></div>"});