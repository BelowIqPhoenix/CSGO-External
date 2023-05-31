#include "config.h"

void config::create_config(std::string filename)
{
	globals::config_path = std::getenv("appdata");
	globals::config_path /= globals::config_folder_name;

	if (!std::filesystem::exists(globals::config_path))
		std::filesystem::create_directory(globals::config_path);

	globals::config_file_path = globals::config_path /= filename;

	nlohmann::json base_config
	{
		{
			"aimbot",
			{
				{ "aimbot_enabled", globals::b_aimbot },
				{ "aimbot_smoothness", globals::f_smoothness },
				{ "aimbot_fov", globals::f_fov },
				{ "aimbot_draw_fov", globals::b_draw_fov },
				{ "aimbot_key", globals::aim_key },
				{ "aimbot_aim_part", globals::aim_part }
			}
		},
			{
				"esp",
				{
					{ "esp_enabled", globals::b_esp },
					{ "esp_color", globals::box_esp.second },
					{ "box_esp_enabled", globals::box_esp.first },
					{ "glow_enabled", globals::glow_esp.first },
					{ "glow_color", globals::glow_esp.second },
					{ "health_esp_enabled", globals::b_health_esp },
					{ "armor_esp_enabled", globals::b_armor_esp },
					{ "distance_esp_enabled", globals::distance_esp.first },
					{ "distance_esp_color", globals::distance_esp.second },
					{ "tracer_enabled", globals::tracer.first },
					{ "tracer_color", globals::tracer.second }
				}
			},
			{
				"misc",
				{
					{ "bhop_enabled", globals::b_bhop }
				}
			},
			{
				"menu",
				{
					{ "text_col", globals::text },
					{ "title_bar_col", globals::title_bar },
					{ "border_col", globals::border },
					{ "child_border_col", globals::child_border },
					{ "frame_bg_col", globals::frame_bg },
					{ "frame_bg_active_col", globals::frame_bg_active },
					{ "frame_bg_hovered_col", globals::frame_bg_hovered }
				}
			}
	};

	if (!std::filesystem::exists(globals::config_file_path))
	{
		std::ofstream config_file(globals::config_file_path, std::ios::out | std::ios::trunc);
		if (!config_file.is_open())
			std::cerr << "Failed to create config file: " << filename << std::endl;

		config_file << base_config.dump(4);
		config_file.close();
	}
}

void config::load_config(std::filesystem::path config_file_path)
{
	std::ifstream config_file(config_file_path);
	if (!config_file.is_open())
	{
		std::cerr << "Failed to open config file: " << config_file_path << std::endl;
		return;
	}

	nlohmann::json j;
	config_file >> j;

	globals::b_aimbot = j["aimbot"]["aimbot_enabled"];
	globals::f_smoothness = j["aimbot"]["aimbot_smoothness"];
	globals::f_fov = j["aimbot"]["aimbot_fov"];
	globals::b_draw_fov = j["aimbot"]["aimbot_draw_fov"];
	globals::aim_key = j["aimbot"]["aimbot_key"];
	globals::aim_part = j["aimbot"]["aimbot_aim_part"];

	globals::b_esp = j["esp"]["esp_enabled"];
	globals::box_esp.second = j["esp"]["esp_color"];
	globals::box_esp.first = j["esp"]["box_esp_enabled"];
	globals::glow_esp.first = j["esp"]["glow_enabled"];
	globals::glow_esp.second = j["esp"]["glow_color"];
	globals::b_health_esp = j["esp"]["health_esp_enabled"];
	globals::b_armor_esp = j["esp"]["armor_esp_enabled"];
	globals::distance_esp.first = j["esp"]["distance_esp_enabled"];
	globals::distance_esp.second = j["esp"]["distance_esp_color"];
	globals::tracer.first = j["esp"]["tracer_enabled"];
	globals::tracer.second = j["esp"]["tracer_color"];

	globals::b_bhop = j["misc"]["bhop_enabled"];

	globals::text = j["menu"]["text_col"];
	globals::title_bar = j["menu"]["title_bar_col"];
	globals::border = j["menu"]["border_col"];
	globals::child_border = j["menu"]["child_border_col"];
	globals::frame_bg = j["menu"]["frame_bg_col"];
	globals::frame_bg_active = j["menu"]["frame_bg_active_col"];
	globals::frame_bg_hovered = j["menu"]["frame_bg_hovered_col"];
}

void config::save_config()
{
	nlohmann::json config
	{
		{
			"aimbot",
			{
				{ "aimbot_enabled", globals::b_aimbot },
				{ "aimbot_smoothness", globals::f_smoothness },
				{ "aimbot_fov", globals::f_fov },
				{ "aimbot_draw_fov", globals::b_draw_fov },
				{ "aimbot_key", globals::aim_key },
				{ "aimbot_aim_part", globals::aim_part }
			}
		},
			{
				"esp",
				{
					{ "esp_enabled", globals::b_esp },
					{ "esp_color", globals::box_esp.second },
					{ "box_esp_enabled", globals::box_esp.first },
					{ "glow_enabled", globals::glow_esp.first },
					{ "glow_color", globals::glow_esp.second },
					{ "health_esp_enabled", globals::b_health_esp },
					{ "armor_esp_enabled", globals::b_armor_esp },
					{ "distance_esp_enabled", globals::distance_esp.first },
					{ "distance_esp_color", globals::distance_esp.second },
					{ "tracer_enabled", globals::tracer.first },
					{ "tracer_color", globals::tracer.second }
				}
			},
			{
				"misc",
				{
					{ "bhop_enabled", globals::b_bhop }
				}
			},
			{
				"menu",
				{
					{ "text_col", globals::text },
					{ "title_bar_col", globals::title_bar },
					{ "border_col", globals::border },
					{ "child_border_col", globals::child_border },
					{ "frame_bg_col", globals::frame_bg },
					{ "frame_bg_active_col", globals::frame_bg_active },
					{ "frame_bg_hovered_col", globals::frame_bg_hovered }
				}
			}
	};

	if (std::filesystem::exists(globals::config_file_path))
		std::filesystem::remove(globals::config_file_path);

	std::ofstream config_file(globals::config_file_path, std::ios::out | std::ios::trunc);
	if (!config_file.is_open())
		std::cerr << "Failed to save config" << std::endl;

	config_file << config.dump(4);
	config_file.close();
}