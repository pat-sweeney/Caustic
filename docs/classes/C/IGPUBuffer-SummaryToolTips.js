﻿NDSummary.OnToolTipsLoaded("CClass:IGPUBuffer",{827:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface to a buffer used by the GPU. When clients need to pass data to a compute shader (i.e. as either a StructuredBuffer, RWStructuredBuffer, AppendStructuredBuffer, or RWByteAddressBuffer) they will pass one of these objects via IShader::SetCSParam().</div></div>",829:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype829\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CopyFromCPU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>*&nbsp;</td><td class=\"PName last\">pData</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">CopyFromCPU() is used to move data from the CPU (i.e. client data) to the underlying GPU buffer.</div></div>",830:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype830\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> CopyToCPU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>*&nbsp;</td><td class=\"PName last\">pData</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">CopyToCPU() is used to move data from the CPU (i.e. client data) to the underlying GPU buffer.</div></div>",831:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype831\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IGPUBuffer&gt; CreateGPUBuffer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IRenderer*&nbsp;</td><td class=\"PName last\">pRenderer,</td></tr><tr><td class=\"PType first\">EBufferType&nbsp;</td><td class=\"PName last\">type,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">numElems,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">elemSize,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Creates a buffer on the GPU</div></div>"});