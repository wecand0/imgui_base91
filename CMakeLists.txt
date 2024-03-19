cmake_minimum_required(VERSION 3.23)
project(imgui_base91)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/misconceptions/cmake-modules)

find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(fmt REQUIRED)

add_subdirectory(thirdparty/base91/cpp/lib)
add_subdirectory(thirdparty/spdlog)

set(SRC
        src/main.cpp
        src/window.h
        src/window.cpp
        src/logger.h
        src/logger.cpp
)

include_directories(include)

add_executable(imgui_base91 ${SRC} include/imgui_common.h)

target_link_libraries(${PROJECT_NAME}
        GL
        glfw
        spdlog
        imgui
        base91-cpp
        fmt::fmt
)

#copy all files from data folder(glyphs, sprites, style.json and etc)
function(copy_recursive SOURCE_PATH DESTINATION_PATH REGEX)
    file(GLOB_RECURSE
            FILES
            ${SOURCE_PATH}
            "${SOURCE_PATH}/${REGEX}")

    foreach (file ${FILES})
        # To recreate the same folder structure we first need to read the base folder
        file(RELATIVE_PATH RELATIVE_FILE_PATH ${SOURCE_PATH} ${file})
        get_filename_component(FOLDER ${RELATIVE_FILE_PATH} DIRECTORY ${SOURCE_PATH})
        file(MAKE_DIRECTORY ${DESTINATION_PATH}/${FOLDER})
        #message(STATUS "COPY ${file} -> ${DESTINATION_PATH}/${RELATIVE_FILE_PATH}")
        configure_file(${file} "${DESTINATION_PATH}/${RELATIVE_FILE_PATH}" COPYONLY)
    endforeach ()
endfunction()

copy_recursive(${CMAKE_SOURCE_DIR}/data/ ${CMAKE_CURRENT_BINARY_DIR}/data/ *)