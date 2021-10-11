﻿NDSummary.OnToolTipsLoaded("CClass:IDepthCameraDevice",{255:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface for depth cameras</div></div>",257:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype257\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> NextFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage**&nbsp;</td><td class=\"PName last\">ppColorImage,</td></tr><tr><td class=\"PType first\">IImage**&nbsp;</td><td class=\"PName last\">ppDepthImage,</td></tr><tr><td class=\"PType first\">IImage**&nbsp;</td><td class=\"PName last\">ppIRImage</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Retrieves the next color/depth/ir image from the camera</div></div>",258:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype258\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetDepthWidth() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the width in pixels of the depth image returned by the camera</div></div>",259:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype259\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetDepthHeight() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the height in pixels of the depth image returned by the camera</div></div>",260:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype260\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Matrix4x4 ColorExtrinsics() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the extrinsics matrix for the color camera.</div></div>",261:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype261\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Matrix3x3 ColorIntrinsics() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the intrinsics matrix for the color camera.</div></div>",262:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype262\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Matrix4x4 DepthExtrinsics() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the extrinsics matrix for the depth camera.</div></div>",263:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype263\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Matrix3x3 DepthIntrinsics() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the intrinsics matrix for the depth camera.</div></div>"});