project(imgui VERSION 1.90.4)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)

set(_imgui_Sources
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_widgets.cpp
        imgui_tables.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h

        misc/cpp/imgui_stdlib.h
        misc/cpp/imgui_stdlib.cpp

        backends/imgui_impl_glfw.h
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
        backends/imgui_impl_opengl3.h

        #emscripten
        examples/libs/emscripten/emscripten_mainloop_stub.h

        #filebrowser
        imgui-filebrowser/imfilebrowser.h
)

add_library(imgui STATIC ${_imgui_Sources})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/stb)
target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw)