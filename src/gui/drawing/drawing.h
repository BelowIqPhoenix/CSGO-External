#pragma once
#include <dwmapi.h>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace drawing
{
	void text(ImVec2 v2_pos, std::string str_text, ImVec4 v4_col, bool b_outline);
	void line(ImVec2 v2_pos, ImVec2 v2_size, ImVec4 v4_col, float f_thickness);
	void line(RECT rect, ImVec4 v4_col, float f_thickness);
	void rect(ImVec2 v2_pos, ImVec2 v2_size, ImVec4 v4_col, float f_thickness, bool b_outline, bool b_inline);
	void rect(RECT rect, ImVec4 v4_col, float f_thickness, bool b_outline, bool b_inline);
	void filled_rect(ImVec2 v2_pos, ImVec2 v2_size, ImVec4 v4_col, bool b_outline);
	void filled_rect(RECT rect, ImVec4 v4_col, bool b_outline);
	void circle(ImVec2 v2_pos, float f_radius, ImVec4 v4_col, float f_thickness, bool b_outline, bool b_inline);
}