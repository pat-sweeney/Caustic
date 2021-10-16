﻿NDSummary.OnToolTipsLoaded("CClass:ISceneElem",{1390:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our base interface common to all scene elements</div></div>",1392:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1392\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> RayIntersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Ray3&amp;&nbsp;</td><td class=\"PName last\">ray,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersection,</td></tr><tr><td class=\"PType first\">IMaterialAttrib**&nbsp;</td><td class=\"PName last\">pMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Traces a single path through the scene</div></div>",1393:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1393\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ESceneElemType GetType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the type of this scene element</div></div>",1394:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1394\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::wstring GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1395:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1395\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetName(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the name of this scene element</div></div>",1396:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1397:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1398:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1398\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the flags associated with this scene element See also: ESceneElemFlags</div></div>",1399:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1399\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the flags associated with this scene element</div></div>",1400:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1400\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IRenderCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderCtx,</td></tr><tr><td class=\"PType first\">SceneCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSceneCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the current element</div></div>",1401:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1401\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3*&nbsp;</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the bounding box of this scene element</div></div>",1402:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1402\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetInPass(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">pass</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets which render passes this element is rendered in</div></div>",1403:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1403\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetInPass() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Gets which render passes this element is rendered in</div></div>"});