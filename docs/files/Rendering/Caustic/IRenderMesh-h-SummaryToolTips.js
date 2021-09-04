﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IRenderMesh.h",{959:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Defines interface for Render mesh. Render mesh is the mirror of IMesh but defines the mesh as a vertex/index buffer pair.</div></div>",960:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the D3D counterpart to an ISubMesh object (i.e. the renderable form of a mesh, that is, a mesh converted from a half-edge structure to a vertex and index buffer).</div></div>",962:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype962\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetMeshFlags() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns a set of flags associated with the mesh. See EMeshFlags for flag definitions.</div></div>",963:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype963\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMeshFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Set the flags associated with the mesh. See EMeshFlags for flag definitions.</div></div>",964:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype964\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFrontMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFrontMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the material to use when rendering front faces</div></div>",965:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype965\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMaterial&gt; GetFrontMaterial() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Material to use when rendering front faces</div></div>",966:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype966\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetBackMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderMaterial&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBackMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the material to use when rendering back faces</div></div>",967:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype967\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMaterial&gt; GetBackMaterial() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Material to use when rendering back faces</div></div>",968:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype968\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMeshData(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">MeshData&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">meshData,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">hasNormals</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns mesh data to the render mesh</div></div>",969:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype969\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual const</span> MeshData &amp;GetMeshData(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">withNormals</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the mesh data assigned to the render mesh</div></div>",970:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype970\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the bounding box of the mesh</div></div>",971:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype971\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the bounding box of the mesh</div></div>",972:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype972\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderCtx,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial*&nbsp;</td><td></td><td class=\"PName last\">pFrontMaterialOverride,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial*&nbsp;</td><td></td><td class=\"PName last\">pBackMaterialOverride,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&amp;&nbsp;</td><td></td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX*&nbsp;</td><td></td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the mesh</div></div>",973:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype973\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderCtx,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader*&nbsp;</td><td></td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial*&nbsp;</td><td></td><td class=\"PName last\">pMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&amp;&nbsp;</td><td></td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX*&nbsp;</td><td></td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the mesh</div></div>",974:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">IRenderMesh is simply a collection of submesh objects (IRenderSubMesh).</div></div>",976:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype976\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> NumberSubMeshes() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Number of submeshes for this mesh</div></div>",977:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype977\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderSubMesh&gt; GetSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Retrieves the Nth submesh</div></div>",978:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype978\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderSubMesh&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSubMesh</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a submesh to the mesh</div></div>",979:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype979\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetBBox(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">BBox3&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pBBox</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the mesh\'s bounding box</div></div>",980:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype980\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMaterials(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IMaterialAttrib&gt;&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">materials</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns list of materials that submesh\'s use</div></div>",981:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype981\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IMaterialAttrib&gt; GetMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">materialID</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Retrieves a material</div></div>",982:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype982\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ComputeNormals() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Computes the normals for the mesh</div></div>",983:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype983\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderCtx&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pRenderCtx,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial*&nbsp;</td><td></td><td class=\"PName last\">pFrontMaterialOverride,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial*&nbsp;</td><td></td><td class=\"PName last\">pBackMaterialOverride,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&amp;&nbsp;</td><td></td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX*&nbsp;</td><td></td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the mesh</div></div>",984:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype984\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderCtx*&nbsp;</td><td></td><td class=\"PName last\">pRenderCtx,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader,</td></tr><tr><td class=\"first\"></td><td></td><td></td><td class=\"PName last\">IRenderMaterial*pMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX*&nbsp;</td><td></td><td class=\"PName last\">pWorld</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders the mesh</div></div>"});