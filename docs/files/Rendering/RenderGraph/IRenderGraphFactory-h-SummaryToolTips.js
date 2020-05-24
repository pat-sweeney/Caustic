NDSummary.OnToolTipsLoaded("File:Rendering/RenderGraph/IRenderGraphFactory.h",{938:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to object used to create IRenderGraphNode objects</div></div>",940:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype940\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraph&gt; CreateRenderGraph() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Creates a new render graph</div></div>",941:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype941\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode_Compute&gt; CreateComputeNode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a new compute node.</div></div>",942:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype942\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode_Mesh&gt; CreateMeshNode() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Creates a new mesh node.</div></div>",943:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype943\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode_LightCollection&gt; CreateLightCollectionNode() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Creates a new light collection node.</div></div>",944:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype944\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode_Group&gt; CreateGroupNode() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Creates a new group node.</div></div>",945:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype945\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode_Material&gt; CreateMaterialNode() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Creates a new material node.</div></div>",946:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype946\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode_PhongMaterial&gt; CreatePhongMaterialNode() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Creates a new Phong lighting model material node.</div></div>",947:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype947\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode_DepthCameraSource&gt; CreateDepthCameraSource(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IDepthCameraDevice&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pDevice</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Creates a new depth camera that delivers images</div></div>",948:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype948\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderGraphNode_ImageToTexture&gt; CreateImageToTextureNode() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Creates a render graph node that accepts an IImage as input and outputs and ITexture</div></div>"});