NDSummary.OnToolTipsLoaded("CClass:IRenderable",{839:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">A Renderable is an mesh, front and back materials, and a shader. The renderer keeps a simple list of these objects which are rendered at each call to IRenderer::RenderFrame. The purpose of this object is to allow the renderer to render a set of objects without forcing the client to maintain an entire scene graph.</div></div>",841:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype841\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> Vector3 GetPos() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the world position of the object</div></div>",842:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype842\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderer&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;ILight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderCtx&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Called by the renderer to render this object</div></div>",843:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype843\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTransform(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the transform on the object</div></div>",844:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype844\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DirectX::XMMATRIX&amp; GetTransform() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the transforms associated with the object</div></div>",845:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype845\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetSubMesh(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSubMesh</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the submesh</div></div>",846:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype846\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderSubMesh&gt; GetSubMesh() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the submesh associated with this oject</div></div>",847:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype847\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFrontMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the front face material</div></div>",848:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype848\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMaterial&gt; GetFrontMaterial() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the front face material associated with this oject</div></div>",849:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype849\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetBackMaterial(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMaterial</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the back face material</div></div>",850:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype850\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderMaterial&gt; GetBackMaterial() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the back face material associated with this oject</div></div>",851:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype851\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> InPass(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">pass</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns whether the object is part of the specified pass</div></div>",852:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype852\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;IRenderable&gt; CreateRenderable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IRenderSubMesh&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSubMesh,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFrontMaterial,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderMaterial&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pBackMaterial,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates a renderable object</div></div>"});