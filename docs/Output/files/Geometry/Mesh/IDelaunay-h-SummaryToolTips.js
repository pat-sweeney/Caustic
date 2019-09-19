NDSummary.OnToolTipsLoaded("File:Geometry/Mesh/IDelaunay.h",{206:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface used for creating Delaunay triangulations from a set of 2D points.</div></div>",208:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype208\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> Open() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Opens the triangulation so points can be added</div></div>",209:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype209\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">pt,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">uv,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">isBoundary</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Opens the triangulation so points can be added</div></div>",210:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype210\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> Close() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Closes the triangulation and performs the Delaunay triangulation on the set of points</div></div>",211:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype211\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetNumberTriangles() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the number of triangles in the triangulation</div></div>",212:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype212\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetTriangle(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">v0,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">v1,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">v2,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">isExterior[3]</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the Nth triangle</div></div>",213:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype213\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> WritePLY() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Helper function for writing the triangulation out as a .PLY file</div></div>"});