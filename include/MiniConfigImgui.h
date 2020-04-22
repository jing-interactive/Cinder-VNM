#pragma once

#include "MiniConfig.h"
#include <cinder/CinderImGui.h>

#include <cinder/Utilities.h>
#include <cinder/app/App.h>

using namespace ci;
using namespace ci::app;
using namespace std;

namespace vnm
{
    bool addImguiParam(const char* label, int& v)
    {
        return ImGui::DragInt(label, &v);
    }

    bool addImguiParam(const char* label, float& v)
    {
        return ImGui::DragFloat(label, &v);
    }

    bool addImguiParam(const char* label, float& v, float min, float max)
    {
        return ImGui::SliderFloat(label, &v, min, max);
    }

    bool addImguiParam(const char* label, int& v, int min, int max)
    {
        return ImGui::SliderInt(label, &v, min, max);
    }

    bool addImguiParam(const char* label, bool& v)
    {
        return ImGui::Checkbox(label, &v);
    }

    bool addImguiParam(const char* label, string& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ImGui::InputText(label, &v, flags);
    }

    bool addImguiParam(const char* label, quat& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ImGui::InputFloat4(label, &v.x, -1, flags);
    }

    bool addImguiParam(const char* label, vec2& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ImGui::InputFloat2(label, &v.x, -1, flags);
    }

    bool addImguiParam(const char* label, ivec2& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ImGui::InputInt2(label, &v.x, flags);
    }

    bool addImguiParam(const char* label, vec3& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ImGui::InputFloat3(label, &v.x, -1, flags);
    }

    bool addImguiParam(const char* label, vec4& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ImGui::InputFloat4(label, &v.x, -1, flags);
    }

    bool addImguiParam(const char* label, Color& v)
    {
        return ImGui::ColorEdit3(label, &v.r);
    }

    bool addImguiParam(const char* label, ColorA& v)
    {
        return ImGui::ColorEdit4(label, &v.r, true);
    }

    void drawMinicofigImgui(bool createNewWindow = false)
    {
        if (createNewWindow)
            ImGui::Begin("Config");

        if (ImGui::Button("Save Config"))
        {
            writeConfig();
        }
        if (ImGui::Button("Remotery Profiler"))
        {
            launchWebBrowser(Url(getAssetPath("vis/index.html").string(), true));
        }

        if (ImGui::Button("Screen-shot"))
        {
            takeScreenShot();
        }
        if (ImGui::Button("Quit"))
        {
            App::get()->quit();
        }
        
#if !defined(NDEBUG) && !defined(IMGUI_DISABLE_DEMO_WINDOWS)
        static bool isDemoWindowOpened = false;
        if (ImGui::Button("ShowDemoWindow"))
            isDemoWindowOpened = !isDemoWindowOpened;
        if (isDemoWindowOpened)
            ImGui::ShowDemoWindow(&isDemoWindowOpened);
#endif  
        
#define GROUP_DEF(grp)                      } if (ImGui::CollapsingHeader(#grp, ImGuiTreeNodeFlags_DefaultOpen)) {
#define ITEM_DEF(type, var, default)        addImguiParam(#var, var);
#define ITEM_DEF_MINMAX(type, var, default, Min, Max)  addImguiParam(#var, var, Min, Max);
        if (true) {
#include "item.def"
        }
#undef ITEM_DEF_MINMAX
#undef ITEM_DEF
#undef GROUP_DEF

        if (createNewWindow)
            ImGui::End();
    }
}

void createConfigImgui(WindowRef window = getWindow(), bool autoDraw = true)
{
    auto optiion = ImGui::Options().window(window);
    ImGui::Initialize(optiion);
    if (autoDraw)
        App::get()->getSignalUpdate().connect([] {vnm::drawMinicofigImgui(true); });
}
