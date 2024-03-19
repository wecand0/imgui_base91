#ifndef IMGUI_BASE91_WINDOW_H
#define IMGUI_BASE91_WINDOW_H

#include <imgui_common.h>

#include "../thirdparty/base91/cpp/lib/base91.h"
#include "logger.h"

class Window {
public:
    Window();
    void draw();

public:
    static void initImGuiSets();
    static void initFont();

private:
    bool isEncoding_{};
    bool isDecoding_{};

private:
    Base91 base91;
    Logger logger_;

    ImGui::FileBrowser fileDialog_;
};


#endif //IMGUI_BASE91_WINDOW_H
