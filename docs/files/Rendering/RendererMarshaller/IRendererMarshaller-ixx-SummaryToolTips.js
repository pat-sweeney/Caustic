﻿NDSummary.OnToolTipsLoaded("File:Rendering/RendererMarshaller/IRendererMarshaller.ixx",{1282:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for talking to marshalled version of the renderer. The marshalled version allows for IRenderer methods to be called from threads other than the render thread.</div></div>",1283:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Creates a renderer that runs on its own thread and method calls are marshalled from the client to render thread.&nbsp; The renderer is on a separate thread where the client is calling from.</div></div>",1285:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1285\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Initialize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">HWND&nbsp;</td><td class=\"PName\">hwnd,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&amp;&nbsp;</td><td class=\"PName\">shaderFolder,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer, IRenderCtx* pRenderCtx, <span class=\"SHKeyword\">int</span> pass)&gt;&nbsp;</td><td class=\"PName\">renderCallback,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer* pRenderer)&gt;&nbsp;</td><td class=\"PName\">prePresentCallback,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">startFrozen</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span>,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName\">desktopIndex</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHNumber\">0</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Initializes the renderer</div></div>",1286:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1286\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Shutdown() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Shuts down the renderer</div></div>",1287:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1287\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RunOnRenderer(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(IRenderer*)&gt;&nbsp;</td><td class=\"PName\">callback,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">wait</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Runs the specified callback function on the renderer\'s thread</div></div>",1288:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1288\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a texture from the specified path</div></div>",1289:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1289\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;ITexture&gt; LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Loads a video texture from the specified path</div></div>",1290:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1290\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SaveScene(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ISceneGraph*&nbsp;</td><td class=\"PName last\">pSceneGraph</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Saves the scene graph to the specified file</div></div>",1291:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Loads the scene graph from the specified file</div></div>",1292:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1292\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IRenderer&gt; GetRenderer() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the renderer this object is marshalling calls to</div></div>"});