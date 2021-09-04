﻿NDSummary.OnToolTipsLoaded("CClass:IPath2",{284:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a 2D Path (collection of bezier curves and lines)</div></div>",286:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Returns the number of PathElems in the path</div></div>",287:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Returns the Ith path element in the path</div></div>",288:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype288\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> BBox2 GetBBox() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the path\'s bounding box. The path\'s bounding box includes the convex hull of any curves. For a tighter fit first flatten the path via [IPath2::Flatten].</div></div>",289:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype289\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPathElem(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">EPathElem&nbsp;</td><td class=\"PName\">elemType,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">Vector2*&nbsp;</td><td class=\"PName\">p0</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr,</td></tr><tr><td class=\"PType first\">Vector2*&nbsp;</td><td class=\"PName\">p1</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr,</td></tr><tr><td class=\"PType first\">Vector2*&nbsp;</td><td class=\"PName\">p2</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Ads a new path element to the end of the path</div></div>",290:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype290\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AppendPathElems(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">EPathElem&nbsp;</td><td></td><td class=\"PName last\">elemType,</td></tr><tr><td class=\"first\"></td><td class=\"PSymbols\">...</td><td class=\"last\"></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Appends a series of new path elements to the end of the path</div></div>",291:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype291\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IPath2&gt; Flatten(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">flatness,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">BBox2*&nbsp;</td><td class=\"PName\">bb</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Flattens the path. This results in a path that only contains line segments</div></div>",292:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype292\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> PointOnPath(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&amp;&nbsp;</td><td class=\"PName last\">pt,</td></tr><tr><td class=\"PType first\">Vector2 *&nbsp;</td><td class=\"PName last\">closestpt,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">tolerance</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Determines whether a 2D point lies on the path (within some tolerance)</div></div>",293:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype293\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> PathInsideBbox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox2&amp;&nbsp;</td><td class=\"PName last\">bbox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Determines whether the path lies within the specified bounding box.</div></div>"});