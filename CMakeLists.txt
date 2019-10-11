# Copyright (c) 2019 Mathieu-André Chiasson
# All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.13)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.216.tar.gz"
    SHA1 "a492a1879bee60c29f7af17ef56b45f0d7766241"
    LOCAL
)

project(dawn.demo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

## https://docs.hunter.sh/en/latest/packages/pkg/glm.html
hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)

hunter_add_package(dawn)
find_package(dawn CONFIG REQUIRED)

add_library(SampleUtils OBJECT
    utils/SampleUtils.cpp
    utils/SampleUtils.h
)

target_link_libraries(SampleUtils PUBLIC
    dawn::dawn_bindings
    dawn::dawn_utils
    dawn::dawn
)

add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp

    # Dawn Backend Implementation file
    imgui/imgui_impl_dawn.cpp
    imgui/imgui_impl_dawn.h
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_glfw.h
)

target_link_libraries(imgui PUBLIC SampleUtils)

macro(dawn_sample target_name)
    add_executable(${target_name} ${target_name}.cpp)
    target_link_libraries(${target_name} PRIVATE SampleUtils)
endmacro()

dawn_sample(Animometer)
dawn_sample(CHelloTriangle)
dawn_sample(ComputeBoids)
dawn_sample(CppHelloTriangle)
dawn_sample(CubeReflection)
dawn_sample(ImguiDemo)

target_link_libraries(ComputeBoids PRIVATE glm)
target_link_libraries(CubeReflection PRIVATE glm)
target_link_libraries(ImguiDemo PRIVATE imgui)
