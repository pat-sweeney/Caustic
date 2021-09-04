﻿NDSummary.OnToolTipsLoaded("File:Rendering/SceneGraph/ISceneGraph.h",{1460:"<div class=\"NDToolTip TEnumeration LC\"><div class=\"TTSummary\">Defines types for each scene element</div></div>",1467:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1467\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">SceneCtx</div></div></div><div class=\"TTSummary\">Used to maintain state while rendering the scene graph</div></div>",1469:"<div class=\"NDToolTip TProperty LC\"><div class=\"TTSummary\">Defines our current rendering device</div></div>",1470:"<div class=\"NDToolTip TProperty LC\"><div class=\"TTSummary\">Defines the current transform</div></div>",1471:"<div class=\"NDToolTip TProperty LC\"><div class=\"TTSummary\">Defines list of current lights effecting this portion of the scene graph</div></div>",1472:"<div class=\"NDToolTip TProperty LC\"><div class=\"TTSummary\">Defines which render pass we are in</div></div>",1473:"<div class=\"NDToolTip TProperty LC\"><div class=\"TTSummary\">Indicates whether we are currently rendering a group of lights+children that cast shadows. Otherwise, during the shadow pass we will skip rendering.</div></div>",1474:"<div class=\"NDToolTip TProperty LC\"><div class=\"TTSummary\">Defines the current material</div></div>",1475:"<div class=\"NDToolTip TProperty LC\"><div class=\"TTSummary\">Defines the current shader</div></div>",1483:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our base interface common to all scene elements</div></div>",1485:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1485\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> RayIntersect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Ray3&amp;&nbsp;</td><td class=\"PName last\">ray,</td></tr><tr><td class=\"PType first\">RayIntersect3*&nbsp;</td><td class=\"PName last\">pIntersection,</td></tr><tr><td class=\"PType first\">IMaterialAttrib**&nbsp;</td><td class=\"PName last\">pMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Traces a single path through the scene</div></div>",1486:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1486\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ESceneElemType GetType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the type of this scene element</div></div>",1487:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1487\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::wstring GetName() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1488:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1488\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetName(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">name</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the name of this scene element</div></div>",1489:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1490:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Returns the name of this scene element</div></div>",1491:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1491\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the flags associated with this scene element See also: ESceneElemFlags</div></div>",1492:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1492\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the flags associated with this scene element</div></div>",1493:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1493\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IRenderCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderCtx,</td></tr><tr><td class=\"PType first\">SceneCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSceneCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the current element</div></div>",1494:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1494\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3*&nbsp;</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the bounding box of this scene element</div></div>",1495:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1495\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetInPass(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">pass</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets which render passes this element is rendered in</div></div>",1496:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1496\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetInPass() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Gets which render passes this element is rendered in</div></div>",1497:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a overlay element in our scene graph. Overlays are used to draw 2D graphics in screen coordinates (e.g. for UI elements).</div></div>",1499:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1499\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetRect(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox2&amp;&nbsp;</td><td class=\"PName last\">bbox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the screen extents for the element</div></div>",1500:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1500\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ITexture*&nbsp;</td><td class=\"PName last\">pTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the texture to render as the 2D overlay</div></div>",1501:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1501\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader*&nbsp;</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a shader to render the 2D overlay with. This is an override. If it isn\'t called the default shader is used.</div></div>",1502:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a line element in our scene graph.</div></div>",1503:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sphere element in our scene graph.</div></div>",1504:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a cube element in our scene graph.</div></div>",1505:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a cylinder element in our scene graph.</div></div>",1506:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a group element in our scene graph. Groups contain other scene elements.</div></div>",1508:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1508\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberChildren() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of child elements part of this group</div></div>",1509:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1509\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ISceneElem&gt; GetChild(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the Nth child element</div></div>",1510:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1510\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddChild(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISceneElem&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pElem</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a new scene element to the group</div></div>",1511:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1511\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> InsertChild(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISceneElem&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pElem,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a new scene element to the group. The order of elements defines the render order.</div></div>",1512:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1512\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTransform(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Matrix4x4&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">transform</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the transform associated with this group.</div></div>",1513:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a material object. Materials define the vertex and pixel shader associated with a given scene element.</div></div>",1515:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1515\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets our shader</div></div>",1516:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1516\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">**</td><td class=\"PName last\">ppMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the material associated with this element</div></div>",1517:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1517\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IMaterialAttrib&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the underlying material associated with the scene material</div></div>",1518:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a compute shader in our scene graph. Compute shaders are run during scene graph traversal but before any rendering elements.</div></div>",1519:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Implements marching cubes on a signed distance function</div></div>",1520:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a point light source in our scene</div></div>",1522:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1522\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">Vector3&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">pos</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the position of our light</div></div>",1523:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1523\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">Caustic::</td><td class=\"PType\">FRGBColor&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">clr</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Defines the color of the light</div></div>",1524:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a collection of lights. These lights only effect the children of this group</div></div>",1526:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1526\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ILight*&nbsp;</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a light to the collection</div></div>",1527:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1527\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RemoveLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ILight*&nbsp;</td><td class=\"PName last\">pLight</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Removes a light from the collection</div></div>",1528:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1528\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberLights() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of lights in the collection</div></div>",1529:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1529\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ILight&gt; GetLight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the Nth light from the collection</div></div>",1530:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a single mesh in our scene</div></div>",1532:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1532\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IMesh*&nbsp;</td><td class=\"PName last\">pMesh</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Updates the mesh elements underlying mesh object</div></div>",1533:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1533\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader*&nbsp;</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns shader used for rendering this mesh</div></div>",1534:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a custom render element in our scene</div></div>",1536:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1536\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetCallback(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer *pRenderer, IRenderCtx *pCtx, SceneCtx *pSceneCtx)&gt;&nbsp;</td><td class=\"PName last\">clientCallback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Updates the callback function associated with the custom render element</div></div>",1537:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Our main interface for manipulating a scene graph</div></div>",1539:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1539\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> PathTrace(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">PathTraceCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pCtx,</td></tr><tr><td class=\"PType first\">IImage&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pDest</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Computes view of the scene using path tracing</div></div>",1540:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1540\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Merge(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ISceneGraph&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pGraph</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Merges a scene graph with another scene graph</div></div>",1541:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1541\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Lock() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Locks a scene graph so it can be updated</div></div>",1542:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1542\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Unlock() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Unlocks a scene graph allowing the renderer to use it</div></div>"});