﻿NDSummary.OnToolTipsLoaded("File:Rendering/RenderGraph/IRenderGraphPin.h",{1361:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines the types that a IRenderGraphPin may return.</div></div>",1365:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a connection point on a IRenderGraphNode object</div></div>",1367:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1367\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ERenderGraphDataType GetDataType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the data type of pin</div></div>",1368:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1368\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of pin</div></div>",1369:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1369\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode&gt; GetParent() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the parent IRenderGraphNode of pin</div></div>",1370:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1370\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDefaultValue(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns a default value to a pin. If value is empty then we will return what ever is connected to that pin (which might be output from another node\'s output pin or might be input to our grandparent\'s node)</div></div>",1371:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1371\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::any GetDefaultValue() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the default value associated with the pin. If no default value has been assigned to the pin than an empty std::any is returned</div></div>",1372:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1372\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> std::any GetValue(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IRenderCtx*&nbsp;</td><td class=\"PName last\">pRenderCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the value from a pin</div></div>",1373:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1373\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> LinkTo(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderGraphPin*&nbsp;</td><td class=\"PName last\">pPin</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Links one pin to another. NOTE: This perform linking in only a single direction. The opposite link (i.e. p1-&gt;LinkTo(p2); p2-&gt;LinkTo(p1);) must also happen.</div></div>",1374:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1374\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberConnections() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of connections (i.e. other pins) this pin is connected to</div></div>",1375:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1375\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; GetConnection(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth connection on a pin</div></div>",1376:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1376\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsInput() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whether the pin is an input or output pin</div></div>",1377:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1377\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> ClearLink(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Removes the Nth connection from the pin</div></div>",1378:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1378\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddLink(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderGraphPin&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPin</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a link from this pin to another</div></div>"});