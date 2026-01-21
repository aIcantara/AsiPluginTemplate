#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "Render.h"

#include <kthook/kthook.hpp>

class CPlugin
{
public:
    CPlugin( HMODULE hModule );
    ~CPlugin() = default;

private:
    CRender m_render;

    HMODULE m_hModule;

    kthook::kthook_simple<HRESULT( __stdcall* )( HWND, UINT, WPARAM, LPARAM )> m_hookWndProc;
    
    HRESULT OnWndProc( const decltype( m_hookWndProc )& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

#endif // __PLUGIN_H__