﻿NDSummary.OnToolTipsLoaded("CClass:IGPUPipelineNode",{586:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the base class for each node in a GPU pipeline</div></div>",588:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype588\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetName(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div></div>",589:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype589\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsEnabled() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns true if node is enabled (i.e. will run) in the pipeline</div></div>",590:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype590\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Enable() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Turns on a GPU node. If enabled when GPUPipeline::Process() is called this node will run (along with all its proceeding pipeline nodes).&nbsp; Otherwise, the node is not executed, and its Output texture is undefined.</div></div>",591:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype591\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Disable() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Turns off the GPU node. This is the oppside of IGPUPipelineNode::Enable</div></div>",592:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype592\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IShader&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Assigns a pixel shader to the node. This shader will be run on an image when the node is processed.</div></div>",593:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype593\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IShader&gt; GetShader() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the shader currently assigned to the node</div></div>",594:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype594\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineNode&gt; GetInput(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pName</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the node that attached to the specified node input parameter (i.e. this is the prior node in the pipeline that provides this node with the input specified by pName)</div></div>",595:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype595\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetInput(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pName,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pSamplerName,</td></tr><tr><td class=\"first\"></td><td></td><td class=\"PName last\">IGPUPipelineNode*pNode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Links a node from earlier in the pipeline to the specified parameter.</div></div>",596:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype596\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetOutputSize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">height</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets the size of the output image from this node</div></div>",597:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype597\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetOutputWidth() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the width of the output image in pixels</div></div>",598:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype598\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetOutputHeight() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the height of the output image in pixels</div></div>",599:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype599\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; GetOutputTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGPUPipeline&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPipeline</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the texture generated as output by this node</div></div>",600:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype600\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Process(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGPUPipeline&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPipeline,</td></tr><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td></td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">IRenderCtx*&nbsp;</td><td></td><td class=\"PName last\">pRenderCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Runs this node</div></div>"});