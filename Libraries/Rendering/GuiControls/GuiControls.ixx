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
import Base.Core.ConvertStr;
import Base.Math.BBox;
import Base.Math.Vector;
import Imaging.Color;
import Geometry.Mesh.IMaterialAttrib;

export namespace Caustic
{
	void ImGui_Vector(const char* pLabel, std::function<Vector3()>getFunc, std::function<void(Vector3 v)>setFunc,
		float minValue, float maxValue)
	{
		Vector3 v = getFunc();
		ImGui::Text(pLabel);
		std::string strLabel(pLabel);
		ImGui::Text("  X:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##X")+strLabel).c_str(), &v.x, minValue, maxValue);
		ImGui::Text("  Y:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##Y")+strLabel).c_str(), &v.y, minValue, maxValue);
		ImGui::Text("  Z:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##Z")+strLabel).c_str(), &v.z, minValue, maxValue);
		setFunc(v);
	}

	void ImGui_BBox3(const char *pLabel, BBox3 &bbox, float minV = 0.0f, float maxV = 1.0f)
	{
		if (bbox.minPt.x == FLT_MAX)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			std::string strLabel((pLabel == nullptr) ? "BBox:" : pLabel);
			ImGui::Text(strLabel.c_str());
			ImGui::SameLine();
			ImGui::Text("Not set");
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
			return;
		}
		std::string strLabel((pLabel == nullptr) ? "BBox:" : pLabel);
		ImGui::Text(strLabel.c_str());
		ImGui::Text("  Min:");
		ImGui::Text("      X:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##BBoxMinX") + strLabel).c_str(), &bbox.minPt.x, minV, maxV);
		ImGui::Text("      Y:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##BBoxMinY") + strLabel).c_str(), &bbox.minPt.y, minV, maxV);
		ImGui::Text("      Z:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##BBoxMinZ") + strLabel).c_str(), &bbox.minPt.z, minV, maxV);
		ImGui::Text("  Max:");
		ImGui::Text("      X:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##BBoxMaxX") + strLabel).c_str(), &bbox.maxPt.x, minV, maxV);
		ImGui::Text("      Y:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##BBoxMaxY") + strLabel).c_str(), &bbox.maxPt.y, minV, maxV);
		ImGui::Text("      Z:"); ImGui::SameLine();
		ImGui::SliderFloat((std::string("##BBoxMaxZ") + strLabel).c_str(), &bbox.maxPt.z, minV, maxV);
	}

	void ImGui_Color(const char* pLabel, uint32 index, std::function<FRGBColor()>getFunc, std::function<void(FRGBColor v)>setFunc)
	{
		FRGBColor clr = getFunc();
		ImGui::Text(pLabel);
		ImGui::SameLine();
		float color[4] = { clr.r, clr.g, clr.b, 1.0f };
		if (ImGui::ColorEdit4(("##" + std::string(pLabel) + std::to_string(index)).c_str(), color))
		{
			FRGBColor q(color[0], color[1], color[2]);
			setFunc(q);
		}
	}
}
