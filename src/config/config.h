#pragma once
#include <filesystem>
#include <fstream>

#include "json/json.h"

#include "globals.h"

namespace config
{
	void create_config(std::string filename);

	void load_config(std::filesystem::path config_file_path);

	void save_config();
}