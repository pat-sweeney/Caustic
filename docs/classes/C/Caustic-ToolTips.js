﻿NDContentPage.OnToolTipsLoaded({114:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype114\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">Vector2</div></div></div><div class=\"TTSummary\">Defines a simple vector with 2 components</div></div>",193:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype193\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">template</span> &lt;typename T&gt; <span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">CRefObj</div></div></div><div class=\"TTSummary\">CRefObj is a smart class (similar to CComPtr) for managing a IRefCount-ed object\'s lifetime.</div></div>",247:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Base interface shared across all image types</div></div>",608:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the base class for each node in a GPU pipeline</div></div>",623:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the a source node. This node is at the start of a pipeline.</div></div>",624:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the a sink node. This node is at the end of a pipeline.</div></div>",629:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype629\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderer&gt; GetRenderer() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the Renderer associated with this pipeline</div></div>",631:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype631\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/3/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/3/2\"><span class=\"SHKeyword\">virtual void</span> Process(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderer&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">pRenderer,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">IRenderCtx&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">pRenderCtx</div><div class=\"PAfterParameters\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"4/1/5/5\" style=\"grid-area:2/5/3/6\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Executes the pipeline</div></div>",632:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype632\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"8\" data-NarrowColumnCount=\"7\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/8\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineSourceNode&gt; CreateSourceNode(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">const</span>&nbsp;</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">wchar_t&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">*</div><div class=\"PName\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pName,</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">IImage&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">*</div><div class=\"PName\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">pImage,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">uint32_t&nbsp;</div><div class=\"PName\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">outputWidth,</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">uint32_t&nbsp;</div><div class=\"PName\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">outputHeight,</div><div class=\"PType\" data-WideGridArea=\"5/3/6/4\" data-NarrowGridArea=\"6/2/7/3\" style=\"grid-area:5/3/6/4\">DXGI_FORMAT&nbsp;</div><div class=\"PName\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"6/4/7/5\" style=\"grid-area:5/5/6/6\">format</div><div class=\"PDefaultValueSeparator\" data-WideGridArea=\"5/6/6/7\" data-NarrowGridArea=\"6/5/7/6\" style=\"grid-area:5/6/6/7\">=&nbsp;</div><div class=\"PDefaultValue InLastParameterColumn\" data-WideGridArea=\"5/7/6/8\" data-NarrowGridArea=\"6/6/7/7\" style=\"grid-area:5/7/6/8\">DXGI_FORMAT_R8G8B8A8_UNORM</div><div class=\"PAfterParameters\" data-WideGridArea=\"5/8/6/9\" data-NarrowGridArea=\"7/1/8/8\" style=\"grid-area:5/8/6/9\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns a source node. The source node will feed the pipeline with the specified image.</div></div>",633:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype633\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"8\" data-NarrowColumnCount=\"7\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/8\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineSinkNode&gt; CreateSinkNode(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">const</span>&nbsp;</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">wchar_t*&nbsp;</div><div class=\"PName\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pName,</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">IShader&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">*</div><div class=\"PName\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">pShader,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">uint32_t&nbsp;</div><div class=\"PName\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">outputWidth,</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">uint32_t&nbsp;</div><div class=\"PName\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">outputHeight,</div><div class=\"PType\" data-WideGridArea=\"5/3/6/4\" data-NarrowGridArea=\"6/2/7/3\" style=\"grid-area:5/3/6/4\">DXGI_FORMAT&nbsp;</div><div class=\"PName\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"6/4/7/5\" style=\"grid-area:5/5/6/6\">format</div><div class=\"PDefaultValueSeparator\" data-WideGridArea=\"5/6/6/7\" data-NarrowGridArea=\"6/5/7/6\" style=\"grid-area:5/6/6/7\">=&nbsp;</div><div class=\"PDefaultValue InLastParameterColumn\" data-WideGridArea=\"5/7/6/8\" data-NarrowGridArea=\"6/6/7/7\" style=\"grid-area:5/7/6/8\">DXGI_FORMAT_R8G8B8A8_UNORM</div><div class=\"PAfterParameters\" data-WideGridArea=\"5/8/6/9\" data-NarrowGridArea=\"7/1/8/8\" style=\"grid-area:5/8/6/9\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns a sink node.</div></div>",634:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype634\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"8\" data-NarrowColumnCount=\"7\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/8\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineNode&gt; CreateNode(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">const</span>&nbsp;</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">wchar_t*&nbsp;</div><div class=\"PName\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pName,</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">IShader&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">*</div><div class=\"PName\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">pShader,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">uint32_t&nbsp;</div><div class=\"PName\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">outputWidth,</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">uint32_t&nbsp;</div><div class=\"PName\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">outputHeight,</div><div class=\"PType\" data-WideGridArea=\"5/3/6/4\" data-NarrowGridArea=\"6/2/7/3\" style=\"grid-area:5/3/6/4\">DXGI_FORMAT&nbsp;</div><div class=\"PName\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"6/4/7/5\" style=\"grid-area:5/5/6/6\">format</div><div class=\"PDefaultValueSeparator\" data-WideGridArea=\"5/6/6/7\" data-NarrowGridArea=\"6/5/7/6\" style=\"grid-area:5/6/6/7\">=&nbsp;</div><div class=\"PDefaultValue InLastParameterColumn\" data-WideGridArea=\"5/7/6/8\" data-NarrowGridArea=\"6/6/7/7\" style=\"grid-area:5/7/6/8\">DXGI_FORMAT_R8G8B8A8_UNORM</div><div class=\"PAfterParameters\" data-WideGridArea=\"5/8/6/9\" data-NarrowGridArea=\"7/1/8/8\" style=\"grid-area:5/8/6/9\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns a GPU node that makes up part of the pipeline.</div></div>",635:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype635\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"6\" data-NarrowColumnCount=\"5\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/3/2\" data-NarrowGridArea=\"1/1/2/6\" style=\"grid-area:1/1/3/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineNode&gt; CreatePredefinedNode(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">const</span>&nbsp;</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">wchar_t*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pName,</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">...</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"2/6/3/7\" data-NarrowGridArea=\"4/1/5/6\" style=\"grid-area:2/6/3/7\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns a GPU node that makes up part of the pipeline. This method is for specific nodes that have a custom Process() step beyond just running some shader.</div></div>",636:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype636\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> AddCustomNode(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IGPUPipelineNode*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pNode</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Adds a custom GPU node to the end of the pipeline</div></div>",766:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an image (CPU based)</div></div>",883:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines an interface to our basic JSON object returned by the parser.</div></div>",899:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype899\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IJSonObj&gt; LoadDOM(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">std::</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">wstring&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">fn</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"3/1/4/5\" style=\"grid-area:1/5/2/6\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Loads the Document object model from the specified JSON file</div></div>",900:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype900\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/3/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/3/2\"><span class=\"SHKeyword\">virtual void</span> SaveDOM(</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">CRefObj&lt;IJSonObj&gt;&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">dom,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">std::</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">wstring&amp;&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">fn</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"4/1/5/5\" style=\"grid-area:2/5/3/6\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Saves the Document object model (DOM) to the specified JSON file</div></div>",1133:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",1193:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",1266:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our basic renderer. IRenderer handles all rendering commands.&nbsp; It is generally expected that this object is running on its own thread.</div></div>"});