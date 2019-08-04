#include "ServerAuthPlayerComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <GridMate/Replica/ReplicaChunk.h>
#include <AzFramework/Network/NetBindingHandlerBus.h>
#include <GridMate/Replica/RemoteProcedureCall.h>
#include <GridMate/Replica/ReplicaFunctions.h>
#include <MultiplayerCharacter/LocalClientBus.h>

using namespace AZ;
using namespace AzFramework;
using namespace GridMate;
using namespace MultiplayerCharacter;

class ServerAuthPlayerComponent::Chunk : public ReplicaChunkBase
{
public:
    GM_CLASS_ALLOCATOR(Chunk);
    Chunk() : m_owningPlayer("Owning Player") {}

    static const char* GetChunkName()
    {
        return "ServerAuthPlayerComponent::Chunk";
    }
    bool IsReplicaMigratable() override { return true; }

    DataSet<MemberIDCompact> m_owningPlayer;
};

void ServerAuthPlayerComponent::Reflect(
    AZ::ReflectContext* context)
{
    if (auto sc = azrtti_cast<SerializeContext*>(context))
    {
        sc->Class<ServerAuthPlayerComponent, Component>()
          ->Version(1);

        if (EditContext* ec = sc->GetEditContext())
        {
            ec->Class<ServerAuthPlayerComponent>(
                  "Server Auth Player Component",
                  "Server Authoritative")
              ->ClassElement(
                  Edit::ClassElements::EditorData, "")
              ->Attribute(Edit::Attributes::Category,
                          "Multiplayer Character")
              ->Attribute(
                  Edit::Attributes::
                  AppearsInAddComponentMenu, AZ_CRC("Game"));
        }
    }

    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(
        ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }
}

void ServerAuthPlayerComponent::SetAssociatedPlayerId(
    GridMate::MemberIDCompact player)
{
    if (Chunk* chunk = static_cast<Chunk*>(m_chunk.get()))
        chunk->m_owningPlayer.Set(player);
}

ReplicaChunkPtr ServerAuthPlayerComponent::GetNetworkBinding()
{
    m_chunk = GridMate::CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void ServerAuthPlayerComponent::SetNetworkBinding(
    GridMate::ReplicaChunkPtr chunk)
{
    m_chunk = chunk;
    m_chunk->SetHandler(this);
}

void ServerAuthPlayerComponent::UnbindFromNetwork()
{
    if (m_chunk)
    {
        m_chunk->SetHandler(nullptr);
        m_chunk = nullptr;
    }
}

void ServerAuthPlayerComponent::Activate()
{
    if (IsDedicated())
    {
        ServerPlayerBodyBus::Handler::BusConnect(GetEntityId());
    }
    else
    {
        BroadcastNewBody();
    }
}

void ServerAuthPlayerComponent::Deactivate()
{
    if (IsDedicated())
    {
        ServerPlayerBodyBus::Handler::BusDisconnect();
    }
}

void ServerAuthPlayerComponent::BroadcastNewBody()
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
    {
        const MemberIDCompact playerId = c->m_owningPlayer.Get();
        AZ_Printf("Book", "BroadcastNewBody %u", playerId);

        if (playerId != 0)
        {
            LocalClientBus::Broadcast(
                &LocalClientBus::Events::AttachToBody,
                playerId, GetEntityId());
        }
    }
}