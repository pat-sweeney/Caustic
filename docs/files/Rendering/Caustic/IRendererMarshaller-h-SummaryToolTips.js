﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/IRendererMarshaller.h",{852:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for talking to marshalled version of the renderer. The marshalled version allows for IRenderer methods to be called from threads other than the render thread.</div></div>",791:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Creates a renderer that runs on its own thread and method calls are marshalled from the client to render thread.&nbsp; The renderer is on a separate thread where the client is calling from.</div></div>",855:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype855\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Initialize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td></td><td class=\"PName\">hwnd,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PSymbols\">&amp;</td><td class=\"PName\">shaderFolder,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td></td><td class=\"PName\">renderCallback,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td></td><td class=\"PName\">prePresentCallback,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName\">startFrozen</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span>,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName\">desktopIndex</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">0</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Initializes the renderer</div></div>",856:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype856\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Shutdown() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Shuts down the renderer</div></div>",857:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype857\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RunOnRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer*)&gt;&nbsp;</td><td class=\"PName\">callback,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">wait</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Runs the specified callback function on the renderer\'s thread</div></div>",858:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype858\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a texture from the specified path</div></div>",859:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype859\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a video texture from the specified path</div></div>",860:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype860\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SaveScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph&nbsp;</td><td class=\"PSymbols\">*</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Saves the scene graph to the specified file</div></div>",861:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Loads the scene graph from the specified file</div></div>",862:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype862\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderer&gt; GetRenderer() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the renderer this object is marshalling calls to</div></div>",793:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype793\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;IRendererMarshaller&gt; CreateRendererMarshaller()</div></div><div class=\"TTSummary\">Global function for creating the RendererMarshaller. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>"});