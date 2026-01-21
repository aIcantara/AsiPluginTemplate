#include "Plugin.h"

#include <memory>

std::unique_ptr<CPlugin> g_pPlugin;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved )
{
    if ( dwReason == DLL_PROCESS_ATTACH )
    {
        DisableThreadLibraryCalls( hModule );

        g_pPlugin = std::make_unique<CPlugin>( hModule );
    }
    else if ( dwReason == DLL_PROCESS_DETACH )
    {
        g_pPlugin.reset();
    }

    return TRUE;
}