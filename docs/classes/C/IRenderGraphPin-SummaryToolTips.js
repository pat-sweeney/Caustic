﻿NDSummary.OnToolTipsLoaded("CClass:IRenderGraphPin",{1583:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a connection point on a IRenderGraphNode object</div></div>",1585:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1585\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ERenderGraphDataType GetDataType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the data type of pin</div></div>",1586:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1586\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of pin</div></div>",1587:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1587\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode&gt; GetParent() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the parent IRenderGraphNode of pin</div></div>",1588:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1588\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> SetDefaultValue(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">std::</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">any&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">v</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"3/1/4/5\" style=\"grid-area:1/5/2/6\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Assigns a default value to a pin. If value is empty then we will return what ever is connected to that pin (which might be output from another node\'s output pin or might be input to our grandparent\'s node)</div></div>",1589:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1589\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::any GetDefaultValue() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the default value associated with the pin. If no default value has been assigned to the pin than an empty std::any is returned</div></div>",1590:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1590\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/3/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/3/2\"><span class=\"SHKeyword\">virtual</span> std::any GetValue(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderer*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pRenderer,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">IRenderCtx*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">pRenderCtx</div><div class=\"PAfterParameters\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"4/1/5/4\" style=\"grid-area:2/4/3/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns the value from a pin</div></div>",1591:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1591\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> LinkTo(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderGraphPin*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pPin</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Links one pin to another. NOTE: This perform linking in only a single direction. The opposite link (i.e. p1-&gt;LinkTo(p2); p2-&gt;LinkTo(p1);) must also happen.</div></div>",1592:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1592\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> uint32_t NumberConnections() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of connections (i.e. other pins) this pin is connected to</div></div>",1593:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1593\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; GetConnection(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">index</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns the Nth connection on a pin</div></div>",1594:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1594\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsInput() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whether the pin is an input or output pin</div></div>",1595:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1595\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> ClearLink(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">index</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Removes the Nth connection from the pin</div></div>",1596:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1596\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> AddLink(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderGraphPin&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">pPin</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"3/1/4/5\" style=\"grid-area:1/5/2/6\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Adds a link from this pin to another</div></div>"});