﻿NDSummary.OnToolTipsLoaded("CClass:ISceneFactory",{1340:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Factory used for creating objects that make up parts of our scene graph</div></div>",1342:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1342\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneMaterialElem&gt; CreateMaterialElem() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a material element</div></div>",1343:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Creates a material element</div></div>",1344:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1344\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneLightCollectionElem&gt; CreateLightCollectionElem() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a light collection element</div></div>",1345:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1345\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneGroupElem&gt; CreateGroupElem() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a group element</div></div>",1346:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1346\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneMarchingCubesElem&gt; CreateMarchingCubesElem(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">subdivisions,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">float</span>(Vector3&amp;)&gt;&nbsp;</td><td class=\"PName last\">sdf,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">drawIndexed</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates an element that uses the marching cubes algorithm to convert a signed distance field into a mesh.</div></div>",1347:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1347\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneMeshElem&gt; CreateMeshElem() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a mesh element</div></div>",1348:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1348\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneLineElem&gt; CreateLineElem(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">p0,</td></tr><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">p1</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a line element</div></div>",1349:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1349\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneSphereElem&gt; CreateSphereElem(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">center,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">radius</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a sphere element</div></div>",1350:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1350\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneCubeElem&gt; CreateCubeElem(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&amp;&nbsp;</td><td class=\"PName last\">center,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">depth</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a cube element</div></div>",1351:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1351\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneCylinderElem&gt; CreateCylinderElem(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">center,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">topRadius,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td></td><td class=\"PName last\">bottomRadius</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Creates a mesh element</div></div>",1352:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Creates a mesh element</div></div>",1353:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1353\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneCustomRenderElem&gt; CreateCustomRenderElem(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRender, IRenderCtx* pCtx, SceneCtx* pSceneCtx)&gt;&nbsp;</td><td class=\"PName last\">clientCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Create a element that will use a callback for rendering</div></div>",1354:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1354\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneOverlay2DElem&gt; CreateOverlay2DElem(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader*&nbsp;</td><td class=\"PName\">pShader</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\">nullptr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Create a overlay element used to render textures in screen space (e.g. UI)</div></div>",1355:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1355\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneGraph&gt; CreateSceneGraph() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Creates a new scene graph</div></div>",1356:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1356\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ISceneFactory&gt; CreateSceneFactory()</div></div><div class=\"TTSummary\">Creates a scene factory. This factory is used to create elements in the scene graph.</div></div>"});