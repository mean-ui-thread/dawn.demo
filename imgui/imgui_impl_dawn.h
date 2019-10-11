#ifndef IMGUI_IMPL_DAWN_H
#define IMGUI_IMPL_DAWN_H

#include "imgui.h"
#include <dawn/dawncpp.h>

IMGUI_IMPL_API bool ImGui_ImplDawn_Init(dawn::Device& device, dawn::Queue& queue);
IMGUI_IMPL_API void ImGui_ImplDawn_Shutdown();
IMGUI_IMPL_API void ImGui_ImplDawn_NewFrame();
IMGUI_IMPL_API void ImGui_ImplDawn_RenderDrawData(ImDrawData* draw_data, dawn::Device& device, dawn::RenderPassEncoder& pass);

#endif // IMGUI_IMPL_DAWN_H
