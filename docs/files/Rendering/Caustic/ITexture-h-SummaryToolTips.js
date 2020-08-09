NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/ITexture.h",{647:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for textures.</div></div>",648:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",655:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype655\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetWidth() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the width of the texture in pixels</div></div>",656:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype656\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetHeight() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the height of the texture in pixels</div></div>",657:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype657\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DXGI_FORMAT GetFormat() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the pixel format of the texture</div></div>",658:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype658\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Update(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Updates the underlying graphics card texture. Primarily used for video textures.</div></div>",659:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype659\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11Texture2D&gt; GetD3DTexture() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the underlying D3D texture</div></div>",660:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype660\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11ShaderResourceView&gt; GetD3DTextureRV() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the underlying D3D shader resource view associated with the texture</div></div>",661:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype661\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GenerateMips(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Generates a set of mipmaps for the texture</div></div>",662:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype662\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">slot,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td></td><td class=\"PName last\">isPixelShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Pushes the specified texture to the GPU</div></div>",1163:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1163\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td></td><td class=\"PName last\">format,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td></td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td></td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates an empty texture.</div></div>",1164:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1164\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td></td><td class=\"PName last\">height,</td></tr><tr><td class=\"PType first\">DXGI_FORMAT&nbsp;</td><td></td><td class=\"PName last\">format</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates an empty texture.</div></div>",1165:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1165\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; CreateTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\">IImage&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pImage,</td></tr><tr><td class=\"PType first\">D3D11_CPU_ACCESS_FLAG&nbsp;</td><td></td><td class=\"PName last\">cpuFlags,</td></tr><tr><td class=\"PType first\">D3D11_BIND_FLAG&nbsp;</td><td></td><td class=\"PName last\">bindFlags</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates a texture from an &lt;IImage at Caustic::IImage&gt;.</div></div>",1166:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1166\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; CheckerboardTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Creates a black and white checkboard texture</div></div>",1167:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1167\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; LoadTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">LoadTexture loads a texture from a file using WIC</div></div>",1168:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1168\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\">CAUSTICAPI CRefObj&lt;ITexture&gt; LoadVideoTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">LoadVideoTexture loads a video and uses it as the texture source</div></div>"});