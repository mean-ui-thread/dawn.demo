// Copyright 2019 Mathieu-Andre Chiasson
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef IMGUI_IMPL_DAWN_H
#define IMGUI_IMPL_DAWN_H

#include "imgui.h"
#include <dawn/dawncpp.h>

IMGUI_IMPL_API bool ImGui_ImplDawn_Init(dawn::Device& device, dawn::Queue& queue);
IMGUI_IMPL_API void ImGui_ImplDawn_Shutdown();
IMGUI_IMPL_API void ImGui_ImplDawn_NewFrame();
IMGUI_IMPL_API void ImGui_ImplDawn_RenderDrawData(ImDrawData* draw_data, dawn::Device& device, dawn::RenderPassEncoder& pass);

#endif // IMGUI_IMPL_DAWN_H
