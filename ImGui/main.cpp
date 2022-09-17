#include "utils.h"
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(0), 0, 0, 0, 0, "Eauth", 0 };
    RegisterClassEx(&wc);
    Window = CreateWindow(wc.lpszClassName, "Eauth", WS_POPUP, 0, 0, 5, 5, 0, 0, wc.hInstance, 0);
    if (!CreateDeviceD3D(Window)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    auth.init();
    ShowWindow(Window, SW_HIDE);
    UpdateWindow(Window);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;
    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2.5;
    icons_config.OversampleV = 2.5;
    ImFontConfig font_config;
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF,
        0x0400, 0x044F,
        0,
    };
    ImFont* gilroy_mini = io.Fonts->AddFontFromMemoryTTF((void*)gilroyfont, sizeof(gilroyfont), 15, &font_config, ranges);
    io.Fonts->AddFontDefault();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplWin32_Init(Window);
    ImGui_ImplDX9_Init(g_pd3dDevice);
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        if (digit[4] != 1)
        {
            ImGui::SetNextWindowSize(ImVec2(digit[2] + 2, digit[3] + 2));
        }
        if (digit[4] == 1)
        {
            ImGui::SetNextWindowSize(ImVec2(digit[5] + 2, digit[6] + 2));
        }
        {
            ImGui::Begin("Eauth", &status[1], ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
            {
                customization();
                styling();
                if (status[5] == true)
                {
                    msg.message = WM_QUIT;
                }
            }
            ImGui::End();
        }
        ImGui::EndFrame();
        g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        HRESULT result = g_pd3dDevice->Present(0, 0, 0, 0);
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!status[1]) {
            msg.message = WM_QUIT;
        }
    }
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    DestroyWindow(Window);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != 0 && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
