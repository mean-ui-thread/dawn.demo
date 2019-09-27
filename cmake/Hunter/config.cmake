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

hunter_config(glfw VERSION 3.3.0-f9923e9-p0 CMAKE_ARGS OpenGL_GL_PREFERENCE=GLVND)

hunter_config(SPIRV-Headers
    VERSION 1.5.1
    URL https://github.com/KhronosGroup/SPIRV-Headers/archive/1.5.1.tar.gz
    SHA1 77018bfe6cb1eceaf824e401dbd206660a25bf66
)

hunter_config(SPIRV-Tools
    VERSION v2019.4-p0
    URL https://github.com/mchiasson/SPIRV-Tools/archive/v2019.4-p0.tar.gz
    SHA1 fde8521fd5c99166de934d7d7d65800940dfba93
)

hunter_config(glslang
    VERSION v7.12.3352-p0
    URL https://github.com/mchiasson/glslang/archive/v7.12.3352-p0.tar.gz
    SHA1 8589282141eb8edb37d38930a8d6420910b6a0cf
    CMAKE_ARGS ENABLE_HLSL=ON
)

hunter_config(SPIRV-Cross
    VERSION v2019.09.06-p0
    URL https://github.com/mchiasson/SPIRV-Cross/archive/v2019.09.06-p0.tar.gz
    SHA1 f6acf88cab07bb45be74c7af1174562a69019f40
)

hunter_config(shaderc
    VERSION v2019.0-p1
    URL https://github.com/mchiasson/shaderc/archive/v2019.0-p1.tar.gz
    SHA1 84918d3d0cf1868ad55dc6a6a27b684a380faa87
    CMAKE_ARGS SHADERC_ENABLE_SPVC=ON
)

hunter_config(dawn
    VERSION v1.0.0-p1
    URL https://github.com/mchiasson/dawn.cmake/releases/download/v1.0.0-p1/source.tar.gz
    SHA1 0955a3cf2c0d7b0a9d55a517a241847de095e5a3
    CMAKE_ARGS
        DAWN_BUILD_SAMPLES=OFF
)
