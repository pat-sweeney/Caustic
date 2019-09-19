NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/Renderer.h",{361:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">Contains the declarations related to the CRenderer class.</div></div>",362:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype362\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">CLineVertex</div></div></div><div class=\"TTSummary\">Vertex structure for drawing lines</div></div>",363:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype363\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CLight</div></div></div><div class=\"TTSummary\">Class implementing IPointLight</div></div>",364:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype364\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CRenderCtx</div></div></div><div class=\"TTSummary\">Class implementing IRenderCtx</div></div>",365:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype365\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CGraphicsBase</div></div></div><div class=\"TTSummary\">Defines the data shared between our CGraphics and CRenderer objects</div></div>",366:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype366\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CGraphics</div></div></div><div class=\"TTSummary\">Defines our basic rendering device. CGraphics differs from CRenderer in that it is essentially just a wrapper around our underlying graphics device. It does not support things such as a scene graph, complex lighting (list of lights), or HMD support. Also, all rendering occurs on whatever thread this object is created on (versus marshalling the rendering over to a render thread). For those features, use CRenderer.&nbsp; See IGraphics</div></div>",367:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype367\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\">CRenderer</div></div></div><div class=\"TTSummary\">Defines our basic renderer. CRenderer handles all rendering commands.&nbsp; It is generally expected that this object is running on its own thread.&nbsp; See IRenderer</div></div>"});