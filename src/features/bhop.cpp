#include "features.h"

void features::bhop(const Memory& mem)
{
	while (globals::b_bhop)
	{
		const auto localplayer = mem.read<std::uintptr_t>(globals::client + OFFSET_LOCAL_PLAYER);
		const auto localplayer_flags = mem.read<std::uintptr_t>(localplayer + OFFSET_FLAGS);

		if (GetAsyncKeyState(VK_SPACE))
			(localplayer_flags & (1 << 0)) ? mem.write<std::uintptr_t>(globals::client + OFFSET_FORCE_JUMP, 6) : mem.write<std::uintptr_t>(globals::client + OFFSET_FORCE_JUMP, 4);
	}

	while (!globals::b_bhop)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	bhop(mem);
}