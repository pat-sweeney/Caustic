﻿NDSummary.OnToolTipsLoaded("CClass:IJSonParser",{418:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines an interface for parsing JSON files.</div></div>",538:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype538\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IJSonObj&gt; LoadDOM(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">fn</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads the Document object model from the specified JSON file</div></div>",539:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype539\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SaveDOM(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">CRefObj&lt;IJSonObj&gt;&amp;&nbsp;</td><td class=\"PName last\">dom,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName last\">fn</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Saves the Document object model (DOM) to the specified JSON file</div></div>",540:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype540\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IJSonObj&gt; ReadDOM(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>*&nbsp;</td><td class=\"PName last\">pBuffer</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Reads a JSON tree from the specified null terminated string buffer</div></div>",541:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype541\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual uint32</span> WriteDOM(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CRefObj&lt;IJSonObj&gt;&amp;&nbsp;</td><td class=\"PName last\">dom,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">char</span>*&nbsp;</td><td class=\"PName last\">pBuffer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">bufLen</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Writes a JSON tree to the specified buffer. To determine how big the buffer must be to contain the JSON text, you may call this method with a nullptr for \'pBuffer\' and it will return the number of characters the buffer requires.</div></div>"});