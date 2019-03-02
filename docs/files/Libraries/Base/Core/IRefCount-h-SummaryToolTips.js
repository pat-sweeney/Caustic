NDSummary.OnToolTipsLoaded("File:Libraries/Base/Core/IRefCount.h",{108:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">Defines the basic interface used by reference counted objects</div></div>",110:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype110\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> AddRef() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Increments the reference count</div></div>",111:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype111\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> Release() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Decrements the reference count. If the reference count goes to zero the underlying object is destroyed.</div></div>",112:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",114:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype114\" class=\"NDPrototype NoParameterForm\">CRefObj()</div><div class=\"TTSummary\">Default constructor</div></div>",115:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype115\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">CRefObj&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Copy constructor</div></div>",116:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype116\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">T&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Constructor from base type</div></div>",117:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype117\" class=\"NDPrototype NoParameterForm\">~CRefObj()</div><div class=\"TTSummary\">Destructor</div></div>"});