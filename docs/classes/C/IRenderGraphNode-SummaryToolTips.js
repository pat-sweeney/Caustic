﻿NDSummary.OnToolTipsLoaded("CClass:IRenderGraphNode",{1366:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines an interface to a node in our render graph</div></div>",1368:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1368\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ERenderGraphNodeType GetType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the type of this node</div></div>",1369:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1369\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of this node</div></div>",1370:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1370\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetName(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the name of this node</div></div>",1371:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1371\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetProperty(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">name,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns the named property the specified value. Properties are values that a node may use but that aren\'t produced by another IRenderGraphNode (think constant buffer values)</div></div>",1372:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1372\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberInputPins() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of input pins</div></div>",1373:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1373\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberOutputPins() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of input pins</div></div>",1374:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1374\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; GetInputPin(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth input pin</div></div>",1375:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1375\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; GetOutputPin(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth Output pin</div></div>",1376:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1376\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; FindInputPin(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the input put with the specified name</div></div>",1377:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1377\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; FindOutputPin(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the output put with the specified name</div></div>",1378:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1378\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode&gt; GetParent() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the parent of this node. Maybe nullptr</div></div>",1379:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1379\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetParent(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderGraphNode&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pParent</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div></div>",1380:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1380\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> std::any GetPinValue(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderGraphPin&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPin,</td></tr><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IRenderCtx*&nbsp;</td><td></td><td class=\"PName last\">pRenderCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Retrieves the value currently assigned to a pin. This value is what a node will return as its output value for the specified pin.</div></div>"});