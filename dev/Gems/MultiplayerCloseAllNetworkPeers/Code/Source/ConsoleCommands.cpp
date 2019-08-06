#include "MultiplayerCloseAllNetworkPeers_precompiled.h"
#include "ConsoleCommands.h"
#include <ISystem.h>
#include <MultiplayerCloseAllNetworkPeers/CloseNetworkPeersBus.h>

using namespace MultiplayerCloseAllNetworkPeers;

void ConsoleCommands::Register()
{
    ISystem* system = nullptr;
    CrySystemRequestBus::BroadcastResult(system, &CrySystemRequestBus::Events::GetCrySystem);
    if (system)
    {
        if (IConsole* console = system->GetIConsole())
        {
            console->AddCommand("closeallnetworkpeers", CloseAll, 0, "Closes the server and all connected clients.");
        }
    }
}

void ConsoleCommands::Unregister()
{
    ISystem* system = nullptr;
    CrySystemRequestBus::BroadcastResult(system, &CrySystemRequestBus::Events::GetCrySystem);
    if (system)
    {
        if (IConsole* console = system->GetIConsole())
        {
            console->RemoveCommand("closeallnetworkpeers");
        }
    }
}

void ConsoleCommands::CloseAll(IConsoleCmdArgs* /*args*/)
{
    CloseNetworkPeersRequestBus::Broadcast(&CloseNetworkPeersRequestBus::Events::CloseAll);
}
