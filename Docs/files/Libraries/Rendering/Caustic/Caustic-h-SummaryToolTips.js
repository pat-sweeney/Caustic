NDSummary.OnToolTipsLoaded("File:Libraries/Rendering/Caustic/Caustic.h",{655:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype655\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">Caustic</div></div></div></div>",228:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines how clients interact with textures</div></div>",671:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype671\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetWidth() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the width of the texture in pixels</div></div>",672:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype672\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetHeight() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the height of the texture in pixels</div></div>",673:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype673\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DXGI_FORMAT GetFormat() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the pixel format of the texture</div></div>",674:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype674\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Update(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Updates the underlying graphics card texture. Primarily used for video textures.</div></div>",675:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype675\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11Texture2D&gt; GetD3DTexture() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the underlying D3D texture</div></div>",676:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype676\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> CComPtr&lt;ID3D11ShaderResourceView&gt; GetD3DTextureRV() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the underlying D3D shader resource view associated with the texture</div></div>",677:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype677\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GenerateMips(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Generates a set of mipmaps for the texture</div></div>",678:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a sampler. Samplers define how the underlying texture is accessed by shaders.</div></div>",680:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype680\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_FILTER GetFilter() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current filter state</div></div>",681:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype681\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFilter(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">D3D11_FILTER&nbsp;</td><td class=\"PName last\">filter</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Set the current filter state</div></div>",690:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype690\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressU() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current addressing mode in the U direction</div></div>",691:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype691\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetAddressU(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">D3D11_TEXTURE_ADDRESS_MODE&nbsp;</td><td class=\"PName last\">mode</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Set the current addressing mode in the U direction</div></div>",692:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype692\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> D3D11_TEXTURE_ADDRESS_MODE GetAddressV() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current addressing mode in the V direction</div></div>",693:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Set the current addressing mode in the V direction</div></div>",694:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype694\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">slot</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets up the sampler for the current rendering</div></div>",695:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype695\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetTexture(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ITexture&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppTexture</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the texture associated with this sampler</div></div>",696:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines our camera object</div></div>",723:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype723\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetParams(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">fov,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">aspectRatio,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">nearZ,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">farZ</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the camera parameters</div></div>",743:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype743\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">eye,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">look,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">up</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the camera\'s position.</div></div>",745:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype745\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pEye,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pLook,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pUp,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pU,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pV,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pN</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Gets the camera\'s position.</div></div>",746:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype746\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">offset</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets an offset that is applied to the camera\'s eye and look point.</div></div>",747:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype747\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetOffset(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">offset</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the current offset</div></div>",748:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype748\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DirectX::XMMATRIX GetProjection() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the camera\'s projection matrix</div></div>",749:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype749\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DirectX::XMMATRIX GetView() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the camera\'s view matrix</div></div>",750:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype750\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual float</span> GetNear() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the camera\'s near plane distance</div></div>",751:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype751\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual float</span> GetFar() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the camera\'s far plane distance</div></div>",752:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype752\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> GetUVN(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">u,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">v,</td></tr><tr><td class=\"PType first\">Vector3&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">n</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns the camera axis in world coordinates</div></div>",707:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype707\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">enum</span> </div><div class=\"CPName\">ETrackballConstrain</div></div></div><div class=\"TTSummary\">Defines available constraints for the trackball object</div></div>",701:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface to our trackball implementation</div></div>",703:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype703\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BeginTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenW,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">screenH</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Called to start tracking</div></div>",704:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype704\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> UpdateTracking(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenX,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td></td><td class=\"PName last\">screenY,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">ETrackballConstrain&nbsp;</td><td></td><td class=\"PName last\">constraint,</td></tr><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pMatrix</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Called when mouse moves during trackball tracking</div></div>",705:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype705\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual void</span> EndTracking() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Called when mouse is released during trackball tracking</div></div>",708:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a point light</div></div>",709:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype709\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">RenderCtxFlags</div></div></div><div class=\"TTSummary\">Defines flags for setting various render modes</div></div>",711:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for setting various features in the renderer</div></div>",764:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype764\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDebugFlags(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">flags</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets debug flags used for rendering</div></div>",765:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype765\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetDebugFlags() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current debug flags used for rendering</div></div>",766:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype766\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetNormalScale(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">normalScale</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the scale used to determine how large debug normal vectors are drawn</div></div>",767:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype767\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual float</span> GetNormalScale() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the scale used for determining how large debug normal vectors are drawn</div></div>",768:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype768\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual uint32</span> GetCurrentPass() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the current render pass</div></div>",769:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype769\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual bool</span> PassBlendable() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns whether the current render pass supports alpha blending</div></div>",712:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface clients use to talk to the renderer</div></div>",713:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Creates a renderer that runs on its own thread and method calls are marshalled from the client to render thread.&nbsp; renderer is on a separate thread where the client is calling from)</div></div>",710:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the public interface for using CShader</div></div>",706:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines a material that has its associated render element (as opposed to an IMaterialAttrib which only defines the attributes of the material).</div></div>",682:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Manages shaders within the renderer</div></div>",684:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype684\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> FindShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">ppShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Finds an existing shader by name</div></div>",685:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype685\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RegisterShader(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShaderName,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IShader&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pShader</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Registers a new shader</div></div>",686:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the main interface for rendered objects</div></div>",688:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype688\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> Vector3 GetPos() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the world position of the object</div></div>",697:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype697\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Render(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">IGraphics&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pGraphics,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">vector&lt;CRefObj&lt;IPointLight&gt;&gt;&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">lights,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IRenderCtx&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">pRenderCtx</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Called by the renderer to render this object</div></div>",698:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype698\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTransform(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">DirectX::</td><td class=\"PType\">XMMATRIX&nbsp;</td><td class=\"PNamePrefix\">&amp;</td><td class=\"PName last\">mat</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Sets the transform on the object</div></div>",699:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype699\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">virtual</span> DirectX::XMMATRIX &amp;GetTransform() = <span class=\"SHNumber\">0</span></div><div class=\"TTSummary\">Returns the transforms associated with the object</div></div>",700:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype700\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> InPass(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">pass</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div><div class=\"TTSummary\">Returns whether the object is part of the specified pass</div></div>"});