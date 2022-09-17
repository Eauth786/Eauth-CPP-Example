#include <iostream>
#include <thread>
#include "eauth/eauth.cpp"
#include "font.h"
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
eauth auth;
bool status[] = { false, true, false, false, false, false };
int digit[] = { 0, 150,335,250, 0,620,500 };
std::string version = "1.0";
char usern[255] = "";
char passw[255] = "";
char licensek[255] = "";
HWND Window = nullptr;
LPDIRECT3DDEVICE9        g_pd3dDevice;
D3DPRESENT_PARAMETERS    g_d3dpp;
LPDIRECT3D9              g_pD3D;
#define LOADER_NAME " "
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
inline bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;
    return true;
}
inline void CleanupDeviceD3D()
{
    if (g_pd3dDevice) {
        g_pd3dDevice->Release(); g_pd3dDevice = NULL;
    }
    if (g_pD3D) {
        g_pD3D->Release(); g_pD3D = NULL;
    }
}
inline void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT Result = g_pd3dDevice->Reset(&g_d3dpp);
    if (Result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}
inline void customization()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiStyle* stylew = &ImGui::GetStyle();
    stylew->Alpha = 1.f;
    stylew->WindowRounding = 5;
    if (digit[4] != 1)
    {
        stylew->FrameRounding = 12;
    }
    else
    {
        stylew->FramePadding = ImVec2(4, 3);
    }
    stylew->WindowPadding = ImVec2(0, 0);
    stylew->ItemInnerSpacing = ImVec2(4, 4);
    stylew->ItemSpacing = ImVec2(8, 10);
    stylew->ScrollbarSize = 2.f;
    stylew->ScrollbarRounding = 12.f;
    stylew->PopupRounding = 0.f;
    ImGui::SetNextWindowBgAlpha(1.0f);
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    const auto w = &ImGui::GetStyle();
    const auto colors = w->Colors;
    colors[ImGuiCol_WindowBg] = ImColor(22, 22, 22);
    colors[ImGuiCol_ChildBg] = ImColor(23, 23, 23);
    colors[ImGuiCol_FrameBg] = ImColor(31, 31, 30);
    colors[ImGuiCol_FrameBgHovered] = ImColor(33, 33, 32);
    colors[ImGuiCol_FrameBgActive] = ImColor(34, 34, 33);
    colors[ImGuiCol_BorderShadow] = ImVec4(100.00f, 100.00f, 100.00f, 100.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImColor(16, 16, 16);
    colors[ImGuiCol_TitleBgCollapsed] = ImColor(16, 16, 16);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.10f, 0.10f, 0.15f, 0.00f);
    colors[ImGuiCol_SliderGrab] = ImColor(7, 127, 243);
    colors[ImGuiCol_SliderGrabActive] = ImColor(7, 127, 243);
    colors[ImGuiCol_CheckMark] = ImColor(7, 127, 243);
    colors[ImGuiCol_Separator] = ImColor(25, 25, 24);
    colors[ImGuiCol_SeparatorHovered] = ImColor(7, 127, 243);
    colors[ImGuiCol_SeparatorActive] = ImColor(7, 127, 243);
    colors[ImGuiCol_Tab] = (ImVec4)ImColor(0.20f, 0.20f, 0.20f, 0.9f);
    colors[ImGuiCol_TabActive] = ImColor(7, 127, 243);
    colors[ImGuiCol_TabHovered] = (ImVec4)ImColor(0.10f, 0.10f, 0.10f, 0.9f);
    colors[ImGuiCol_TabUnfocused] = (ImVec4)ImColor(0.15f, 0.15f, 0.15f, 0.9f);
    colors[ImGuiCol_TabUnfocusedActive] = ImColor(7, 127, 243);
    if (digit[4] == 1)
    {
        colors[ImGuiCol_Button] = ImColor(22, 22, 22);
        colors[ImGuiCol_ButtonHovered] = ImColor(232, 17, 35);
        colors[ImGuiCol_ButtonActive] = ImColor(236, 21, 39);
    }
}
class GradientLine {
public:
    static bool Render(ImVec2 size)
    {
        static ImColor gradient_colors[] =
        {
            ImColor(1,167,250,255),
            ImColor(1,167,250,255),
            ImColor(1,167,250,255),
            ImColor(7, 127, 243),
            ImColor(7, 127, 243),
            ImColor(7, 127, 243),
            ImColor(7, 127, 243)
        };
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2      screen_pos = ImGui::GetCursorScreenPos();
        static int pos = 0;
        if (size.x - pos == 0)
            pos = 0;
        else
            pos++;
        for (int i = 0; i < 6; ++i)
        {
            ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
            auto render = [&](int displacement)
            {
                draw_list->AddRectFilledMultiColor
                (
                    ImVec2((screen_pos.x - item_spacing.x - displacement) + (i) * (size.x / 6), (screen_pos.y - item_spacing.y)),
                    ImVec2((screen_pos.x - item_spacing.x + (item_spacing.x * 2) - displacement) + (i + 1) * (size.x / 6), (screen_pos.y - item_spacing.y) + (size.y)),
                    gradient_colors[i], gradient_colors[i + 1], gradient_colors[i + 1], gradient_colors[i]
                );
            };
            render((pos)); render((pos - size.x));
        }
        return true;
    }
};
inline void styling()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiStyle* stylew = &ImGui::GetStyle();
    ImGuiStyle& style = ImGui::GetStyle();
    const auto w = &ImGui::GetStyle();
    const auto colors = w->Colors;
    if (digit[4] != 1)
    {
        ImGui::Spacing();
        GradientLine::Render(ImVec2(ImGui::GetContentRegionAvail().x * 3, 2));
        ImGui::Spacing();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("Eauth | Main  ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        colors[ImGuiCol_Button] = ImColor(22, 22, 22);
        colors[ImGuiCol_ButtonHovered] = ImColor(232, 17, 35);
        colors[ImGuiCol_ButtonActive] = ImColor(236, 21, 39);
        if (ImGui::Button("X"))
        {
            status[5] = true;
        }
        colors[ImGuiCol_Button] = ImColor(7, 127, 243);
        colors[ImGuiCol_ButtonHovered] = ImColor(7, 127, 243);
        colors[ImGuiCol_ButtonActive] = ImColor(7, 127, 243);
        ImGui::Spacing();
        ImGui::PushItemWidth(250);
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::InputText(" ", usern, IM_ARRAYSIZE(usern, ImVec2(-1.0f, 0.0f)));
        if (status[4])
        {
            ImGui::Text("   ");
            ImGui::SameLine();
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::InputText("  ", passw, IM_ARRAYSIZE(passw, ImVec2(-1.0f, 0.0f)));
        }
        else
        {
            ImGui::Text("   ");
            ImGui::SameLine();
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::InputText("   ", passw, IM_ARRAYSIZE(passw), ImGuiInputTextFlags_Password);
        }
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::InputText("     ", licensek, IM_ARRAYSIZE(licensek, ImVec2(-1.0f, 0.0f)));
        if (ImGui::Button("Login", ImVec2(-0.0001f, 0.0f)))
        {
            if (auth.signin(usern,passw))
            {
                digit[5] = digit[5] + version.length() + 255 - sizeof(usern);
                digit[4] = 1;
                MessageBox(Window, "You have successfully logged in!", "Eauth", MB_OK | MB_ICONINFORMATION);
            }
        }
        if (ImGui::Button("Create Account", ImVec2(-0.0001f, 0.0f)))
        {
            if (auth.signup(usern, passw, licensek))
            {
                MessageBox(Window, "You have successfully registered!", "Eauth", MB_OK | MB_ICONINFORMATION);
            }
        }
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Checkbox("Show password", &status[4]);
    }
    else if (digit[4] == 1)
    {
        ImGui::Spacing();
        GradientLine::Render(ImVec2(ImGui::GetContentRegionAvail().x * 3, 2));
        stylew->FrameRounding = 12;
        ImGui::Spacing();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("Eauth | Main");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        colors[ImGuiCol_Button] = ImColor(22, 22, 22);
        colors[ImGuiCol_ButtonHovered] = ImColor(232, 17, 35);
        colors[ImGuiCol_ButtonActive] = ImColor(236, 21, 39);
        if (ImGui::Button("X"))
        {
            status[5] = true;
        }
        stylew->FrameRounding = 0;
        colors[ImGuiCol_Button] = ImColor(21, 21, 20);
        colors[ImGuiCol_ButtonHovered] = ImColor(21, 21, 20);
        colors[ImGuiCol_ButtonActive] = ImColor(21, 21, 20);
        ImGui::Separator();
        ImGui::BeginChild("##LeftSide", ImVec2(ImGui::GetWindowWidth() / 4, ImGui::GetWindowHeight() / 2 + 147));
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##RightSide", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 2 + 147));
        ImGui::Text("\n");
        ImGui::Text("  Username: %s", auth.NAME);
        ImGui::Text("  Rank: %s", auth.RANK);
        ImGui::Text("  Create Date: %s", auth.CREATEDATE);
        ImGui::Text("  Expire Date: %s", auth.EXPIREDATE);
        ImGui::EndChild();
        ImGui::BeginChild(" ");
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(7, 127, 243), "EAUTH");
        ImGui::SameLine();
        ImGui::Text("%s", version);
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text("\t");
        ImGui::SameLine();
        ImGui::Text(usern);
        ImGui::SameLine();
        ImGui::TextColored(ImColor(7, 127, 243), "%s", "3");
        ImGui::EndChild();
    }
}