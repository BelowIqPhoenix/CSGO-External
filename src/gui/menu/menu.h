#pragma once

#include "../gui.h"
#include "../widgets/hotkey.h"

namespace menu
{
	void draw(const Memory& mem);

	void aimbot_tab();
	void visuals_tab();
	void misc_tab();
	void settings_tab();

	void unload(const Memory& mem);
}