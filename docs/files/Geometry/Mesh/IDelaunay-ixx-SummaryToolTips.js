﻿NDSummary.OnToolTipsLoaded("File:Geometry/Mesh/IDelaunay.ixx",{375:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Header with class/methods for performing Delaunay triangulation</div></div>",376:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface used for creating Delaunay triangulations from a set of 2D points.</div></div>",378:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype378\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Open() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Opens the triangulation so points can be added</div></div>",379:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype379\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddPoint(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pt,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">uv,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">isBoundary</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Opens the triangulation so points can be added</div></div>",380:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype380\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Close() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Closes the triangulation and performs the Delaunay triangulation on the set of points</div></div>",381:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype381\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetNumberTriangles() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of triangles in the triangulation</div></div>",382:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype382\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetTriangle(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">index,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v0,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v1,</td></tr><tr><td class=\"PType first\">Vector2&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">v2,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">isExterior[<span class=\"SHNumber\">3</span>]</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth triangle</div></div>",383:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype383\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> WritePLY() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Helper function for writing the triangulation out as a .PLY file</div></div>"});