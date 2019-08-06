#include "MultiplayerCloseAllNetworkPeers_precompiled.h"
#include <MultiplayerCloseAllNetworkPeers/ShutdownApplication.h>
#include <ISystem.h>
#include <IConsole.h>

void MultiplayerCloseAllNetworkPeers::ShutdownApplication()
{
    AZ_TracePrintf("CloseAll", "Shutting down");

    ISystem* system = nullptr;
    CrySystemRequestBus::BroadcastResult(system,
        &CrySystemRequestBus::Events::GetCrySystem);
    if (system && system->GetIConsole())
    {
        system->GetIConsole()->ExecuteString("quit");
    }
}
