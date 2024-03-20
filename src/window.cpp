#include "window.h"

#include <fstream>

Window::Window() {
    try {
        Logger logger;
        logger.init();
        logger_ = logger.getInstance();
    } catch (spdlog::spdlog_ex &ex) {
        printf("%s", "failed to init spdlog");
    }

    fileDialog_.SetTitle("Choose the file");
}

static void helpMarker(const std::string &desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void Window::draw() {
    static bool use_work_area = true;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();

    auto mainWinSize = use_work_area ? viewport->WorkSize : viewport->Size;
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize({mainWinSize.x, mainWinSize.y}); // /2

    static ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
    {
        ImGui::Begin("TopWindow", {}, flags);
        float w = ImGui::GetWindowSize().x / 3;
        float h = ImGui::GetWindowSize().y;

        ImGui::BeginChild("TopFileWindowChild", ImVec2(w, h), true);
        ImGui::SeparatorText("File");

        ImGui::Text("Choose file");
        ImGui::SameLine();
        helpMarker("The file will be saved in the same directory with the name that you have chosen.");


        if (!isEncoding_ && !isDecoding_) {
            ImGui::BeginDisabled();
            if (ImGui::Button("Choose encoding/decoding")) {}
            ImGui::EndDisabled();
        }

        if (isEncoding_) {
            if (ImGui::Button("Open file for encoding"))
                fileDialog_.Open();
        }
        if (isDecoding_) {
            if (ImGui::Button("Open file for decoding"))
                fileDialog_.Open();
        }
        static std::string outputFileName;
        ImGui::InputText("Output file name", &outputFileName, ImGuiInputTextFlags_CharsNoBlank);


        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::InvisibleButton("vsplitterWindowChild", ImVec2(2.0f, h));
        ImGui::SameLine();

        ImGui::BeginChild("TopChooseModeWindowChild11", ImVec2(w, h), true);
        ImGui::SeparatorText("Choose mode");

        if (ImGui::RadioButton("Encode", isEncoding_)) {
            isEncoding_ = true;
            isDecoding_ = false;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Decode", isDecoding_)) {
            isEncoding_ = false;
            isDecoding_ = true;
        }

        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::InvisibleButton("vsplitterWindowChild", ImVec2(2.0f, h));
        ImGui::SameLine();


        ImGui::SameLine();
        ImGui::InvisibleButton("vsplitterWindowChild", ImVec2(2.0f, h));
        ImGui::SameLine();

        ImGui::BeginChild("TopClipboardWindowChild", ImVec2(0, h), true);
        ImGui::SeparatorText("Clipboard/Input from keyboard");

        static std::string inputText;
        static std::string outputText;
        ImGui::InputText("Input", &inputText);

        if (!inputText.empty()) {
            if (isEncoding_) {
                try {
                    base91.encode(inputText);
                } catch (const std::exception &e) {
                    spdlog::error(e.what());
                }
            }
            if (isDecoding_) {
                try {
                    base91.decode(inputText);
                } catch (const std::exception &e) {
                    spdlog::error(e.what());
                }
            }
            outputText = base91.getData();
        } else {
            outputText.clear();
        }
        ImGui::InputText("Output", &outputText, ImGuiInputTextFlags_ReadOnly);

        ImGui::EndChild();
        ImGui::End();

        fileDialog_.Display();

        //TODO make async
        if (fileDialog_.HasSelected()) {
            readFile(fileDialog_.GetSelected().string());
            writeFile(outputFileName);
            fileDialog_.ClearSelected();
        }
    }
}

void Window::initImGuiSets() {
    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.57f, 0.57f, 0.57f, 0.70f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    colors[ImGuiCol_Button] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    colors[ImGuiCol_Header] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    colors[ImGuiCol_Separator] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.36f, 0.36f, 0.36f, 0.54f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


    colors[ImGuiCol_PopupBg] = ImVec4({0.100f, 0.100f, 0.100f, 0.860f});
    colors[ImGuiCol_HeaderActive] = ImVec4({0.730f, 0.130f, 0.370f, 1.000f});
    colors[ImGuiCol_HeaderHovered] = ImVec4({0.450f, 0.190f, 0.300f, 0.430f});
    colors[ImGuiCol_Header] = ImVec4({0.730f, 0.130f, 0.370f, 0.580f});
}

void Window::initFont() {
    ImGuiIO &io = ImGui::GetIO();

    IM_DELETE(io.Fonts);
    io.Fonts = IM_NEW(ImFontAtlas);

    ImFontConfig config;
    config.OversampleH = 1;
    config.OversampleV = 1;
    config.PixelSnapH = false;
    config.RasterizerMultiply = 1.1f;

    //todo merge with binary file
    std::string fontPath = "data/JetBrainsMono-Regular.ttf";
    FILE *fontFile = fopen(fontPath.c_str(), "r");
    if (fontFile) {
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f, &config, io.Fonts->GetGlyphRangesCyrillic());
        fclose(fontFile);
    }
    io.Fonts->Build();
}

void Window::readFile(const std::string &_path) {
    std::ifstream ifstream(fileDialog_.GetSelected().string(), std::ios::binary);
    if (!ifstream.is_open()) {
        spdlog::error("Unable to open file");
        return;
    }
    std::string fileContents((std::istreambuf_iterator<char>(ifstream)),
                             std::istreambuf_iterator<char>());
    ifstream.close();
    if (isEncoding_) {
        try {
            base91.encode(fileContents);
        } catch (const std::exception &e) {
            spdlog::error(e.what());
        }
    }
    if (isDecoding_) {
        try {
            base91.decode(fileContents);
        } catch (const std::exception &e) {
            spdlog::error(e.what());
        }
    }

}

void Window::writeFile(const std::string &_outputFileName) {
    std::string outputFilePath = fileDialog_.GetPwd();
    outputFilePath += std::filesystem::path::preferred_separator;
    outputFilePath += _outputFileName;

    std::ofstream fileOutput(outputFilePath, std::ios::binary);
    if (!fileOutput.is_open()) {
        spdlog::error("{} {}", "Unable to open file: ");
    }

    fileOutput.write(base91.getData().c_str(), (std::streamsize) base91.getData().size());
    fileOutput.close();
    spdlog::info("{}", "File written successfully.");

}
