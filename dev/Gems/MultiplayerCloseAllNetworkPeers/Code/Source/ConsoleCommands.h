#pragma once
#include <IConsole.h>

namespace MultiplayerCloseAllNetworkPeers
{
    class ConsoleCommands
    {
    public:
        static void Register();
        static void Unregister();
        static void CloseAll(IConsoleCmdArgs* args);
    };
}
