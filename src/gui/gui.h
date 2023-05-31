#pragma once
#include <d3d11.h>
#include <dwmapi.h>

#include "../utils/memory.h"
#include "../config/globals.h"
#include "menu/menu.h"
#include "../features/features.h"
#include "widgets/colors.h"
#include "fonts/fonts.h"

namespace gui
{
	constexpr int WIDTH = 1920;
	constexpr int HEIGHT = 1080;

	inline bool isRunning = false;

	// winapi window vars
	inline HWND window = nullptr;
	inline WNDCLASSEX wc = { };

	// direct x state vars
	inline ID3D11Device* device = nullptr;
	inline ID3D11DeviceContext* device_context = nullptr;
	inline IDXGISwapChain* swap_chain = nullptr;
	inline ID3D11RenderTargetView* render_target_view = nullptr;

	void CreateCheatWindow(const char* window_name);
	void DestroyCheatWindow();

	bool CreateDevice();
	void DestroyDevice();

	void CreateRenderTarget();
	void DestroyRenderTarget();

	void CreateImGui();
	void DestroyImGui();

	void BeginRender();
	void EndRender();

	void SetWindowBorderless();

	void init_style();
	bool init(const Memory& mem);

	void Render(const Memory& mem);
}