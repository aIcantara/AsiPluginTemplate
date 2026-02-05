#ifndef __RENDER_H__
#define __RENDER_H__

#include <kthook/kthook.hpp>

#include <imgui.h>

#include <d3d9.h>

class CRender
{
public:
    CRender();
    ~CRender();

    bool ToggleMenu();

private:
    bool m_menuState = false;

    kthook::kthook_signal<HRESULT(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> m_hookPresent;
    kthook::kthook_signal<HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> m_hookReset;

    std::optional<HRESULT> OnPresent(const decltype(m_hookPresent)& hook, IDirect3DDevice9* pDevice, const RECT*, const RECT*, HWND, const RGNDATA*);
    std::optional<HRESULT> OnLost(const decltype(m_hookReset)& hook, IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
    void OnReset(const decltype(m_hookReset)& hook, HRESULT& returnValue, IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);

    void* GetFunctionAddress(int virtualTableIndex);

    void ShowCursor(bool state);
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // __RENDER_H__