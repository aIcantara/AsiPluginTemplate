#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "Network.h"

#include <kthook/kthook.hpp>

class CPlugin
{
public:
    CPlugin( HMODULE hModule );
    ~CPlugin() = default;

private:
    CNetwork m_network;

    HMODULE m_hModule;
    
    kthook::kthook_simple<void( __cdecl* )()> m_hookGameLoop;
    kthook::kthook_simple<HRESULT( __stdcall* )( HWND, UINT, WPARAM, LPARAM )> m_hookWndProc;

    void OnGameLoop( const decltype( m_hookGameLoop )& hook );
    HRESULT OnWndProc( const decltype( m_hookWndProc )& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    void AttachConsole();
};

#endif // __PLUGIN_H__