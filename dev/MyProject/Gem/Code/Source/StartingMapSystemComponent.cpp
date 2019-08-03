#include "StartingMapSystemComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzFramework/Components/ConsoleBus.h>
#include <platform.h>
#include <platform_impl.h>
#include <CrySystemBus.h>
#include <ISystem.h>

using namespace AZ;
using namespace MyProject;

void StartingMapSystemComponent::Reflect(
    AZ::ReflectContext* context)
{
    auto s = azrtti_cast<SerializeContext*>(context);
    if (!s) return;

    // Shortcut for readability
    using Self = StartingMapSystemComponent;

    s->Class<Self, Component>()
        ->Version(1)
        ->Field("Client Map", &Self::m_clientMap)
        ->Field("Server Map", &Self::m_serverMap);

    EditContext* ec = s->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    using namespace AZ::Edit::UIHandlers;
    ec->Class<Self>(
        "Starting Map System Component",
        "[Loads the starting map]")
        ->ClassElement(Edit::ClassElements::EditorData, "")
        // mark it as a system component
        ->Attribute(AppearsInAddComponentMenu, AZ_CRC("System"))
        ->DataElement(Default, &Self::m_clientMap,
            "Map to load on a client", "")
        ->DataElement(Default, &Self::m_serverMap,
            "Map to load on the server", "");
}

void StartingMapSystemComponent::Activate()
{
    SystemTickBus::Handler::BusConnect();
}

void StartingMapSystemComponent::Deactivate()
{
    if (SystemTickBus::Handler::BusIsConnected())
        SystemTickBus::Handler::BusDisconnect();
}

void StartingMapSystemComponent::OnSystemTick()
{
    ISystem* system = nullptr;
    CrySystemRequestBus::BroadcastResult(system,
        &CrySystemRequestBus::Events::GetCrySystem);
    if (system && system->GetGlobalEnvironment() &&
        system->GetGlobalEnvironment()->IsEditor())
    {
        BusDisconnect();
        return; // skip running these commands in the editor
    }

    AZStd::string mapCommand("map ");
#if defined(DEDICATED_SERVER)
    AzFramework::ConsoleRequestBus::Broadcast(
        &AzFramework::ConsoleRequests::ExecuteConsoleCommand,
        "mphost");

    mapCommand += m_serverMap;
#else
    mapCommand += m_clientMap;
#endif

    AzFramework::ConsoleRequestBus::Broadcast(
        &AzFramework::ConsoleRequests::ExecuteConsoleCommand,
        mapCommand.c_str());

    SystemTickBus::Handler::BusDisconnect();
}