#include "Render.h"

#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

using namespace std::placeholders;

CRender::CRender()
{
    m_hookPresent.set_dest(GetFunctionAddress(17));
    m_hookPresent.before.connect(std::bind(&CRender::OnPresent, this, _1, _2, _3, _4, _5, _6));
    m_hookPresent.install();

    m_hookReset.set_dest(GetFunctionAddress(16));
    m_hookReset.before.connect(std::bind(&CRender::OnLost, this, _1, _2, _3));
    m_hookReset.after.connect(std::bind(&CRender::OnReset, this, _1, _2, _3, _4));
    m_hookReset.install();
}

CRender::~CRender()
{
    if (m_menuState)
    {
        ToggleMenu();
    }

    if (ImGui::GetCurrentContext())
    {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
}

bool CRender::ToggleMenu()
{
    m_menuState = !m_menuState;

    ShowCursor(m_menuState);

    ImGui::GetIO().MouseDrawCursor = m_menuState;

    return m_menuState;
}

std::optional<HRESULT> CRender::OnPresent(const decltype(m_hookPresent)& hook, IDirect3DDevice9* pDevice, const RECT*, const RECT*, HWND, const RGNDATA*)
{
    static bool initialized = false;

    if (!initialized)
    {
        ImGui::CreateContext();

        ImGui_ImplWin32_Init(**reinterpret_cast<HWND**>(0xC17054));
        ImGui_ImplDX9_Init(pDevice);

        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.IniFilename = nullptr;

#pragma warning(push)
#pragma warning(disable : 4996)
        std::string font = getenv("WINDIR");
        font += "\\Fonts\\Arialbd.TTF";
#pragma warning(pop)

        io.Fonts->AddFontFromFileTTF(font.c_str(), 15.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

        initialized = true;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    if (m_menuState)
    {
        ImGui::SetNextWindowPos(ImVec2(100.f, 100.f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300.f, 300.f), ImGuiCond_FirstUseEver);

        ImGui::Begin("AsiPluginImGui", nullptr);
        {
            ImGui::Text(u8"Текст");
        }
        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return std::nullopt;
}

std::optional<HRESULT> CRender::OnLost(const decltype(m_hookReset)& hook, IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    return std::nullopt;
}

void CRender::OnReset(const decltype(m_hookReset)& hook, HRESULT& returnValue, IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
{
    ImGui_ImplDX9_CreateDeviceObjects();
}

void* CRender::GetFunctionAddress(int virtualTableIndex)
{
    static std::uintptr_t device = [](std::size_t length)
    {
        std::string pathTo(MAX_PATH, '\0');

        if (auto size = GetSystemDirectoryA(pathTo.data(), MAX_PATH))
        {
            pathTo.resize(size);

            pathTo += "\\d3d9.dll";

            std::uintptr_t objBase = reinterpret_cast<std::uintptr_t>(LoadLibraryA(pathTo.c_str()));

            while (objBase++ < objBase + length)
            {
                if (*reinterpret_cast<std::uint16_t*>(objBase + 0x00) == 0x06C7 &&
                    *reinterpret_cast<std::uint16_t*>(objBase + 0x06) == 0x8689 &&
                    *reinterpret_cast<std::uint16_t*>(objBase + 0x0C) == 0x8689)
                {
                    objBase += 2;

                    break;
                }
            }

            return objBase;
        }

        return std::uintptr_t(0);
    }(0x128000);

    return (*reinterpret_cast<void***>(device))[virtualTableIndex];
}

void CRender::ShowCursor(bool state)
{
    static unsigned long updateMouseProtection, rsMouseSetPosProtFirst, rsMouseSetPosProtSecond;

    if (state)
    {
        VirtualProtect(reinterpret_cast<void*>(0x53F3C6U), 5U, PAGE_EXECUTE_READWRITE, &updateMouseProtection);
        VirtualProtect(reinterpret_cast<void*>(0x53E9F1U), 5U, PAGE_EXECUTE_READWRITE, &rsMouseSetPosProtFirst);
        VirtualProtect(reinterpret_cast<void*>(0x748A1BU), 5U, PAGE_EXECUTE_READWRITE, &rsMouseSetPosProtSecond);

        *reinterpret_cast<unsigned char*>(0x53F3C6U) = 0xE9U;
        *reinterpret_cast<unsigned int*>(0x53F3C6U + 1U) = 0x15BU;

        memset(reinterpret_cast<void*>(0x53E9F1U), 0x90, 5U);
        memset(reinterpret_cast<void*>(0x748A1BU), 0x90, 5U);
    }
    else
    {
        memcpy(reinterpret_cast<void*>(0x53F3C6U), "\xE8\x95\x6C\x20\x00", 5U);
        memcpy(reinterpret_cast<void*>(0x53E9F1U), "\xE8\xAA\xAA\x0D\x00", 5U);
        memcpy(reinterpret_cast<void*>(0x748A1BU), "\xE8\x80\x0A\xED\xFF", 5U);

        reinterpret_cast<void(__cdecl*)()>(0x541BD0U)();
        reinterpret_cast<void(__cdecl*)()>(0x541DD0U)();

        VirtualProtect(reinterpret_cast<void*>(0x53F3C6U), 5U, updateMouseProtection, &updateMouseProtection);
        VirtualProtect(reinterpret_cast<void*>(0x53E9F1U), 5U, rsMouseSetPosProtFirst, &rsMouseSetPosProtFirst);
        VirtualProtect(reinterpret_cast<void*>(0x748A1BU), 5U, rsMouseSetPosProtSecond, &rsMouseSetPosProtSecond);
    }
}