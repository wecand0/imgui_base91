cmake_minimum_required(VERSION 3.23)
project(base91-cpp)

set(BASE91_CPP_LIBRARY_STATIC ON)

include_directories(.)
set(SRC base91.cpp base91.h)

if(${BASE91_CPP_LIBRARY_STATIC})
    add_library(${PROJECT_NAME} STATIC ${SRC})
else()
    add_library(${PROJECT_NAME} SHARED ${SRC})
endif()


