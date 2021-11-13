﻿NDSummary.OnToolTipsLoaded("File:Rendering/RenderGraph/IRenderGraphPin.h",{1384:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines the types that a IRenderGraphPin may return.</div></div>",1388:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a connection point on a IRenderGraphNode object</div></div>",1390:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1390\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ERenderGraphDataType GetDataType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the data type of pin</div></div>",1391:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1391\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of pin</div></div>",1392:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1392\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode&gt; GetParent() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the parent IRenderGraphNode of pin</div></div>",1393:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1393\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDefaultValue(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns a default value to a pin. If value is empty then we will return what ever is connected to that pin (which might be output from another node\'s output pin or might be input to our grandparent\'s node)</div></div>",1394:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1394\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::any GetDefaultValue() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the default value associated with the pin. If no default value has been assigned to the pin than an empty std::any is returned</div></div>",1395:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1395\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> std::any GetValue(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IRenderCtx*&nbsp;</td><td class=\"PName last\">pRenderCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the value from a pin</div></div>",1396:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1396\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LinkTo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderGraphPin*&nbsp;</td><td class=\"PName last\">pPin</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Links one pin to another. NOTE: This perform linking in only a single direction. The opposite link (i.e. p1-&gt;LinkTo(p2); p2-&gt;LinkTo(p1);) must also happen.</div></div>",1397:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1397\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberConnections() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of connections (i.e. other pins) this pin is connected to</div></div>",1398:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1398\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; GetConnection(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth connection on a pin</div></div>",1399:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1399\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsInput() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whether the pin is an input or output pin</div></div>",1400:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1400\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> ClearLink(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Removes the Nth connection from the pin</div></div>",1401:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1401\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddLink(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderGraphPin&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPin</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a link from this pin to another</div></div>"});