//**********************************************************************
// Copyright Patrick Sweeney 2021-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <map>
#include <any>
#include <string>
#include <stdio.h>
#include <vector>
#include "..\External\imgui\imgui.h"

module Parsers.JSon.JSonTree;
import Parsers.JSon.IJSonParser;

namespace Caustic
{
    void JSONTree(IJSonObj* pObj, const char* pTreeName)
    {
        switch (pObj->GetType())
        {
        case Caustic::CJSonType::Array:
            {
                std::vector<CRefObj<IJSonObj>> data = std::any_cast<std::vector<CRefObj<IJSonObj>>>(pObj->GetValue());
                for (size_t i = 0; i < data.size(); i++)
                {
                    char buf[512];
                    sprintf_s(buf, "[%d] ", (int)i);
                    JSONTree(data[i], buf);
                }
            }
            break;
        case Caustic::CJSonType::Bool:
            ImGui::Text((std::any_cast<bool>(pObj->GetValue())) ? "True" : "False");
            break;
        case Caustic::CJSonType::Null:
            ImGui::Text("Null");
            break;
        case Caustic::CJSonType::Integer:
            {
                char buf[256];
                sprintf_s(buf, "%d", std::any_cast<int>(pObj->GetValue()));
                ImGui::Text(buf);
            }
            break;
        case Caustic::CJSonType::Number:
            {
                char buf[256];
                sprintf_s(buf, "%f", std::any_cast<float>(pObj->GetValue()));
                ImGui::Text(buf);
            }
            break;
        case Caustic::CJSonType::Object:
            if (ImGui::TreeNode((pObj->GetName().empty()) ? pTreeName : pObj->GetName().c_str()))
            {
                std::map<std::string, CRefObj<IJSonObj>> data = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>>(pObj->GetValue());
                for (auto it : data)
                {
                    JSONTree(it.second, it.first.c_str());
                }
                ImGui::TreePop();
            }
            break;
        case Caustic::CJSonType::String:
            {
                std::string valName = std::any_cast<std::string>(pObj->GetValue());
                std::string propName = pObj->GetName();
                ImGui::Text(((propName.empty() ? pTreeName : propName) + " = \"" + valName + "\"").c_str());
            }
            break;
        }
    }
}