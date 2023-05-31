#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <filesystem>

#include "imgui/imgui.h"

#include "../utils/vector.h"
#include "../utils/memory.h"

#define OFFSET_LOCAL_PLAYER                  0xDE997C
#define OFFSET_ENTITY_LIST                   0x4DFEF08
#define OFFSET_CLIENT_STATE                  0x59F19C //engine.dll
#define OFFSET_VIEW_MATRIX                   0x4DEFD54
#define OFFSET_GLOW_OBJECT_MANAGER           0x5359988
#define OFFSET_FORCE_JUMP                    0x52BAC68

#define OFFSET_HEALTH                        0x100
#define OFFSET_ARMOR                         0x117CC
#define OFFSET_TEAM                          0xF4
#define OFFSET_LIFE_STATE                    0x25F
#define OFFSET_BONE_MATRIX                   0x26A8
#define OFFSET_ORIGIN                        0x138 //Feet pos
#define OFFSET_VIEW_ANGLES                   0x4D90 //pitch yaw
#define OFFSET_VIEW_OFFSET                   0x108 //Decreases when crouch. Increases when uncrouch
#define OFFSET_AIM_PUNCH_ANGLE               0x303C //Notes on how to find: Search for 0.0000000000 then shoot pause game and search for decreased value. Unpause and search for increased value. Repeat
#define OFFSET_VISIBLE                       0x980
#define OFFSET_GLOW_INDEX                    0x10488
#define OFFSET_FLAGS                         0x104

struct Entity
{
	int health = 0;
	int armor = 0;
	int team = 0;
	bool dead = 0;
	DWORD bone_matrix = 0;
	Vector3 view_angles = 0;
	Vector3 head_pos = 0;
	Vector3 chest_pos = 0;
	Vector3 feet_pos = 0;
	Vector3 view_offset = 0;
	Vector3 eye_pos = 0;
	Vector3 aim_punch = 0;
	bool visible = 0;
	int glow_index = 0;
};

namespace globals
{
	inline std::uintptr_t client = 0;
	inline std::uintptr_t engine = 0;

	inline HWND h_Game = nullptr;

	//config settings
	inline constinit std::string config_folder_name = "CSGO External";
	inline std::filesystem::path config_path;
	inline std::filesystem::path config_file_path;

	//menu
	inline bool b_menu_open = false;
	inline ImFont* small_font = nullptr;
	inline ImFont* product_sans = nullptr;
	inline std::array<float, 4U> text = { 1.00f, 1.00f, 1.00f, 0.80f };
	inline std::array<float, 4U> title_bar = { 1.f, 0.00f, 0.00f, 0.65f };
	inline std::array<float, 4U> border = { 1.f, 0.00f, 0.00f, 1.00f };
	inline std::array<float, 4U> child_border = { 0.50f, 0.00f, 0.00f, 0.00f };
	inline std::array<float, 4U> frame_bg = { 0.50f, 0.00f, 0.00f, 0.00f };
	inline std::array<float, 4U> frame_bg_active = { 0.80f, 0.80f, 0.80f, 0.40f };
	inline std::array<float, 4U> frame_bg_hovered = { 0.80f, 0.80f, 0.80f, 0.20f };

	//aimbot
	inline bool b_aimbot = true;
	inline float f_smoothness = 3.f;
	inline float f_fov = 10.f;
	inline bool b_draw_fov = true;
	inline int aim_part = 0;
	inline const char* const aim_parts[] = { "Head", "Chest" };
	inline int aim_key = VK_RBUTTON;

	//esp
	inline bool b_esp = true;
	inline std::pair<bool, std::array<float, 4U>> box_esp
	{
		true,
		{ 1.f, 1.f, 1.f, 1.f }
	};

	inline std::pair<bool, std::array<float, 4U>> glow_esp
	{
		true,
		{ 1.f, 1.f, 1.f, 1.f }
	};

	inline bool b_health_esp = true;
	inline bool b_armor_esp = true;

	inline std::pair<bool, std::array<float, 4U>> distance_esp
	{
		true,
		{ 1.f, 1.f, 1.f, 1.f }
	};

	inline std::pair<bool, std::array<float, 4U>> tracer
	{
		true,
		{ 1.f, 1.f, 1.f, 1.f }
	};

	//misc
	inline bool b_bhop = true;

	//utils
	static Entity get_local_player(const Memory& mem)
	{
		uintptr_t localplayer = mem.read<uintptr_t>(globals::client + OFFSET_LOCAL_PLAYER);
		uintptr_t client_state = mem.read<uintptr_t>(globals::engine + OFFSET_CLIENT_STATE);

		int health = mem.read<int>(localplayer + OFFSET_HEALTH);
		int armor = mem.read<int>(localplayer + OFFSET_ARMOR);
		int team = mem.read<int>(localplayer + OFFSET_TEAM);
		bool dead = mem.read<bool>(localplayer + OFFSET_LIFE_STATE);
		DWORD bones = mem.read<DWORD>(localplayer + OFFSET_BONE_MATRIX);
		Vector3 view_angles = mem.read<Vector3>(client_state + OFFSET_VIEW_ANGLES);
		Vector3 head_pos
		{
			mem.read<float>(bones + 0x30 * 8 + 0x0C),
			mem.read<float>(bones + 0x30 * 8 + 0x1C),
			mem.read<float>(bones + 0x30 * 8 + 0x2C),
		};
		Vector3 chest_pos
		{
			mem.read<float>(bones + 0x30 * 6 + 0x0C),
			mem.read<float>(bones + 0x30 * 6 + 0x1C),
			mem.read<float>(bones + 0x30 * 6 + 0x2C),
		};
		Vector3 feet_pos = mem.read<Vector3>(localplayer + OFFSET_ORIGIN);
		Vector3 view_offset = mem.read<Vector3>(localplayer + OFFSET_VIEW_OFFSET);
		Vector3 eye_pos = feet_pos + view_offset;
		Vector3 aim_punch = mem.read<Vector3>(localplayer + OFFSET_AIM_PUNCH_ANGLE);
		bool visible = mem.read<bool>(localplayer + OFFSET_VISIBLE);
		int glow_index = mem.read<int>(localplayer + OFFSET_GLOW_INDEX);

		Entity player;
		player.health = health;
		player.armor = armor;
		player.team = team;
		player.dead = dead;
		player.bone_matrix = bones;
		player.view_angles = view_angles;
		player.head_pos = head_pos;
		player.chest_pos = chest_pos;
		player.feet_pos = feet_pos;
		player.view_offset = view_offset;
		player.eye_pos = eye_pos;
		player.aim_punch = aim_punch;
		player.visible = visible;
		player.glow_index = glow_index;

		return player;
	}

	static std::vector<Entity> get_entity_list(const Memory& mem)
	{
		std::vector<Entity> entities;
		uintptr_t entitylist = mem.read<uintptr_t>(globals::client + OFFSET_ENTITY_LIST);
		uintptr_t client_state = mem.read<uintptr_t>(globals::engine + OFFSET_CLIENT_STATE);

		for (int i = 0; i < 32; i++)
		{
			uintptr_t entity = mem.read<uintptr_t>(entitylist + i * 0x10);

			if (!entity)
				continue;

			int health = mem.read<int>(entity + OFFSET_HEALTH);
			int armor = mem.read<int>(entity + OFFSET_ARMOR);
			int team = mem.read<int>(entity + OFFSET_TEAM);
			bool dead = mem.read<bool>(entity + OFFSET_LIFE_STATE);
			DWORD bones = mem.read<DWORD>(entity + OFFSET_BONE_MATRIX);
			Vector3 view_angles = mem.read<Vector3>(client_state + OFFSET_VIEW_ANGLES);
			Vector3 head_pos
			{
				mem.read<float>(bones + 0x30 * 8 + 0x0C),
				mem.read<float>(bones + 0x30 * 8 + 0x1C),
				mem.read<float>(bones + 0x30 * 8 + 0x2C),
			};
			Vector3 chest_pos
			{
				mem.read<float>(bones + 0x30 * 6 + 0x0C),
				mem.read<float>(bones + 0x30 * 6 + 0x1C),
				mem.read<float>(bones + 0x30 * 6 + 0x2C),
			};
			Vector3 feet_pos = mem.read<Vector3>(entity + OFFSET_ORIGIN);
			Vector3 view_offset = mem.read<Vector3>(entity + OFFSET_VIEW_OFFSET);
			Vector3 eye_pos = feet_pos + view_offset;
			Vector3 aim_punch = mem.read<Vector3>(entity + OFFSET_AIM_PUNCH_ANGLE);
			bool visible = mem.read<bool>(entity + OFFSET_VISIBLE);
			int glow_index = mem.read<int>(entity + OFFSET_GLOW_INDEX);

			Entity ent;
			ent.health = health;
			ent.armor = armor;
			ent.team = team;
			ent.dead = dead;
			ent.bone_matrix = bones;
			ent.view_angles = view_angles;
			ent.head_pos = head_pos;
			ent.chest_pos = chest_pos;
			ent.feet_pos = feet_pos;
			ent.view_offset = view_offset;
			ent.eye_pos = eye_pos;
			ent.aim_punch = aim_punch;
			ent.visible = visible;
			ent.glow_index = glow_index;

			entities.push_back(ent);
		}
		return entities;
	}
}