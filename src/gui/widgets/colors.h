#pragma once

#include "imgui/imgui.h"

namespace colors
{
	static ImVec4 rainbow(float speed)
	{
		auto isFrames = ImGui::GetFrameCount();
		static float isRed = 0.0f, isGreen = 0.01f, isBlue = 0.0f;

		ImVec4 rainbow4 = ImVec4(isRed, isGreen, isBlue, 1.0f);

		int intspeed = static_cast<int>(speed);

		if (intspeed < 1)
			intspeed = 1;

		if (isFrames % intspeed == 0.f)
		{
			if (isGreen == 0.01f && isBlue == 0.0f)
				isRed += 0.01f;

			if (isRed > 0.99f && isBlue == 0.0f) { isRed = 1.0f; isGreen += 0.01f; }

			if (isGreen > 0.99f && isBlue == 0.0f) { isGreen = 1.0f; isRed -= 0.01f; }

			if (isRed < 0.01f && isGreen == 1.0f) { isRed = 0.0f; isBlue += 0.01f; }

			if (isBlue > 0.99f && isRed == 0.0f) { isBlue = 1.0f; isGreen -= 0.01f; }

			if (isGreen < 0.01f && isBlue == 1.0f) { isGreen = 0.0f; isRed += 0.01f; }

			if (isRed > 0.99f && isGreen == 0.0f) { isRed = 1.0f; isBlue -= 0.01f; }

			if (isBlue < 0.01f && isGreen == 0.0f)
			{
				isBlue = 0.0f;
				isRed -= 0.01f;

				if (isRed < 0.01f)
					isGreen = 0.01f;
			}
		}
		return(rainbow4);
	}
}