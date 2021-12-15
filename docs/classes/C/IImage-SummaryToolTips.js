﻿NDSummary.OnToolTipsLoaded("CClass:IImage",{658:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface to an image (CPU based)</div></div>",660:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype660\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Clear() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Erases the image (setting it to black with full alpha)</div></div>",661:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype661\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> CRefObj&lt;IImage&gt; Clone() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Makes a copy of the image</div></div>",662:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype662\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> TakeDataOwnership() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">If the image was created with a pre-existing block of image data, this function will transfer ownership of that data to the image class.&nbsp; After calling this the Image class will be responsible for deleting the data when the image is detroyed. Otherwise, the app will be responsible for freeing the data.</div></div>",663:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype663\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPixel(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">y,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>&nbsp;</td><td class=\"PName last\">color[<span class=\"SHNumber\">4</span>]</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel in the image. This method assumes the underlying image is BGRA.</div></div>",664:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype664\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPixel(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">y,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>&nbsp;</td><td class=\"PName last\">gray</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel in the image. This method assumes the underlying image is gray scale.</div></div>",665:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype665\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPixel(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">x,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">y,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint16</span>&nbsp;</td><td class=\"PName last\">v</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets a pixel in the image. This method assumes the underlying image is 16b grayscale.</div></div>",666:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype666\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawCircle(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">Vector2&amp;&nbsp;</td><td class=\"PName last\">center,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint32</span>&nbsp;</td><td class=\"PName last\">radius,</td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">uint8</span>&nbsp;</td><td class=\"PName last\">color[<span class=\"SHNumber\">4</span>]</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a circle on the image</div></div>",667:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype667\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawLine(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&amp;&nbsp;</td><td class=\"PName last\">v0,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">const</span>&nbsp;</td><td class=\"PType\">Vector2&amp;&nbsp;</td><td class=\"PName last\">v1,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">uint8</span>&nbsp;</td><td class=\"PName last\">color[<span class=\"SHNumber\">4</span>]</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a line on the image</div></div>",668:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype668\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> DrawPath(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IPath2*&nbsp;</td><td class=\"PName last\">pPath</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Renders a path on the image</div></div>"});