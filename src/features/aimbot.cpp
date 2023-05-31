#include "features.h"

void features::aimbot(const Memory& mem)
{
	while (globals::b_aimbot)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!GetAsyncKeyState(globals::aim_key))
			continue;

		Entity localplayer = globals::get_local_player(mem);
		std::vector<Entity> ents = globals::get_entity_list(mem);

		uintptr_t client_state = mem.read<uintptr_t>(globals::engine + OFFSET_CLIENT_STATE);
		Vector3 aim_punch = localplayer.aim_punch * 2;

		if (globals::f_fov < 1)
			globals::f_fov = 1;

		if (globals::f_fov > 1000)
			globals::f_fov = 1000;
		float best_fov = globals::f_fov;
		Vector3 best_angle = Vector3{};

		for (Entity ent : ents)
		{
			if (ent.team == localplayer.team)
				continue;

			if (!ent.health)
				continue;

			if (!ent.visible)
				continue;

			Vector3 angle;
			if (globals::aim_part == 0)
				angle = math::CalculateAngle(localplayer.eye_pos, ent.head_pos, localplayer.view_angles + aim_punch);
			else
				angle = math::CalculateAngle(localplayer.eye_pos, ent.chest_pos, localplayer.view_angles + aim_punch);

			const float fov = std::hypotf(angle.x, angle.y);

			if (fov < best_fov)
			{
				best_fov = fov;
				best_angle = angle;
			}
		}

		if (!best_angle.IsZero())
		{
			if (!globals::f_smoothness)
				mem.write<Vector3>(client_state + OFFSET_VIEW_ANGLES, localplayer.view_angles + best_angle);
			else
				mem.write<Vector3>(client_state + OFFSET_VIEW_ANGLES, localplayer.view_angles + best_angle / globals::f_smoothness);
		}
	}

	while (!globals::b_aimbot)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	aimbot(mem);
}