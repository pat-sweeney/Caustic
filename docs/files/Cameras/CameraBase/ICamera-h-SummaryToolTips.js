﻿NDSummary.OnToolTipsLoaded("File:Cameras/CameraBase/ICamera.h",{254:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype254\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">Caustic</div></div></div></div>",255:"<div class=\"NDToolTip TStruct LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype255\"><div class=\"CPEntry TStruct Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">struct</span></div><div class=\"CPName\">CameraIntrinsics</div></div></div><div class=\"TTSummary\">Defines intrinsics for a camera</div></div>",256:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Base interface shared across all camera types</div></div>",258:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype258\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> NextFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage**&nbsp;</td><td class=\"PName last\">ppColorImage</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Retrieves the next color/depth/ir image from the camera Parameters: ppColorImage - returns the next color image retrieved from the camera. Maybe nullptr.</div></div>",259:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype259\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetColorWidth() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the width in pixels of the color image returned by the camera</div></div>",260:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype260\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetColorHeight() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the height in pixels of the color image returned by the camera</div></div>",261:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Interface for depth cameras</div></div>",263:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype263\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> NextFrame(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">IImage**&nbsp;</td><td class=\"PName last\">ppColorImage,</td></tr><tr><td class=\"PType first\">IImage**&nbsp;</td><td class=\"PName last\">ppDepthImage,</td></tr><tr><td class=\"PType first\">IImage**&nbsp;</td><td class=\"PName last\">ppIRImage</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Retrieves the next color/depth/ir image from the camera</div></div>",264:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype264\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetDepthWidth() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the width in pixels of the depth image returned by the camera</div></div>",265:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype265\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual uint32</span> GetDepthHeight() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the height in pixels of the depth image returned by the camera</div></div>",266:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype266\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Matrix4x4 ColorExtrinsics() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the extrinsics matrix for the color camera.</div></div>",267:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype267\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Matrix3x3 ColorIntrinsics() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the intrinsics matrix for the color camera.</div></div>",268:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype268\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Matrix4x4 DepthExtrinsics() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the extrinsics matrix for the depth camera.</div></div>",269:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype269\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> Matrix3x3 DepthIntrinsics() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the intrinsics matrix for the depth camera.</div></div>"});