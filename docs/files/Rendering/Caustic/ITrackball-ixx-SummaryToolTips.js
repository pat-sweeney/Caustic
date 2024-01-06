﻿NDSummary.OnToolTipsLoaded("File:Rendering/Caustic/ITrackball.ixx",{980:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">This file defines the published interface for talking to the track ball manipulator.</div></div>",32:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype32\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">enum</span> </div><div class=\"CPName\">ETrackballConstraint</div></div></div><div class=\"TTSummary\">Defines available constraints for the trackball object</div></div>",226:"<div class=\"NDToolTip TInterface LC\"><div class=\"TTSummary\">Defines the interface to our trackball implementation</div></div>",984:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype984\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"4\" data-NarrowColumnCount=\"3\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/5/2\" data-NarrowGridArea=\"1/1/2/4\" style=\"grid-area:1/1/5/2\"><span class=\"SHKeyword\">virtual void</span> BeginTracking(</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"1/2/2/3\" data-NarrowGridArea=\"2/1/3/2\" style=\"grid-area:1/2/2/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\">screenX,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"2/2/3/3\" data-NarrowGridArea=\"3/1/4/2\" style=\"grid-area:2/2/3/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\">screenY,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"3/2/4/3\" data-NarrowGridArea=\"4/1/5/2\" style=\"grid-area:3/2/4/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">screenW,</div><div class=\"PType InFirstParameterColumn\" data-WideGridArea=\"4/2/5/3\" data-NarrowGridArea=\"5/1/6/2\" style=\"grid-area:4/2/5/3\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">screenH</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"4/4/5/5\" data-NarrowGridArea=\"6/1/7/4\" style=\"grid-area:4/4/5/5\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Called to start tracking</div></div>",985:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype985\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><div class=\"PParameterCells\" data-WideColumnCount=\"6\" data-NarrowColumnCount=\"5\"><div class=\"PBeforeParameters\" data-WideGridArea=\"1/1/5/2\" data-NarrowGridArea=\"1/1/2/6\" style=\"grid-area:1/1/5/2\"><span class=\"SHKeyword\">virtual bool</span> UpdateTracking(</div><div class=\"PType\" data-WideGridArea=\"1/3/2/4\" data-NarrowGridArea=\"2/2/3/3\" style=\"grid-area:1/3/2/4\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"1/5/2/6\" data-NarrowGridArea=\"2/4/3/5\" style=\"grid-area:1/5/2/6\">screenX,</div><div class=\"PType\" data-WideGridArea=\"2/3/3/4\" data-NarrowGridArea=\"3/2/4/3\" style=\"grid-area:2/3/3/4\"><span class=\"SHKeyword\">int</span>&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"2/5/3/6\" data-NarrowGridArea=\"3/4/4/5\" style=\"grid-area:2/5/3/6\">screenY,</div><div class=\"PType\" data-WideGridArea=\"3/3/4/4\" data-NarrowGridArea=\"4/2/5/3\" style=\"grid-area:3/3/4/4\">ETrackballConstraint&nbsp;</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"3/5/4/6\" data-NarrowGridArea=\"4/4/5/5\" style=\"grid-area:3/5/4/6\">constraint,</div><div class=\"PModifierQualifier InFirstParameterColumn\" data-WideGridArea=\"4/2/5/3\" data-NarrowGridArea=\"5/1/6/2\" style=\"grid-area:4/2/5/3\">DirectX::</div><div class=\"PType\" data-WideGridArea=\"4/3/5/4\" data-NarrowGridArea=\"5/2/6/3\" style=\"grid-area:4/3/5/4\">XMMATRIX&nbsp;</div><div class=\"PSymbols\" data-WideGridArea=\"4/4/5/5\" data-NarrowGridArea=\"5/3/6/4\" style=\"grid-area:4/4/5/5\">*</div><div class=\"PName InLastParameterColumn\" data-WideGridArea=\"4/5/5/6\" data-NarrowGridArea=\"5/4/6/5\" style=\"grid-area:4/5/5/6\">pMatrix</div><div class=\"PAfterParameters NegativeLeftSpaceOnWide\" data-WideGridArea=\"4/6/5/7\" data-NarrowGridArea=\"6/1/7/6\" style=\"grid-area:4/6/5/7\">) = <span class=\"SHNumber\">0</span></div></div></div></div><div class=\"TTSummary\">Called when mouse moves during trackball tracking</div></div>",986:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype986\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> EndTracking() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Called when mouse is released during trackball tracking</div></div>",227:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype227\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">CRefObj&lt;ITrackball&gt; CreateTrackball()</div></div><div class=\"TTSummary\">Global function for creating a track ball. This method should generally not be called. Use the ICausticFactory to create new Caustic objects.</div></div>"});