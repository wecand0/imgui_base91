#ifndef IMGUI_BASE91_WINDOW_H
#define IMGUI_BASE91_WINDOW_H

#include <imgui_common.h>

#include <base91.h>
#include "logger.h"

class Window {
public:
    Window();
    void draw();

public:
    static void initImGuiSets();
    static void initFont();

private:
    void readFile(const std::string &_path);
    void writeFile(const std::string &_path);

private:
    bool isEncoding_{};
    bool isDecoding_{};

private:
    Base91 base91;

    std::shared_ptr<spdlog::async_logger> logger_;
    ImGui::FileBrowser fileDialog_;
};


#endif //IMGUI_BASE91_WINDOW_H
