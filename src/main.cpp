#include <iostream>

#include "utils/memory.h"
#include "config/globals.h"
#include "features/features.h"
#include "gui/gui.h"
#include "config/config.h"

int main()
{
	std::cout << R"(
   ____ ____   ____  ___    _______  _______ _____ ____  _   _    _    _     
  / ___/ ___| / ___|/ _ \  | ____\ \/ /_   _| ____|  _ \| \ | |  / \  | |    
 | |   \___ \| |  _| | | | |  _|  \  /  | | |  _| | |_) |  \| | / _ \ | |    
 | |___ ___) | |_| | |_| | | |___ /  \  | | | |___|  _ <| |\  |/ ___ \| |___ 
  \____|____/ \____|\___/  |_____/_/\_\ |_| |_____|_| \_\_| \_/_/   \_\_____|
)" << "\n";

	Memory mem = { "csgo.exe" };

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	if (mem.processId == 0)
	{
		std::cout << "[+] Waiting for csgo.exe" << std::endl;
		while (mem.processId == 0) {
			mem = Memory{ "csgo.exe" };
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	while (!mem.GetModuleAddress("csgo.exe"))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	
	while (!mem.GetModuleAddress("client.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		
	globals::client = mem.GetModuleAddress("client.dll");

	while (!mem.GetModuleAddress("engine.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	globals::engine = mem.GetModuleAddress("engine.dll");

	std::cout << "[+] Process ID = " << mem.processId << std::endl;
	std::cout << "[+] client.dll = " << globals::client << std::endl;
	std::cout << "[+] engine.dll = " << globals::engine << std::endl;

	std::cout << "\n";

	std::thread(gui::init, mem).detach();
	std::cout << "[+] GUI/ESP Initialized!" << std::endl;

	std::thread(features::aimbot, mem).detach();
	std::cout << "[+] Aimbot Initialized!" << std::endl;

	std::thread(features::bhop, mem).detach();
	std::cout << "[+] Bhop Initialized!" << std::endl;

	config::create_config("settings.json");
	config::load_config(globals::config_file_path);

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		config::save_config();
	}

	return 0;
}