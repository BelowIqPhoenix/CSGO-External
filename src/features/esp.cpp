#include "features.h"

void features::esp(const Memory& mem)
{
	Entity localplayer = globals::get_local_player(mem);
	ViewMatrix view_matrix = mem.read<ViewMatrix>(globals::client + OFFSET_VIEW_MATRIX);

	std::vector<Entity> ents = globals::get_entity_list(mem);

	for (const Entity ent : ents)
	{
		if (ent.team == localplayer.team)
			continue;

		if (ent.dead)
			continue;

		Vector3 head_pos = ent.head_pos;
		Vector3 feet_pos = ent.feet_pos;

		Vector2 top;
		Vector2 bottom;
		if (math::world_to_screen(head_pos + Vector3{ 0.f, 0.f, 11.0f }, top, view_matrix) && math::world_to_screen(feet_pos, bottom, view_matrix))
		{
			const float h = bottom.y - top.y;
			const float w = h * 0.30;

			RECT box = { bottom.x - w, top.y, bottom.x + w, bottom.y };

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(1920, 1080));
			ImGui::Begin("ESP", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_None | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing);

			if (globals::box_esp.first)
				drawing::rect(box, math::RGBAToFloat4(globals::box_esp.second.data()), 2.0f, true, true);

			if (globals::glow_esp.first)
			{
				uintptr_t glow_object_manager = mem.read<uintptr_t>(globals::client + OFFSET_GLOW_OBJECT_MANAGER);

				mem.write<ImVec4>(glow_object_manager + (ent.glow_index * 0x38) + 0x8, math::RGBAToFloat4(globals::glow_esp.second.data()));
				mem.write<bool>(glow_object_manager + (ent.glow_index * 0x38) + 0x28, true);
				mem.write<bool>(glow_object_manager + (ent.glow_index * 0x38) + 0x29, false);
			}

			if (globals::b_health_esp)
			{
				float health_frac = ent.health * 0.01f;

				ImVec4 health_col = math::RGBAToFloat4((1.f - health_frac) * 255, 255 * health_frac, 0, 255);
				RECT health_box = { (bottom.x - w) - 7, bottom.y - (h * health_frac), (bottom.x - w) - 4, bottom.y };
				drawing::filled_rect(health_box, health_col, true);
			}

			if (globals::b_armor_esp)
			{
				float health_frac = ent.armor * 0.01f;

				ImVec4 health_col = { 0.f, 0.f, 1.f, 1.f };
				RECT health_box = { (bottom.x - w) - 14, bottom.y - (h * health_frac), (bottom.x - w) - 11, bottom.y };
				drawing::filled_rect(health_box, health_col, true);
			}

			if (globals::distance_esp.first)
			{
				const auto& local_pos = localplayer.feet_pos;
				const auto& ent_pos = ent.feet_pos;

				float distance = math::get_distance(local_pos, ent_pos);
				std::string distance_str = std::to_string(distance).substr(0, 5) + "m";

				ImVec2 textSize{ ImGui::CalcTextSize(distance_str.c_str()) };
				const float offset{ textSize.x / 2.0f };
				ImVec2 distance_pos = { top.x - offset, top.y - 25 };
				drawing::text(distance_pos, distance_str, math::RGBAToFloat4(globals::distance_esp.second.data()), true);
			}

			if (globals::tracer.first)
			{
				ImVec2 origin = { 1920 / 2, 1080 / 2 };
				ImVec2 player = { bottom.x, bottom.y };

				if (bottom.x <= 1 || bottom.y <= 1)
					continue;

				drawing::line(origin, player, math::RGBAToFloat4(globals::tracer.second.data()), 1.f);
			}

			ImGui::End();
		}
	}
}