﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/BuildRenderMesh.cpp",{1441:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1441\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/4/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/4/2\"><span class=\"SHKeyword\">static bool</span> IsDegenerate(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">CGeomVertex*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">v0,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">CGeomVertex*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">v1,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\">CGeomVertex*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">v2</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"3/4/4/5\" data-NarrowGridArea=\"5/1/6/4\" style=\"grid-area:3/4/4/5\">)</div></div></div></div><div class=\"TTSummary\">Determines if the specified triangle is degenerate (i.e. area == 0)</div></div>",1442:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1442\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"6\" data-NarrowColumnCount=\"5\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/6\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">static void</span> BuildVertexBuffer(</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">IRenderer*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pRenderer,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">std::</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">vector&lt;CGeomVertex&gt;&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">&amp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">verts,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">IShaderInfo*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">pShaderInfo,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"4/2/5/3\" data-NarrowGridArea=\"5/1/6/2\" style=\"grid-area:4/2/5/3\">std::</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">vector&lt;<span class=\"SHKeyword\">int</span>&gt;&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">vertexReferenced,</div><div class=\"PType\" data-WideGridArea=\"5/3/6/4\" data-NarrowGridArea=\"6/2/7/3\" style=\"grid-area:5/3/6/4\">MeshData*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"6/4/7/5\" style=\"grid-area:5/5/6/6\">pMeshData</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"5/6/6/7\" data-NarrowGridArea=\"7/1/8/6\" style=\"grid-area:5/6/6/7\">)</div></div></div></div><div class=\"TTSummary\">Creates a vertex buffer using the specified vertex list.</div></div>",1443:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1443\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">void</span> BuildIndexBuffer(</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">IRenderer*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">pRenderer,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">std::</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">vector&lt;CGeomFace&gt;&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">faces,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\">std::</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">vector&lt;CGeomVertex&gt;&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/4/4/5\" data-NarrowGridArea=\"4/3/5/4\" style=\"grid-area:3/4/4/5\">verts,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"4/2/5/3\" data-NarrowGridArea=\"5/1/6/2\" style=\"grid-area:4/2/5/3\">std::</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">vector&lt;<span class=\"SHKeyword\">int</span>&gt;&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/4/5/5\" data-NarrowGridArea=\"5/3/6/4\" style=\"grid-area:4/4/5/5\">vertexReferenced,</div><div class=\"PType\" data-WideGridArea=\"5/3/6/4\" data-NarrowGridArea=\"6/2/7/3\" style=\"grid-area:5/3/6/4\">MeshData*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"5/4/6/5\" data-NarrowGridArea=\"6/3/7/4\" style=\"grid-area:5/4/6/5\">pMeshData</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"7/1/8/5\" style=\"grid-area:5/5/6/6\">)</div></div></div></div><div class=\"TTSummary\">Creates a index buffer using the specified vertex and face lists.</div></div>",1444:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1444\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"6\" data-NarrowColumnCount=\"5\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/4/2\" data-NarrowGridArea=\"1/1/2/6\" style=\"grid-area:1/1/4/2\"><span class=\"SHKeyword\">static void</span> BuildReferencedVertexList(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">std::</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">vector&lt;CGeomFace&gt;&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">&amp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">faces,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">std::</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">vector&lt;CGeomVertex&gt;&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">&amp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">verts,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\">std::</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">vector&lt;<span class=\"SHKeyword\">int</span>&gt;&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">vertexReferenced</div><div class=\"PAfterParameters\" data-WideGridArea=\"3/6/4/7\" data-NarrowGridArea=\"5/1/6/6\" style=\"grid-area:3/6/4/7\">)</div></div></div></div><div class=\"TTSummary\">Determines which vertices are part of non-degenerate faces. This function is used to prune out degenerate faces when building the vertex/index buffers.</div></div>",1445:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1445\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"6\" data-NarrowColumnCount=\"5\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/5/2\" data-NarrowGridArea=\"1/1/2/6\" style=\"grid-area:1/1/5/2\">CRefObj&lt;IRenderSubMesh&gt; BuildRenderSubMesh(</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">IRenderer*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pRenderer,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">std::</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">vector&lt;CGeomFace&gt;&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">&amp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">faces,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\">std::</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">vector&lt;CGeomVertex&gt;&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"3/4/4/5\" data-NarrowGridArea=\"4/3/5/4\" style=\"grid-area:3/4/4/5\">&amp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">verts,</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">IShader*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">pShader</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"4/6/5/7\" data-NarrowGridArea=\"6/1/7/6\" style=\"grid-area:4/6/5/7\">)</div></div></div></div><div class=\"TTSummary\">Converts a list of faces and vertices into a IRenderSubMesh.</div></div>",1446:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1446\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/5/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/5/2\">CRefObj&lt;IRenderSubMesh&gt; CreateDepthGridMesh(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderer*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pRenderer,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">uint32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">width,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\">uint32_t&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">height,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"4/2/5/3\" data-NarrowGridArea=\"5/1/6/2\" style=\"grid-area:4/2/5/3\">IShader*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">pShader</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"4/4/5/5\" data-NarrowGridArea=\"6/1/7/4\" style=\"grid-area:4/4/5/5\">)</div></div></div></div><div class=\"TTSummary\">Creates a render submesh that is a grid. This is used for rendering a depth map as a mesh.</div></div>"});