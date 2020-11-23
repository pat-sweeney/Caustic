NDContentPage.OnToolTipsLoaded({61:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",439:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an image (CPU based)</div></div>",704:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",730:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",1278:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the base class for each node in a GPU pipeline</div></div>",1279:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the a source node. This node is at the start of a pipeline.</div></div>",1280:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the a sink node. This node is at the end of a pipeline.</div></div>",1282:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1282\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IImage&gt; GetOutput(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGPUPipeline&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPipeline</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the image that resulted from the pipeline processing</div></div>",1283:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The IGPUPipeline object is used to create a graph of GPU shaders that will process images.&nbsp; A pipeline is built out of a series of IGPUPipelineNode\'s. Each node is conceptually just a pixel shader to be run on its input image data.&nbsp; The general structure of a GPU graph looks something like:</div></div>",1285:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1285\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderer&gt; GetRenderer() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the Renderer associated with this pipeline</div></div>",1287:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1287\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> Process() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Executes the pipeline</div></div>",1288:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1288\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineSourceNode&gt; CreateSourceNode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName\">pImage,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName\">outputWidth,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName\">outputHeight,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td></td><td class=\"PName\">format&nbsp;</td><td class=\"PDefaultValueSeparator\">=&nbsp;</td><td class=\"PDefaultValue last\">DXGI_FORMAT_R8G8B8A8_UNORM</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns a source node. The source node will feed the pipeline with the specified image.</div></div>",1289:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1289\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineSinkNode&gt; CreateSinkNode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName\">pShader,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName\">outputWidth,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName\">outputHeight,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td></td><td class=\"PName\">format&nbsp;</td><td class=\"PDefaultValueSeparator\">=&nbsp;</td><td class=\"PDefaultValue last\">DXGI_FORMAT_R8G8B8A8_UNORM</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns a sink node.</div></div>",1290:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1290\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineNode&gt; CreateNode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName\">pShader,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName\">outputWidth,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName\">outputHeight,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td></td><td class=\"PName\">format&nbsp;</td><td class=\"PDefaultValueSeparator\">=&nbsp;</td><td class=\"PDefaultValue last\">DXGI_FORMAT_R8G8B8A8_UNORM</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns a GPU node that makes up part of the pipeline.</div></div>",1308:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1308\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineNode&gt; CreatePredefinedNode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"PModifierQualifier first\">...</td><td></td><td></td><td class=\"last\"></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns a GPU node that makes up part of the pipeline. This method is for specific nodes that have a custom Process() step beyond just running some shader.</div></div>"});