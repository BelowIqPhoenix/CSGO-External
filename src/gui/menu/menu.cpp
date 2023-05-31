#include <Windows.h>

#include "menu.h"

#include "imgui/imgui.h"

void menu::draw(const Memory& mem)
{
	ImGui::PushStyleColor(ImGuiCol_Text, math::RGBAToFloat4(globals::text.data()));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, math::RGBAToFloat4(globals::title_bar.data()));
	ImGui::PushStyleColor(ImGuiCol_Border, math::RGBAToFloat4(globals::border.data()));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, math::RGBAToFloat4(globals::child_border.data()));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, math::RGBAToFloat4(globals::frame_bg.data()));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, math::RGBAToFloat4(globals::frame_bg_hovered.data()));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, math::RGBAToFloat4(globals::frame_bg_active.data()));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

	ImGui::SetNextWindowSize({ 500, 320 });
	ImGui::PushStyleColor(ImGuiCol_Text, { 1.f, 1.f, 1.f, 1.f });
	ImGui::Begin("CSGO External", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
	ImGui::PopStyleColor();
	
	ImGui::PushFont(globals::product_sans);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 25, 25 });

	static int current_tab = 0;

	if (ImGui::BeginChild(
		1,
		{ ImGui::GetContentRegionAvail().x * 0.28f, ImGui::GetContentRegionAvail().y },
		true)) {
		constexpr auto button_height = 25;
		constexpr auto button_width = 100;
		if (ImGui::Button("Aimbot", { button_width, button_height })) { current_tab = 0; }
		if (ImGui::Button("Visuals", { button_width, button_height })) { current_tab = 1; }
		if (ImGui::Button("Misc", { button_width, button_height })) { current_tab = 2; }
		if (ImGui::Button("Settings", { button_width, button_height })) { current_tab = 3; }
		if (ImGui::Button("Unload", { button_width, button_height })) { unload(mem); }

		ImGui::EndChild();
	}
	ImGui::PopStyleVar(1);

	ImGui::SameLine();

	if (ImGui::BeginChild(
		2,
		ImGui::GetContentRegionAvail(),
		true)) {

		switch (current_tab) {
		case 0:
			aimbot_tab();
			break;

		case 1:
			visuals_tab();
			break;

		case 2:
			misc_tab();
			break;

		case 3:
			settings_tab();
			break;
		}

		ImGui::EndChild();
	}

	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void menu::aimbot_tab()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 20, 20 });
	ImGui::Checkbox("Aimbot", &globals::b_aimbot);
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
	ImGui::SliderFloat("##smoothing", &globals::f_smoothness, 0.f, 10.f, "Smoothness: %.01f", ImGuiColorEditFlags_NoLabel);
	ImGui::PopItemWidth();

	ImGui::Checkbox("Draw Fov", &globals::b_draw_fov);
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
	ImGui::InputFloat("##fov", &globals::f_fov, 0.f, 0.f, "Fov: %.01f", ImGuiColorEditFlags_NoLabel);
	ImGui::PopItemWidth();

	ImGui::Text("Aim Part");
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
	ImGui::Combo("##aim_part", &globals::aim_part, globals::aim_parts, IM_ARRAYSIZE(globals::aim_parts));
	ImGui::PopItemWidth();

	ImGui::Text("Aim Key");
	ImGui::SameLine();
	ImGui::Hotkey(&globals::aim_key, { 130, 30 });
	ImGui::PopStyleVar();
}

void menu::visuals_tab()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 7, 7 });
	ImGui::Checkbox("Esp", &globals::b_esp);
	ImGui::Checkbox("Box Esp", &globals::box_esp.first);
	ImGui::SameLine();
	ImGui::ColorEdit4("Box Color##box_esp", globals::box_esp.second.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Checkbox("Glow Esp", &globals::glow_esp.first);
	ImGui::SameLine();
	ImGui::ColorEdit4("Glow Color##glow_esp", globals::glow_esp.second.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Checkbox("Health Esp", &globals::b_health_esp);
	ImGui::Checkbox("Armor Esp", &globals::b_armor_esp);

	ImGui::Checkbox("Distance Esp", &globals::distance_esp.first);
	ImGui::SameLine();
	ImGui::ColorEdit4("Distance Color##distance_esp", globals::distance_esp.second.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Checkbox("Tracers", &globals::tracer.first);
	ImGui::SameLine();
	ImGui::ColorEdit4("Tracer Color##tracer", globals::tracer.second.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	ImGui::PopStyleVar();
}

void menu::misc_tab()
{
	ImGui::Checkbox("Bhop", &globals::b_bhop);
}

void menu::settings_tab()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 7, 7 });
	ImGui::Text("Text Color");
	ImGui::SameLine();
	ImGui::ColorEdit4("Text Color", globals::text.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Text("Title Bar Color");
	ImGui::SameLine();
	ImGui::ColorEdit4("Title bar Color", globals::title_bar.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Text("Border Color");
	ImGui::SameLine();
	ImGui::ColorEdit4("Border Color", globals::border.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Text("Child Border Color");
	ImGui::SameLine();
	ImGui::ColorEdit4("Child Border Color", globals::child_border.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Text("Frame BG Color");
	ImGui::SameLine();
	ImGui::ColorEdit4("Frame BG Color", globals::frame_bg.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Text("Frame BG a Color");
	ImGui::SameLine();
	ImGui::ColorEdit4("Frame BG a Color", globals::frame_bg_active.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	ImGui::Text("Frame BG h Color");
	ImGui::SameLine();
	ImGui::ColorEdit4("Frame BG h Color", globals::frame_bg_hovered.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	ImGui::PopStyleVar();
}

void menu::unload(const Memory& mem)
{
	gui::isRunning = false;

	exit(0);
}