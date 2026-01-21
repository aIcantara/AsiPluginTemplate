#include "Plugin.h"

#include <sampapi/CChat.h>

using namespace std::placeholders;

namespace SAMP = sampapi::v037r1;

CPlugin::CPlugin( HMODULE hModule ) : m_hModule( hModule )
{
    AttachConsole();

    m_hookGameLoop.set_dest( 0x561B10 );
    m_hookGameLoop.set_cb( std::bind( &CPlugin::OnGameLoop, this, _1 ) );
    m_hookGameLoop.install();

    m_hookWndProc.set_dest( 0x747EB0 );
    m_hookWndProc.set_cb( std::bind( &CPlugin::OnWndProc, this, _1, _2, _3, _4, _5 ) );
    m_hookWndProc.install();
}

void CPlugin::OnGameLoop( const decltype( m_hookGameLoop )& hook )
{
    static bool initialized = false;

    if ( !initialized && SAMP::RefChat() && m_network.Initialize() )
    {
        SAMP::RefChat()->AddMessage( -1, "Plugin Loaded" );

        initialized = true;
    }

    return hook.call_trampoline();
}

HRESULT CPlugin::OnWndProc( const decltype( m_hookWndProc )& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( uMsg == WM_KEYDOWN )
    {
        if ( wParam == VK_F9 && ( HIWORD( lParam ) & KF_REPEAT ) != KF_REPEAT )
        {
            printf( "[Plugin] F9 Pressed\n" );
        }
    }

    return hook.call_trampoline( hWnd, uMsg, wParam, lParam );
}

void CPlugin::AttachConsole()
{
    if ( !AllocConsole() )
    {
        return;
    }

    FILE* file;

    freopen_s( &file, "CONOUT$", "w", stdout );
    freopen_s( &file, "CONOUT$", "w", stderr );
    freopen_s( &file, "CONIN$", "r", stdin );

    SetConsoleCP( 1251 );
    SetConsoleOutputCP( 1251 );
}