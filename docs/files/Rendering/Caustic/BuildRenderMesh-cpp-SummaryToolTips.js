﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/BuildRenderMesh.cpp",{1266:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1266\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static bool</span> IsDegenerate(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">CGeomVertex*&nbsp;</td><td class=\"PName last\">v0,</td></tr><tr><td class=\"PType first\">CGeomVertex*&nbsp;</td><td class=\"PName last\">v1,</td></tr><tr><td class=\"PType first\">CGeomVertex*&nbsp;</td><td class=\"PName last\">v2</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Determines if the specified triangle is degenerate (i.e. area == 0)</div></div>",1267:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1267\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static void</span> BuildVertexBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CGeomVertex&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">verts,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShaderInfo*&nbsp;</td><td></td><td class=\"PName last\">pShaderInfo,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;<span class=\"SHKeyword\">int</span>&gt;&amp;&nbsp;</td><td></td><td class=\"PName last\">vertexReferenced,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">MeshData*&nbsp;</td><td></td><td class=\"PName last\">pMeshData</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a vertex buffer using the specified vertex list.</div></div>",1268:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1268\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> BuildIndexBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CGeomFace&gt;&amp;&nbsp;</td><td class=\"PName last\">faces,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CGeomVertex&gt;&amp;&nbsp;</td><td class=\"PName last\">verts,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;<span class=\"SHKeyword\">int</span>&gt;&amp;&nbsp;</td><td class=\"PName last\">vertexReferenced,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">MeshData*&nbsp;</td><td class=\"PName last\">pMeshData</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a index buffer using the specified vertex and face lists.</div></div>",1269:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1269\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">static void</span> BuildReferencedVertexList(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CGeomFace&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">faces,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CGeomVertex&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">verts,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;<span class=\"SHKeyword\">int</span>&gt;&amp;&nbsp;</td><td></td><td class=\"PName last\">vertexReferenced</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Determines which vertices are part of non-degenerate faces. This function is used to prune out degenerate faces when building the vertex/index buffers.</div></div>",1270:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1270\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IRenderSubMesh&gt; BuildRenderSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CGeomFace&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">faces,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CGeomVertex&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">verts,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader*&nbsp;</td><td></td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Converts a list of faces and vertices into a IRenderSubMesh.</div></div>",1271:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1271\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IRenderSubMesh&gt; CreateDepthGridMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">IShader*&nbsp;</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a render submesh that is a grid. This is used for rendering a depth map as a mesh.</div></div>"});