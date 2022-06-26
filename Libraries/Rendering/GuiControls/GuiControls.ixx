//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <functional>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

export module Rendering.GuiControls.Common;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Base.Math.Vector;
import Imaging.Color;

export namespace Caustic
{
	void ImGui_Vector(const char* pLabel, std::function<Vector3()>getFunc, std::function<void(Vector3 v)>setFunc)
	{
		Vector3 v = getFunc();
		ImGui::Text(pLabel);
		std::string strLabel(pLabel);
		ImGui::SliderFloat((std::string("X:##")+strLabel).c_str(), &v.x, 0.0f, 100000.0f);
		ImGui::SliderFloat((std::string("Y:##")+strLabel).c_str(), &v.y, 0.0f, 100000.0f);
		ImGui::SliderFloat((std::string("Z:##")+strLabel).c_str(), &v.z, 0.0f, 100000.0f);
		setFunc(v);
	}

	void ImGui_Color(const char* pLabel, uint32 index, std::function<FRGBColor()>getFunc, std::function<void(FRGBColor v)>setFunc)
	{
		FRGBColor clr = getFunc();
		std::string strLabel(pLabel);
		float color[4] = { clr.r, clr.g, clr.b, 1.0f };
		if (ImGui::ColorEdit4((strLabel+"##"+std::to_string(index)).c_str(), color))
		{
			FRGBColor q(color[0], color[1], color[2]);
			setFunc(q);
		}
	}
}
