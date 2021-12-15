﻿NDSummary.OnToolTipsLoaded("File:Rendering/SceneGraph/ISceneGroupElem.ixx",{1542:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a group element in our scene graph. Groups contain other scene elements.</div></div>",1544:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1544\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberChildren() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of child elements part of this group</div></div>",1545:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1545\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneElem&gt; GetChild(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the Nth child element</div></div>",1546:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1546\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddChild(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISceneElem*&nbsp;</td><td class=\"PName last\">pElem</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a new scene element to the group</div></div>",1547:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1547\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> InsertChild(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISceneElem*&nbsp;</td><td class=\"PName last\">pElem,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a new scene element to the group. The order of elements defines the render order.</div></div>",1548:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1548\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTransform(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Matrix4x4&amp;&nbsp;</td><td class=\"PName last\">transform</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the transform associated with this group.</div></div>"});