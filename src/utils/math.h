#pragma once

#include "imgui/imgui.h"

#include "vector.h"
#include "memory.h"
#include "../config/globals.h"

struct ViewMatrix {
	ViewMatrix()
		: data() {}

	float* operator[](int index)
	{
		return data[index];
	}

	const float* operator[](int index) const
	{
		return data[index];
	}

	float data[4][4];
};

namespace math
{
	static bool world_to_screen(const Vector3& world, Vector2& screen_pos, const ViewMatrix& vm)
	{
		float w = vm[3][0] * world.x + vm[3][1] * world.y + vm[3][2] * world.z + vm[3][3];

		if (w < 0.001f)
			return false;

		const float x = world.x * vm[0][0] + world.y * vm[0][1] + world.z * vm[0][2] + vm[0][3];
		const float y = world.x * vm[1][0] + world.y * vm[1][1] + world.z * vm[1][2] + vm[1][3];

		w = 1.f / w;
		float nx = x * w;
		float ny = y * w;

		const ImVec2 size = ImGui::GetIO().DisplaySize;

		screen_pos.x = (size.x * 0.5f * nx) + (nx + size.x * 0.5f);
		screen_pos.y = -(size.y * 0.5f * ny) + (ny + size.y * 0.5f);

		return true;
	}

	static ImVec4 RGBAToFloat4(float* col_arr)
	{
		return ImVec4(col_arr[0], col_arr[1], col_arr[2], col_arr[3]);
	}

	static ImVec4 RGBAToFloat4(float r, float g, float b, float a)
	{
		float x = round(r / 255);
		float y = round(g / 255);
		float z = round(b / 255);
		float w = a;
		ImVec4 col = ImVec4(x, y, z, w);

		return(col);
	}

	static float get_distance(Vector3 local_pos, Vector3 ent_pos)
	{
		return sqrt(pow((local_pos.x - ent_pos.x), 2) + pow((local_pos.y - ent_pos.y), 2) + pow((local_pos.z - ent_pos.z), 2)) / 100;
	}

	static Vector3 CalculateAngle(Vector3 src, Vector3 dst)
	{
		float PI = 3.1415927f;
		Vector3 angles = {};
		angles.x = -atan2f(dst.x - src.x, dst.y - src.y) / PI * 180.0f + 180.0f;
		angles.y = asinf((dst.z - src.z) / get_distance(src, dst)) * 180.0f / PI;
		angles.z = 0.0f;

		return angles;
	}

	static Vector3 CalculateAngle(Vector3 localPosition, Vector3 enemyPosition, Vector3 viewAngles)
	{
		return ((enemyPosition - localPosition).ToAngle() - viewAngles);
	}
}