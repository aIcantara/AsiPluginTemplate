#include "Plugin.h"

using namespace std::placeholders;

CPlugin::CPlugin(HMODULE hModule) : m_hModule(hModule)
{
    m_hookWndProc.set_dest(0x747EB0);
    m_hookWndProc.set_cb(std::bind(&CPlugin::OnWndProc, this, _1, _2, _3, _4, _5));
    m_hookWndProc.install();
}

HRESULT CPlugin::OnWndProc(const decltype(m_hookWndProc)& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
    {
        return 1;
    }

    if (uMsg == WM_KEYDOWN && wParam == VK_F9 && (HIWORD(lParam) & KF_REPEAT) != KF_REPEAT)
    {
        m_render.ToggleMenu();
    }

    return hook.call_trampoline(hWnd, uMsg, wParam, lParam);
}