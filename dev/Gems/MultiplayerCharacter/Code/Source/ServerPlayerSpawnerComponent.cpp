#include "ServerPlayerSpawnerComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzFramework/Network/NetBindingHandlerBus.h>
#include <ISystem.h>
#include <INetwork.h>
#include <LmbrCentral/Scripting/SpawnerComponentBus.h>
#include <MultiplayerCharacter/ServerPlayerBodyBus.h>

using namespace AZ;
using namespace AzFramework;
using namespace GridMate;
using namespace MultiplayerCharacter;

void ServerPlayerSpawnerComponent::Reflect(ReflectContext* rc)
{
    if (auto sc = azrtti_cast<SerializeContext*>(rc))
    {
        sc->Class<ServerPlayerSpawnerComponent, Component>()
            ->Version(1);

        if (EditContext* ec = sc->GetEditContext())
        {
            ec->Class<ServerPlayerSpawnerComponent>(
                "Server Player Spawner",
                "Server-authoritative body logic")
                ->ClassElement(
                    Edit::ClassElements::EditorData, "")
                ->Attribute(Edit::Attributes::Category,
                    "Multiplayer Character")
                ->Attribute(Edit::Attributes::
                    AppearsInAddComponentMenu, AZ_CRC("Game"));
        }
    }
}

void ServerPlayerSpawnerComponent::Activate()
{
#if defined(DEDICATED_SERVER)
    ISystem* system = nullptr;
    CrySystemRequestBus::BroadcastResult(system,
        &CrySystemRequestBus::Events::GetCrySystem);
    if (system)
    {
        SessionEventBus::Handler::BusConnect(
            system->GetINetwork()->GetGridMate());
    }
#endif // DEDICATED_SERVER
}

void ServerPlayerSpawnerComponent::Deactivate()
{
    if (SessionEventBus::Handler::BusIsConnected())
    {
        SessionEventBus::Handler::BusDisconnect();
    }
}

void ServerPlayerSpawnerComponent::OnMemberJoined(
    GridMate::GridSession* session,
    GridMate::GridMember* member)
{
    const MemberIDCompact playerId = member->GetIdCompact();
     if (session->GetMyMember()->GetIdCompact() == playerId)
        return; // ignore ourselves, the server

    SpawnPlayer(playerId);
}

void ServerPlayerSpawnerComponent::OnSliceInstantiated(
    const AZ::Data::AssetId&,
    const SliceComponent::SliceInstanceAddress& address)
{
    const SliceInstantiationTicket& ticket =
        *SliceInstantiationResultBus::GetCurrentBusId();
    const auto iter = m_joiningPlayers.find(ticket);
    if (iter != m_joiningPlayers.end())
    {
        const MemberIDCompact playerId = iter->second;
        SliceInstantiationResultBus::MultiHandler::BusDisconnect(
            ticket);

        for (Entity* entity : address.second->
            GetInstantiated()->m_entities)
        {
            ServerPlayerBodyBus::Event(entity->GetId(),
                &ServerPlayerBodyBus::Events::
                    SetAssociatedPlayerId, playerId);
        }
    }
    m_joiningPlayers.erase(iter);
}

void ServerPlayerSpawnerComponent::SpawnPlayer(
    GridMate::MemberIDCompact playerId)
{
    const Transform t = Transform::CreateTranslation(
        Vector3::CreateAxisY(10.f - m_playerCount * 1.f));
    m_playerCount++;

    AZ_Printf("Book", "Spawning player @ %f %f %f",
        static_cast<float>(t.GetTranslation().GetX()),
        static_cast<float>(t.GetTranslation().GetY()),
        static_cast<float>(t.GetTranslation().GetZ()));

    AzFramework::SliceInstantiationTicket tckt;
    using LmbrCentral::SpawnerComponentRequestBus;
    SpawnerComponentRequestBus::EventResult(tckt, GetEntityId(),
        &SpawnerComponentRequestBus::Events::SpawnRelative, t);

    m_joiningPlayers[tckt] = playerId;
    SliceInstantiationResultBus::MultiHandler::BusConnect(tckt);
}