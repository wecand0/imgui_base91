cmake_minimum_required(VERSION 3.23)
project(imgui_base91)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/misconceptions/cmake-modules)

find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)

add_subdirectory(thirdparty/base91/cpp)

set(SRC src/main.cpp)

add_executable(imgui_base91 ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC GL glfw imgui)