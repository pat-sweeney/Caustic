﻿NDSummary.OnToolTipsLoaded("File:Imaging/Image/IImage.ixx",{614:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype614\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">export namespace</span> </div><div class=\"CPName\">Caustic</div></div></div><div class=\"TTSummary\">{Link:import Imaging.Image.Image{Imaging/Image/Image.ixx}}</div></div>",615:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype615\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">enum</span></div><div class=\"CPName\">EImageType</div></div></div><div class=\"TTSummary\">Defines the types of images we currently support.</div></div>",616:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Base interface shared across all image types</div></div>",618:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype618\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> EImageType GetImageType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the type of data stored in the image</div></div>",619:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype619\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint8</span>* GetData() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns a pointer to the image data</div></div>",620:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype620\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetWidth() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the image\'s width in pixels</div></div>",621:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype621\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetHeight() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the image\'s height in pixels</div></div>",622:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype622\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetSubX() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the X offset into our parent image (if this is a subimage)</div></div>",623:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype623\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetSubY() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the Y offset into our parent image (if this is a subimage)</div></div>",1650:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1650\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IImageBase&gt; GetParent() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the parent image. This is only not nullptr if the image is a subimage of another image.</div></div>",624:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype624\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetBPP() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the image\'s bites per pixel</div></div>",625:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype625\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetStride() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the image\'s stride (bytes/scanline)</div></div>",626:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype626\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetBytesPerPixel() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of bytes in each pixel</div></div>",627:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype627\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> GetRGBOrder() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whethers the pixel component layout in memory is Red/Green/Blue.&nbsp; This is the default. Otherwise, it\'s in Blue/Green/Red ordering.</div></div>",628:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype628\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetRGBOrder(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">isRGB</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets whethers the pixel component layout in memory is Red/Green/Blue.&nbsp; This is the default. Otherwise, it\'s in Blue/Green/Red ordering.</div></div>",629:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an image (CPU based)</div></div>",631:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype631\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Clear() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Erases the image (setting it to black with full alpha)</div></div>",632:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype632\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IImage&gt; Clone() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Makes a copy of the image</div></div>",633:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype633\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> TakeDataOwnership() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">If the image was created with a pre-existing block of image data, this function will transfer ownership of that data to the image class.&nbsp; After calling this the Image class will be responsible for deleting the data when the image is detroyed. Otherwise, the app will be responsible for freeing the data.</div></div>",634:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype634\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPixel(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">y,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>&nbsp;</td><td class=\"PName last\">color[<span class=\"SHNumber\">4</span>]</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel in the image. This method assumes the underlying image is BGRA.</div></div>",635:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype635\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPixel(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">y,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>&nbsp;</td><td class=\"PName last\">gray</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel in the image. This method assumes the underlying image is gray scale.</div></div>",636:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype636\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPixel(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">y,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint16</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel in the image. This method assumes the underlying image is 16b grayscale.</div></div>",637:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype637\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawCircle(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&amp;&nbsp;</td><td class=\"PName last\">center,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">radius,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>&nbsp;</td><td class=\"PName last\">color[<span class=\"SHNumber\">4</span>]</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a circle on the image</div></div>",638:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype638\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&amp;&nbsp;</td><td class=\"PName last\">v0,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&amp;&nbsp;</td><td class=\"PName last\">v1,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint8</span>&nbsp;</td><td class=\"PName last\">color[<span class=\"SHNumber\">4</span>]</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a line on the image</div></div>",639:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype639\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawPath(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPath2*&nbsp;</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a path on the image</div></div>",640:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an integral image (see https://&#8203;en&#8203;.wikipedia&#8203;.org&#8203;/wiki&#8203;/Summed-area_table)</div></div>",642:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype642\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual uint32</span> GetSum(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">channel,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">x1,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">y1,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">x2,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">y2</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Returns the sum of the requested area</div></div>",643:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype643\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IImage&gt; BoxBlur(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">width,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">height</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Blurs the image</div></div>",644:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype644\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\">CRefObj&lt;IImage&gt; LoadImageFile(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFilename</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Loads an image from a file</div></div>",645:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype645\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> StoreImage(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">wchar_t</span>*&nbsp;</td><td class=\"PName last\">pFilename,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">IImage*&nbsp;</td><td class=\"PName last\">pImage</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Saves an image from a file</div></div>"});