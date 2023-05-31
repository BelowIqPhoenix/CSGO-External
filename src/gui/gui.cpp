#include "gui.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include <thread>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

long __stdcall WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SYSCOMMAND:
	{
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}return 0;
	}

	return DefWindowProc(window, msg, wParam, lParam);
}

void gui::CreateCheatWindow(const char* window_name)
{
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandleA(0);
	wc.hIcon = 0;
	wc.hCursor = 0;
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "class001";
	wc.hIconSm = 0;

	RegisterClassEx(&wc);

	window = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
		"class001",
		window_name,
		WS_POPUP,
		0,
		0,
		WIDTH,
		HEIGHT,
		0,
		0,
		wc.hInstance,
		0
	);

	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(window, &margins);
	SetLayeredWindowAttributes(window, 0, 1.0f, LWA_ALPHA);
	SetLayeredWindowAttributes(window, 0, RGB(0, 0, 0), LWA_COLORKEY);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

	long style = GetWindowLongPtr(window, GWL_EXSTYLE);
	style |= WS_EX_LAYERED;
	SetWindowLongPtr(window, GWL_EXSTYLE, style);
}

void gui::DestroyCheatWindow()
{
	DestroyWindow(window);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

bool gui::CreateDevice()
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL level;
	const D3D_FEATURE_LEVEL levels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &device, &level, &device_context);
	if (res == DXGI_ERROR_UNSUPPORTED)
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, levels, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &device, &level, &device_context);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void gui::DestroyDevice()
{
	DestroyRenderTarget();
	if (swap_chain) { swap_chain->Release(); swap_chain = nullptr; }
	if (device_context) { device_context->Release(); device_context = nullptr; }
	if (device) { device->Release(); device = nullptr; }
}

void gui::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	device->CreateRenderTargetView(pBackBuffer, nullptr, &render_target_view);
	pBackBuffer->Release();
}

void gui::DestroyRenderTarget()
{
	if (render_target_view) { render_target_view->Release(); render_target_view = nullptr; }
}

void gui::CreateImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	init_style();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, device_context);
}

void gui::DestroyImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender()
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender()
{
	ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	ImGui::Render();
	device_context->OMSetRenderTargets(1, &render_target_view, nullptr);
	device_context->ClearRenderTargetView(render_target_view, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swap_chain->Present(1, 0);
}

void gui::SetWindowBorderless()
{
	globals::h_Game = FindWindow(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
	WINDOWPLACEMENT g_wpPrev;
	DWORD dwStyle = GetWindowLong(globals::h_Game, GWL_STYLE);
	MONITORINFO mi = { sizeof(mi) };
	if (GetWindowPlacement(globals::h_Game, &g_wpPrev) &&
		GetMonitorInfo(MonitorFromWindow(globals::h_Game,
			MONITOR_DEFAULTTOPRIMARY), &mi)) {
		SetWindowLong(globals::h_Game, GWL_STYLE,
			dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(globals::h_Game, HWND_TOP,
			mi.rcMonitor.left, mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

void gui::init_style()
{
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowTitleAlign = { 0.5f, 0.5f };
	style->WindowPadding = { 15, 15 };
	style->ChildRounding = 2.5f;
	style->WindowRounding = 5.0f;
	style->ScrollbarRounding = 1.f;
	style->FrameRounding = 2.f;
	style->ItemSpacing = { 10, 10 };
	style->ScrollbarSize = 3.f;
	style->GrabMinSize = 5.f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.f, 0.80f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.50f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.17f, 0.00f, 0.52f, 0.26f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.50f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_TitleBg] = ImVec4(1.f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(1.f, 0.00f, 0.00f, 0.65f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.f, 1.f, 1.f, 0.20f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.33f, 0.67f, 0.40f);
	colors[ImGuiCol_Header] = ImVec4(0.42f, 0.32f, 0.67f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.41f, 0.73f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.50f, 1.00f, 0.35f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.29f, 0.84f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.24f, 0.80f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImFontConfig config;
	config.OversampleH = 7;
	config.OversampleV = 7;
	config.RasterizerMultiply = 1.0f;
	config.PixelSnapH = true;
	config.SizePixels = 13.0f;
	
	//io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\cascadiamono.ttf", 20.f);
	io.Fonts->AddFontFromMemoryCompressedTTF(apex_compressed_data, apex_compressed_size, 20.f, &config);
	globals::small_font = io.Fonts->AddFontFromMemoryCompressedTTF(apex_compressed_data, apex_compressed_size, 15.f, &config);
	globals::product_sans = io.Fonts->AddFontFromMemoryCompressedTTF(product_sans_compressed_data, product_sans_compressed_size, 20.f, &config);
}

bool gui::init(const Memory& mem)
{
	CreateCheatWindow("Cheat Menu");

	if (!CreateDevice())
	{
		DestroyDevice();
		return 1;
	}
	CreateImGui();

	isRunning = true;
	while (isRunning)
	{
		BeginRender();
		Render(mem);
		EndRender();
	}

	DestroyImGui();
	DestroyDevice();
	DestroyCheatWindow();
	return 0;
}

void gui::Render(const Memory& mem)
{
	globals::h_Game = FindWindow(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		globals::b_menu_open = !globals::b_menu_open;
		long style = GetWindowLongPtr(window, GWL_EXSTYLE);
		if (globals::b_menu_open) {
			style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(window, GWL_EXSTYLE, style);
			SetForegroundWindow(window);
		}
		else {
			style |= WS_EX_LAYERED;
			SetWindowLongPtr(window, GWL_EXSTYLE, style);
			SetForegroundWindow(globals::h_Game);
		}
	}

	if (GetForegroundWindow() == globals::h_Game || GetForegroundWindow() == window)
	{
		ImGui::PushFont(globals::small_font);
		ImGui::GetBackgroundDrawList()->AddText({ 10, 1 }, ImGui::ColorConvertFloat4ToU32(colors::rainbow(3.5f)), "made by sussy");
		ImGui::PopFont();

		if (globals::b_menu_open)
			menu::draw(mem);

		if (globals::b_draw_fov)
		{
			if (globals::f_fov < 1)
				globals::f_fov = 1;

			if (globals::f_fov > 1000)
				globals::f_fov = 1000;

			ImVec2 cam;
			cam.x = GetSystemMetrics(SM_CXSCREEN) / 2;
			cam.y = GetSystemMetrics(SM_CYSCREEN) / 2;

			ImGui::GetBackgroundDrawList()->AddCircle(cam, globals::f_fov, ImColor(255, 255, 255), 500, 1.f);
		}

		if (globals::b_esp)
			features::esp(mem);
	}
	else
	{
		long style = GetWindowLongPtr(window, GWL_EXSTYLE);
		style |= WS_EX_LAYERED;
		SetWindowLongPtr(window, GWL_EXSTYLE, style);
	}
}