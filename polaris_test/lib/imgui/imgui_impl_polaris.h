#pragma once
#ifndef IMGUI_DISABLE
#include "imgui.h"
#include <polaris/polaris.hpp>

struct ImGui_ImplPolaris_InitInfo
{
	u8 FramesInFlight = 2;
	u8 MSAASamples = 1;
};

IMGUI_IMPL_API bool ImGui_ImplPolaris_Init(ImGui_ImplPolaris_InitInfo* info);
IMGUI_IMPL_API void ImGui_ImplPolaris_Shutdown();
IMGUI_IMPL_API void ImGui_ImplPolaris_NewFrame();
IMGUI_IMPL_API void ImGui_ImplPolaris_RenderDrawData(ImDrawData* draw_data, pl::CommandBuffer& command_buffer);

#endif
