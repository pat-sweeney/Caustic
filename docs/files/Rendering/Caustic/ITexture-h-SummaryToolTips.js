NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/ITexture.h",{627:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for textures.</div></div>",628:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",630:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype630\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetWidth() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the width of the texture in pixels</div></div>",631:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype631\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetHeight() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the height of the texture in pixels</div></div>",632:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype632\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DXGI_FORMAT GetFormat() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the pixel format of the texture</div></div>",633:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype633\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Update(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Updates the underlying graphics card texture. Primarily used for video textures.</div></div>",634:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype634\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11Texture2D&gt; GetD3DTexture() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the underlying D3D texture</div></div>",635:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype635\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11ShaderResourceView&gt; GetD3DTextureRV() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the underlying D3D shader resource view associated with the texture</div></div>",636:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype636\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GenerateMips(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Generates a set of mipmaps for the texture</div></div>",637:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype637\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">slot,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">isPixelShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Pushes the specified texture to the GPU</div></div>"});