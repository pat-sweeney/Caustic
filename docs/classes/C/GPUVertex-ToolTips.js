NDContentPage.OnToolTipsLoaded({61:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",439:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an image (CPU based)</div></div>",704:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",730:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",1278:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the base class for each node in a GPU pipeline</div></div>",1282:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1282\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IImage&gt; GetOutput(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGPUPipeline&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPipeline</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the image that resulted from the pipeline processing</div></div>",1283:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The IGPUPipeline object is used to create a graph of GPU shaders that will process images.&nbsp; A pipeline is built out of a series of IGPUPipelineNode\'s. Each node is conceptually just a pixel shader to be run on its input image data.&nbsp; The general structure of a GPU graph looks something like:</div></div>",1371:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1371\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Assigns a pixel shader to the node. This shader will be run on an image when the node is processed.</div></div>",1372:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1372\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShader&gt; GetShader() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the shader currently assigned to the node</div></div>",1373:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1373\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineNode&gt; GetInput(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the node that attached to the specified node input parameter (i.e. this is the prior node in the pipeline that provides this node with the input specified by pName)</div></div>",1374:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1374\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetInput(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pSamplerName,</td></tr><tr><td class=\"first\"></td><td></td><td></td><td class=\"PName last\">IGPUPipelineNode*pNode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Links a node from earlier in the pipeline to the specified parameter.</div></div>",1378:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1378\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetOutputTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGPUPipeline&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPipeline</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the texture generated as output by this node</div></div>",1379:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1379\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Process(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGPUPipeline&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pPipeline</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Runs this node</div></div>"});