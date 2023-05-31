#pragma once
#include <iostream>
#include <thread>
#include <string>

#include "../utils/math.h"
#include "../config/globals.h"
#include "../gui/drawing/drawing.h"

namespace features
{
	void aimbot(const Memory& mem);
	void esp(const Memory& mem);
	void bhop(const Memory& mem);
}