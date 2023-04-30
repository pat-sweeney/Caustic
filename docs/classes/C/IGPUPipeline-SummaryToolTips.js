﻿NDSummary.OnToolTipsLoaded("CClass:IGPUPipeline",{632:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">The IGPUPipeline object is used to create a graph of GPU shaders that will process images.&nbsp; A pipeline is built out of a series of IGPUPipelineNode\'s. Each node is conceptually just a pixel shader to be run on its input image data.&nbsp; The general structure of a GPU graph looks something like:</div></div>",634:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype634\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderer&gt; GetRenderer() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the Renderer associated with this pipeline</div></div>",635:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype635\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/2/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/2/2\"><span class=\"SHKeyword\">virtual void</span> RenderQuad(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IShader*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">pShader</div><div class=\"PAfterParameters\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"3/1/4/4\" style=\"grid-area:1/4/2/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Draws the shader using a fullscreen quad</div></div>",636:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype636\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"5\" data-NarrowColumnCount=\"4\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/3/2\" data-NarrowGridArea=\"1/1/2/5\" style=\"grid-area:1/1/3/2\"><span class=\"SHKeyword\">virtual void</span> Process(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\">IRenderer&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">pRenderer,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\">IRenderCtx&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">pRenderCtx</div><div class=\"PAfterParameters\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"4/1/5/5\" style=\"grid-area:2/5/3/6\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Executes the pipeline</div></div>",637:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype637\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"8\" data-NarrowColumnCount=\"7\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/8\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineSourceNode&gt; CreateSourceNode(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">const</span>&nbsp;</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"1/4/2/5\" data-NarrowGridArea=\"2/3/3/4\" style=\"grid-area:1/4/2/5\">*</div><div class=\"PName\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pName,</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">IImage&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">*</div><div class=\"PName\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">pImage,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\"><span class=\"SHKeyword\">uint32</span>&nbsp;</div><div class=\"PName\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">outputWidth,</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\"><span class=\"SHKeyword\">uint32</span>&nbsp;</div><div class=\"PName\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">outputHeight,</div><div class=\"PType\" data-WideGridArea=\"5/3/6/4\" data-NarrowGridArea=\"6/2/7/3\" style=\"grid-area:5/3/6/4\">DXGI_FORMAT&nbsp;</div><div class=\"PName\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"6/4/7/5\" style=\"grid-area:5/5/6/6\">format</div><div class=\"PDefaultValueSeparator\" data-WideGridArea=\"5/6/6/7\" data-NarrowGridArea=\"6/5/7/6\" style=\"grid-area:5/6/6/7\">=&nbsp;</div><div class=\"PDefaultValue InLastParameterColumn\" data-WideGridArea=\"5/7/6/8\" data-NarrowGridArea=\"6/6/7/7\" style=\"grid-area:5/7/6/8\">DXGI_FORMAT_R8G8B8A8_UNORM</div><div class=\"PAfterParameters\" data-WideGridArea=\"5/8/6/9\" data-NarrowGridArea=\"7/1/8/8\" style=\"grid-area:5/8/6/9\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns a source node. The source node will feed the pipeline with the specified image.</div></div>",638:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype638\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"8\" data-NarrowColumnCount=\"7\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/8\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineSinkNode&gt; CreateSinkNode(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">const</span>&nbsp;</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</div><div class=\"PName\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pName,</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">IShader&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">*</div><div class=\"PName\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">pShader,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\"><span class=\"SHKeyword\">uint32</span>&nbsp;</div><div class=\"PName\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">outputWidth,</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\"><span class=\"SHKeyword\">uint32</span>&nbsp;</div><div class=\"PName\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">outputHeight,</div><div class=\"PType\" data-WideGridArea=\"5/3/6/4\" data-NarrowGridArea=\"6/2/7/3\" style=\"grid-area:5/3/6/4\">DXGI_FORMAT&nbsp;</div><div class=\"PName\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"6/4/7/5\" style=\"grid-area:5/5/6/6\">format</div><div class=\"PDefaultValueSeparator\" data-WideGridArea=\"5/6/6/7\" data-NarrowGridArea=\"6/5/7/6\" style=\"grid-area:5/6/6/7\">=&nbsp;</div><div class=\"PDefaultValue InLastParameterColumn\" data-WideGridArea=\"5/7/6/8\" data-NarrowGridArea=\"6/6/7/7\" style=\"grid-area:5/7/6/8\">DXGI_FORMAT_R8G8B8A8_UNORM</div><div class=\"PAfterParameters\" data-WideGridArea=\"5/8/6/9\" data-NarrowGridArea=\"7/1/8/8\" style=\"grid-area:5/8/6/9\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns a sink node.</div></div>",639:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype639\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"8\" data-NarrowColumnCount=\"7\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/6/2\" data-NarrowGridArea=\"1/1/2/8\" style=\"grid-area:1/1/6/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineNode&gt; CreateNode(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">const</span>&nbsp;</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</div><div class=\"PName\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pName,</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">IShader&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">*</div><div class=\"PName\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">pShader,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\"><span class=\"SHKeyword\">uint32</span>&nbsp;</div><div class=\"PName\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">outputWidth,</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\"><span class=\"SHKeyword\">uint32</span>&nbsp;</div><div class=\"PName\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">outputHeight,</div><div class=\"PType\" data-WideGridArea=\"5/3/6/4\" data-NarrowGridArea=\"6/2/7/3\" style=\"grid-area:5/3/6/4\">DXGI_FORMAT&nbsp;</div><div class=\"PName\" data-WideGridArea=\"5/5/6/6\" data-NarrowGridArea=\"6/4/7/5\" style=\"grid-area:5/5/6/6\">format</div><div class=\"PDefaultValueSeparator\" data-WideGridArea=\"5/6/6/7\" data-NarrowGridArea=\"6/5/7/6\" style=\"grid-area:5/6/6/7\">=&nbsp;</div><div class=\"PDefaultValue InLastParameterColumn\" data-WideGridArea=\"5/7/6/8\" data-NarrowGridArea=\"6/6/7/7\" style=\"grid-area:5/7/6/8\">DXGI_FORMAT_R8G8B8A8_UNORM</div><div class=\"PAfterParameters\" data-WideGridArea=\"5/8/6/9\" data-NarrowGridArea=\"7/1/8/8\" style=\"grid-area:5/8/6/9\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns a GPU node that makes up part of the pipeline.</div></div>",640:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype640\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"6\" data-NarrowColumnCount=\"5\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/3/2\" data-NarrowGridArea=\"1/1/2/6\" style=\"grid-area:1/1/3/2\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IGPUPipelineNode&gt; CreatePredefinedNode(</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">const</span>&nbsp;</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">pName,</div><div class=\"PSymbols\" data-WideGridArea=\"2/4/3/5\" data-NarrowGridArea=\"3/3/4/4\" style=\"grid-area:2/4/3/5\">...</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"2/6/3/7\" data-NarrowGridArea=\"4/1/5/6\" style=\"grid-area:2/6/3/7\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Returns a GPU node that makes up part of the pipeline. This method is for specific nodes that have a custom Process() step beyond just running some shader.</div></div>",641:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype641\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> IncrementCurrentEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Increments the current Epoch. The current epoch is a value indicating how many times the processing pipeline has been run. This is useful for determining whether a node in the pipeline has been run during the current frame.</div></div>",642:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype642\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetCurrentEpoch() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the current Epoch</div></div>",644:"<div class=\"NDToolTip TConstant LC\"><div class=\"TTSummary\">Defines name for a GPU node that creates a mesh from a depth map and renders that depth data into a render target. This is used primarily to map the depth to its corresponding color camera.</div></div>",645:"<div class=\"NDToolTip TConstant LC\"><div class=\"TTSummary\">Defines name for a GPU node that creates a mesh from a depth map and renders that the texture coordinates into a map. The map then contains texture coordinates for mapping color texture coordinates to depth texture coordinates.</div></div>"});