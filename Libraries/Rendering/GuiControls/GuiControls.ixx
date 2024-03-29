//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
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
#include <d3d11.h>
#include <sstream>
#include <cstdlib>

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
    //**********************************************************************
    // Function: ImGui_ScalableSlider
    // Builds a ImGui slider that can decrease/increase the minimum/maximum
    // value allowed by the slider. This is useful for instance when we want
    // to postion an object in a scene that currently only covers the range
    // -1..+1. Now if we set the slider min/max to -1000/+1000 it would be
    // nearly impossible to set a value of 1.5, but we still want to ultimately
    // be able to set a max value of 1000. Instead, we grow the range of
    // the allowed slider values by 5% each time the min or max value is set.
    // 
    // Parameters:
    // pLabel - Label for UI. Maybe nullptr.
    // setUniqueID - should we generate a unique label ID for this element? (i.e. "#<uniqueID>")
    // value - reference to value to be modified
    // minValue - minimum value we can set
    // maxValue - maximum value we can set
    //**********************************************************************
    bool ImGui_ScalableSlider(const char *pLabel, int uniqueID, float &value, float &minValue, float &maxValue)
    {
        std::string label((pLabel) ? "" : pLabel);
        std::stringstream ss;
        ss << label << "#" << uniqueID;
        bool changed = false;
        if (ImGui::SliderFloat(ss.str().c_str(), &value, minValue, maxValue))
        {
            if (value == maxValue)
            {
                float delta = (maxValue - minValue) * 0.05f;
                minValue += delta;
                maxValue += delta;
            }
            changed = true;
        }
        return changed;
    }

    //**********************************************************************
    // Function: ImGui_Vector
    // Builds ImGui UI for modifying a Vector3 using a scalable range.
    // 
    // Parameters:
    // pLabel - Label for UI
    // getFunc - lambda that returns the current value we want to modify
    // setFunc - lambda that is called to set the current value we are modifying
    // minValue - minimum value we can set
    // maxValue - maximum value we can set
    // scalableRange - is min/max value adjusted if we select min/max?
    //**********************************************************************
    bool ImGui_Vector(const char* pLabel, std::function<Vector3()>getFunc, std::function<void(Vector3 v)>setFunc,
        float &minValue, float &maxValue, bool scalableRange)
    {
        Vector3 v = getFunc();
        ImGui::Text(pLabel);
        std::string strLabel(pLabel);
        ImGui::Text("  X:"); ImGui::SameLine();
        bool changed = false;
        bool adjustRange = false;
        if (ImGui::SliderFloat((std::string("##X") + strLabel).c_str(), &v.x, minValue, maxValue))
        {
            if (v.x == maxValue || v.x == minValue)
                adjustRange = true;
            changed = true;
        }
        ImGui::Text("  Y:"); ImGui::SameLine();
        if (ImGui::SliderFloat((std::string("##Y") + strLabel).c_str(), &v.y, minValue, maxValue))
        {
            if (v.x == maxValue || v.x == minValue)
                adjustRange = true;
            changed = true;
        }
        ImGui::Text("  Z:"); ImGui::SameLine();
        if (ImGui::SliderFloat((std::string("##Z") + strLabel).c_str(), &v.z, minValue, maxValue))
        {
            if (v.x == maxValue || v.x == minValue)
                adjustRange = true;
            changed = true;
        }
        if (scalableRange && adjustRange)
        {
            float delta = (maxValue - minValue) * 0.05f;
            minValue += delta;
            maxValue += delta;
        }
        setFunc(v);
        return changed;
    }

    bool ImGui_Vector(const char* pLabel, std::function<Vector3()>getFunc, std::function<void(Vector3 v)>setFunc,
        float minValue, float maxValue)
    {
        return ImGui_Vector(pLabel, getFunc, setFunc, minValue, maxValue, false);
    }

    bool ImGui_BBox3(const char *pLabel, BBox3 &bbox, float minV = 0.0f, float maxV = 1.0f)
    {
        bool changed = false;
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
            return false;
        }
        std::string strLabel((pLabel == nullptr) ? "BBox:" : pLabel);
        ImGui::Text(strLabel.c_str());
        ImGui::Text("  Min:");
        ImGui::Text("      X:"); ImGui::SameLine();
        if (ImGui::SliderFloat((std::string("##BBoxMinX") + strLabel).c_str(), &bbox.minPt.x, minV, maxV))
            changed = true;
        ImGui::Text("      Y:"); ImGui::SameLine();
        if (ImGui::SliderFloat((std::string("##BBoxMinY") + strLabel).c_str(), &bbox.minPt.y, minV, maxV))
            changed = true;
        ImGui::Text("      Z:"); ImGui::SameLine();
        if (ImGui::SliderFloat((std::string("##BBoxMinZ") + strLabel).c_str(), &bbox.minPt.z, minV, maxV))
            changed = true;
        ImGui::Text("  Max:");
        ImGui::Text("      X:"); ImGui::SameLine();
        if (ImGui::SliderFloat((std::string("##BBoxMaxX") + strLabel).c_str(), &bbox.maxPt.x, minV, maxV))
            changed = true;
        ImGui::Text("      Y:"); ImGui::SameLine();
        if (ImGui::SliderFloat((std::string("##BBoxMaxY") + strLabel).c_str(), &bbox.maxPt.y, minV, maxV))
            changed = true;
        ImGui::Text("      Z:"); ImGui::SameLine();
        if (ImGui::SliderFloat((std::string("##BBoxMaxZ") + strLabel).c_str(), &bbox.maxPt.z, minV, maxV))
            changed = true;
        return changed;
    }

    bool ImGui_FillMode(D3D11_FILL_MODE &fillMode)
    {
        const char* fillModes[] = { "Wireframe", "Solid" };
        bool changed = false;
        int fillIndex = 0;
        switch (fillMode)
        {
        case D3D11_FILL_MODE::D3D11_FILL_WIREFRAME: fillIndex = 0; break;
        case D3D11_FILL_MODE::D3D11_FILL_SOLID: fillIndex = 1; break;
        }
        ImGui::Text("FillMode:");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##FillMode", fillModes[fillIndex]))
        {
            for (int n = 0; n < IM_ARRAYSIZE(fillModes); n++)
            {
                bool is_selected = (fillIndex == n);
                if (ImGui::Selectable(fillModes[n], is_selected))
                {
                    if (n == 0)
                        fillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
                    else
                        fillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
                    changed = true;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        return changed;
    }

    bool ImGui_CullMode(D3D11_CULL_MODE &cullMode)
    {
        const char* cullModes[] = { "Front", "Back", "None" };
        bool changed = false;
        int cullIndex = 0;
        switch (cullMode)
        {
        case D3D11_CULL_MODE::D3D11_CULL_FRONT: cullIndex = 0; break;
        case D3D11_CULL_MODE::D3D11_CULL_BACK: cullIndex = 1; break;
        case D3D11_CULL_MODE::D3D11_CULL_NONE: cullIndex = 2; break;
        }
        ImGui::Text("CullMode:");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##CullMode", cullModes[cullIndex]))
        {
            for (int n = 0; n < IM_ARRAYSIZE(cullModes); n++)
            {
                bool is_selected = (cullIndex == n);
                if (ImGui::Selectable(cullModes[n], is_selected))
                {
                    switch (n)
                    {
                    case 0: cullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT; changed = true;  break;
                    case 1: cullMode = D3D11_CULL_MODE::D3D11_CULL_BACK; changed = true; break;
                    case 2: cullMode = D3D11_CULL_MODE::D3D11_CULL_NONE; changed = true; break;
                    }
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        return changed;
    }

    void ImGui_Color(const char* pLabel, uint32_t index, std::function<FRGBColor()>getFunc, std::function<void(FRGBColor v)>setFunc)
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
