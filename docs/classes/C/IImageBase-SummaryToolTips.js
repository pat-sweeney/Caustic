﻿NDSummary.OnToolTipsLoaded("CClass:IImageBase",{420:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Base interface shared across all image types</div></div>",446:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype446\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> EImageType GetImageType() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the type of data stored in the image</div></div>",447:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype447\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint8</span> *GetData() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns a pointer to the image data</div></div>",448:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype448\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetWidth() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the image\'s width in pixels</div></div>",449:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype449\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetHeight() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the image\'s height in pixels</div></div>",450:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype450\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetSubX() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the X offset into our parent image (if this is a subimage)</div></div>",451:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype451\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetSubY() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the Y offset into our parent image (if this is a subimage)</div></div>",452:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype452\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetBPP() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the image\'s bites per pixel</div></div>",453:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype453\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetStride() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the image\'s stride (bytes/scanline)</div></div>",454:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype454\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetBytesPerPixel() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of bytes in each pixel</div></div>",455:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype455\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> GetRGBOrder() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns whethers the pixel component layout in memory is Red/Green/Blue.&nbsp; This is the default. Otherwise, it\'s in Blue/Green/Red ordering.</div></div>",456:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype456\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetRGBOrder(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">isRGB</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Sets whethers the pixel component layout in memory is Red/Green/Blue.&nbsp; This is the default. Otherwise, it\'s in Blue/Green/Red ordering.</div></div>",548:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype548\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> LexToken PeekToken() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the next token without advancing the reader</div></div>",549:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype549\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> LexToken ReadToken() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the next token.</div></div>",550:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype550\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetParseTable(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ParseTableEntry*&nbsp;</td><td class=\"PName last\">pTable</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Set the list of keywords that the parser will isolate as individual tokens.</div></div>"});