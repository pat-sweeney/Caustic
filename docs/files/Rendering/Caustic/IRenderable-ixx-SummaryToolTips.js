﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IRenderable.ixx",{1202:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for renderables.&nbsp; A Renderable is an mesh, front and back materials, and a shader. The renderer keeps a simple list of these objects which are rendered at each call to IRenderer::RenderFrame. The purpose of this object is to allow the renderer to render a set of objects without forcing the client to maintain an entire scene graph.</div></div>",1203:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">A Renderable is an mesh, front and back materials, and a shader. The renderer keeps a simple list of these objects which are rendered at each call to IRenderer::RenderFrame. The purpose of this object is to allow the renderer to render a set of objects without forcing the client to maintain an entire scene graph.</div></div>",1205:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1205\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Vector3 GetPos() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the world position of the object</div></div>",1206:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1206\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"6\" data-NarrowColumnCount=\"5\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/4/2\" data-NarrowGridArea=\"1/1/2/6\" style=\"grid-area:1/1/4/2\"><span class=\"SHKeyword\">virtual void</span> Render(</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">IRenderer&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pRenderer,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">std::</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">&amp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">lights,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">IRenderCtx&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"3/4/4/5\" data-NarrowGridArea=\"4/3/5/4\" style=\"grid-area:3/4/4/5\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">pRenderCtx</div><div class=\"PAfterParameters\" data-WideGridArea=\"3/6/4/7\" data-NarrowGridArea=\"5/1/6/6\" style=\"grid-area:3/6/4/7\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Called by the renderer to render this object</div></div>",1207:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1207\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> SetTransform(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">DirectX::</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">XMMATRIX&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">mat</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"3/1/4/5\" style=\"grid-area:1/5/2/6\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Sets the transform on the object</div></div>",1208:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1208\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> DirectX::XMMATRIX&amp; GetTransform() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the transforms associated with the object</div></div>",1209:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1209\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> SetSubMesh(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderSubMesh*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pSubMesh</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Sets the submesh</div></div>",1210:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1210\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderSubMesh&gt; GetSubMesh() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the submesh associated with this oject</div></div>",1211:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1211\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> SetFrontMaterial(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderMaterial*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pMaterial</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Sets the front face material</div></div>",1212:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1212\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMaterial&gt; GetFrontMaterial() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the front face material associated with this oject</div></div>",1213:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1213\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> SetBackMaterial(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderMaterial*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pMaterial</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Sets the back face material</div></div>",1214:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1214\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMaterial&gt; GetBackMaterial() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the back face material associated with this oject</div></div>",1215:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1215\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual bool</span> InPass(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pass</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns whether the object is part of the specified pass</div></div>",1216:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1216\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/5/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/5/2\">CRefObj&lt;IRenderable&gt; CreateRenderable(</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">IRenderSubMesh*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">pSubMesh,</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">IRenderMaterial*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">pFrontMaterial,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">IRenderMaterial*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/4/4/5\" data-NarrowGridArea=\"4/3/5/4\" style=\"grid-area:3/4/4/5\">pBackMaterial,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"4/2/5/3\" data-NarrowGridArea=\"5/1/6/2\" style=\"grid-area:4/2/5/3\">DirectX::</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">XMMATRIX&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/4/5/5\" data-NarrowGridArea=\"5/3/6/4\" style=\"grid-area:4/4/5/5\">mat</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"6/1/7/5\" style=\"grid-area:4/5/5/6\">)</div></div></div></div><div class=\"TTSummary\">Creates a renderable object</div></div>"});