﻿NDSummary.OnToolTipsLoaded("File:Rendering/RenderGraph/IRenderGraphNode.h",{1379:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines an interface to a node in our render graph</div></div>",1381:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1381\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ERenderGraphNodeType GetType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the type of this node</div></div>",1382:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1382\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of this node</div></div>",1383:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1383\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetName(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the name of this node</div></div>",1384:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1384\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetProperty(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">name,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">any&nbsp;</td><td class=\"PName last\">value</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns the named property the specified value. Properties are values that a node may use but that aren\'t produced by another IRenderGraphNode (think constant buffer values)</div></div>",1385:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1385\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberInputPins() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of input pins</div></div>",1386:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1386\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberOutputPins() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of input pins</div></div>",1387:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1387\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; GetInputPin(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth input pin</div></div>",1388:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1388\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; GetOutputPin(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth Output pin</div></div>",1389:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1389\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; FindInputPin(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the input put with the specified name</div></div>",1390:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1390\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphPin&gt; FindOutputPin(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the output put with the specified name</div></div>",1391:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1391\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode&gt; GetParent() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the parent of this node. Maybe nullptr</div></div>",1392:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1392\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetParent(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderGraphNode&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pParent</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div></div>",1393:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1393\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> std::any GetPinValue(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderGraphPin&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPin,</td></tr><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IRenderCtx*&nbsp;</td><td></td><td class=\"PName last\">pRenderCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Retrieves the value currently assigned to a pin. This value is what a node will return as its output value for the specified pin.</div></div>"});