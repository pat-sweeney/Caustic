﻿NDSummary.OnToolTipsLoaded("File:Rendering/SceneGraph/ISceneElem.ixx",{1578:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines types for each scene element</div></div>",1591:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our base interface common to all scene elements</div></div>",1593:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1593\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> RayIntersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Ray3&amp;&nbsp;</td><td class=\"PName last\">ray,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersection,</td></tr><tr><td class=\"PType first\">IMaterialAttrib**&nbsp;</td><td class=\"PName last\">pMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Traces a single path through the scene</div></div>",1594:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1594\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ESceneElemType GetType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the type of this scene element</div></div>",1595:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1595\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::wstring GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1596:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1596\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetName(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the name of this scene element</div></div>",1597:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1598:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1599:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1599\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the flags associated with this scene element See also: ESceneElemFlags</div></div>",1600:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1600\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the flags associated with this scene element</div></div>",1601:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1601\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IRenderCtx*&nbsp;</td><td class=\"PName last\">pRenderCtx,</td></tr><tr><td class=\"PType first\">SceneCtx*&nbsp;</td><td class=\"PName last\">pSceneCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the current element</div></div>",1602:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1602\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3*&nbsp;</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the bounding box of this scene element</div></div>",1603:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1603\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetInPass(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">pass</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets which render passes this element is rendered in</div></div>",1604:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1604\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetInPass() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Gets which render passes this element is rendered in</div></div>"});