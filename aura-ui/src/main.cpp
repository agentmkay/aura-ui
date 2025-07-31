// Modern Cheat Menu UI with Rounded Elements and Smooth Animations

#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_win32.h"
#include "./imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Cheat menu variables
bool enable_aimbot = false;
bool enable_esp = true;
bool enable_team_esp = false;
bool enable_snaplines = true;
bool enable_bunnyhop = false;
bool enable_radar = true;
bool watermark = true;
bool show_fps = true;
bool show_spectators = false;
bool enable_glow = true;
bool enable_chams = false;
bool enable_recoil = false;
bool enable_triggerbot = false;
bool enable_silent_aim = false;
bool enable_visible_check = false;
bool enable_auto_fire = false;
bool enable_box_esp = false;
bool enable_health_bars = false;
bool enable_distance_indicator = false;
bool enable_auto_pistol = false;
bool enable_no_flash = false;
bool enable_hit_sound = false;

float aimbot_fov = 3.0f;
float aimbot_smooth = 5.0f;
float glow_strength = 1.0f;
int hitbox_selection = 0;
int theme_selection = 0;
const char* hitbox_items[] = { "Head", "Neck", "Chest", "Pelvis" };
const char* theme_items[] = { "Purple", "Blue", "Red", "Green" };

// Custom colors
ImVec4 accent_color = ImVec4(0.58f, 0.27f, 0.88f, 1.00f);
ImVec4 accent_hover = ImVec4(0.68f, 0.37f, 0.98f, 1.00f);
ImVec4 accent_active = ImVec4(0.78f, 0.47f, 1.00f, 1.00f);

void ApplyModernStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Main colors
    style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.98f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Frame colors
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered] = accent_hover;
    style.Colors[ImGuiCol_FrameBgHovered].w = 0.4f;
    style.Colors[ImGuiCol_FrameBgActive] = accent_active;
    style.Colors[ImGuiCol_FrameBgActive].w = 0.6f;

    // Title colors
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = accent_color;
    style.Colors[ImGuiCol_TitleBgActive].w = 0.8f;
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);

    // Interactive elements
    style.Colors[ImGuiCol_CheckMark] = accent_color;
    style.Colors[ImGuiCol_SliderGrab] = accent_color;
    style.Colors[ImGuiCol_SliderGrabActive] = accent_active;
    style.Colors[ImGuiCol_Button] = accent_color;
    style.Colors[ImGuiCol_Button].w = 0.6f;
    style.Colors[ImGuiCol_ButtonHovered] = accent_hover;
    style.Colors[ImGuiCol_ButtonActive] = accent_active;

    // Header colors
    style.Colors[ImGuiCol_Header] = accent_color;
    style.Colors[ImGuiCol_Header].w = 0.4f;
    style.Colors[ImGuiCol_HeaderHovered] = accent_hover;
    style.Colors[ImGuiCol_HeaderHovered].w = 0.8f;
    style.Colors[ImGuiCol_HeaderActive] = accent_active;

    // Tab colors
    style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_TabHovered] = accent_hover;
    style.Colors[ImGuiCol_TabHovered].w = 0.8f;
    style.Colors[ImGuiCol_TabActive] = accent_color;
    style.Colors[ImGuiCol_TabActive].w = 0.8f;
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

    // Rounding and spacing
    style.WindowPadding = ImVec2(10, 10);
    style.WindowRounding = 12.0f;
    style.FramePadding = ImVec2(8, 6);
    style.FrameRounding = 8.0f;
    style.ItemSpacing = ImVec2(10, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.IndentSpacing = 21.0f;
    style.ScrollbarSize = 14.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 12.0f;
    style.GrabRounding = 8.0f;
    style.TabRounding = 8.0f;
    style.ChildRounding = 8.0f;
    style.PopupRounding = 8.0f;

    // Other settings
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
}

void UpdateTheme()
{
    switch (theme_selection)
    {
    case 0: // Purple
        accent_color = ImVec4(0.58f, 0.27f, 0.88f, 1.00f);
        accent_hover = ImVec4(0.68f, 0.37f, 0.98f, 1.00f);
        accent_active = ImVec4(0.78f, 0.47f, 1.00f, 1.00f);
        break;
    case 1: // Blue
        accent_color = ImVec4(0.27f, 0.47f, 0.88f, 1.00f);
        accent_hover = ImVec4(0.37f, 0.57f, 0.98f, 1.00f);
        accent_active = ImVec4(0.47f, 0.67f, 1.00f, 1.00f);
        break;
    case 2: // Red
        accent_color = ImVec4(0.88f, 0.27f, 0.27f, 1.00f);
        accent_hover = ImVec4(0.98f, 0.37f, 0.37f, 1.00f);
        accent_active = ImVec4(1.00f, 0.47f, 0.47f, 1.00f);
        break;
    case 3: // Green
        accent_color = ImVec4(0.27f, 0.88f, 0.37f, 1.00f);
        accent_hover = ImVec4(0.37f, 0.98f, 0.47f, 1.00f);
        accent_active = ImVec4(0.47f, 1.00f, 0.57f, 1.00f);
        break;
    }
    ApplyModernStyle();
}

void RenderCheatMenu(ImGuiIO& io)
{
    ImGui::SetNextWindowSize(ImVec2(650, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("aura-ui Menu", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoResize);

    // Tabs
    if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_FittingPolicyScroll))
    {
        // Aimbot Tab
        if (ImGui::BeginTabItem("Aimbot"))
        {
            ImGui::BeginChild("AimbotSettings", ImVec2(0, 0), true);

            ImGui::Checkbox("Enable Aimbot", &enable_aimbot);
            ImGui::SameLine(0, 20);
            ImGui::Checkbox("Silent Aim", &enable_silent_aim);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::SliderFloat("Aimbot FOV", &aimbot_fov, 1.0f, 10.0f, "%.1f");
            ImGui::SliderFloat("Smoothing", &aimbot_smooth, 1.0f, 10.0f, "%.1f");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Combo("Hitbox", &hitbox_selection, hitbox_items, IM_ARRAYSIZE(hitbox_items));
            ImGui::Checkbox("Visible Check", &enable_visible_check);
            ImGui::Checkbox("Auto Fire", &enable_auto_fire);
            ImGui::Checkbox("Recoil Control", &enable_recoil);

            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        // Visuals Tab
        if (ImGui::BeginTabItem("Visuals"))
        {
            ImGui::BeginChild("VisualsSettings", ImVec2(0, 0), true);

            ImGui::Columns(2, "visuals_cols", false);

            // Left column
            ImGui::Checkbox("Enable ESP", &enable_esp);
            ImGui::Checkbox("Team ESP", &enable_team_esp);
            ImGui::Checkbox("Snaplines", &enable_snaplines);
            ImGui::Checkbox("Box ESP", &enable_box_esp);

            // Right column
            ImGui::NextColumn();
            ImGui::Checkbox("Radar", &enable_radar);
            ImGui::Checkbox("Glow Effect", &enable_glow);
            ImGui::SliderFloat("Glow Strength", &glow_strength, 0.1f, 2.0f, "%.1f");
            ImGui::Checkbox("Chams", &enable_chams);

            ImGui::Columns(1);
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Checkbox("Show Spectators", &show_spectators);
            ImGui::Checkbox("Health Bars", &enable_health_bars);
            ImGui::Checkbox("Distance Indicator", &enable_distance_indicator);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Combo("UI Theme", &theme_selection, theme_items, IM_ARRAYSIZE(theme_items));
            UpdateTheme();

            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        // Misc Tab
        if (ImGui::BeginTabItem("Misc"))
        {
            ImGui::BeginChild("MiscSettings", ImVec2(0, 0), true);

            ImGui::Checkbox("Bunny Hop", &enable_bunnyhop);
            ImGui::Checkbox("Auto Pistol", &enable_auto_pistol);
            ImGui::Checkbox("No Flash", &enable_no_flash);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Checkbox("Watermark", &watermark);
            ImGui::Checkbox("Show FPS", &show_fps);
            ImGui::Checkbox("Hit Sound", &enable_hit_sound);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("Config System");
            if (ImGui::Button("Save Config", ImVec2(100, 30)))
            {
                // Save config logic
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Config", ImVec2(100, 30)))
            {
                // Load config logic
            }

            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        // System Tab
        if (ImGui::BeginTabItem("System"))
        {
            ImGui::BeginChild("SystemSettings", ImVec2(0, 0), true);

            ImGui::Text("Performance Metrics:");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("System Information:");
            ImGui::Text("Renderer: DirectX 11");
            ImGui::Text("ImGui Version: %s", ImGui::GetVersion());

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Unload Cheat", ImVec2(120, 40)))
            {
                // Unload logic
            }

            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

int main(int, char**)
{
    // Make process DPI aware
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Modern Cheat Menu", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Modern Cheat Menu", WS_OVERLAPPEDWINDOW, 100, 100, (int)(650 * main_scale), (int)(500 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Apply modern style
    ApplyModernStyle();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Render our custom UI
        RenderCheatMenu(io);

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.10f, 0.10f, 0.10f, 1.00f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        g_pSwapChain->Present(1, 0); // Present with vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions (same as before)
bool CreateDeviceD3D(HWND hWnd)
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
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Win32 message handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}