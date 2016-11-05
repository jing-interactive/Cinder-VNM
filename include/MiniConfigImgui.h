#pragma once

#include "MiniConfig.h"
#include "Cinder-ImGui/include/CinderImGui.h"

#include <cinder/Utilities.h>
#include <cinder/app/App.h>

using namespace ci;
using namespace ci::app;
using namespace std;

namespace
{
    void addImguiParam(const char* label, int& v)
    {
        ui::DragInt(label, &v);
    }

    void addImguiParam(const char* label, float& v)
    {
        ui::SliderFloat(label, &v, 0.0f, 1.0f);
    }

    void addImguiParam(const char* label, float& v, float min, float max)
    {
        ui::SliderFloat(label, &v, min, max);
    }

    void addImguiParam(const char* label, bool& v)
    {
        ui::Checkbox(label, &v);
    }

    void addImguiParam(const char* label, string& v)
    {
        ui::InputText(label, &v);
    }

    void addImguiParam(const char* label, quat& v)
    {
        ui::InputFloat4(label, &v.x);
    }

    void addImguiParam(const char* label, vec3& v)
    {
        ui::InputFloat3(label, &v.x);
    }

    void addImguiParam(const char* label, Color& v)
    {
        ui::ColorEdit3(label, &v.r);
    }

    void addImguiParam(const char* label, ColorA& v)
    {
        ui::ColorEdit4(label, &v.r, true);
    }

    void drawImgui()
    {
        if (ui::Button("Save Config"))
        {
            writeConfig();
        }

        if (ui::Button("Save Image"))
        {
            takeScreenShot();
        }
#define GROUP_DEF(grp)                     ui::Separator();
#define ITEM_DEF(type, var, default)        addImguiParam(#var, var);
#define ITEM_DEF_MINMAX(type, var, default, Min, Max)  addImguiParam(#var, var, Min, Max);
#include "item.def"
#undef ITEM_DEF_MINMAX
#undef ITEM_DEF
#undef GROUP_DEF

        if (ui::Button("Quit"))
        {
            App::get()->quit();
        }
    }
}

void createConfigImgui()
{
    getWindow()->getSignalDraw().connect(drawImgui);
}
