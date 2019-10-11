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

hunter_config(SPIRV-Headers
    VERSION 1.5.1
    URL https://github.com/KhronosGroup/SPIRV-Headers/archive/1.5.1.tar.gz
    SHA1 77018bfe6cb1eceaf824e401dbd206660a25bf66
    CMAKE_ARGS SPIRV_HEADERS_SKIP_EXAMPLES=ON
)

hunter_config(SPIRV-Tools
    VERSION v2019.4-p0
    URL https://github.com/mchiasson/SPIRV-Tools/archive/v2019.4-p0.tar.gz
    SHA1 fde8521fd5c99166de934d7d7d65800940dfba93
    CMAKE_ARGS BUILD_TESTING=OFF SPIRV_SKIP_EXECUTABLES=ON SPIRV_SKIP_TESTS=ON
)

hunter_config(mattc_glslang
    VERSION v7.12.3352-p1
    URL https://github.com/mchiasson/glslang/archive/v7.12.3352-p1.tar.gz
    SHA1 d25c10c725f71cc6b7eb5fa2140b9b4d7c06f74c
    CMAKE_ARGS BUILD_TESTING=OFF ENABLE_GLSLANG_BINARIES=OFF ENABLE_HLSL=ON
)

hunter_config(mattc_SPIRV-Cross
    VERSION v2019.09.06-p2
    URL https://github.com/mchiasson/SPIRV-Cross/archive/v2019.09.06-p2.tar.gz
    SHA1 fd82bceef9be48903b604166346984102a4eb1c0
    CMAKE_ARGS BUILD_TESTING=OFF SPIRV_CROSS_CLI=OFF SPIRV_CROSS_ENABLE_TESTS=OFF SPIRV_CROSS_FORCE_PIC=ON
)

hunter_config(shaderc
    VERSION v2019.0-p5
    URL https://github.com/mchiasson/shaderc/archive/v2019.0-p5.tar.gz
    SHA1 e63f63e89bce234767e671f6d14ca94a1ecaa0d9
    CMAKE_ARGS BUILD_TESTING=OFF SHADERC_ENABLE_SPVC=ON SHADERC_SKIP_TESTS=ON
)

hunter_config(dawn
    VERSION v1.0.0-p3
    URL https://github.com/mchiasson/dawn.cmake/releases/download/v1.0.0-p3/source.tar.gz
    SHA1 272cb157688474f37fb8ea5d399cce42086be6fc
    CMAKE_ARGS DAWN_BUILD_SAMPLES=OFF
)
