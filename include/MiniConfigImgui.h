#pragma once

#include "MiniConfig.h"
#include "Cinder-ImGui/include/CinderImGui.h"

#include <cinder/Utilities.h>
#include <cinder/app/App.h>

using namespace ci;
using namespace ci::app;
using namespace std;

namespace vnm
{
    bool addImguiParam(const char* label, int& v)
    {
        return ui::DragInt(label, &v);
    }

    bool addImguiParam(const char* label, float& v)
    {
        return ui::SliderFloat(label, &v, 0.0f, 1.0f);
    }

    bool addImguiParam(const char* label, float& v, float min, float max)
    {
        return ui::SliderFloat(label, &v, min, max);
    }

    bool addImguiParam(const char* label, bool& v)
    {
        return ui::Checkbox(label, &v);
    }

    bool addImguiParam(const char* label, string& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ui::InputText(label, &v, flags);
    }

    bool addImguiParam(const char* label, quat& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ui::InputFloat4(label, &v.x, -1, flags);
    }

    bool addImguiParam(const char* label, vec3& v)
    {
        ImGuiInputTextFlags flags = 0;
        if (label[0] == '_') flags = ImGuiInputTextFlags_ReadOnly;
        return ui::InputFloat3(label, &v.x, -1, flags);
    }

    bool addImguiParam(const char* label, Color& v)
    {
        return ui::ColorEdit3(label, &v.r);
    }

    bool addImguiParam(const char* label, ColorA& v)
    {
        return ui::ColorEdit4(label, &v.r, true);
    }

    void drawImgui()
    {
        ui::ScopedWindow window("Config");
        if (ui::Button("Save Config"))
        {
            writeConfig();
        }

        if (ui::Button("Screen-shot"))
        {
            takeScreenShot();
        }
        if (ui::Button("Quit"))
        {
            App::get()->quit();
        }
        
#ifdef _DEBUG
        static bool isTestWindowOpened = false;
        if (ui::Button("ShowTestWindow"))
            isTestWindowOpened = !isTestWindowOpened;
        if (isTestWindowOpened)
            ui::ShowTestWindow(&isTestWindowOpened);
#endif  
        
#define GROUP_DEF(grp)                      } if (ui::CollapsingHeader(#grp, ImGuiTreeNodeFlags_DefaultOpen)) {
#define ITEM_DEF(type, var, default)        addImguiParam(#var, var);
#define ITEM_DEF_MINMAX(type, var, default, Min, Max)  addImguiParam(#var, var, Min, Max);
        if (true) {
#include "item.def"
        }
#undef ITEM_DEF_MINMAX
#undef ITEM_DEF
#undef GROUP_DEF
    }
}

void createConfigImgui()
{
    ui::initialize();
    App::get()->getSignalUpdate().connect(vnm::drawImgui);
}
